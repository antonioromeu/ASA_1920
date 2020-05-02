#include <iostream>
#include <vector>
using namespace std;

class Graph {
    int _avenues, _streets;
    vector<vector<int>> _nodes;

    public:
    Graph(int avenues, int streets) {
        _avenues = avenues;
        _streets = streets;
    }
    
    int getAvenues() { return _avenues; }
    int getStreets() { return _streets; }
    vector<vector<int>> getNodes() { return _nodes; }

    void addNode(int avenues, int streets) {
        vector<int> aux;
        aux.push_back(avenues);
        aux.push_back(streets);
        _nodes.push_back(aux);
    }
};

Graph* graphInit(int avenues, int streets) {
    Graph* graph = new Graph(avenues, streets);
    for (int i = 0; i < avenues; i++)
        for (int j = 0; j < streets; j++)
            graph->addNode(i, j);
    return graph;
}

vector<vector<int>> findNeighbors(Graph* g, vector<int> node) {
    vector<vector<int>> dirs = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    vector<vector<int>> result;
    for (vector<int> dir : dirs) {
        vector<int> neighbor = {node[0] + dir[0], node[1] + dir[1]};
        for (vector<int> node : g->getNodes())
            if (neighbor == node)
                result.push_back(neighbor);
    }
    return result;
}

void printGraph(Graph* g) {
    printf("avenues: %d\nstreets: %d\nnodes:\n", g->getAvenues(), g->getStreets());
    for (vector<int> v : g->getNodes())
        printf("(%d, %d)\n", v[0], v[1]);
}

int main() {
    int avenues, streets, markets, citizens;
    if (!scanf("%d %d", &avenues, &streets)) {
        printf("Incorrect input\n");
        exit(EXIT_FAILURE);
    }
    if (avenues < 2 || streets < 2) {
        printf("Invalid number of avenues and/or streets");
        exit(EXIT_FAILURE);
    }
    if (!scanf("%d %d", &markets, &citizens)) {
        printf("Incorrect input\n");
        exit(EXIT_FAILURE);
    }
    if (markets < 1 || citizens < 1) {
        printf("Invalid number of markets and/or citizens\n");
        exit(EXIT_FAILURE);
    }
    
    Graph *graph = graphInit(avenues, streets);
    vector<vector<int>> neighbors = findNeighbors(graph, {1, 1});
    for (vector<int> v : neighbors)
        printf("(%d, %d)\n", v[0], v[1]);
    //printGraph(graph);
    
    return 0;
}
