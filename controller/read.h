#ifndef READ_H
#define READ_H

#include "bitboard.h"
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <stdint.h>

// Both boan be used as input or output depending on your design
// choices, it shouldn't matter too much
const uint8_t ROW_PINS[8] = {0, 1, 2, 3, 4, 5, 6, 7};
const uint8_t COLUMN_PINS[8] = {8, 9, 10, 11, 12, 13, 14, 15};

/**
 * Sets up the pins, must be called before any attempt to read the board
 * (otherwise it will just fail miserably, duh)
 */
void read_setup() {
    for(uint8_t i = 0; i < 8; ++i) {
        gpio_init(ROW_PINS[i]);
        gpio_set_dir(ROW_PINS[i], GPIO_OUT);

        gpio_init(COLUMN_PINS[i]);
        gpio_set_dir(COLUMN_PINS[i], GPIO_IN);
    }
}

/**
 * @return a bitboard representation of the board (1 for occupancy, 0 for empty)
 */
bitboard read_board() {
    bitboard state = EMPTY;

    for(uint8_t i = 0; i < 8; ++i) {
        gpio_put(ROW_PINS[i], 1);
        for(uint8_t j = 0; j < 8; ++j) {
            if(gpio_get(COLUMN_PINS[j])) { state |= square_bitmask(i, j); }
        }
        gpio_put(ROW_PINS[i], 0);
    }

    return state;
}

#endif