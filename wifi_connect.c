#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define WIFI_IFACE     "wlan0"
#define WPA_CONF_PATH  "/tmp/wpa_supplicant.conf"
#define MAX_RETRIES    5
#define RETRY_DELAY    3   // seconds

// Run shell command and return exit code
static int run_cmd(const char *cmd)
{
    printf("CMD: %s\n", cmd);
    return system(cmd);
}

// Generate wpa_supplicant.conf for WPA/WPA2 networks
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
    printf("Starting WiFi connection to SSID: %s\n", ssid);

    // Step 1: Create WPA configuration file
    if (create_wpa_conf(ssid, password) < 0)
        return -1;

    // Step 2: Bring interface up
    run_cmd("ip link set " WIFI_IFACE " up");

    // Step 3: Kill possible old wpa_supplicant instances
    run_cmd("killall wpa_supplicant 2>/dev/null");

    // Step 4: Start wpa_supplicant with iw
    char cmd[256];
    snprintf(cmd, sizeof(cmd),
             "wpa_supplicant -B -i %s -c %s",
             WIFI_IFACE, WPA_CONF_PATH);

    if (run_cmd(cmd) != 0) {
        printf("Error: Failed to start wpa_supplicant\n");
        return -1;
    }

    // Step 5: Retry loop for connection
    for (int i = 0; i < MAX_RETRIES; i++) {

        printf("Checking WiFi connection... attempt %d/%d\n",
               i + 1, MAX_RETRIES);

        if (check_wifi_connected()) {
            printf("✅ WiFi CONNECTED successfully!\n");

            // Get IP address using DHCP
            run_cmd("dhclient " WIFI_IFACE);

            return 0;
        }

        sleep(RETRY_DELAY);
    }

    printf("❌ WiFi connection FAILED after retries.\n");
    return -1;
}
