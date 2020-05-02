#include <iostream>
#include <vector>
#define MARKET -1
#define CLIENT 1
using namespace std;

class Graph {
    int _avenues, _streets, _clients = 0, _markets = 0;
    vector<vector<int>> _nodes;
    
    public:
    Graph(int avenues, int streets) {
        _avenues = avenues;
        _streets = streets;
        for (int i = 0; i < avenues; i++) {
            vector<int> *temp = new vector<int>(streets, 0);
            _nodes.push_back(*temp);
        }
    }
    
    int getAvenues() { return _avenues; }
    int getStreets() { return _streets; }
    int getClients() { return _clients; }
    int getMarkets() { return _markets; }
    vector<vector<int>> getNodes() { return _nodes; }
    int getValue(int avenue, int street) { return _nodes[avenue][street]; }

    void setNode(int avenue, int street, int value) { _nodes[avenue][street] = value; }
    
    void incrementClients() { _clients++; }
    void incrementMarkets() { _markets++; }
};

/*
vector<vector<int>> findNeighbors(Graph* g, vector<int> node) {
    vector<vector<int>> dirs = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
            graph->setValue(x - 1, y - 1,)
    vector<vector<int>> result;
    for (vector<int> dir : dirs) {
        vector<int> neighbor = {node[0] + dir[0], node[1] + dir[1]};
        for (vector<int> node : g->getNodes())
            if (neighbor == node)
                result.push_back(neighbor);
    }
    return result;
}
*/

void printGraph(Graph* g) {
    printf("avenues: %d\nstreets: %d\nmarkets: %d\nnodes:\n", g->getAvenues(), g->getStreets(), g->getMarkets());
    for (int i = 0; i < g->getAvenues(); i++) {
        for (int j = 0; j < g->getStreets(); j++)
            printf("(%d, %d) - %d\n", i + 1, j + 1, g->getValue(i, j));
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
        int x, y;
        scanf("%d %d", &x, &y);
        if (graph->getValue(x - 1, y - 1) == 0) {
            graph->setNode(x - 1, y - 1, MARKET);
            graph->incrementMarkets();
        }
    }
    for (int i = 0; i < clients; i++) {
        int x, y;
        scanf("%d %d", &x, &y);
        if (graph->getValue(x - 1, y - 1) == 0) {
            graph->setNode(x - 1, y - 1, CLIENT);
            graph->incrementClients();
        }
    }
    printGraph(graph);
    /*
    vector<vector<int>> neighbors = findNeighbors(graph, {1, 1});
    for (vector<int> v : neighbors)
        printf("(%d, %d)\n", v[0], v[1]);
    //printGraph(graph);
    */
    return 0;
}
