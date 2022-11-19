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
    LinkedList *ll = (LinkedList *) malloc(sizeof(LinkedList));
    if (ll) {
        ll->mtf = mtf;
        ll->length = 2;
        ll->head = node_create(NULL, NULL);
        ll->tail = node_create(NULL, NULL);
        ll->head->next = ll->tail;
        ll->tail->prev = ll->head;
    }
    return ll;
}

void ll_delete(LinkedList **ll) {
    Node *n = ll[0]->head;
    Node *n_next = ll[0]->head->next;
    while (n_next) {
        free(n);
        n = n_next;
        n_next = n->next;
    }
    free(*ll);
    ll = NULL;
}

uint32_t ll_length(LinkedList *ll) {
    return ll->length;
}

Node *ll_lookup(LinkedList *ll, char *oldspeak) {
    seeks += 1;
    Node *n = ll->head;
    Node *n_next = ll->head->next;
    while (n_next) {
        links += 1;
        if (n->oldspeak == oldspeak) {
            if (ll->mtf) {
                n->prev->next = n->next;
                n->next->prev = n->prev;
                n->next = ll->head->next;
                ll->head->next = n;
                n->prev = ll->head;
            }
            return n;
        }
        n = n_next;
        n_next = n->next;
    }
    return NULL;
}

void ll_insert(LinkedList *ll, char *oldspeak, char *newspeak) {
    if (ll_lookup(ll, oldspeak)) {return;}
    Node *n = node_create(oldspeak, newspeak);
    n->next = ll->head->next;
    n->prev = ll->head;
    ll->head->next = n;
    ll->length += 1;
}

void ll_print(LinkedList *ll) {
    Node *n = ll->head;
    Node *n_next = ll->head->next;
    while (n_next) {
        node_print(n);
        n = n_next;
        n_next = n->next;
    }
}

void ll_stats(uint32_t *n_seeks, uint32_t *n_links) {
    *n_seeks = seeks;
    *n_links = links;
}
