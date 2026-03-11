#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdlib.h>
#include <time.h>
#include <syslog.h>
#include <errno.h>

#define INPUT_DEV "/dev/input/event0"
#define LONG_PRESS_SEC 3
#define WAKE_IGNORE_SEC 2

int open_input()
{
    int fd;

    while (1) {
        fd = open(INPUT_DEV, O_RDONLY | O_NONBLOCK);
        if (fd >= 0)
            return fd;

        syslog(LOG_ERR, "Waiting for input device...");
        sleep(1);
    }
}

void flush_events(int fd)
{
    struct input_event ev;
    while (read(fd, &ev, sizeof(ev)) > 0);
}

int main()
{
    struct input_event ev;
    int fd;
    time_t press_time = 0;
    time_t last_resume = 0;

    openlog("power-mgr", LOG_PID, LOG_USER);
    syslog(LOG_INFO, "Power manager starting");

    fd = open_input();

    syslog(LOG_INFO, "Input device opened");

    while (1) {

        if (read(fd, &ev, sizeof(ev)) != sizeof(ev)) {
            usleep(10000);
            continue;
        }

        /* ignore events shortly after wake */
        if (time(NULL) - last_resume < WAKE_IGNORE_SEC)
            continue;

        if (ev.type != EV_KEY || ev.code != KEY_POWER)
            continue;

        /* ignore key repeat */
        if (ev.value == 2)
            continue;

        /* button pressed */
        if (ev.value == 1) {
            press_time = time(NULL);
            syslog(LOG_INFO, "Power button pressed");
        }

        /* button released */
        if (ev.value == 0) {

            time_t duration = time(NULL) - press_time;

            if (duration >= LONG_PRESS_SEC) {

                syslog(LOG_INFO, "Long press -> shutdown");

                system("sync");
                system("poweroff");

            } else {

                syslog(LOG_INFO, "Short press -> suspend");

                system("echo 0 > /sys/class/gpio/gpio507/value");

                system("sync");

                /* flush old input events */
                flush_events(fd);

                usleep(300000);

                syslog(LOG_INFO, "Entering suspend");

                system("echo mem > /sys/power/state");

                syslog(LOG_INFO, "System resumed");

                system("echo 1 > /sys/class/gpio/gpio507/value");

                last_resume = time(NULL);

                flush_events(fd);
            }
        }
    }

    close(fd);
    closelog();
    return 0;
}