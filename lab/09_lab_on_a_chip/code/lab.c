#include <stdio.h>

#include "pico/stdlib.h"
#include "pio_i2c.h"
#include "apds_registers.h"
#include "neopixel.h"

#define PIN_SDA 22
#define PIN_SCL 23

bool reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

void configure_apds(PIO pio, uint sm){
    uint8_t txbuf[2] = {0};

    txbuf[0] = ATIME_REGISTER;
    txbuf[1] = (uint8_t)(0x81);
    pio_i2c_write_blocking(pio, sm, APDS_ADDRESS, txbuf, 2);

    txbuf[0] = APDS_CONTROL_ONE_REGISTER;
    txbuf[1] = APDS_CONTROL_ONE_AGAIN;
    pio_i2c_write_blocking(pio, sm, APDS_ADDRESS, txbuf, 2);

    txbuf[0] = APDS_ENABLE_REGISTER;
    txbuf[1] = APDS_ENABLE_PON | APDS_ENABLE_AEN | APDS_ENABLE_PEN;
    pio_i2c_write_blocking(pio, sm, APDS_ADDRESS, txbuf, 2);
}

void read_from_apds(PIO pio, uint sm, uint8_t reg_addr, uint8_t *rxbuf, uint num_bytes) {
    pio_i2c_write_blocking(pio, sm, APDS_ADDRESS, &reg_addr, 1);
    pio_i2c_read_blocking(pio, sm, APDS_ADDRESS, rxbuf, num_bytes);
}

int main() {
    stdio_init_all();

    PIO pio = pio1;
    uint sm = 1;
    uint offset = pio_add_program(pio, &i2c_program);
    i2c_program_init(pio, sm, offset, PIN_SDA, PIN_SCL);
    neopixel_init();

    configure_apds(pio, sm);

    while (1) {
        neopixel_set_rgb(0);
        uint8_t rx_buffer[1] = {0};
        read_from_apds(pio, sm, STATUS_REGISTER, rx_buffer, 1);
        read_from_apds(pio, sm, ID_REGISTER, rx_buffer, 1);

        uint8_t data_arr[8] = {0};
        if ((rx_buffer[0] & STATUS_REGISTER_PVALID) == STATUS_REGISTER_PVALID) {
            read_from_apds(pio, sm, PROXIMITY_DATA_REGISTER, data_arr, 1);
            printf("Proximity= %d\n", data_arr[0] - 230);
        }
        uint16_t c;
        uint16_t r;
        uint16_t g;
        uint16_t b;
        if ((rx_buffer[0] & STATUS_REGISTER_AVALID) == STATUS_REGISTER_AVALID) {
            read_from_apds(pio, sm, RGBC_DATA_REGISTER_CDATAL, data_arr, 8);
            c = (data_arr[1] << 8 | data_arr[0]);
            r = (data_arr[3] << 8 | data_arr[2]);
            g = (data_arr[5] << 8 | data_arr[4]);
            b= (data_arr[7] << 8 | data_arr[6]);
            printf("Color Data (c,r,g,b) : (%d, %d, %d, %d)\n", c, r, g, b);
        }
        uint32_t color = 0;
        color = color | ((uint8_t)((r*255.0) / 65536) << 16) | ((uint8_t)((g*255.0) / 65536) << 8) |
                (uint8_t)((b*255.0) / 65536);
        neopixel_set_rgb(color);
        sleep_ms(500);
    }
    return 0;
}