//include libraries
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "node.h"
#include "pq.h"
#include "code.h"
#include "io.h"
#include "stack.h"

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
    if (pq_size(pq) != 0) {printf("pq_size fail");}
    enqueue(pq, right);
    if (pq_size(pq) != 1) {printf("pq_size fail");}
    enqueue(pq, left);
    if (pq_size(pq) != 2) {printf("pq_size fail");}
    enqueue(pq, n);
    if (pq_size(pq) != 3) {printf("pq_size fail");}
    Node *fail = node_create(16, 72);
    if (enqueue(pq, fail)) {printf("enqueue fail");}
    Node *l;
    dequeue(pq, &l);
    if (l != left) {printf("dequeue fail");}
    if (pq_size(pq) != 2) {printf("pq_size fail");}
    dequeue(pq, &l);
    if (l != right) {printf("dequeue fail");}
    if (pq_size(pq) != 1) {printf("pq_size fail");}
    dequeue(pq, &l);
    if (l != n) {printf("dequeue fail");}
    if (pq_size(pq) != 0) {printf("pq_size fail");}
    if (dequeue(pq, &l)) {printf("dequeue fail");}
    if (pq_size(pq) != 0) {printf("pq_size fail");}
    pq_print(pq);
    pq_delete(&pq);

    //stack tests
    Stack *s = stack_create(2);
    if (stack_size(s) != 0) {printf("stack_size fail");}
    stack_push(s, n);
    if (stack_size(s) != 1) {printf("stack_size fail");}
    stack_push(s, left);
    if (stack_size(s) != 2) {printf("stack_size fail");}
    if (stack_push(s, fail)) {printf("stack_push fail");}
    stack_pop(s, &l);
    if (l != left) {printf("stack_pop fail");}
    if (stack_size(s) != 1) {printf("stack_size fail");}
    stack_push(s, right);
    if (stack_size(s) != 2) {printf("stack_size fail");}
    stack_pop(s, &l);
    if (l != right) {printf("stack_pop fail");}
    if (stack_size(s) != 1) {printf("stack_size fail");}
    stack_pop(s, &l);
    if (l != n) {printf("stack_pop fail");}
    if (stack_size(s) != 0) {printf("stack_size fail");}
    if (stack_pop(s, &l)) {printf("stack_pop fail");}
    stack_delete(&s);
    node_delete(&n);
    node_delete(&left);
    node_delete(&right);
    node_delete(&fail);
    //code tests
    Code c = code_init();
    if (code_size(&c) != 0) {printf("code_size fail\n");}
    code_push_bit(&c, 0);
    if (code_size(&c) != 1) {printf("code_size fail\n");}
    code_push_bit(&c, 0);
    if (code_size(&c) != 2) {printf("code_size fail\n");}
    code_push_bit(&c, 1);
    if (code_size(&c) != 3) {printf("code_size fail\n");}
    uint8_t bit;
    code_pop_bit(&c, &bit);
    if (!bit) {printf("code_pop_bit fail\n");}
    if (code_size(&c) != 2) {printf("code_size fail\n");}
    code_pop_bit(&c, &bit);
    if (bit) {printf("code_pop_bit fail\n");}
    if (code_size(&c) != 1) {printf("code_size fail\n");}
    code_pop_bit(&c, &bit);
    if (bit) {printf("code_pop_bit fail\n");}
    if (code_size(&c) != 0) {printf("code_size fail\n");}
    if (code_pop_bit(&c, &bit)) {printf("code_pop_bit fail\n");}
    if (code_size(&c) != 0) {printf("code_size fail\n");}
    for (int i = 0; i < ALPHABET; i += 1) {
        if (i % 2 == 0) {
            if (!code_push_bit(&c, 1)) {printf("code_push_bit fail\n");}
        }
        else {
            if (!code_push_bit(&c, 0)) {printf("code_push_bit fail\n");}
        }
    }
    if (code_push_bit(&c, 1)) {printf("code_push_bit fail\n");}

    //io tests
    fclose(fopen("test.txt", "a"));
    int test_file = open("test.txt", O_WRONLY);
    for (int i = 0; i < 129; i += 1) {
        write_code(test_file, &c);
    }
    flush_codes(test_file);
    close(test_file);
    test_file = open("test.txt", O_RDONLY);
    for (int i = 0; i < 33024; i += 1) {
        if (!read_bit(test_file, &bit)) {printf("io fail\n");}
        if (i % 2 == 0) {
            if (bit != 1) {
                printf("io fail\n");
                return 0;
            }
        }
        else {
            if (bit != 0) {
                printf("io fail\n");
                return 0;
            }
        }
    }
    close(test_file);
    remove("test.txt");

    return 0;
}
