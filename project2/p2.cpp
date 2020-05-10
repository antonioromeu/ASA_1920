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
    vector<map<int, int>> _adjacencies;// _flow;

    public:
    Graph(int avenues, int streets) {
        _avenues = avenues;
        _streets = streets;
        _totalNodes = (_avenues * _streets + 2) * 2;
        map<int, int> aux;
        aux.insert({1, min(_avenues, _streets)});
        _adjacencies.push_back(aux);
        aux.clear();
        _adjacencies.push_back(aux);
        for (int i = 2; i < _totalNodes - 2; i++) {
            if (i % 2 == 1) {
                for (int j = 2; j < _totalNodes - 2; j+=2) {
                    if (j == i - 1 - (2 * _avenues)) aux.insert({j, 1}); //cima
                    if (j == i - 1 + (2 * _avenues)) aux.insert({j, 1}); //baixo
                    if (((i - 1) / 2) % _avenues != 1 && j == i - 3) aux.insert({j, 1}); //esquerda
                    if (((i - 1) / 2) % _avenues != 0 && j == i + 1) aux.insert({j, 1}); //direita
                }
            }
            else aux.insert({i + 1, 1});
            _adjacencies.push_back(aux);
            aux.clear();
        }
        aux.insert({_totalNodes - 1, min(_avenues, _streets)});
        _adjacencies.push_back(aux);
        aux.clear();
        _adjacencies.push_back(aux);
    }
    
    map<int, int> getAdjacencies(int i) { return _adjacencies[i]; }
    int getAvenues() { return _avenues; }
    int getStreets() { return _streets; }
    int getTotalNodes() { return _totalNodes; }
    int getIndexFromCoordinates(int av, int st) { return (st - 1) * _avenues + av; }

    void changeFlow(int prev, int cur, int flow) { _adjacencies[prev].find(cur)->second -= flow; }

    void addMarket(int av, int st) { 
        int index = getIndexFromCoordinates(av, st) * 2 + 1;
        _adjacencies[index].insert({_totalNodes - 2, 1}); 
    }
    void addClient(int av, int st) { _adjacencies[1].insert({getIndexFromCoordinates(av, st) * 2, 1}); }
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
            if (parentsList[next.first] == -1 && next.second > 0 && graph->getAdjacencies(next.first + 1).find(cur - 1)->second == 0) { 
                parentsList[next.first] = cur;
                int new_flow = 1;
                if (next.first == t)
                    return new_flow;
                q.push({next.first, new_flow});
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
            graph->changeFlow(prev, cur, flow);
            //printf("%d\n", graph->getAdjacencies(prev).find(cur)->second);
            if (prev != cur + 1)
                graph->changeFlow(cur, prev, flow);
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
        for (pair<int, int> next : g->getAdjacencies(i))
            if (i%2) printf("%de ", (next.first)/2);
            else printf("%ds ", (next.first)/2);
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
    
    edmondsKarp(graph, 1, graph->getTotalNodes() - 2);
    //printGraph(graph);
    return 0;
}


//python2.7 p2_gerador.py -n 1 -N 10 -m 1 -M 10 -s 10 -S 50 -c 10 -C 50 [-Z <some_random_seed_int>]
