#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <queue>
#define MARKET -1
#define CLIENT 1
using namespace std;

vector<int> parentsList;
vector<bool> visitedList;

int maxFlow = 0;

class Graph {
    int _avenues, _streets, _totalNodes;
    vector<vector<int>> _adjacencies;// _flow;

    public:
    Graph(int avenues, int streets) {
        _avenues = avenues;
        _streets = streets;
        _totalNodes = (_avenues * _streets + 2) * 2;
        vector<int> flow(_totalNodes, 0);
        _adjacencies.push_back({1}); _adjacencies.push_back({});
        for (int i = 2; i < _totalNodes - 2; i++) {
            vector<int> adj;
            if (i % 2 == 1) {
                for (int j = 2; j < _totalNodes - 2; j+=2) {
                    if (j == i - 1 - (2 * _avenues)) adj.push_back(j); //cima
                    if (j == i - 1 + (2 * _avenues)) adj.push_back(j); //baixo
                    if (((i - 1) / 2) % _avenues != 1 && j == i - 3) adj.push_back(j); //esquerda
                    if (((i - 1) / 2) % _avenues != 0 && j == i + 1) adj.push_back(j); //direita
                }
            }
            else adj.push_back(i+1);
            _adjacencies.push_back(adj);       
        }
        _adjacencies.push_back({_totalNodes - 1});
        _adjacencies.push_back({});
    }
    
    vector<int> getAdjacencies(int i) { return _adjacencies[i]; }
    int getAvenues() { return _avenues; }
    int getStreets() { return _streets; }
    int getTotalNodes() { return _totalNodes; }
    int getIndexFromCoordinates(int av, int st) { return (st - 1) * _avenues + av; }

    
    void addMarket(int av, int st) { 
        int index = getIndexFromCoordinates(av, st) * 2 + 1;
        _adjacencies[index].push_back(_totalNodes - 2); 
    }
    void addClient(int av, int st) { _adjacencies[1].push_back(getIndexFromCoordinates(av, st) * 2); }
};

int bfs(Graph *graph, int s, int t) {
    fill(parentsList.begin(), parentsList.end(), -1);
    parentsList[s] = -2;
    queue<pair<int, int>> q;
    q.push({s, maxFlow}); 
    
    while (!q.empty()) {
        int cur = q.front().first;
        q.pop();

        for (int next : graph->getAdjacencies(cur)) {
            if (parentsList[next] == -1 && (visitedList[next] == false || next == t)) { 
                parentsList[next] = cur;
                int new_flow = 1;
                if (next == t)
                    return new_flow;
                q.push({next, new_flow});
            }
        }
    }
    return 0;
}

void edmondsKarp(Graph *graph, int s, int t) {
    int flow = 0;
    int new_flow;

    while ((new_flow = bfs(graph, s, t)) != 0) {
        flow += new_flow;
        int cur = t;
        while (cur != s) {
            int prev = parentsList[cur];
            visitedList[cur] = true;
            cur = prev;
        }
    }
    printf("%d\n", flow);
}

void printGraph(Graph* g) {
    for (int i = 0; i < g->getTotalNodes(); i++) {
        if (i%2) printf("%ds: ", (i+1)/2 - (i%2));
        else printf("%de: ", (i+1)/2 - (i%2));
        //printf("%d: ", i);
        for (int next : g->getAdjacencies(i))
            if (i%2) printf("%de ", next/2);
            else printf("%ds ", next/2);
        printf("\n");
    }
}

int main() {
    int avenues, streets, markets, clients;
    
    if (!scanf("%d %d", &avenues, &streets)) {
        printf("Incorrect input\n");
        exit(EXIT_FAILURE);
    }
    if (avenues < 2 || streets < 2) {
        printf("Invalid number of avenues and/or streets");
        exit(EXIT_FAILURE);
    }
    if (!scanf("%d %d", &markets, &clients)) {
        printf("Incorrect input\n");
        exit(EXIT_FAILURE);
    }
    if (markets < 1 || clients < 1) {
        printf("Invalid number of markets and/or citizens\n");
        exit(EXIT_FAILURE);
    }
    
    Graph *graph = new Graph(avenues, streets);
    int totalNodes = graph->getTotalNodes();
    maxFlow = min(markets, clients);

    for (int i = 0; i < totalNodes; i++) {
        parentsList.push_back(-1);
        visitedList.push_back(false);
    }
    for (int i = 0; i < markets; i++) {
        int av, st;
        scanf("%d %d", &av, &st);
        graph->addMarket(av, st);
    }
    for (int i = 0; i < clients; i++) {
        int av, st;
        scanf("%d %d", &av, &st);
        graph->addClient(av, st);
    }
    
    //edmondsKarp(graph, 0, graph->getTotalNodes() - 1);
    printGraph(graph);
    return 0;
}


//python2.7 p2_gerador.py -n 1 -N 10 -m 1 -M 10 -s 10 -S 50 -c 10 -C 50 [-Z <some_random_seed_int>]
