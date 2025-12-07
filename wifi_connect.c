#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int exec_cmd(const char *cmd)
{
    int rc = system(cmd);
    if (rc != 0)
        printf("[WiFi] cmd failed: %s (rc=%d)\n", cmd, rc);
    return rc;
}

int wifi_connect(const char *ssid, const char *pw)
{
    printf("[WiFi] wifi_connect(): Using wpa_cli\n");

    char cmd[256];

    /* remove previously configured networks */
    exec_cmd("wpa_cli -i wlan0 remove_network all");

    /* add network */
    exec_cmd("wpa_cli -i wlan0 add_network");

    /* set ssid */
    snprintf(cmd, sizeof(cmd),
             "wpa_cli -i wlan0 set_network 0 ssid '\"%s\"'", ssid);
    exec_cmd(cmd);

    /* set psk */
    snprintf(cmd, sizeof(cmd),
             "wpa_cli -i wlan0 set_network 0 psk '\"%s\"'", pw);
    exec_cmd(cmd);

    /* optional: WPA2/WPA3 etc; for now allow WPA2-PSK */
    exec_cmd("wpa_cli -i wlan0 set_network 0 key_mgmt WPA-PSK");

    /* enable */
    exec_cmd("wpa_cli -i wlan0 enable_network 0");

    /* save config (optional) */
    exec_cmd("wpa_cli -i wlan0 save_config");

    /* reconfigure */
    exec_cmd("wpa_cli -i wlan0 reconfigure");

    printf("[WiFi] waiting for connection...\n");
    sleep(3);

    /* check status */
    int rc = system(
        "wpa_cli -i wlan0 status | grep -q 'wpa_state=COMPLETED'"
    );

    if (rc == 0) {
        printf("[WiFi] Connected!\n");
        return 0;
    }

    printf("[WiFi] Failed to connect.\n");
    return -1;
}
