//include necessary libraries
#include "node.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <ctype.h>

//Creates node with symbol = symbol and frequency = frequency and returns it
Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *n = (Node *) malloc(sizeof(Node));
    n->symbol = symbol;
    n->frequency = frequency;
    n->left = NULL;
    n->right = NULL;
    return n;
}

//frees memory for node and sets pointer to NULL
void node_delete(Node **n) {
    (void) free(*n);
    *n = NULL;
    n = NULL;
}

//Creates a new node with symbol set to $ and frequency set to the sum of frequencies of left and right
//Sets the nodes left = left and right = right and returns it
Node *node_join(Node *left, Node *right) {
    Node *n = node_create(36, left->frequency + right->frequency);
    n->left = left;
    n->right = right;
    return n;
}

//prints nodes symbol and frequency
void node_print(Node *n) {
    if (!iscntrl(n->symbol) && isprint(n->symbol)) {
        printf("%c ", (char) n->symbol);
    }
    else {
        printf("0x%02" PRIx8 " ", n->symbol);
    }
    printf("%" PRIu64 "\n", n->frequency);
}

//returns true if n->frequency is greater than m->frequency and false otherwise
bool node_cmp(Node *n, Node *m) {
    if (n->frequency > m->frequency) {return true;}
    return false;
}

//prints node's symbol
void node_print_sym(Node *n) {
    if (!iscntrl(n->symbol) && isprint(n->symbol)) {
        printf("%c\n", (char) n->symbol);
    }
    else {
        printf("0x%02" PRIx8 "\n", n->symbol);
    }
}
