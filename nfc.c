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
#define TLV_AUTH     0x1003
#define TLV_ENCR     0x100F

extern int running;

static pthread_mutex_t i2c_lock = PTHREAD_MUTEX_INITIALIZER;

static uint16_t be16(const uint8_t *p)
{
    return ((uint16_t)p[0] << 8) | p[1];
}

int i2c_read_block(int fd, uint8_t start_addr, uint8_t *data, size_t len){
    if(read(fd,data,len)!=(ssize_t)len){ perror("I2C Read failed"); return -1; }
    return 0;
}

void read_wifi(){
    int fd=open(I2C_BUS,O_RDWR);
    if(fd<0){ perror("Open I2C"); return; }
    if(ioctl(fd,I2C_SLAVE,NFC_ADDR)<0){ perror("Set I2C addr"); close(fd); return; }

    uint8_t buf[READ_LEN]; 
    if(i2c_read_block(fd,0x00,buf,READ_LEN)<0){ close(fd); return; }

    wifi_record_t record={0}; int record_count=0; int offset=0;
    while(offset<READ_LEN-4){
        uint16_t tlv_type=be16(&buf[offset]);
        uint16_t tlv_len=be16(&buf[offset+2]);
        uint8_t *data=&buf[offset+4];
        if(tlv_len==0||offset+4+tlv_len>READ_LEN){ offset++; continue; }

        int is_wifi_tlv=0;
        if(tlv_type==TLV_SSID){ strncpy(record.ssid,(char*)data,tlv_len); record.ssid[tlv_len]=0; record.has_data=1; record.offset=offset; is_wifi_tlv=1;}
        else if(tlv_type==TLV_PASSWORD){ strncpy(record.password,(char*)data,tlv_len); record.password[tlv_len]=0; record.has_data=1; is_wifi_tlv=1;}
        // else if(tlv_type==TLV_AUTH){ uint16_t v=be16(data); strncpy(record.auth,auth_type(v),sizeof(record.auth)-1); record.has_data=1; is_wifi_tlv=1;}
        // else if(tlv_type==TLV_ENCR){ uint16_t v=be16(data); strncpy(record.encr,encr_type(v),sizeof(record.encr)-1); record.has_data=1; is_wifi_tlv=1;}

        if(is_wifi_tlv){ offset+=4+tlv_len;
            if(offset>=READ_LEN-4||(be16(&buf[offset])!=TLV_SSID&&be16(&buf[offset])!=TLV_PASSWORD)){
                if(record.has_data){ record_count++;
                    printf("Wi-Fi Record #%d (offset 0x%X)\n",record_count,record.offset);
                    if(record.ssid[0]) printf("  SSID: %s\n",record.ssid);
                    if(record.password[0]) printf("  Password: %s\n",record.password);
                    // if(record.auth[0]) printf("  Auth: %s\n",record.auth);
                    // if(record.encr[0]) printf("  Encryption: %s\n",record.encr);
                    printf("\n"); memset(&record,0,sizeof(record));
                }
            }
        } else offset++;
    }

    if(record_count==0) printf("No Wi-Fi NDEF records found.\n");
    close(fd);
}

/************* NFC Thread *************/
void *nfc_thread(void *arg)
{
    // // Open I2C once
    // int fd = open(I2C_BUS, O_RDWR);
    // if (fd < 0) {
    //     perror("[NFC] open");
    //     return NULL;
    // }
    // if (ioctl(fd, I2C_SLAVE, NFC_ADDR) < 0) {
    //     perror("[NFC] ioctl");
    //     close(fd);
    //     return NULL;
    // }

    printf("[NFC] Thread started, waiting for tap...\n");

    // wifi_record_t last = {0};  
    while (running)
    {
        pthread_mutex_lock(&i2c_lock);
        read_wifi();
        pthread_mutex_unlock(&i2c_lock);

        // if (rec.has_data)
        // {
        //     // Avoid duplicate Wi-Fi updates
        //     if (strcmp(rec.ssid, last.ssid) != 0 ||
        //         strcmp(rec.password, last.password) != 0)
        //     {
        //         printf("[NFC] New Wi-Fi credentials received:\n");
        //         printf("  SSID: %s\n", rec.ssid);
        //         printf("  PW  : %s\n\n", rec.password);

        //         wifi_update_credentials(rec.ssid, rec.password, 0);

        //         last = rec; // cache last
        //     }
        // }

        // usleep(500000); // 500 ms
    }

    // close(fd);
    // return NULL;
}
