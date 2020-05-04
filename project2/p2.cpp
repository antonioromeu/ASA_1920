#include <iostream>
#include <vector>
#include <queue>
#define MARKET -1
#define CLIENT 1
using namespace std;

class Graph {
    int _avenues, _streets, _totalNodes;
    vector<vector<int>> _adjacencies, _flow;
    vector<int> _parentsList, _currentPathCapacity;

    public:
    Graph(int avenues, int streets) {
        _avenues = avenues;
        _streets = streets;
        _totalNodes = _avenues * _streets + 2;
        vector<int> tmpzeros(avenues * streets + 2, 0);
        _adjacencies.push_back(tmpzeros);
        _flow.push_back(tmpzeros);
        for (int i = 1; i < avenues * streets + 1; i++) {
            vector<int> tmp;
            for (int j = 0; j < avenues*streets + 2; j++) {
                if (i % _avenues != 1 && j == i - 1) tmp.push_back(1);
                else if (j == i - _avenues) tmp.push_back(1);
                else if (i % _avenues != 0 && j== i+1) tmp.push_back(1);
                else if (j == i + _avenues) tmp.push_back(1);
                else tmp.push_back(0);
            }
            _adjacencies.push_back(tmp);
            _flow.push_back(tmpzeros);
        }
        _adjacencies.push_back(tmpzeros);
        _flow.push_back(tmpzeros);
        vector<int> temp1(avenues * streets, -1);
        _parentsList = temp1;
        vector<int> temp2(avenues * streets, 0);
        _currentPathCapacity = temp2;
    }

    vector<vector<int>> getAdjacencies() { return _adjacencies; }
    vector<vector<int>> getFlow() { return _flow; }
    
    int getAvenues() { return _avenues; }
    int getStreets() { return _streets; }
    int getTotalNodes() { return _totalNodes; }
    int getIndexFromCoordinates(int av, int st) { return (st-1) * _avenues + av; }
    
    void addMarket(int av, int st) { _adjacencies[getIndexFromCoordinates(av, st)][_avenues * _streets + 1] = 1; }

    void addClient(int av, int st) { _adjacencies[0][getIndexFromCoordinates(av, st)] = 1; }
    
    /*
    int BFS(int source, int sink) {
        queue<int> q;
        q.push(source);
        _parentsList[source] = -1;
        _currentPathCapacity[source] = -1;
        while (!q.empty()) {
            int currentNode = q.front();
            q.pop();
            for (int i = 0; i < _totalNodes; i++) {
                if (_adjacencies[currentNode][i] == 1) {
                    int to = _adjacencies[currentNode][i];
                    if (_parentsList[to] == -1) {
                        if (_flow[currentNode][to] == 0) {
                            _parentsList[to] = currentNode;
                            _currentPathCapacity[to] = 1;
                            if (to == sink)
                                return _currentPathCapacity[sink];
                            q.push(to);
                        }
                    }
                }
            }
        }
        return 0;
    }
    */

    int bfs(int s, int t, vector<int>& parent) {
        fill(parent.begin(), parent.end(), -1);
        parent[s] = -2;
        queue<pair<int, int>> q;
        q.push({s, 999});

        while (!q.empty()) {
            int cur = q.front().first;
            //int flow = q.front().second;
            q.pop();

            //for (int next : _adjacencies[cur]) {
            for (int i = 0; i < _totalNodes; i++) {
                int val = _adjacencies[cur][i];
                printf("current: %d adjacent: %d\n",cur, i);
                printf("parent[adjacent]: %d\n", parent[i]);
                //if (next == 1) {
                if (parent[i] == -1 && (val==1 && _flow[cur][i]==0 && _flow[i][cur]==0)) {
                    parent[i] = cur;
                    int new_flow = 1;
                    if (i == t)
                        return new_flow;
                    q.push({i, new_flow});
                   // }
                }
            }
        }
        printf("retornou 0\n");
        return 0;
    }

    int edmondsKarp(int s, int t) {
        int flow = 0;
        vector<int> parent(_totalNodes);
        int new_flow;

        printf("inicio do ek\n");

        while ((new_flow = bfs(s, t, parent)) != 0) {
            printf("dentro do while\n");
            flow += new_flow;
            int cur = t;
            while (cur != s) {
                int prev = parent[cur];
                _flow[prev][cur] -= new_flow;
                _flow[cur][prev] += new_flow;
                cur = prev;
            }
        }
        printf("%d\n", flow);
        return flow;
    }
};

void printGraph(Graph* g) {
    for (int i = 0; i < (g->getAvenues() * g->getStreets()) + 2; i++) {
        for (int j = 0; j < g->getTotalNodes(); j++)
            printf("%d ", g->getAdjacencies()[i][j]);
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
    //printGraph(graph);
    printFlow(graph);

    return 0;
}
