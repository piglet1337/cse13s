//include necassary libraries
#include "bf.h"
#include "city.h"

//define constant N_HASHES
#define N_HASHES 5

//define structure for BloomFiler
struct BloomFilter {
  uint64_t salts[N_HASHES];
  uint32_t n_keys;
  uint32_t n_hits;
  uint32_t n_misses;
  uint32_t n_bits_examined;
  BitVector *filter;
};

//define default salts
static uint64_t default_salts[] = {0x5adf08ae86d36f21, 0x419d292ea2ffd49e,
                                   0x50d8bb08de3818df, 0x272347aea4045dd5,
                                   0x7c8e16f768811a21};

//creates bloom filter
BloomFilter *bf_create(uint32_t size) {
  BloomFilter *bf = (BloomFilter *)malloc(sizeof(BloomFilter));
  if (bf) {
    bf->n_keys = bf->n_hits = 0;
    bf->n_misses = bf->n_bits_examined = 0;
    for (int i = 0; i < N_HASHES; i++) {
      bf->salts[i] = default_salts[i];
    }
    bf->filter = bv_create(size);
    if (bf->filter == NULL) {
      free(bf);
      bf = NULL;
    }
  }
  return bf;
}

//deletes bloom filter freeing memory
void bf_delete(BloomFilter **bf) {
  free(*bf);
  bf = NULL;
}

//returns size of bloom filter
uint32_t bf_size(BloomFilter *bf) {
  return bv_length(bf->filter);
}

//inserts oldspeak into bloom filter
void bf_insert(BloomFilter *bf, char *oldspeak) {
  bf->n_keys += 1;
  for (int i = 0; i < N_HASHES; i += 1) {
    bv_set_bit(bf->filter, hash(bf->salts[i], oldspeak) % bf_size(bf));
  }
}

//checks if oldspeak is in bloom filter returning true if it is
bool bf_probe(BloomFilter *bf, char *oldspeak) {
  for (int i = 0; i < N_HASHES; i += 1) {
    bf->n_bits_examined += 1;
    if (!bv_get_bit(bf->filter, hash(bf->salts[i], oldspeak) % bf_size(bf))) {
      bf->n_misses += 1;
      return false;
    }
  }
  bf->n_hits += 1;
  return true;
}

//counts how many bits have been set in the bitvector
uint32_t bf_count(BloomFilter *bf) {
  uint32_t count = 0;
  for (uint32_t i = 0; i < bv_length(bf->filter); i += 1) {
    if (bv_get_bit(bf->filter, i)) {
      count += 1;
    }
  }
  return count;
}

//prints bloom filter
void bf_print(BloomFilter *bf) { bv_print(bf->filter); }

//sets pointers equal to stats tracted in the bloom filter
void bf_stats(BloomFilter *bf, uint32_t *nk, uint32_t *nh, uint32_t *nm,
              uint32_t *ne) {
  *nk = bf->n_keys;
  *nh = bf->n_hits;
  *nm = bf->n_misses;
  *ne = bf->n_bits_examined;
}
