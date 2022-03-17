#include <stdio.h>
#include <stdbool.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>

const uint IN_PINS[2] = {14, 15};
const uint OUT_PINS[2] = {0, 1};
bool last_state[2][2] = {{0, 0}, {0, 0}};

int loop() {
    bool is_changed = false;

    for(uint i = 0; i < 2; ++i) {
        gpio_put(OUT_PINS[i], 1);
        for(uint j = 0; j < 2; ++j) {
            bool state = gpio_get(IN_PINS[j]);
            is_changed = (state != last_state[i][j]) || is_changed;
            last_state[i][j] = state;
        }
        sleep_ms(250);
        gpio_put(OUT_PINS[i], 0);
    }

    if(is_changed) printf("current state:\n%i %i\n%i %i\n\n", last_state[0][0], last_state[0][1], last_state[1][0], last_state[1][1]);
}

int setup() {
    for(int in = 0; in < 2; ++in) {
        gpio_init(IN_PINS[in]);
        gpio_set_dir(IN_PINS[in], GPIO_IN);
    }
    for(int out = 0; out < 2; ++out) {
        gpio_init(OUT_PINS[out]);
        gpio_set_dir(OUT_PINS[out], GPIO_OUT);
    }
}

int main() {
    stdio_init_all();

    setup();

    while (true) {
        loop();
        sleep_ms(25);
    }

    return 0;
}

