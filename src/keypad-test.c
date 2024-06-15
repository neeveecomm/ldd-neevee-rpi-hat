#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <errno.h>
#include <string.h>

// Define the key names for the 4x4 keypad
const char *key_names[] = {
    "KEY_1", "KEY_2", "KEY_3", "KEY_A",
    "KEY_4", "KEY_5", "KEY_6", "KEY_B",
    "KEY_7", "KEY_8", "KEY_9", "KEY_C",
    "KEY_KPASTERISK", "KEY_0", "KEY_KP0", "KEY_KPDOT"
};

const int key_codes[] = {
    KEY_1, KEY_2, KEY_3, KEY_A,
    KEY_4, KEY_5, KEY_6, KEY_B,
    KEY_7, KEY_8, KEY_9, KEY_C,
    KEY_KPASTERISK, KEY_0, KEY_KP0, KEY_KPDOT
};

int main(void) {
    const char *device = "/dev/input/event3";
    int fd;
    struct input_event ev;
    ssize_t n;

    // Open the input device
    fd = open(device, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Failed to open event device %s: %s\n", device, strerror(errno));
        return EXIT_FAILURE;
    }

    while (1) {
        // Read an input event
        n = read(fd, &ev, sizeof(ev));
        if (n == (ssize_t)-1) {
            if (errno == EINTR) {
                continue;
            } else {
                perror("Error reading input event");
                close(fd);
                return EXIT_FAILURE;
            }
        } else if (n != sizeof(ev)) {
            fprintf(stderr, "Unexpected read size: %zd\n", n);
            close(fd);
            return EXIT_FAILURE;
        }

        // Check if the event is a key event
        if (ev.type == EV_KEY) {
            // Find the key name based on the key code
            const char *key_name = NULL;
            for (size_t i = 0; i < sizeof(key_codes)/sizeof(key_codes[0]); ++i) {
                if (ev.code == key_codes[i]) {
                    key_name = key_names[i];
                    break;
                }
            }

	    unsigned char ver = ev.code;
	    printf("The data is %c\n", ver);
            /*if (key_name) {
                printf("Key %s %s\n", key_name, ev.value ? "pressed" : "released");
            } else {
                printf("Unknown key code: 00x%x\n", ev.code);
            }*/
        }
    }

    // Close the device
    close(fd);
    return EXIT_SUCCESS;
}

