#include "wifi_thread.h"
#include "event.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

extern int running;
pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;

static char g_ssid[64] = {0};
static char g_pw[64] = {0};
static int g_pending = 0;

void wifi_update_credentials(const char *ssid, const char *pw)
{
    pthread_mutex_lock(&g_lock);
    if (strcmp(g_ssid, ssid) != 0 || strcmp(g_pw, pw) != 0) {
        strncpy(g_ssid, ssid, sizeof(g_ssid)-1);
        g_ssid[sizeof(g_ssid)-1] = 0;
        strncpy(g_pw, pw, sizeof(g_pw)-1);
        g_pw[sizeof(g_pw)-1] = 0;
        g_pending = 1;
    }
    pthread_mutex_unlock(&g_lock);
}

void *wifi_thread(void *arg)
{
    while (running) {
        pthread_mutex_lock(&g_lock);
        int pending = g_pending;
        char ssid[64], pw[64];
        strncpy(ssid, g_ssid, sizeof(ssid));
        strncpy(pw, g_pw, sizeof(pw));
        pthread_mutex_unlock(&g_lock);

        if (!pending) {
            usleep(200000);
            continue;
        }

        printf("[WiFi] Connecting to SSID=%s\n", ssid);
        int status = wifi_connect(ssid, pw);

        push_wifi_status_event(ssid, status);

        pthread_mutex_lock(&g_lock);
        g_pending = 0;
        pthread_mutex_unlock(&g_lock);
    }
    return NULL;
}
