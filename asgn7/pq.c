//include libraries
#include "pq.h"
#include <stdlib.h>

//structure of PriorityQueue
struct PriorityQueue {
    uint32_t capacity;
    uint32_t size;
    Node **queue;
};

//creates a priority queue with capacity = capacity and returns it
PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *pq = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    pq->capacity = capacity;
    pq->size = 0;
    pq->queue = (Node **) calloc(capacity, sizeof(Node*));
    return pq;
}

//deletes the priorityqueue q points at freeing any memory used and sets the pointer to NULL
void pq_delete(PriorityQueue **q) {
    free((*q)->queue);
    free(*q);
    *q = NULL;
    q = NULL;
}

//returns true if q is empty otherwise false
bool pq_empty(PriorityQueue *q) {
    if (q->size == 0) {return true;}
    return false;
}

//returns true if q is full otherwise false
bool pq_full(PriorityQueue *q) {
    if (q->size == q->capacity) {return true;}
    return false;
}

//returns size of q
uint32_t pq_size(PriorityQueue *q) {
    return q->size;
}

//adds n to the queue, inserting it in the right place according to its frequency
bool enqueue(PriorityQueue *q, Node *n) {
    if (pq_full(q)) {return false;}
    uint32_t i;
    for (i = 0; i < q->size; i += 1) {
        if (q->queue[i]->frequency > n->frequency) {
            continue;
        }
        break;
    }
    for (uint32_t j = q->size; j > i; j -= 1) {
        q->queue[j] = q->queue[j-1];
    }
    q->queue[i] = n;
    q->size += 1;
    return true;
}

//Removes highest priority item from queue and passes it through n
bool dequeue(PriorityQueue *q, Node **n) {
    if (pq_empty(q)) {return false;}
    q->size -= 1;
    *n = q->queue[q->size];
    return true;
}

//Prints out priorityqueue in order from highest priority to least
void pq_print(PriorityQueue *q) {
    for (uint32_t i = q->size; i > 0; i -= 1) {
        node_print(q->queue[i-1]);
    }
}
