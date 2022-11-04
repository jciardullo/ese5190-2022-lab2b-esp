
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "registers.h"

void request_input(const char* prompt, char* buffer, int len) {
    printf("%s", prompt);
    char next = getchar();
    int i = 0;
    while (next != '\r' && next != '\n') {
        if ((next == 127 || next == 8) && i > 0) {
            i--;
            printf("\x1b[1D");
            printf("\x1b[0K");
        } else if (next != 127 && next != 8 && (i < len - 1)) {
            buffer[i] = next;
            putchar(next);
            i++;
        }
        next = getchar();
    }
    buffer[i] = '\0';
    putchar('\n');
}

int main() {

    stdio_init_all();
    while (!stdio_usb_connected());

    while (1) {
        char action[2] = {0};
        request_input("Read or Write? (Enter R or W): ", action, 2);

        char location[9] = {0};
        request_input("Address? (Write an 8 digit hex value that is a multiple of 4): 0x", location, 9);

        ADDRESS addr = (ADDRESS) strtol(location, NULL, 16);

        if (action[0] == 'W') {
            char value[9] = {0};
            request_input("Value at address? (Write an 8 digit hex value): 0x", value, 9);
            VALUE val = (VALUE) strtol(value, NULL, 16);
            register_write(addr, val);
        }

        printf("Memory address: 0x%p\n", addr);
        printf("Value at address:  0x%08x\n", register_read(addr));

        sleep_ms(1000);
    }

    return 0;
}                  
