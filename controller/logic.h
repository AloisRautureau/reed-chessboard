#ifndef LOGIC_H
#define LOGIC_H

#include "bitboard.h"
#include "hardware/gpio.h"
#include "move.h"
#include "read.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const uint MOVE_VALIDATION_PIN = 16;
static bool move_validated = false;

static uint64_t last_state = EMPTY;
static uint64_t intermediate_states[2048] = {};
static uint16_t intermediate_states_count = 0;

static uint64_t diagonal_step_mask[64] = {};
static uint64_t castling_mask = 0x5400000000000054;

/**
 * Records a new intermediate state if it's different from the previous one
 * and there is room for it
 */
void record_state(uint64_t state) {
    if(intermediate_states_count < 2048 && intermediate_states[intermediate_states_count - 1] != state) {
        intermediate_states[intermediate_states_count] = state;
        ++intermediate_states_count;
    }
}

/**
 * Resets the current state to mirror the actual board after a move
 */
void reset_state() {
    intermediate_states_count = 0;
    last_state = read_board();
}

/**
 * Produces a heatmap of changes in intermediate states for each square
 */
uint32_t* intermediate_states_heatmap() {
    size_t heatmap_size = 64 * sizeof(uint32_t);
    uint32_t* heatmap = (uint32_t*) malloc(heatmap_size);
    memset(heatmap, 0, heatmap_size);
    for(int i = 1; i < intermediate_states_count; ++i) {
        // We assume that only one square changes between each state.
        // This might not be absolutely true tbh, but as long as no bugs
        // rise up because of this, let's not hit performance trying to
        // do a serialization.
        uint64_t delta = intermediate_states[i] ^ intermediate_states[i - 1];
        heatmap[bitscan_forward(delta)] += 1;
    }
    return heatmap;
}

/**
 * @param excluded an index of the heatmap to exclude from the search
 * @return the square which has changed state the most during intermediate states
 */
uint8_t most_changed_square(uint8_t excluded) {
    uint32_t* heatmap = intermediate_states_heatmap();
    uint8_t max_delta = 0, most_changed_square = 0;
    for(int i = 0; i < 64; ++i) {
        if(heatmap[i] > max_delta && i != excluded) {
            most_changed_square = i;
            max_delta = heatmap[i];
        }
    }
    return most_changed_square;
    free(heatmap);
}


/**
 * Deduces the last played move given the information
 * fetched between the last and current state.
 */
move get_move() {
    uint64_t current_state = read_board();
    uint64_t changed_squares = current_state ^ last_state;
    uint8_t distance = hamming_distance(current_state, last_state);

    // We can (mostly) deduce what move was played using Hamming Distance
    // between current and last state.
    move m = INVALID_MOVE;
    switch (distance) {
        case 1: { // Capture
            // This is a special case: the origin square is the only one which state's changes.
            // To find the destination square, we assume that it will have its state changed more than
            // any other (remove the captured piece, place the new one) excluding the origin just in case.
            uint8_t from = bitscan_forward(changed_squares);
            m = encode(
                from,
                most_changed_square(from)
            );
            break;
        };
        case 2: { // Simple push
            m = encode(
                bitscan_forward(last_state & changed_squares),
                bitscan_forward(current_state & changed_squares)
            );
            break;
        }
        case 3: { // En passant capture
            uint8_t destination_sq = bitscan_forward(current_state & changed_squares);
            m = encode(
                bitscan_forward(last_state & changed_squares & diagonal_step_mask[destination_sq]),
                destination_sq
            );
            break;
        }
        case 4: { // Castling
            m = encode(
                bitscan_forward(last_state & changed_squares & castling_mask),
                bitscan_forward(current_state & changed_squares & castling_mask)
            );
            break;
        };
    }
    return m;
}


move check_for_move() {
    move m = INVALID_MOVE;
    if(gpio_get(MOVE_VALIDATION_PIN)) {
        m = get_move();
        reset_state();
    }
    else {
        record_state(read_board());
    }
    return m;
}


// ---------------
// SETUP FUNCTIONS
// ---------------

/**
 * Calculates the diagonal steps mask, used to check for en passant moves
 * TODO: This is disgusting, might want to clean it up later
 */
void diagonal_step_mask_calculation() {
    uint64_t outer_mask = 0xff818181818181ff;
    uint64_t valid_mask = 0x7effc3c3c3c3ff7e;
    int8_t deltas[4] = {7, 9, -7, -9};

    for(uint8_t from_index = 0; from_index < 64; ++from_index) {
        diagonal_step_mask[from_index] = EMPTY;
        for(int i = 0; i < 4; ++i) {
            int8_t delta = deltas[i];
            uint8_t current_index = from_index + delta;
            if(SINGLE << from_index & outer_mask) {
                if(SINGLE << current_index & valid_mask) {
                    diagonal_step_mask[from_index] |= (SINGLE << current_index);
                }
            }
            else {
                diagonal_step_mask[from_index] |= (SINGLE << current_index);
            }
        }
    }
}

/**
 * Sets up any calculations we might need to do before using
 * the move finding logic
 */
void logic_setup() {
    gpio_init(MOVE_VALIDATION_PIN);
    gpio_set_dir(MOVE_VALIDATION_PIN, GPIO_IN);
    reset_state();
    diagonal_step_mask_calculation();
}


#endif