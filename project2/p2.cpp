#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
using namespace std;

vector<int> parentsList;
int maxFlow = 0;

class Graph {
    int _avenues, _streets, _totalNodes;
    vector<vector<vector<int>>> _adjacencies;

    public:
    Graph(int avenues, int streets) {
        _avenues = avenues;
        _streets = streets;
        _totalNodes = _avenues * _streets * 2 + 2;
        vector<vector<int>> aux;
        //vector<int> maria;
        _adjacencies.push_back(aux);
        for (int i = 1; i < _totalNodes - 1; i++) {
            if (i % 2 == 0) {
                //maria = {i - 1, 0};
                aux.push_back({i - 1, 0});
                for (int j = 1; j < _totalNodes - 1; j += 2) {
                    if (j == i - 1 - (2 * _avenues)) { //cima
                        aux.push_back({j, 1});
                        _adjacencies[j].push_back({i, 0});
                        _adjacencies[i - 1].push_back({j + 1, 0});
                    }
                    else if ((i / 2) % _avenues != 1 && j == i - 3) { //esquerda
                        aux.push_back({j, 1}); 
                        _adjacencies[j].push_back({i, 0});
                        _adjacencies[i - 1].push_back({j + 1, 0});
                    }
                    else if (j == i - 1 + (2 * _avenues)) //baixo
                        aux.push_back({j, 1});
                    else if ((i / 2) % _avenues != 0 && j == i + 1) //direita
                        aux.push_back({j, 1});
                }
            }
            else
                aux.push_back({i + 1, 1});
            _adjacencies.push_back(aux);
            aux.clear();
        }
        _adjacencies.push_back(aux);
    }
    
    vector<vector<int>> getAdjacencies(int i) { return _adjacencies[i]; }
    int getTotalNodes() { return _totalNodes; }
    int getIndexFromCoordinates(int av, int st) { return (st - 1) * _avenues + av; }
    
    void sendFlow(int prev, int cur, int flow) {
        for (int i = 0; i < (int) _adjacencies[prev].size(); i++)
            if (_adjacencies[prev][i][0] == cur) {
                _adjacencies[prev][i][1] -= flow;
                break;
            }
        for (int i = 0; i < (int) _adjacencies[cur].size(); i++)
            if (_adjacencies[cur][i][0] == prev) {
                _adjacencies[cur][i][1] += flow;
                break;
            }
    }

    void addMarket(int av, int st) { 
        int index = getIndexFromCoordinates(av, st) * 2;
        _adjacencies[index].push_back({_totalNodes - 1, 1}); 
        _adjacencies[_totalNodes - 1].push_back({index, 0}); 
    }
    
    void addClient(int av, int st) { 
        int index = getIndexFromCoordinates(av, st) * 2 - 1;
        _adjacencies[0].push_back({index, 1}); 
        _adjacencies[index].push_back({0, 0});
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
        for (vector<int> next : graph->getAdjacencies(cur)) {
            if (parentsList[next[0]] == -1 && next[1] > 0) { 
                parentsList[next[0]] = cur;
                if (next[0] == t)
                    return 1;
                q.push({next[0], 1});
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
        if (i % 2) printf("%de: ", (i + 1)/2);
        else printf("%ds: ", (i + 1)/2);
        for (vector<int> next : g->getAdjacencies(i))
            if (i % 2) printf("%ds ", (next[0] / 2) + (next[0] % 2));
            else printf("%de ", (next[0] / 2) + (next[0] % 2));
        printf("\n");
    }
}

int main() {
    int avenues, streets, markets, clients;
    
    if (!scanf("%d %d", &avenues, &streets)) {
        printf("Incorrect input\n");
        exit(EXIT_FAILURE);
    }
    
    if (!scanf("%d %d", &markets, &clients)) {
        printf("Incorrect input\n");
        exit(EXIT_FAILURE);
    }
    
    Graph *graph = new Graph(avenues, streets);
    int totalNodes = graph->getTotalNodes();
    maxFlow = min(markets, clients);

    for (int i = 0; i < totalNodes; i++)
        parentsList.push_back(-1);
    
    for (int i = 0; i < markets; i++) {
        int av, st;
        if (!scanf("%d %d", &av, &st))
            exit(EXIT_FAILURE);
        graph->addMarket(av, st);
    }
    
    for (int i = 0; i < clients; i++) {
        int av, st;
        if (!scanf("%d %d", &av, &st))
            exit(EXIT_FAILURE);
        graph->addClient(av, st);
    }
    
    //printGraph(graph);
    edmondsKarp(graph, 0, graph->getTotalNodes() - 1);
    return 0;
}
