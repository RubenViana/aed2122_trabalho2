// AED 2021/2022 - Aula Pratica 11
// Pedro Ribeiro (DCC/FCUP) [17/01/2022]

#include"graph.h"
#include <climits>
#include <algorithm>

#define INF (INT_MAX/2)


// Constructor: nr nodes and direction (default: undirected)
Graph::Graph(int num, bool dir) : n(num), hasDir(dir), nodes(num+1) {
}

// Add edge from source to destination with a certain weight
void Graph::addEdge(int src, int dest, double weight, string line) {
    if (src<1 || src>n || dest<1 || dest>n) return;
    nodes[src].adj.push_back({dest, weight, line});
    if (!hasDir) nodes[dest].adj.push_back({src, weight, line});
}


int Graph::dijkstra_distance(int a, int b) {
    dijkstra(a);
    if (nodes[b].dist == INF) return -1;
    return nodes[b].dist;
}

list<int> Graph::dijkstra_path(int a, int b) { // Caminho que percorre o caminho com menor distância
    list<int> path;
    dijkstra(a);
    if (nodes[b].dist == INF) return path;
    while(a != b) {
        path.push_front(b);
        b = nodes[b].pred;
    }
    path.push_front(a);
    return path;
}

list<string> Graph::dijkstra_pathLines(int a, int b) {
    list<string> path;
    dijkstra(a);
    if (nodes[b].dist == INF) return path;
    while(a != b) {
        path.push_front(nodes[b].predLine);
        b = nodes[b].pred;
    }
    return path;
}

void Graph::dijkstra(int s) {
    MinHeap<int, int> q(n, -1);
    for (int v=1; v<=n; v++) {
        nodes[v].dist = INF;
        q.insert(v, INF);
        nodes[v].visited = false;
    }
    nodes[s].dist = 0;
    q.decreaseKey(s, 0);
    nodes[s].pred = s;
    while (q.getSize()>0) {
        int u = q.removeMin();
        nodes[u].visited = true;
        for (auto e : nodes[u].adj) {
            int v = e.dest;
            int w = e.weight;
            if (!nodes[v].visited && nodes[u].dist + w < nodes[v].dist) {
                nodes[v].dist = nodes[u].dist + w;
                q.decreaseKey(v, nodes[v].dist);
                nodes[v].pred = u;
                nodes[v].predLine = e.line;
            }
        }
    }
}

void Graph::bfs(int v) {
    for (int v=1; v<=n; v++){
        nodes[v].visited = false;
        nodes[v].dist = -1;
    }
    queue<int> q; // queue of unvisited nodes
    q.push(v);
    nodes[v].dist = 0;
    nodes[v].visited = true;
    nodes[v].pred = v;
    while (!q.empty()) { // while there are still unvisited nodes
        int u = q.front(); q.pop();
        for (auto e : nodes[u].adj) {
            int w = e.dest;
            if (!nodes[w].visited) {
                q.push(w);
                nodes[w].visited = true;
                nodes[w].dist = nodes[u].dist + 1;
                nodes[w].pred = u;
            }
        }
    }
}

list<int> Graph::bfs_path(int a, int b){ // Caminho que percorre o menor número de paragens
    list<int> path;
    bfs(a);
    while(a != b) {
        path.push_front(b);
        b = nodes[b].pred;
    }
    path.push_front(a);
    return path;
}

int Graph::prim(int r) {

    MinHeap<int,int> primHeap = MinHeap<int,int> (n,-1);

    for(int i = 1; i <= n; i++){
        nodes[i].dist = INF;
        primHeap.insert(i, INF);
    }
    nodes[r].dist = 0;
    primHeap.decreaseKey(r,0);
    while(primHeap.getSize() != 0){
        int smallestDisKey = primHeap.removeMin();
        for(auto it = nodes[smallestDisKey].adj.begin(); it != nodes[smallestDisKey].adj.end(); it++){
            if(primHeap.hasKey(it->dest) && it->weight < nodes[it->dest].dist){
                nodes[it->dest].parent = smallestDisKey;
                nodes[it->dest].dist = it->weight;
                primHeap.decreaseKey(it->dest,it->weight);
            }
        }
    }
    int sum = 0;
    for(int j = 1; j <= n; j++){
        sum += nodes[j].dist;
    }
    return sum;
}

int Graph::kruskal() {
    int var = 0;
    DisjointSets<int> newSect;
    typedef pair<int,int> edgesWeight; // (u,v)
    vector<pair<int,edgesWeight>> newGraph; // (edgeWeight,(u,v))
    for(int i = 1; i <= n; i++){
        newSect.makeSet(i);
        for(auto it = nodes[i].adj.begin() ; it != nodes[i].adj.end(); it++){
            newGraph.push_back({it->weight,{i,it->dest}});
        }
    }
    sort(newGraph.begin(),newGraph.end());

    for (auto it = newGraph.begin(); it != newGraph.end(); it++) {
        if(newSect.find(it->second.first) != newSect.find(it->second.second)){
            var += it->first;
            newSect.unite(it->second.first,it->second.second);
        }
    }
    return var;
}


void Graph::print () {
    for (int i = 1; i <= n; i++){
        for (auto &d: nodes[i].adj)
            cout << i << " -- " << d.weight << " -> " << d.dest << " : Line " << d.line << endl;
    }
}