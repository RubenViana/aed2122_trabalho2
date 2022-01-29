// AED 2021/2022 - Aula Pratica 11
// Pedro Ribeiro (DCC/FCUP) [17/01/2022]

#ifndef _GRAPH_H_
#define _GRAPH_H_

#include "disjointSet.h"
#include "minHeap.h"
#include <vector>
#include <list>
#include <iostream>
#include <string>
#include <queue>

using namespace std;


class Graph {
    struct Edge {
        int dest;   // Destination node
        double weight; // An integer weight
        string line; //
    };

    struct Node {
        list<Edge> adj; // The list of outgoing edges (to adjacent nodes)
        int dist;
        int pred;
        int parent;
        string predLine;
        bool visited;
    };

    int n;              // Graph size (vertices are numbered from 1 to n)
    bool hasDir;        // false: undirect; true: directed
    vector<Node> nodes; // The list of nodes being represented

    void dijkstra(int s);

    void bfs(int v);

public:

    int kruskal();

    int prim(int v);

    void print();

    // Constructor: nr nodes and direction (default: undirected)
    Graph(int nodes, bool dir = false);

    // Add edge from source to destination with a certain weight
    void addEdge(int src, int dest, double weight, string line);

    int dijkstra_distance(int a, int b);
    list<int> dijkstra_path(int a, int b);
    list<string> dijkstra_pathLines(int a, int b);
    list<int> bfs_path(int a, int b);
    list<string> bfs_pathLines(int a, int b);

};

#endif
