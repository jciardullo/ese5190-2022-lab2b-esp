/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// PIO logic analyser example
//
// This program captures samples from a group of pins, at a fixed rate, once a
// trigger condition is detected (level condition on one pin). The samples are
// transferred to a capture buffer using the system DMA.
//
// 1 to 32 pins can be captured, at a sample rate no greater than system clock
// frequency.

#include <stdio.h>
#include <stdlib.h>
#include "neopixel.h"
#include "pico/stdlib.h"
#include "ws2812.pio.h"
#include "adafruit_qtpy_rp2040.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/structs/bus_ctrl.h"

// Some logic to analyse:
#include "hardware/structs/pwm.h"

#define QTPY_BOOT_PIN 21

const uint CAPTURE_PIN_BASE = QTPY_BOOT_PIN;
const uint CAPTURE_PIN_COUNT = 1;
const uint CAPTURE_N_SAMPLES = 1;


int main() {
    stdio_init_all();
    gpio_init(QTPY_BOOT_PIN);
    gpio_set_dir(QTPY_BOOT_PIN, GPIO_IN);
    neopixel_init();

    PIO pio = pio0;
    int sm1 = 1;
    uint offset1 = pio_add_program(pio, &boot_status_program);
    boot_status_program_init(pio, sm1, offset1, QTPY_BOOT_PIN);

    while (!stdio_usb_connected());

    while (true) {
        char action = getchar();
        if (action == 'R') {
            printf("2");
            for (int i = 0; i < 500; i++) {
                if (pio_sm_get(pio, sm1)) { // poll every cycle, 0 = "pressed"
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
