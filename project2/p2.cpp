#include <iostream>
#include <vector>
#include <queue>
#define MARKET -1
#define CLIENT 1
using namespace std;

class Graph {
    int _avenues, _streets, _totalNodes;
    vector<vector<int>> _adjacencies, _flow;
    vector<int> _parentsList;//, _currentPathCapacity;
    vector<bool> _visited;


    public:
    Graph(int avenues, int streets) {
        _avenues = avenues;
        _streets = streets;
        _totalNodes = _avenues * _streets + 2;
        vector<int> tmpzeros(avenues * streets + 2, 0);
        _adjacencies.push_back({});
        _flow.push_back(tmpzeros);
        for (int i = 1; i < _totalNodes - 1; i++) {
            vector<int> tmp;
            for (int j = 1; j < _totalNodes - 1; j++) {
                if (i % _avenues != 1 && j == i - 1) tmp.push_back(j);
                else if (j == i - _avenues) tmp.push_back(j);
                else if (i % _avenues != 0 && j== i+1) tmp.push_back(j);
                else if (j == i + _avenues) tmp.push_back(j);
            }
            _adjacencies.push_back(tmp);
            _flow.push_back(tmpzeros);
        }
        _adjacencies.push_back({});
        _flow.push_back(tmpzeros);
        vector<int> parentsList(_totalNodes, -1);
        vector<bool> visitedList(_totalNodes, false);
        _parentsList = parentsList;
        _visited = visitedList;
    }

    vector<int> getAdjacencies(int i) { return _adjacencies[i]; }
    vector<vector<int>> getFlow() { return _flow; }
    
    int getAvenues() { return _avenues; }
    int getStreets() { return _streets; }
    int getTotalNodes() { return _totalNodes; }
    int getIndexFromCoordinates(int av, int st) { return (st-1) * _avenues + av; }
    
    void addMarket(int av, int st) { _adjacencies[getIndexFromCoordinates(av, st)].push_back(_totalNodes - 1); }

    void addClient(int av, int st) { _adjacencies[0].push_back(getIndexFromCoordinates(av, st)); }

    int bfs(int s, int t) {
        fill(_parentsList.begin(), _parentsList.end(), -1);
        //vector<bool> visitedList(_totalNodes, false);
        _parentsList[s] = -2;
        queue<pair<int, int>> q;
        q.push({s, _totalNodes});

        while (!q.empty()) {
            int cur = q.front().first;
            q.pop();
            for (int next : _adjacencies[cur]) {
                if (_parentsList[next] == -1 && _flow[cur][next] == 0 && _flow[next][cur] == 0 && _visited[next]==false) {
                    _parentsList[next] = cur;
                    int new_flow = 1;
                    if (next == t) 
                        return new_flow;
                    q.push({next, new_flow});
                }
            }
        }
        return 0;
    }

    int edmondsKarp(int s, int t) {
        int flow = 0;
        //vector<int> parent(_totalNodes);
        int new_flow;

        while ((new_flow = bfs(s, t)) != 0) {
            flow += new_flow;
            int cur = t;
            while (cur != s) {
                int prev = _parentsList[cur];
                _visited[prev] = true;
                _flow[prev][cur] += new_flow;
                _flow[cur][prev] += new_flow;
                cur = prev;
            }
        }
        printf("%d\n", flow);
        return flow;
    }
};

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
            printf("%d ", g->getFlow()[i][j]);
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
    graph->edmondsKarp(0, graph->getTotalNodes()-1);
    return 0;
}
