// #include <stdio.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <linux/i2c-dev.h>
// #include <sys/ioctl.h>
// #include <stdint.h>

// #include "nfc.h"
// #include "main.h"

// #define I2C_BUS "/dev/i2c-1"
// #define NFC_ADDR 0x53
// #define READ_LEN 512

// // extern int running;

// void *nfc_thread(void *arg)
// {
//     while (running)
//     {
//         int fd = open(I2C_BUS, O_RDWR);
//         if (fd < 0) { perror("I2C open"); usleep(300000); continue; }

//         if (ioctl(fd, I2C_SLAVE, NFC_ADDR) < 0) {
//             perror("I2C addr");
//             close(fd);
//             usleep(300000);
//             continue;
//         }

//         uint8_t buf[READ_LEN];
//         int r = read(fd, buf, READ_LEN);
//         if (r > 0)
//         {
//             printf("[NFC] %d bytes read\n", r);

//             // TODO: Send data to LVGL (optional)
//         }

//         close(fd);
//         usleep(300000);
//     }
//     return NULL;
// }
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "nfc.h"
#include "main.h"

// Stub SSID + Password for simulator
static const char *STUB_SSID = "Home_WiFi_5G";
static const char *STUB_PW   = "MySecretPassword123";

int read_ssid_pw(char *ssid, char *pw)
{
    // --- SIMULATOR MODE ---
    printf("[NFC STUB] Simulating NFC read...\n");

    strcpy(ssid, STUB_SSID);
    strcpy(pw,   STUB_PW);

    usleep(100000); // simulate delay
    return 0;
}

// Optional idle thread
void *nfc_thread(void *arg)
{
    while (running) {
        usleep(200000);
    }
    return NULL;
}
