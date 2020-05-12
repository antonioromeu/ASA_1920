#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
#include <map>
#include <unordered_map>
#include <queue>
using namespace std;

vector<int> parentsList;

class Graph {
    int _avenues, _streets, _totalNodes;
    vector<unordered_map<int, int>> _adjacencies;

    public:
    Graph(int avenues, int streets) {
        _avenues = avenues;
        _streets = streets;
        _totalNodes = _avenues * _streets * 2 + 2;
        unordered_map<int, int> aux;
        _adjacencies.push_back(aux);
        for (int i = 1; i < _totalNodes - 1; i++) {
            if (i % 2 == 0) {
                aux.insert({i - 1, 0});
                for (int j = 1; j < _totalNodes - 1; j += 2) {
                    if (j == i - 1 - (2 * _avenues)) { //cima
                        aux.insert({j, 1});
                        _adjacencies[j].insert({i, 0});
                        _adjacencies[i - 1].insert({j + 1, 0});
                    }
                    else if ((i / 2) % _avenues != 1 && j == i - 3) { //esquerda
                        aux.insert({j, 1}); 
                        _adjacencies[j].insert({i, 0});
                        _adjacencies[i - 1].insert({j + 1, 0});
                    }
                    else if (j == i - 1 + (2 * _avenues)) //baixo
                        aux.insert({j, 1});
                    else if ((i / 2) % _avenues != 0 && j == i + 1) //direita
                        aux.insert({j, 1});
                }
            }
            else
                aux.insert({i + 1, 1});
            _adjacencies.push_back(aux);
            aux.clear();
        }
        _adjacencies.push_back(aux);
    }
    
    unordered_map<int, int> getAdjacencies(int i) { return _adjacencies[i]; }
    int getTotalNodes() { return _totalNodes; }
    int getIndexFromCoordinates(int av, int st) { return (st - 1) * _avenues + av; }
    
    void sendFlow(int prev, int cur, int flow) {
        _adjacencies[prev].find(cur)->second -= flow;
        _adjacencies[cur].find(prev)->second += flow;
    }

    void addMarket(int av, int st) { 
        int index = getIndexFromCoordinates(av, st) * 2;
        _adjacencies[index].insert({_totalNodes - 1, 1}); 
        _adjacencies[_totalNodes - 1].insert({index, 0}); 
    }
    
    void addClient(int av, int st) { 
        int index = getIndexFromCoordinates(av, st) * 2 - 1;
        _adjacencies[0].insert({index, 1}); 
        _adjacencies[index].insert({0, 0});
    }

    bool dfs(int s, int t) { 
        stack<int> stack; 
        stack.push(s); 
        fill(parentsList.begin(), parentsList.end(), -1);
        parentsList[s] = -2;
        while (!stack.empty()) { 
            s = stack.top(); 
            stack.pop();
            for (pair<int, int> next : _adjacencies[s]) {
                if (parentsList[next.first] == -1 && next.second > 0) {
                    parentsList[next.first] = s;
                    stack.push(next.first); 
                }
                if (next.first == t) 
                    return true;
            }
        } 
        return false;
    } 

    void fordFulkerson(int s, int t) {
        int flow = 0;
        while (dfs(s, t)) {
            flow++;
            int cur = t;
            while (cur != s) {
                int prev = parentsList[cur];
                sendFlow(prev, cur, flow);
                cur = prev;
            }
        }
        printf("%d\n", flow);
    }
};

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

    graph->fordFulkerson(0, graph->getTotalNodes() - 1);
    return 0;
}
