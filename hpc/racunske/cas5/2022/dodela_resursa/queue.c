#include "queue.h"

queue* initialize(int cap){
    queue* q = (queue*)malloc(sizeof(queue));
    q->elements = (int*)malloc(sizeof(int) * cap);
    q->front = q->back = 0;
    q->capacity = cap;
    return q;
}

void expand(queue* q){
    printf("exapnding!\n");
    q->capacity++; 
    q->capacity <<= 1;
    q->elements = (int*)reallocarray(q->elements, q->capacity, sizeof(int));
}

void destroy(queue* q){
    if(q->elements){
        free(q->elements);
        q->elements = NULL;
    }
}

int is_full(queue* q){
    return (q->back + 1) % q->capacity == q->front;
}

int is_empty(queue* q){
    return q->front == q->back;
}

int next(queue* q){
    int ret = q->front;
    q->front = (q->front + 1) % q->capacity;
    return ret;
}

int end(queue* q){
    int ret = q->back;
    q->back = (q->back + 1) % q->capacity;
    return ret;
}

void enqueue(queue* q, int el){
    if(is_full(q))
        expand(q);
    q->elements[end(q)] = el;
}

int dequeue(queue* q){
    return 
    is_empty(q)
    ?
    -1:
    q->elements[next(q)];
}