#include <unordered_map>
#include <map>
#include <string.h>
#include <iostream>
#include <list>

using namespace std;

struct Node{
    string name;
    int value;
    list<Edge*> edges;
};

struct Edge{
    Node* left;
    Node* right;
    int weight;
};

struct Graph{
    map<string, Node*> nodes;
    map<int, Edge*> edges;
}

int main(){
    struct Graph g = Graph{
        map<string, Node*>{new Node{}}
    }
    unordered_map<string, Node*> visited;
    list<Node*> to_visit;


}