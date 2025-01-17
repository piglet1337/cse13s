//include necessary libraries
#include "node.h"
#include <stdio.h>
#include <stdlib.h>

//creates node and returns it
Node *node_create(char *oldspeak, char *newspeak) {
  Node *n = (Node *)malloc(sizeof(Node));
  if (oldspeak != NULL) {
    int length = 0;
    for (int i = 0; oldspeak[i] != '\0'; i += 1) {
      length += 1;
    }
    n->oldspeak = (char *)calloc(length, sizeof(char));
    for (int i = 0; oldspeak[i] != '\0'; i += 1) {
      n->oldspeak[i] = oldspeak[i];
    }
  } else {n->oldspeak = NULL;}
  if (newspeak != NULL) {
    int length = 0;
    for (int i = 0; newspeak[i] != '\0'; i += 1) {
      length += 1;
    }
    n->newspeak = (char *)calloc(length, sizeof(char));
    for (int i = 0; newspeak[i] != '\0'; i += 1) {
      n->newspeak[i] = newspeak[i];
    }
  } else {n->newspeak = NULL;}
  return n;
}

//deletes not freeing any memory used
void node_delete(Node **n) {
  free((*n)->newspeak);
  free((*n)->oldspeak);
  free(*n);
  *n = NULL;
}

//prints node
void node_print(Node *n) {
  if (n == NULL) {return;}
  if (n->oldspeak && n->newspeak) {
    printf("%s -> %s\n", n->oldspeak, n->newspeak);
  } else if (n->oldspeak) {
    printf("%s\n", n->oldspeak);
  }
}
