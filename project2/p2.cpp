#include <iostream>
#include <vector>
#include <queue>
#define MARKET -1
#define CLIENT 1
using namespace std;

vector<int> parentsList;
vector<bool> visitedList;

class Graph {
    int _avenues, _streets, _totalNodes;
    vector<vector<int>> _adjacencies, _flow;

    public:
    Graph(int avenues, int streets) {
        _avenues = avenues;
        _streets = streets;
        _totalNodes = _avenues * _streets + 2;
        vector<int> tmp(_totalNodes, 0);
        _adjacencies.push_back({});
        _flow.push_back(tmp);
        for (int i = 1; i < _totalNodes - 1; i++) {
            vector<int> adj;
            for (int j = 1; j < _totalNodes - 1; j++) {
                if (i % _avenues != 1 && j == i - 1) adj.push_back(j);
                else if (j == i - _avenues) adj.push_back(j);
                else if (i % _avenues != 0 && j== i+1) adj.push_back(j);
                else if (j == i + _avenues) adj.push_back(j);
            }
            _adjacencies.push_back(adj);
            _flow.push_back(tmp);
        }
        _adjacencies.push_back({});
        _flow.push_back(tmp);
    }
    
    int getFlow(int av, int st) { return _flow[av][st]; }
    vector<int> getAdjacencies(int i) { return _adjacencies[i]; }
    int getAvenues() { return _avenues; }
    int getStreets() { return _streets; }
    int getTotalNodes() { return _totalNodes; }
    int getIndexFromCoordinates(int av, int st) { return (st - 1) * _avenues + av; }
    
    void setFlow(int av, int st, int val) { _flow[av][st] = val; }

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
    q.push({s, graph->getTotalNodes()});

    while (!q.empty()) {
        int cur = q.front().first;
        q.pop();
        for (int next : graph->getAdjacencies(cur)) {
            if (parentsList[next] == -1 && graph->getFlow(cur, next) == 0
                    && graph->getFlow(next, cur) == 0
                    && (visitedList[next] == false || cur == s)) {
                
                //printf("cur:%d -- next:%d\n", cur, next);
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
            visitedList[prev] = true;
            graph->setFlow(prev, cur, 1);
            graph->setFlow(cur, prev, 1);
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

void printFlow(Graph* g) {
    for (int i = 0; i < g->getTotalNodes(); i++) {
        for (int j = 0; j < g->getTotalNodes(); j++)
            printf("%d ", g->getFlow(i, j));
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
        visitedList[graph->addClient(av, st)] = true;
    }
    
    edmondsKarp(graph, 0, graph->getTotalNodes() - 1);
    return 0;
}
