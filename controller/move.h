#ifndef MOVE_H
#define MOVE_H

#include <stdint.h>

typedef uint8_t move;

/**
 * @return an encoded move as origin-destination square
 */
move encode(uint8_t from, uint8_t to) {
    return (from << 8) | to;
}
/**
 * @return the origin square of a move
 */
uint8_t get_origin(move m) { return m >> 8; }
/**
 * @return the destination square of a move
 */
uint8_t get_destination(move m) { return m & 0b11111111; }

#endif