#include <stdio.h>

#include "ll.h"
#include "node.h"

int main() {
    LinkedList *ll = ll_create(false);
    // LinkedList **p = &ll;

    ll_insert(ll, "hello", "world");
    ll_insert(ll, "world", "hello");
    ll_insert(ll, "thigny", "thang");
    ll_insert(ll, "world", "hello");
    ll_print(ll);
    ll_lookup(ll, "hello");
    ll_lookup(ll, "hello");
    // node_print(n);
    printf("\n");
    ll_print(ll);
    return 0;
}

