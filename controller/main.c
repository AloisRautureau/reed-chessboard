#include "read.h"
#include <stdio.h>
#include <stdbool.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>

static bitboard last_state = 0;

int main() {
    stdio_init_all();

    read_setup();

    while (true) {
        bitboard state = read_board();
        if(state != last_state) {
            last_state = state;
            printf("current state = %llu\n", state);
        }
        sleep_ms(25);
    }

    return 0;
}

