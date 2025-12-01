#include <termios.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdio.h>

#include "interrupt.h"
#include "main.h"

static struct termios old_tio, new_tio;

static void init_kb()
{
    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

static void close_kb()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
}

static int kbhit()
{
    struct timeval tv = {0,0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
}

// void *keyboard_thread(void *arg)
// {
//     init_kb();

//     while (running)
//     {
//         if (kbhit())
//         {
//             char c = getchar();

//             if (c == 'q') {
//                 running = 0;
//                 break;
//             }

//             printf("Key Pressed: %c\n", c);

//             // If needed: send event to LVGL
//         }
//         usleep(10000);
//     }

//     close_kb();
//     return NULL;
// }

void *keyboard_thread(void *arg)
{
    init_kb();

    while (running)
    {
        if (kbhit())
        {
            char c = getchar();

            if (c == 'q') {
                running = 0;
                break;
            }

            printf("Key Pressed: %c\n", c);

            char ssid[64] = {0};
            char pw[64] = {0};

            if (read_ssid_pw(ssid, pw) == 0)
            {
                printf("SSID: %s\n", ssid);
                printf("PW  : %s\n", pw);
            }
            else
            {
                printf("Failed reading NFC I2C\n");
            }
        }

        usleep(10000);
    }

    close_kb();
    return NULL;
}
