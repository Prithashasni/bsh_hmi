#include "nfc.h"
#include "wifi_thread.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <stdint.h> 

#define I2C_BUS "/dev/i2c-1"
#define NFC_ADDR 0x53
#define READ_LEN 512

#define TLV_SSID      0x1045
#define TLV_PASSWORD  0x1027

extern int running;

static uint16_t be16(const uint8_t *p) {
    return ((uint16_t)p[0] << 8) | p[1];
}

int read_ssid_pw(char *out_ssid, char *out_pw)
{
    printf("[NFC] Reading SSID and Password from NFC module...\n");
    int fd = open(I2C_BUS, O_RDWR);
    if (fd < 0) return -2;
    if (ioctl(fd, I2C_SLAVE, NFC_ADDR) < 0) { close(fd); return -2; }

    uint8_t buf[READ_LEN];
    uint8_t start = 0x00;
    if (write(fd, &start, 1) != 1) { close(fd); return -2; }
    if (read(fd, buf, READ_LEN) != READ_LEN) { close(fd); return -2; }

    close(fd);

    out_ssid[0] = 0;
    out_pw[0] = 0;
    int offset = 0;
    int found = 0;

    while (offset < READ_LEN - 4) {
        uint16_t type = be16(&buf[offset]);
        uint16_t len  = be16(&buf[offset + 2]);

        if (len == 0 || offset + 4 + len > READ_LEN) {
            offset++;
            continue;
        }

        uint8_t *data = &buf[offset + 4];

        if (type == TLV_SSID) {
            int copy_len = len < 63 ? len : 63;
            strncpy(out_ssid, (char*)data, copy_len);
            out_ssid[copy_len] = 0;
            found = 1;
        } else if (type == TLV_PASSWORD) {
            int copy_len = len < 63 ? len : 63;
            strncpy(out_pw, (char*)data, copy_len);
            out_pw[copy_len] = 0;
            found = 1;
        }

        offset += 4 + len;
    }

    return found ? 0 : -1;
}

void *nfc_thread(void *arg)
{
    char last_ssid[64] = {0};
    char last_pw[64] = {0};

    while (running) {
        char ssid[64], pw[64];
        if (read_ssid_pw(ssid, pw) == 0) {
            // Only update if credentials changed
            if (strcmp(ssid, last_ssid) != 0 || strcmp(pw, last_pw) != 0) {
                wifi_update_credentials(ssid, pw);
                printf("[NFC] Updated WiFi credentials: SSID='%s', PW='%s'\n", ssid, pw);
                strncpy(last_ssid, ssid, sizeof(last_ssid)-1);
                last_ssid[sizeof(last_ssid)-1] = 0;
                printf("[NFC] Updated WiFi credentials: SSID='%s', PW='%s'\n", ssid, pw);
                strncpy(last_pw, pw, sizeof(last_pw)-1);
                last_pw[sizeof(last_pw)-1] = 0;
                printf("[NFC] Waiting for WiFi connection...\n");
            }
        }
        usleep(100000);
    }
    return NULL;
}
