#include "logic.h"
#include "move.h"
#include "pico/time.h"
#include "read.h"
#include <stdio.h>
#include <stdbool.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>

int main() {
    stdio_init_all();

    read_setup();
    logic_setup();

    while(true) {
        move played = wait_for_move();
        printf("played: %i %i\n", get_origin(played), get_destination(played));
        printf("state: %llu\n", last_state);

        // Wait for the validation pin to be low
        while(gpio_get(MOVE_VALIDATION_PIN)) { sleep_ms(25); }
    }
    
    return 0;
}

