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

#define TLV_SSID     0x1045
#define TLV_PASSWORD 0x1027
// #define TLV_AUTH     0x1003
// #define TLV_ENCR     0x100F

extern int running;

static pthread_mutex_t i2c_lock = PTHREAD_MUTEX_INITIALIZER;

static uint16_t be16(const uint8_t *p)
{
    return ((uint16_t)p[0] << 8) | p[1];
}

int i2c_read_block(int fd, uint8_t start_addr, uint8_t *data, size_t len){
    // if(write(fd,&start_addr,1)!=1){ perror("Set start addr"); return -1; }
    if(read(fd,data,len)!=(ssize_t)len){ perror("I2C Read failed"); return -1; }
    return 0;
}

/* Reads SSID/PW from NFC */
static wifi_record_t read_ssid_pw_fd(int fd)
{
    wifi_record_t record = {0};

    uint8_t buf[READ_LEN];
    if (i2c_read_block(fd, 0x00, buf, READ_LEN) < 0) {
        printf("[NFC] Read block failed\n");
        return record;
    }

    int offset = 0;
    while (offset < READ_LEN - 4)
    {
        uint16_t tlv_type = be16(&buf[offset]);
        uint16_t tlv_len  = be16(&buf[offset+2]);
        uint8_t *data     = &buf[offset+4];

        if (tlv_len == 0 || offset + 4 + tlv_len > READ_LEN) {
            offset++;
            continue;
        }

        switch (tlv_type) {
            case TLV_SSID:
                strncpy(record.ssid, (char*)data, tlv_len);
                record.ssid[tlv_len] = '\0';
                record.has_data = 1;
                break;

            case TLV_PASSWORD:
                strncpy(record.password, (char*)data, tlv_len);
                record.password[tlv_len] = '\0';
                record.has_data = 1;
                break;

            // case TLV_AUTH: {
            //     uint16_t v = be16(data);
            //     strncpy(record.auth, auth_type(v), sizeof(record.auth)-1);
            //     record.has_data = 1;
            //     break;
            // }
            // case TLV_ENCR: {
            //     uint16_t v = be16(data);
            //     strncpy(record.encr, encr_type(v), sizeof(record.encr)-1);
            //     record.has_data = 1;
            //     break;
            // }
        }

        offset += 4 + tlv_len;
    }

    return record;
}

/************* NFC Thread *************/
void *nfc_thread(void *arg)
{
    // Open I2C once
    int fd = open(I2C_BUS, O_RDWR);
    if (fd < 0) {
        perror("[NFC] open");
        return NULL;
    }
    if (ioctl(fd, I2C_SLAVE, NFC_ADDR) < 0) {
        perror("[NFC] ioctl");
        close(fd);
        return NULL;
    }

    printf("[NFC] Thread started, waiting for tap...\n");

    wifi_record_t last = {0};   // store previous to avoid duplicates

    while (running)
    {
        pthread_mutex_lock(&i2c_lock);
        wifi_record_t rec = read_ssid_pw_fd(fd);
        pthread_mutex_unlock(&i2c_lock);

        if (rec.has_data)
        {
            // Avoid duplicate Wi-Fi updates
            if (strcmp(rec.ssid, last.ssid) != 0 ||
                strcmp(rec.password, last.password) != 0)
            {
                printf("[NFC] New Wi-Fi credentials received:\n");
                printf("  SSID: %s\n", rec.ssid);
                printf("  PW  : %s\n\n", rec.password);

                wifi_update_credentials(rec.ssid, rec.password, 0);

                last = rec; // cache last
            }
        }

        usleep(500000); // 500 ms
    }

    close(fd);
    return NULL;
}
