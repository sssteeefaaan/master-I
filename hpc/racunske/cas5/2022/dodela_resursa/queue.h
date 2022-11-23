#include <stdlib.h>
#include <stdio.h>

typedef struct{
    int* elements;
    int back;
    int front;
    int capacity;
} queue;

queue* initialize(int);

void destroy(queue*);

void enqueue(queue*, int);

int dequeue(queue*);

void expand(queue*);

int is_empty(queue*);

int is_full(queue*);