#include "event.h"
#include <string.h>
#include <stdio.h>

pthread_mutex_t queue_lock = PTHREAD_MUTEX_INITIALIZER;
event_item_t event_queue[8];
int event_count = 0;

void push_wifi_event(const char *ssid, const char *pw)
{
    pthread_mutex_lock(&queue_lock);
    if (event_count < 8) {
        event_queue[event_count].type = EVENT_WIFI_CRED;
        strncpy(event_queue[event_count].ssid, ssid, sizeof(event_queue[event_count].ssid)-1);
        event_queue[event_count].ssid[sizeof(event_queue[event_count].ssid)-1] = 0;
        strncpy(event_queue[event_count].pw, pw, sizeof(event_queue[event_count].pw)-1);
        event_queue[event_count].pw[sizeof(event_queue[event_count].pw)-1] = 0;
        event_count++;
    }
    pthread_mutex_unlock(&queue_lock);
}

void push_wifi_status_event(const char *ssid, int status)
{
    pthread_mutex_lock(&queue_lock);
    if (event_count < 8) {
        event_queue[event_count].type = EVENT_WIFI_STATUS;
        strncpy(event_queue[event_count].ssid, ssid, sizeof(event_queue[event_count].ssid)-1);
        event_queue[event_count].ssid[sizeof(event_queue[event_count].ssid)-1] = 0;
        event_queue[event_count].status = status;
        event_count++;
    }
    pthread_mutex_unlock(&queue_lock);
}
