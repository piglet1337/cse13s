//include libraries
#include "io.h"
#include "defines.h"
#include <unistd.h>

//initiliaze variables
uint64_t bytes_read = 0;
uint64_t bytes_written = 0;
static uint8_t read_buffer[BLOCK];
static uint16_t read_index = 0;
static int read_num_bytes;
static uint8_t code_buffer[BLOCK];
static uint16_t code_index = 0;

//read nbytes from infile into buf and return the number of bytes read
int read_bytes(int infile, uint8_t *buf, int nbytes) {
    int total = 0;
    while (nbytes != total) {
        int result = read(infile, buf, nbytes-total);
        total += result;
        if (result == 0) {break;}
    }
    bytes_read += total;
    return total;
}

//writes nbytes from buf into outfile
int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    int total = 0;
    while (nbytes != total) {
        int result = write(outfile, buf, nbytes-total);
        total += result;
        if (result == 0) {break;}
    }
    bytes_written += total;
    return total;
}

//reads next bit from infile and passes it through bit
bool read_bit(int infile, uint8_t *bit) {
    if (read_index == 0) {
        read_num_bytes = read_bytes(infile, read_buffer, BLOCK);
        if (read_num_bytes == 0) {return false;}
    }
    *bit = ((read_buffer[read_index/8]) & (1UL << read_index%8)) >> read_index%8;
    read_index += 1;
    if (read_index/8 == read_num_bytes) {
        read_index = 0;
    }
    return true;
}

//adds c to the buffer and writes to outfile when the buffer fills up
void write_code(int outfile, Code *c) {
    if (code_index == 0) {
        for (int i = 0; i < BLOCK; i += 1) {
            code_buffer[i] = 0;
        }
    }
    for (uint32_t i = 0; i < code_size(c); i += 1) {
        if (code_get_bit(c, i)) {
            code_buffer[code_index/8] |= 1UL << code_index%8;
        }
        code_index += 1;
        if (code_index == BLOCK*8) {
            write_bytes(outfile, code_buffer, BLOCK);
            code_index = 0;
            for (int j = 0; j < BLOCK; j += 1) {
                code_buffer[i] = 0;
            }
        }
    }
}

//writes buffer to outfile
void flush_codes(int outfile) {
    write_bytes(outfile, code_buffer, (code_index/8) + 1);
}
