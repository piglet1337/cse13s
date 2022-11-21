#include "ll.h"
#include <stdlib.h>

uint64_t seeks = 0;
uint64_t links = 0;

struct LinkedList {
  uint32_t length;
  Node *head;
  Node *tail;
  bool mtf;
};


LinkedList *ll_create(bool mtf) {
  LinkedList *ll = (LinkedList *)malloc(sizeof(LinkedList));
  if (ll) {
    ll->mtf = mtf;
    ll->length = 0;
    ll->head = node_create(NULL, NULL);
    ll->tail = node_create(NULL, NULL);
    ll->head->next = ll->tail;
    ll->head->prev = NULL;
    ll->tail->prev = ll->head;
    ll->tail->next = NULL;
  }
  return ll;
}

void ll_delete(LinkedList **ll) {
  Node **n = &(*ll)->head;
  Node **n_next = &(*ll)->head->next;
  do {
    node_delete(n);
    n = n_next;
    n_next = &(*n)->next;
  } while (*n_next != (*ll)->tail);
  free(*ll);
  *ll = NULL;
}

uint32_t ll_length(LinkedList *ll) { return ll->length; }

int str_cmp(char *str1, char *str2) {
  for (int i = 0; str1[i] != '\0' || str2[i] != '\0'; i += 1) {
    if (str1[i] != str2[i]) {return 0;}
  }
  return 1;
}

Node *ll_lookup(LinkedList *ll, char *oldspeak) {
  seeks += 1;
  Node *n = ll->head;
  Node *n_next = ll->head->next;
  while (n_next != ll->tail) {
    if (str_cmp(n_next->oldspeak, oldspeak)) {
      if (ll->mtf) {
        n_next->prev->next = n_next->next;
        n_next->next->prev = n_next->prev;
        ll->head->next->prev = n_next;
        
        n_next->next = ll->head->next;
        n_next->prev = ll->head;
        ll->head->next = n_next;
      }
      return n_next;
    }
    n = n_next;
    n_next = n->next;
  }
  return NULL;
}

void ll_insert(LinkedList *ll, char *oldspeak, char *newspeak) {
  if (ll_lookup(ll, oldspeak)) {
    return;
  }
  Node *n = node_create(oldspeak, newspeak);
  ll->head->next->prev = n;
  n->next = ll->head->next;
  n->prev = ll->head;
  ll->head->next = n;
  ll->length += 1;
}

void ll_print(LinkedList *ll) {
  Node *n = ll->head;
  Node *n_next = ll->head->next;
  while (n_next != ll->tail) {
    node_print(n_next);
    n = n_next;
    n_next = n->next;
  }
}

void ll_stats(uint32_t *n_seeks, uint32_t *n_links) {
  *n_seeks = seeks;
  *n_links = links;
}
