#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define LL long long

struct node{
    float value;
    struct node* left;
    struct node* right;
};

struct tree{
    struct node* head;
};

void initialize_tree_random(struct tree** t, LL N){
    #pragma omp single
    {
        *t = (struct tree*) malloc(sizeof(struct tree));
        (*t)->head = (struct node*) malloc(sizeof(struct node));
        initialize_tree_node_random((*t)->head, N, 1, 100);
    }
}

void initialize_tree_node_random(struct node* n, LL size, LL lower, LL upper)
{
    n->value = (random() % upper + lower) * 3.14;
    if(size == 1){
        n->left = NULL;
        n->right = NULL;
        return;
    }
    n->left = (struct node*) malloc(sizeof(struct node));
    #pragma omp task
    initialize_tree_node_random(n->left, size >> 1, lower, upper);
    n->right = (struct node*) malloc(sizeof(struct node));
    #pragma omp task
    initialize_tree_node_random(n->right, size >> 1, lower, upper);
}

void free_tree(struct tree **t){
    #pragma omp single
    if(*t != NULL)
    {
        free_tree_node(&(*t)->head);
        free(*t);
        *t = NULL;
    }
}

void free_tree_node(struct node ** n){
    if((*n) != NULL)
    {
        #pragma omp task
        free_tree_node(&(*n)->left);
        #pragma omp task
        free_tree_node(&(*n)->right);
        free(*n);
        *n = NULL;
    }
}

float add_tree_node_value(struct node* n){
    float sum = 0;
    if(n != NULL){
        sum = n->value;
        #pragma omp task
        sum += add_tree_node_value(n->left);
        #pragma omp task
        sum += add_tree_node_value(n->right);
        printf("T[%d]: %f\n", omp_get_thread_num(), sum);
    }
    return sum;
}


float sum_tree_node(struct tree *t){
    float res = 0;
    if(t != NULL)
        res = add_tree_node_value(t->head);
    printf("sum = %f\n", res);
    return res;
}