#ifndef NFC_H
#define NFC_H

typedef struct{
    char ssid[64];
    char password[64];
    char auth[32];
    char encr[32];
    int has_data;
    int offset;
} wifi_record_t;

static wifi_record_t read_ssid_pw_fd(int fd);
void *nfc_thread(void *arg);

#endif
