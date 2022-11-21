#include "ht.h"
#include "city.h"

struct HashTable {
  uint64_t salt;
  uint32_t size;
  uint32_t n_keys;
  uint32_t n_hits;
  uint32_t n_misses;
  uint32_t n_examined;
  bool mtf;
  LinkedList **lists;
};

HashTable *ht_create(uint32_t size, bool mtf) {
  HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
  if (ht != NULL) {
    ht->mtf = mtf;
    ht->salt = 0x9846e4f157fe8840;
    ht->n_hits = ht->n_misses = ht->n_examined = 0;
    ht->n_keys = 0;
    ht->size = size;
    ht->lists = (LinkedList **)calloc(size, sizeof(LinkedList *));
    if (!ht->lists) {
      free(ht);
      ht = NULL;
    }
  }
  return ht;
}

void ht_delete(HashTable **ht) {
  for (uint32_t i = 0; i < ht_size(*ht); i += 1) {
    if ((*ht)->lists[i]) {
      ll_delete(&(*ht)->lists[i]);
    }
  }
  free((*ht)->lists);
  free(*ht);
  *ht = NULL;
}

uint32_t ht_size(HashTable *ht) { return ht->size; }

Node *ht_lookup(HashTable *ht, char *oldspeak) {
  uint32_t n_links;
  uint32_t n_seeks;
  ll_stats(&n_seeks, &n_links);
  uint64_t hash_result = hash(ht->salt, oldspeak) % ht_size(ht);
  Node *ht_node = NULL;
  if (ht->lists[hash_result]) {
    ht_node = ll_lookup(ht->lists[hash_result], oldspeak);
    ht->n_hits += 1;
  } else {ht->n_misses += 1;}
  uint32_t new_links;
  ll_stats(&n_seeks, &new_links);
  ht->n_examined += new_links - n_links;
  return ht_node;
}

void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
  uint64_t hash_result = hash(ht->salt, oldspeak) % ht_size(ht);
  if (!ht->lists[hash_result]) {
    ht->lists[hash_result] = ll_create(ht->mtf);
  }
  uint32_t temp = ll_length(ht->lists[hash_result]);
  ll_insert(ht->lists[hash_result], oldspeak, newspeak);
  if (temp - ll_length(ht->lists[hash_result]) != 0) {
    ht->n_keys += 1;
  }
}

uint32_t ht_count(HashTable *ht) {
  uint32_t count = 0;
  for (uint32_t i = 0; i < ht->size; i += 1) {
    if (ht->lists[i]) {
      count += 1;
    }
  }
  return count;
}

void ht_print(HashTable *ht) {
  for (uint32_t i = 0; i < ht->size; i += 1) {
    if (ht->lists[i]) {
      ll_print(ht->lists[i]);
    }
  }
}

#include <stdio.h>
#include <inttypes.h>
void ht_stats(HashTable *ht, uint32_t *nk, uint32_t *nh, uint32_t *nm,
              uint32_t *ne) {
  *nk = ht->n_keys;
  *nh = ht->n_hits;
  *nm = ht->n_misses;
  *ne = ht->n_examined;
}
