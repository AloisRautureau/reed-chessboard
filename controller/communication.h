#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "logic.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pico/stdlib.h>

const uint16_t MAX_BUFFER_LENGTH = 32;

enum ControllerMessage {
    ACK = 0x01,
    MOVE = 0x10,
};

enum DriverMessage {
    SYN = 0x01,
    PARSED_MOVE = 0x11,
    ILLEGAL_MOVE = 0x12,
    RESET = 0xF0,
};

uint16_t read_line(char* buffer) {
    uint16_t size = 0;
    while(true) {
        int c = getchar_timeout_us(100);
        if(c != PICO_ERROR_TIMEOUT && size < MAX_BUFFER_LENGTH) {
            buffer[size++] = (c & 0xFF);
        } else { 
            break; 
        }
    }
    return size;
}


/**
 * Reads the first command found in standard input and deals with it
 * accordingly
 */
void handle_serial() {
    uint8_t command_byte = getchar_timeout_us(100);

    switch (command_byte) {
        case SYN: putchar(ACK); break;
        case PARSED_MOVE: break;
        case ILLEGAL_MOVE: break;
        case RESET: reset_state(); break;
        default: break;
    }
}

/**
 * @param move_bytes the bytes to send
 */
void send_move_bytes(uint16_t move_bytes) {
    putchar(MOVE);
    putchar(get_origin(move_bytes));
    putchar(get_destination(move_bytes));
}

#endif