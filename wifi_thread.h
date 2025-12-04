#ifndef WIFI_THREAD_H
#define WIFI_THREAD_H

void wifi_update_credentials(const char *ssid, const char *pw, int force);
void *wifi_thread(void *arg);
int wifi_connect(const char *ssid, const char *pw);

#endif
