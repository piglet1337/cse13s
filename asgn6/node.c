#include "node.h"
#include <stdio.h>
#include <stdlib.h>

Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = (Node *) malloc(sizeof(Node));
    if (oldspeak != NULL) {
        char *ospeak = (char*)malloc(sizeof(oldspeak));
        for (int i = 0; oldspeak[i] != '\0'; i += 1) {
            ospeak[i] = oldspeak[i];
        }
        *n->oldspeak = *ospeak;
    }
    else {n->oldspeak = NULL;}
    if (newspeak != NULL) {
        char *nspeak = (char*)malloc(sizeof(newspeak));
        for (int i = 0; newspeak[i] != '\0'; i += 1) {
            nspeak[i] = newspeak[i];
        }
        *n->newspeak = *nspeak;
    }
    else {n->newspeak = NULL;}
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
