#include <stdio.h>
#include "node.h"

int main () {
    //node tests
    Node *left = node_create(1, 414);
    Node *right = node_create(56, 418);
    Node *n = node_join(left, right);
    if (n->symbol != 36 || n->frequency != 832 || node_cmp(left, right)) {
        printf("node test failed\n");
    }
    node_delete(&n);
    node_delete(&left);
    node_delete(&right);

    return 0;
}
