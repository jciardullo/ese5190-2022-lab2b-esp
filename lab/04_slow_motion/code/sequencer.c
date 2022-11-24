#include "pico/stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include "neopixel.h"
#include "hardware/gpio.h"

#define QTPY_BOOT_PIN 21

int main() {

    stdio_init_all();
    gpio_init(QTPY_BOOT_PIN);
    gpio_set_dir(QTPY_BOOT_PIN, GPIO_IN);
    neopixel_init();

    while (!stdio_usb_connected());

    while (true) {
        char action = getchar();
        if (action == 'R') {
            printf("2");
            for (int i = 0; i < 500; i++) {
                if (gpio_get(QTPY_BOOT_PIN)) { // poll every cycle, 0 = "pressed"
                    neopixel_set_rgb(0x00000000);
                    printf("0");
                } else {
                    neopixel_set_rgb(0x00ff0000);
                    printf("1");
                }
                sleep_ms(10);
            }
            neopixel_set_rgb(0x00000000);
            printf("3");
        } else if (action == 'P' || action == 'L' ) {
            char type = getchar();
            int factor = getchar() - '0';
            printf("2");
            if (action == 'P') {
                for (int i = 0; i < 500; i++) {
                    char next_byte = getchar();
                    if (next_byte == '0') { // poll every cycle, 0 = "pressed"
                        neopixel_set_rgb(0x00000000);
                    } else {
                        neopixel_set_rgb(0x00ff0000);
                    }
                    if (type == 'F') {
                        sleep_ms((int) (10.0 / factor));
                    } else {
                        sleep_ms(10 * factor);
                    }
                }
                neopixel_set_rgb(0x00000000);
                printf("3");
            } else {
                while (1) {
                    for (int i = 0; i < 500; i++) {
                        char next_byte = getchar();
                        if (next_byte == '0') { // poll every cycle, 0 = "pressed"
                            neopixel_set_rgb(0x00000000);
                        } else {
                            neopixel_set_rgb(0x00ff0000);
                        }
                        if (type == 'F') {
                            sleep_ms((int) (10.0 / factor));
                        } else {
                            sleep_ms(10 * factor);
                        }
                    }
                    printf("3");
                }
            }
        }
    }
    return 0;
}                  
