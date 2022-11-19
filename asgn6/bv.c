#include "bv.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

struct BitVector {
    uint32_t length;
    uint64_t *vector;
};

BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    if (bv) {
        bv->length = length;
        bv->vector = (uint64_t *) calloc((length/64)+1, sizeof(uint64_t));
        for (uint32_t i = 0; i < (length/64+1); i += 1) {
            bv->vector[i] = 0;
        }
        if (!bv->vector) {
            free(bv);
            bv = NULL;
        }
    }
    return bv;
}

void bv_delete(BitVector **bv) {
    free(*bv);
    bv = NULL;
}

uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

void bv_set_bit(BitVector *bv, uint32_t i) {
    bv->vector[i/64] |= (1U << (i%64));
}

void bv_clr_bit(BitVector *bv, uint32_t i) {
    bv->vector[i/64] &= ~(1U << (i%64));
}

uint8_t bv_get_bit(BitVector *bv, uint32_t i) {
    return (bv->vector[i/64] & (1U << (i%64))) >> (i%64);
}

void bv_print(BitVector *bv) {
    for (uint32_t i = 0; i < bv_length(bv); i += 1) {
        printf("%" PRIu8 " ", bv_get_bit(bv, i));
    }
}
