#include <iostream>
#include <vector>
#include <algorithm>
#define MARKET -1
#define CLIENT 1
using namespace std;
/*
class Graph {
    int _avenues, _streets, _clients = 0, _markets = 0, attempt = 0;
    vector<vector<int>> _adj, _assignCM, _clientsCoord, _marketsCoord;
    vector<int> _combination;
    
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


    void pretty_print(const vector<int>& v) {
        static int count = 0;
        cout << "combination no " << (++count) << ": [ ";
        for (int i = 0; i < (int)v.size(); ++i) { cout << v[i] << " "; }
        cout << "] " << endl;
    }

    void addClient(int av, int st) { 
        _clientsCoord.push_back(vector<int> {av, st});
        _clients++;
    }
    void addMarket(int av, int st) { 
        _marketsCoord.push_back(vector<int> {av, st}); 
        _markets++;
    }

    void generate(int offset, int k, vector<int> markets) {
        if (k == 0) {
            while(next_permutation(begin(_combination), end(_combination))) {
                _assignCM.push_back(_combination);
                //pretty_print(_combination);
            }
            return;
        }
        for (int i = offset; i <= _markets - k; ++i) {
            _combination.push_back(markets[i]);
            generate(i + 1, k - 1, markets);
            _combination.pop_back();
        }
    }
    void generateAssigns() {
        vector<int> markets;
        for (int i = 0; i < _markets; i++) markets.push_back(i);
        generate(0, _clients, markets);
    }
};*/
class Graph {
    int _avenues, _streets;
    vector<vector<int>> _adjacencies, _flow;
    
    public:
    Graph(int avenues, int streets) {
        _avenues = avenues;
        _streets = streets;
        vector<int> tmpzeros(avenues*streets + 2, 0);
        _adjacencies.push_back(tmpzeros);
        _flow.push_back(tmpzeros);
        for (int i = 1; i < avenues*streets + 1; i++) {
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
    }
    vector<vector<int>> getAdjacencies() { return _adjacencies; }
    
    int getAvenues() { return _avenues; }
    int getStreets() { return _streets; }

    int getIndexFromCoordinates(int av, int st) { return (st-1) * _avenues + av; }
    
    void addMarket(int av, int st) { _adjacencies[getIndexFromCoordinates(av, st)][_avenues * _streets + 1] = 1; }

    void addClient(int av, int st) { _adjacencies[0][getIndexFromCoordinates(av, st)] = 1; }
};

void printGraph(Graph* g) {
    for (int i = 0; i < (g->getAvenues() * g->getStreets()) + 2; i++) {
        for (int j = 0; j < (g->getAvenues() * g->getStreets()) + 2; j++)
            printf("%d ", g->getAdjacencies()[i][j]);
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

    printGraph(graph);

    return 0;
}
