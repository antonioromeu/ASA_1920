#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#define MARKET -1
#define CLIENT 1
using namespace std;

vector<int> parentsList;
int maxFlow = 0;

class Graph {
    int _avenues, _streets, _totalNodes;
    vector<map<int, int>> _adjacencies;

    public:
    Graph(int avenues, int streets) {
        _avenues = avenues;
        _streets = streets;
        _totalNodes = _avenues * _streets * 2 + 2;
        map<int, int> aux;
        _adjacencies.push_back(aux);
        for (int i = 1; i < _totalNodes - 1; i++) {
            if (i % 2 == 0) {
                aux.insert({i - 1, 1});
                for (int j = 1; j < _totalNodes - 1; j+=2) {
                    if (j == i - 1 - (2 * _avenues)) { //cima
                        aux.insert({j, 1});
                        _adjacencies[j].insert({i, 0});
                        _adjacencies[i - 1].insert({j - 1, 0});
                    }
                    if ((i / 2) % _avenues != 1 && j == i - 3) { //esquerda
                        aux.insert({j, 1}); 
                        _adjacencies[j].insert({i, 0});
                        _adjacencies[i - 1].insert({j + 1, 0});
                    }
                    if (j == i - 1 + (2 * _avenues)) aux.insert({j, 1}); //baixo
                    if ((i / 2) % _avenues != 0 && j == i + 1) aux.insert({j, 1}); //direita
                }
            }
            else aux.insert({i + 1, 1});
            _adjacencies.push_back(aux);
            aux.clear();
        }
        _adjacencies.push_back(aux);
    }
    
    map<int, int> getAdjacencies(int i) { return _adjacencies[i]; }
    int getAvenues() { return _avenues; }
    int getStreets() { return _streets; }
    int getTotalNodes() { return _totalNodes; }
    int getIndexFromCoordinates(int av, int st) { return (st - 1) * _avenues + av; }

    void sendFlow(int prev, int cur, int flow) { 
        _adjacencies[prev].find(cur)->second -= flow;
        _adjacencies[cur].find(prev)->second += flow;
    }

    void addMarket(int av, int st) { 
        int index = getIndexFromCoordinates(av, st) * 2;
        _adjacencies[index].insert({_totalNodes - 1, 1}); 
        _adjacencies[_totalNodes - 1].insert({index, 1}); 
    }
    
    void addClient(int av, int st) { 
        int index = getIndexFromCoordinates(av, st) * 2 - 1;
        _adjacencies[0].insert({index, 1}); 
        _adjacencies[index].insert({0, 1});
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
        for (pair<int, int> next : graph->getAdjacencies(cur)) {
            if (parentsList[next.first] == -1 && next.second > 0 && next.second <= 2) { 
                parentsList[next.first] = cur;
                if (next.first == t)
                    return 1;
                q.push({next.first, 1});
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
            graph->sendFlow(prev, cur, flow);
            cur = prev;
        }
    }
    printf("%d\n", flow);
}

void printGraph(Graph* g) {
    for (int i = 0; i < g->getTotalNodes(); i++) {
        if (i%2) printf("%de: ", (i+1)/2);
        else printf("%ds: ", (i+1)/2);
        for (pair<int, int> next : g->getAdjacencies(i))
            if (i%2) printf("%ds ", (next.first/2) + (next.first%2));
            else printf("%de ", (next.first/2) + (next.first%2));
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

    for (int i = 0; i < totalNodes; i++)
        parentsList.push_back(-1);
    
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
    
    //printGraph(graph);
    edmondsKarp(graph, 0, graph->getTotalNodes() - 1);
    return 0;
}
