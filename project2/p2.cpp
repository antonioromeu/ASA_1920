#include <iostream>
#include <vector>
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
        _totalNodes = _avenues * _streets + 2;
        vector<int> flow(_totalNodes, 0);
        _adjacencies.push_back({});

        for (int i = 1; i < _totalNodes - 1; i++) {
            vector<int> adj;
            for (int j = 1; j < _totalNodes - 1; j++) {
                if (j == i - _avenues) adj.push_back(j); //cima
                if (j == i + _avenues) adj.push_back(j); //baixo
                if (i % _avenues != 1 && j == i - 1) adj.push_back(j); //esquerda
                if (i % _avenues != 0 && j == i+1) adj.push_back(j); //direita
            }
            _adjacencies.push_back(adj);
        }       

    }
    
    vector<int> getAdjacencies(int i) { return _adjacencies[i]; }
    int getAvenues() { return _avenues; }
    int getStreets() { return _streets; }
    int getTotalNodes() { return _totalNodes; }
    int getIndexFromCoordinates(int av, int st) { return (st - 1) * _avenues + av; }
    
    void addMarket(int av, int st) { _adjacencies[getIndexFromCoordinates(av, st)].push_back(_totalNodes - 1); }
    int addClient(int av, int st) {
        _adjacencies[0].push_back(getIndexFromCoordinates(av, st));
        return getIndexFromCoordinates(av, st);
    }
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
            printf("c: %d p: %d \n", cur, prev);
            cur = prev;
        }
    }
    printf("%d\n", flow);
}

void printGraph(Graph* g) {
    for (int i = 0; i < g->getTotalNodes(); i++) {
        printf("%d: ", i);
        for (int next : g->getAdjacencies(i))
            printf("%d ", next);
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
    
    edmondsKarp(graph, 0, graph->getTotalNodes() - 1);
    return 0;
}


//python2.7 p2_gerador.py -n 1 -N 10 -m 1 -M 10 -s 10 -S 50 -c 10 -C 50 [-Z <some_random_seed_int>]
