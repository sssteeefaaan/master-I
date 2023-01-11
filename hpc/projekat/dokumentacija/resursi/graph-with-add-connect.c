#include <stdlib.h>
#include <stdio.h>

struct node{
    struct node* next;
    struct edge* neighbours;
    int value;
};

struct edge{
    struct edge* next;
    struct node* source;
    struct node* destination;
    int weight;
};

struct graph{
    struct node* nodes;
    struct edge* edges;
    int nodes_numb;
    int edges_numb;
};

void add(struct graph* G, int value){
    if(G){
        struct node* new_node = (struct node*) malloc(sizeof(struct  node));
        new_node->next->next = NULL;
        new_node->neighbours = NULL;
        new_node->value = value;

        struct node* tmp = NULL;
        struct node* curr = G->nodes;

        while(curr)
            tmp = curr, curr = curr->next;

        if(tmp)
            tmp->next = new_node;
        else
            G->nodes = new_node;
    }
}

void connect(struct graph *G, struct node* src, struct node* dst, int w){
    if(G){
        struct edge* new_edge = (struct edge*) malloc(sizeof(struct edge));
        new_edge->destination = dst;
        new_edge->source = src;
        new_edge->weight = w;
        new_edge->next = NULL;

        struct edge* tmp = NULL;
        struct edge* curr = G->edges;

        while(curr)
            tmp = curr, curr = curr->next;

        if(tmp)
            tmp->next = new_edge;
        else
            G->edges = new_edge;

        tmp = NULL;
        curr = src->neighbours;

        while(curr)
            tmp = curr, curr = curr->next;

        if(tmp)
            tmp->next = new_edge;
        else
            src->neighbours = new_edge;
    }
}