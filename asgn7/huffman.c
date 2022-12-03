//include libraries
#include "huffman.h"
#include "pq.h"
#include "code.h"
#include "stack.h"
#include <unistd.h>

//define static Code c
static Code c;

//Builds the tree using the given histogram and returns the root node
Node *build_tree(uint64_t hist[static ALPHABET]) {
    PriorityQueue *pq = pq_create(ALPHABET);
    for (int i = 0; i < ALPHABET; i += 1) {
        if (hist[i] == 0) {continue;}
        Node *n = node_create(i, hist[i]);
        enqueue(pq, n);
    }
    while (pq_size(pq) > 1) {
        Node *left;
        Node *right;
        dequeue(pq, &left);
        dequeue(pq, &right);
        Node *parent = node_join(left, right);
        enqueue(pq, parent);
    }
    Node *root;
    dequeue(pq, &root);
    pq_delete(&pq);
    return root;
}

//initiliaze code
void init_code() {
    c = code_init();
}

//Creates code table
void build_codes(Node *root, Code table[static ALPHABET]) {
    if (root != NULL) {
        if (root->left == NULL && root->right == NULL) {
            table[root->symbol] = c;
        }
        else {
            code_push_bit(&c, 0);
            build_codes(root->left, table);
            uint8_t bit;
            code_pop_bit(&c, &bit);

            code_push_bit(&c, 1);
            build_codes(root->right, table);
            code_pop_bit(&c, &bit);
        }
    }
}

void dump_tree(int outfile, Node *root) {
    if (root != NULL) {
        dump_tree(outfile, root->left);
        dump_tree(outfile, root->right);
        if (root->left == NULL && root->right == NULL) {
            uint8_t buf[1];
            buf[0] = 'L';
            write(outfile, buf, 1);
            buf[0] = root->symbol;
            write(outfile, buf, 1);
        }
        else {
            uint8_t buf[1];
            buf[0] = 'I';
            write(outfile, buf, 1);
        }
    }
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
    Stack *s = stack_create(nbytes);
    for (uint16_t i = 0; i < nbytes; i += 1) {
        if (tree[i] == 'L') {
            i += 1;
            Node *n = node_create(tree[i], 1);
            stack_push(s, n);
        }
        if (tree[i] == 'I') {
            Node *right;
            Node *left;
            stack_pop(s, &right);
            stack_pop(s, &left);
            Node *parent = node_join(left, right);
            stack_push(s, parent);
        }
    }
    Node *root;
    stack_pop(s, &root);
    stack_delete(&s);
    return root;
}

void delete_tree(Node **root) {
    if ((*root)->left) {
        delete_tree(&(*root)->left);
    }
    if ((*root)->right) {
        delete_tree(&(*root)->right);
    }
    node_delete(root);
    root = NULL;
}
