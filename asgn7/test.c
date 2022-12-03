#include <stdio.h>
#include "node.h"
#include "pq.h"
#include "code.h"
#include <inttypes.h>

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

    //code tests
    Code c = code_init();
    code_push_bit(&c, 0);
    code_push_bit(&c, 0);
    code_push_bit(&c, 1);
    uint8_t bit;
    code_pop_bit(&c, &bit);
    if (!bit) {printf("code fail\n");}
    code_pop_bit(&c, &bit);
    if (bit) {printf("code fail\n");}
    code_pop_bit(&c, &bit);
    if (bit) {printf("code fail\n");}
    if (code_pop_bit(&c, &bit)) {printf("code fail\n");}
    for (int i = 0; i < ALPHABET; i += 1) {
        if (!code_push_bit(&c, 1)) {printf("code fail\n");}
    }
    if (code_push_bit(&c, 1)) {printf("code fail\n");}
    return 0;
}
