#ifndef EVENT_H
#define EVENT_H

#include <pthread.h>

#define EVENT_WIFI_CRED   1
#define EVENT_WIFI_STATUS 2

typedef struct {
    int type;         // event type
    char ssid[64];
    char pw[64];      // only used for EVENT_WIFI_CRED
    int status;       // 0=success, -1=failure (used for EVENT_WIFI_STATUS)
} event_item_t;

extern pthread_mutex_t queue_lock;
extern event_item_t event_queue[8];
extern int event_count;

void push_wifi_event(const char *ssid, const char *pw);
void push_wifi_status_event(const char *ssid, int status);

#endif
