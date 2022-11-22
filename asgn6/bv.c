//include necessary libraries
#include "bv.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

//define structure of BitVector
struct BitVector {
  uint32_t length;
  uint64_t *vector;
};

//creates a bitvector and returns it
BitVector *bv_create(uint32_t length) {
  BitVector *bv = (BitVector *)malloc(sizeof(BitVector));
  if (bv) {
    bv->length = length;
    bv->vector = (uint64_t *)calloc((length / 64) + 1, sizeof(uint64_t));
    for (uint32_t i = 0; i < (length / 64 + 1); i += 1) {
      bv->vector[i] = 0;
    }
    if (!bv->vector) {
      free(bv);
      bv = NULL;
    }
  }
  return bv;
}

//deletes a bitvector freeing any used memory
void bv_delete(BitVector **bv) {
  free(*bv);
  *bv = NULL;
}

//returns length of bitvector
uint32_t bv_length(BitVector *bv) { return bv->length; }

//sets specific bit in bitvector to 1
void bv_set_bit(BitVector *bv, uint32_t i) {
  bv->vector[i / 64] |= (1UL << (i % 64));
}

//sets specific bit in bitvector to 0
void bv_clr_bit(BitVector *bv, uint32_t i) {
  bv->vector[i / 64] &= ~(1UL << (i % 64));
}

//returns specific bit in bitvector
uint8_t bv_get_bit(BitVector *bv, uint32_t i) {
  return (bv->vector[i / 64] & (1UL << (i % 64))) >> (i % 64);
}

//prints bitvector
void bv_print(BitVector *bv) {
  for (uint32_t i = 0; i < bv_length(bv); i += 1) {
    printf("%" PRIu8 " ", bv_get_bit(bv, i));
  }
}
