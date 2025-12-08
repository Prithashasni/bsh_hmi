#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define WIFI_IFACE     "wlan0"
#define WPA_CONF_PATH  "/etc/wpa_supplicant.conf"
#define MAX_RETRIES    5
#define RETRY_DELAY    3   

// Run shell command and return exit code
static int run_cmd(const char *cmd)
{
    printf("CMD: %s\n", cmd);
    return system(cmd);
}

static int create_wpa_conf(const char *ssid, const char *password)
{
    FILE *fp = fopen(WPA_CONF_PATH, "w");
    if (!fp) {
        perror("Failed to create WPA config");
        return -1;
    }

    fprintf(fp,
        "ctrl_interface=/var/run/wpa_supplicant\n"
        "network={\n"
        "    ssid=\"%s\"\n"
        "    psk=\"%s\"\n"
        "}\n",
        ssid, password
    );

    fclose(fp);
    return 0;
}

// Check if connected via wpa_cli
static int check_wifi_connected()
{
    FILE *fp = popen("wpa_cli -i " WIFI_IFACE " status | grep wpa_state=COMPLETED", "r");
    if (!fp) return 0;

    char buffer[256];
    int connected = 0;

    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strstr(buffer, "COMPLETED")) {
            connected = 1;
            break;
        }
    }

    pclose(fp);
    return connected;
}

// Main WiFi connection function
int wifi_connect(const char *ssid, const char *password)
{
    printf("[WiFi] Starting connection to SSID='%s'\n", ssid);

    if (create_wpa_conf(ssid, password) < 0) {
        printf("[WiFi] ERROR: Failed to write wpa_supplicant.conf\n");
        return -1;
    }

    run_cmd("ip link set " WIFI_IFACE " down");
    run_cmd("ip link set " WIFI_IFACE " up");

    run_cmd("killall wpa_supplicant 2>/dev/null");

    char cmd[256];
    snprintf(cmd, sizeof(cmd),
             "wpa_supplicant -B -i %s -c %s",
             WIFI_IFACE, WPA_CONF_PATH);

    if (run_cmd(cmd) != 0) {
        printf("[WiFi] ERROR: Unable to start wpa_supplicant\n");
        return -1;
    }

    printf("[WiFi] Waiting for association...\n");

    for (int i = 0; i < MAX_RETRIES; i++) {
        printf("[WiFi] Check attempt %d/%d\n", i+1, MAX_RETRIES);

        if (check_wifi_connected()) {
            printf("[WiFi] CONNECTED!\n");
            run_cmd("dhclient " WIFI_IFACE);
            return 0;
        }

        sleep(RETRY_DELAY);
    }

    printf("[WiFi] FAILED to connect!\n");
    return -1;
}
