#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pico/stdlib.h>

const uint16_t MAX_BUFFER_LENGTH = 32;

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
 * Reads the last commands sent and deals with them
 * accordingly
 */
void handle_serial() {
    char* command;
    int command_length = read_line(command);

    if(strcmp(command, "rcdprobe")) {
        printf("rccok");
    }
}

#endif