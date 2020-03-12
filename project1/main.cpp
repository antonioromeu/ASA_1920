#include <iostream>
#include <vector>
#include <stack>
#define NIL -1
#define max(a, b) (a < b ? b : a)
using namespace std;

class Graph {
    int _students, _relationships, *_grades;
    bool *_disc;
    vector<int> *_adj;

    public:
    Graph(int students, int relationships) {
        _students = students;
        _relationships = relationships;
        _adj = new vector<int>[_students];
        _grades = new int[students];
        _disc = new bool[students];
    }

    void setStudents(int students) { _students = students; }
    void setRelationships(int relationships) { _relationships = relationships; }
    void setGrade(int index, int grade) { _grades[index] = grade; }
    void setDisc(int index, bool b) { _disc[index] = b; }
    vector<int> getAdjList(int index) { return _adj[index]; }
    void setAdj(int index, int adj) { _adj[index].push_back(adj); }
    void freeG() { delete[] _adj; }

    void propagateDiscs(int vertix) {
        _disc[vertix] = false;
        int grade = _grades[vertix];
        for (int i = 0; i < (int) getAdjList(vertix).size(); i++) {
            int adjacent = getAdjList(vertix)[i];
            printf("v-%d a-%d\n", vertix, adjacent); fflush(stdout);
            if (_disc[adjacent] && _grades[adjacent] < grade)
                propagateDiscs(adjacent);
            _disc[i] = false;
        }
    }

    void DFSaux(int vertix) {
        _disc[vertix] = true;
        for (int i = 0; i < (int) getAdjList(vertix).size(); i++) {
            while (true) {
                int grade = _grades[vertix];
                int adjacent = getAdjList(vertix)[i];
                if (!_disc[adjacent])
                    DFSaux(adjacent);
                _grades[vertix] = max(_grades[vertix], _grades[adjacent]);

                if (_grades[vertix] == grade) break;
                else propagateDiscs(vertix);
            }
        }
    }

    void DFS() {
        for (int i = 0; i < _students; i++)
            if (!_disc[i])
                DFSaux(i);
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
        graph->setGrade(i, grade);
        graph->setDisc(i, false);
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
    if (!scanf("%d,%d", &students, &relationships)) {
        printf("Error on scanf\n");
        exit(EXIT_FAILURE);
    }
    if (students < 2 || relationships < 1) {
        printf("Error on input\n");
        exit(EXIT_FAILURE);
    }
    Graph *g = graphInit(students, relationships);
    g->DFS();
    g->printGrades();
    free(g);
    return 0;
}