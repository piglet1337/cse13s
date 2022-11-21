#include "node.h"
#include <stdio.h>
#include <stdlib.h>

Node *node_create(char *oldspeak, char *newspeak) {
    char *ospeak = (char*)malloc(sizeof(oldspeak));
    char *nspeak = (char*)malloc(sizeof(newspeak));
    for (int i = 0; oldspeak[i] != '\0'; i += 1) {
        ospeak[i] = oldspeak[i];
    }
    for (int i = 0; newspeak[i] != '\0'; i += 1) {
        nspeak[i] = newspeak[i];
    }
    Node *n = (Node *) malloc(sizeof(Node));
    *n->oldspeak = *ospeak;
    *n->newspeak = *nspeak;
    return n;
}

void node_delete(Node **n) {
    free((*n)->oldspeak);
    free((*n)->newspeak);
    free(*n);
    n = NULL;
}

void node_print(Node *n) {
    if (n->oldspeak && n->newspeak) {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    }
    else if (n->oldspeak) {
        printf("%s\n", n->oldspeak);
    }
}
