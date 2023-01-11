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