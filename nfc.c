#include "nfc.h"
#include "wifi_thread.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>

#define I2C_BUS "/dev/i2c-1"
#define NFC_ADDR 0x53
#define READ_LEN 512

#define TLV_SSID 0x1045
#define TLV_PASSWORD 0x1027

extern int running;

// Mutex for thread-safe I2C access
static pthread_mutex_t i2c_lock = PTHREAD_MUTEX_INITIALIZER;

// Convert big-endian two bytes to uint16
static uint16_t be16(const uint8_t * p)
{
    return ((uint16_t)p[0] << 8) | p[1];
}

// Read NFC EEPROM using an open FD (persistent)
static int read_ssid_pw_fd(int fd, char * out_ssid, char * out_pw)
{
    uint8_t buf[READ_LEN];
    uint8_t start = 0x00;

    if(write(fd, &start, 1) != 1) {
        perror("[NFC] Set start address failed");
        return -1;
    }
    usleep(5000); 

    if(read(fd, buf, READ_LEN) != READ_LEN) {
        perror("[NFC] I2C read failed");
        return -1;
    }

    out_ssid[0] = 0;
    out_pw[0]   = 0;

    int offset = 0;
    while(offset < READ_LEN - 4) {
        uint16_t type = be16(&buf[offset]);
        uint16_t len  = be16(&buf[offset + 2]);
        if(len == 0 || offset + 4 + len > READ_LEN) {
            offset++;
            continue;
        }
        uint8_t * data = &buf[offset + 4];

        if(type == TLV_SSID) {
            int copy_len = len < 63 ? len : 63;
            strncpy(out_ssid, (char *)data, copy_len);
            out_ssid[copy_len] = 0;
        } else if(type == TLV_PASSWORD) {
            int copy_len = len < 63 ? len : 63;
            strncpy(out_pw, (char *)data, copy_len);
            out_pw[copy_len] = 0;
        }

        offset += 4 + len;
    }

    return (out_ssid[0] && out_pw[0]) ? 0 : -1;
}

// NFC polling thread
void * nfc_thread(void * arg)
{
    static char cached_ssid[64] = {0};
    static char cached_pw[64]   = {0};

    // Open I2C once
    int fd = open(I2C_BUS, O_RDWR);
    if(fd < 0) {
        perror("[NFC] Open I2C failed");
        return NULL;
    }
    if(ioctl(fd, I2C_SLAVE, NFC_ADDR) < 0) {
        perror("[NFC] Set I2C addr failed");
        close(fd);
        return NULL;
    }

    while(running) {
        char ssid[64] = {0}, pw[64] = {0};
        int ret;

        pthread_mutex_lock(&i2c_lock);
        ret = read_ssid_pw_fd(fd, ssid, pw);
        pthread_mutex_unlock(&i2c_lock);

        if(ret == 0) {
            // Successful NFC read → update cached
            strncpy(cached_ssid, ssid, sizeof(cached_ssid) - 1);
            cached_ssid[sizeof(cached_ssid) - 1] = 0;
            strncpy(cached_pw, pw, sizeof(cached_pw) - 1);
            cached_pw[sizeof(cached_pw) - 1] = 0;

            wifi_update_credentials(ssid, pw, 0);
            printf("[NFC] Read credentials: SSID='%s', PW='%s'\n", ssid, pw);
        } else {
            // Failed → use cached
            if(cached_ssid[0] && cached_pw[0]) {
                printf("[NFC] NFC read failed. Using cached credentials.\n");
                wifi_update_credentials(cached_ssid, cached_pw, 1); // force reconnect
            } else {
                printf("[NFC] NFC read failed. No cached credentials.\n");
            }
        }

        usleep(500000); 
    }

    close(fd);
    return NULL;
}
