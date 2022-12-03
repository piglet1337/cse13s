//include libraries
#include "stack.h"
#include <stdlib.h>

//define structure for Stack
struct Stack {
    uint32_t top;
    uint32_t capacity;
    Node **items;
};

//creates a stack with capacity = capacity and returns it
Stack *stack_create(uint32_t capacity) {
    Stack *s= (Stack *) malloc(sizeof(Stack));
    s->capacity = capacity;
    s->top = 0;
    s->items = (Node **) calloc(capacity, sizeof(Node*));
    return s;
}

//deletes the stack s points at, freeing any used memory, and setting the pointer equal to NULL
void stack_delete(Stack **s) {
    free((*s)->items);
    free(*s);
    *s = NULL;
    s = NULL;
}

//returns true if the stack is empty, false otherwise
bool stack_empty(Stack *s) {
    if (s->top == 0) {return true;}
    return false;
}

//returns true if the stack is full, false otherwise
bool stack_full(Stack *s) {
    if (s->top == s->capacity) {return true;}
    return false;
}

//returns the size of s
uint32_t stack_size(Stack *s) {
    return s->top;
}

//pushes node n onto stack s
bool stack_push(Stack *s, Node *n) {
    if (stack_full(s)) {return false;}
    s->items[s->top] = n;
    s->top += 1;
    return true;
}

//pops node from stack s passing it through n
bool stack_pop(Stack *s, Node **n) {
    if (stack_empty(s)) {return false;}
    s->top -= 1;
    *n = s->items[s->top];
    return true;
}

//prints out all the nodes on the stack
void stack_print(Stack *s) {
    for (uint32_t i = 0; i < s->top; i += 1) {
        node_print(s->items[i]);
    }
}
