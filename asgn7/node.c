#include "node.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <ctype.h>

Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *n = (Node *) malloc(sizeof(Node));
    n->symbol = symbol;
    n->frequency = frequency;
    return n;
}

void node_delete(Node **n) {
    (void) free(*n);
    *n = NULL;
    n = NULL;
}

Node *node_join(Node *left, Node *right) {
    Node *n = node_create(36, left->frequency + right->frequency);
    n->left = left;
    n->right = right;
    return n;
}

void node_print(Node *n) {
    if (!iscntrl(n->symbol) && isprint(n->symbol)) {
        printf("%c ", (char) n->symbol);
    }
    else {
        printf("0x%02" PRIx8 " ", n->symbol);
    }
    printf("%" PRIu64 "\n", n->frequency);
}

bool node_cmp(Node *n, Node *m) {
    if (n->frequency > m->frequency) {return true;}
    return false;
}

void node_print_sym(Node *n) {
    if (!iscntrl(n->symbol) && isprint(n->symbol)) {
        printf("%c\n", (char) n->symbol);
    }
    else {
        printf("0x%02" PRIx8 "\n", n->symbol);
    }
}
