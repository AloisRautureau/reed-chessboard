#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdint.h>

typedef uint64_t bitboard;

const bitboard EMPTY = 0b0;
const bitboard SINGLE = 0b1;
const bitboard UNIVERSE = UINT64_MAX;

/**
 * @return the bitmask for a square given its row and column
 */
bitboard square_bitmask(int row, int column) {
    return SINGLE << (8*row + column);
}

/**
 * @return the Hamming Distance between two bitboards
 */
uint8_t hamming_distance(bitboard b1, bitboard b2) {
    return __builtin_popcount(b1 ^ b2);
}

const int deBruijnIndex64[64] = {
    0,  1, 48,  2, 57, 49, 28,  3,
   61, 58, 50, 42, 38, 29, 17,  4,
   62, 55, 59, 36, 53, 51, 43, 22,
   45, 39, 33, 30, 24, 18, 12,  5,
   63, 47, 56, 27, 60, 41, 37, 16,
   54, 35, 52, 21, 44, 32, 23, 11,
   46, 26, 40, 15, 34, 20, 31, 10,
   25, 14, 19,  9, 13,  8,  7,  6
};
const uint64_t debruijn64 = 0x03f79d71b4cb0a89;

/**
 * bitScanForward
 * @author Martin Läuter (1997)
 *         Charles E. Leiserson
 *         Harald Prokop
 *         Keith H. Randall
 * "Using de Bruijn Sequences to Index a 1 in a Computer Word"
 * @param b bitboard to scan
 * @precondition b != 0
 * @return index (0..63) of least significant one bit
 */
uint8_t bitscan_forward(bitboard b) {
    return deBruijnIndex64[((b & -b) * debruijn64) >> 58];
}

#endif