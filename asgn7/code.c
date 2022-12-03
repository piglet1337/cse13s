//includes
#include "code.h"
#include <stdio.h>
#include <inttypes.h>

//initializes a Code setting top = 0 and zeroing out the bits array then returns it
Code code_init(void) {
    Code c;
    c.top = 0;
    for (int i = 0; i < MAX_CODE_SIZE; i += 1) {
        c.bits[i] = 0;
    }
    return c;
}

//returns size of c
uint32_t code_size(Code *c) {
    return c->top;
}

//if c is empty returns true, otherwise returns false
bool code_empty(Code *c) {
    if (c->top == 0) {return true;}
    return false;
}

//if c is full returns true, otherwise returns false
bool code_full(Code *c) {
    if (c->top == ALPHABET) {return true;}
    return false;
}

//sets bit number i to 1 in c
bool code_set_bit(Code *c, uint32_t i) {
    if (i > ALPHABET-1) {return false;}
    c->bits[i/8] |= 1UL << i%8;
    return true;
}

//sets bit number i to 0 in c
bool code_clr_bit(Code *c, uint32_t i) {
    if (i > ALPHABET-1) {return false;}
    c->bits[i/8] &= ~(1UL << i%8);
    return true;
}

//returns true if bit number i is 1, and false otherwise
bool code_get_bit(Code *c, uint32_t i) {
    if (i > ALPHABET-1) {return false;}
    if ((c->bits[i/8] & (1UL << i%8)) >> (i%8)) {return true;}
    return false;
}

//pushes a bit = bit onto c
bool code_push_bit(Code *c, uint8_t bit) {
    if (code_full(c)) {return false;}
    if (bit) {code_set_bit(c, c->top);}
    else {code_clr_bit(c, c->top);}
    c->top += 1;
    return true;
}

//pops a bit from c passing it through bit
bool code_pop_bit(Code *c, uint8_t *bit) {
    if (code_empty(c)) {return false;}
    c->top -= 1;
    if (code_get_bit(c, c->top)) {*bit = 1;}
    else {*bit = 0;}
    code_clr_bit(c, c->top);
    return true;
}

//prints out c->bits in hex
void code_print(Code *c) {
    for (int i = 0; i < MAX_CODE_SIZE; i += 1) {
        printf("%" PRIx8, c->bits[i]);
    }
}
