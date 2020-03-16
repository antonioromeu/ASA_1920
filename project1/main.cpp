#include <iostream>
#include <vector>
#include <chrono>
#define NIL -1
#define max(a, b) (a < b ? b : a)
using namespace std;

class Graph {
    int _students, _relationships;
    bool *_disc;
    vector<int> _grades;
    vector<int> *_adj;

    public:
    Graph(int students, int relationships) {
        _students = students;
        _relationships = relationships;
        _adj = new vector<int>[students];
        _grades = *(new vector<int>[students]);
        _disc = new bool[students];
    }

    void setGrade(int grade) { _grades.push_back(grade); }
    void setDisc(int index, bool b) { _disc[index] = b; }
    void setAdj(int index, int adj) { _adj[index].push_back(adj); }
    
    vector<int> getAdjList(int index) { return _adj[index]; }
    vector<int> getGrades() { return _grades; }
    
    void freeG() { delete[] _adj; delete[] _disc; }

    void DFSaux(int vertix) {
        _disc[vertix] = true;
        for (int i = 0; i < (int) getAdjList(vertix).size(); i++) {
            int adjacent = getAdjList(vertix)[i];
            if (!_disc[adjacent])
                DFSaux(adjacent);
            _grades[vertix] = max(_grades[vertix], _grades[adjacent]);
        }
    }

    vector<int> DFS() {
        for (int i = 0; i < _students; i++)
            setDisc(i, false);

        for (int i = 0; i < _students; i++)
            if (!_disc[i])
                DFSaux(i);
        return _grades;
    }

    void printGrades() {
        for (int i = 0; i < _students; i++)
            cout << _grades[i] << endl;
    }
};

Graph* graphInit(int students, int relationships) {
    int grade, from, to;
    Graph* graph = new Graph(students, relationships);
    
    for (int i = 0; i < students; i++) {
        scanf("%d", &grade);
        graph->setGrade(grade);
    }

    for (int i = 0; i < relationships; i++) {
        scanf("%d %d", &from, &to);
        graph->setAdj(from-1, to-1);
    }

    return graph;
}

void free(Graph *graph) { graph->freeG(); }

int main() {
    int students, relationships;

    auto start = chrono::steady_clock::now();

    if (!scanf("%d,%d", &students, &relationships)) {
        printf("Error on scanf\n");
        exit(EXIT_FAILURE);
    }
    if (students < 2 || relationships < 1) {
        printf("Error on input\n");
        exit(EXIT_FAILURE);
    }
    Graph *g = graphInit(students, relationships);

    vector<int> grades = *(new vector<int>[students]);
    vector<int> newGrades = *(new vector<int>[students]);
    grades = g->getGrades();
    while (grades != (newGrades = g->DFS())) {
        grades = newGrades;
    }

    g->printGrades();
    free(g);

    auto end = chrono::steady_clock::now(); //////////////////////////
    auto diff = end - start;
    
    cout << chrono::duration <double, milli> (diff).count() << " ms" << endl;


    printf("%d %d\n", students, relationships);
    return 0;
}