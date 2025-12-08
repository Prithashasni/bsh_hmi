#ifndef WIFI_SCREEN_H
#define WIFI_SCREEN_H

extern void create_wifi_screen();
// void update_wifi_screen(const char *ssid, const char *pw);
void show_wifi_screen(void);
void hide_wifi_screen(void);
void update_wifi_screen(const char *ssid, const char *status);

#endif
