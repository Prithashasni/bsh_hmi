#ifndef NFC_H
#define NFC_H

int read_ssid_pw(char *out_ssid, char *out_pw);
void *nfc_thread(void *arg);

#endif
