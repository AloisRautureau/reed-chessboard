#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdint.h>

typedef uint64_t bitboard;

const bitboard EMPTY = 0b0;
const bitboard SINGLE = 0b1;
const bitboard UNIVERSE = UINT64_MAX;

bitboard square_bitmask(int row, int column) {
    return SINGLE << (8*row + column);
}

#endif