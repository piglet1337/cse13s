#include <stdio.h>
#include "node.h"
#include "pq.h"

int main () {
    //node tests
    Node *left = node_create(130, 414);
    Node *right = node_create(56, 418);
    Node *n = node_join(left, right);
    if (n->symbol != 36 || n->frequency != 832 || node_cmp(left, right)) {
        printf("node test failed\n");
    }

    //priority queue tests
    PriorityQueue *pq = pq_create(3);
    enqueue(pq, right);
    enqueue(pq, left);
    enqueue(pq, n);
    Node *fail = node_create(16, 72);
    if (enqueue(pq, fail)) {printf("enqueue fail");}
    node_delete(&fail);
    Node *l;
    dequeue(pq, &l);
    if (l != left) {printf("dequeue fail");}
    dequeue(pq, &l);
    if (l != right) {printf("dequeue fail");}
    dequeue(pq, &l);
    if (l != n) {printf("dequeue fail");}
    if (dequeue(pq, &l)) {printf("dequeue fail");}
    pq_print(pq);
    pq_delete(&pq);
    node_delete(&n);
    node_delete(&left);
    node_delete(&right);

    return 0;
}
