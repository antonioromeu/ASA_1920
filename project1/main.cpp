#include <iostream>
#include <vector>
#include <stack>
#define NIL -1
#define min(a, b) (a < b ? a : b)
#define max(a, b) (a < b ? b : a)
using namespace std;

int *grades, *disc, *low;
bool *stackMember;
int time_ = 0;
stack<int> *st = new stack<int>();

class Graph {
    int _students, _relationships;
    vector<int> *_adj;

    public:
    Graph(int students, int relationships) {
        _students = students;
        _relationships = relationships;
        _adj = new vector<int>[_students];
    }

    void setStudents(int students) { _students = students; }
    void setRelationships(int relationships) { _relationships = relationships; }
    vector<int> getAdjList(int index) { return _adj[index]; }
    void setAdj(int index, int adj) { _adj[index].push_back(adj); }
    void freeG() { delete[] _adj; }

    void SCCaux(int vertix) {
        disc[vertix] = low[vertix] = ++time_;
        stackMember[vertix] = true;
        st->push(vertix);
        for (int i = 0; i < (int) getAdjList(vertix).size(); i++) {
            int adjacent = getAdjList(vertix)[i];
            //grades[vertix] = max(grades[vertix], grades[adjacent]);
            if (disc[adjacent] == NIL) {
                SCCaux(adjacent);
                low[vertix] = min(low[vertix], low[adjacent]);
                //grades[vertix] = max(grades[vertix], grades[adjacent]);
            }
            else if (stackMember[adjacent]) {
                low[vertix] = min(low[vertix], disc[adjacent]);
            }
            grades[vertix] = max(grades[vertix], grades[adjacent]);
        }
        if (low[vertix] == disc[vertix]) { 
            int v;
            while (st->top() != vertix) { 
                v = (int) st->top(); 
                stackMember[v] = false;
                st->pop();
            } 
            v = (int) st->top(); 
            stackMember[v] = false;
            st->pop();
        }
    }

    void SCC() {
        for (int i = 0; i < _students; i++) {
            if (disc[i] == NIL)
                SCCaux(i);
        }
    }
};

Graph* graphInit(int students, int relationships) {
    int grade, from, to;
    Graph* graph = new Graph(students, relationships);
    grades = new int[students];
    disc = new int[students];
    low = new int[students];
    stackMember = new bool[students];
    for (int i = 0; i < students; i++) {
        scanf("%d", &grade);
        grades[i] = grade;
        low[i] = NIL;
        disc[i] = NIL;
        stackMember[i] = false;
    }
    for (int i = 0; i < relationships; i++) {
        scanf("%d %d", &from, &to);
        graph->setAdj(from-1, to-1);
    }
    return graph;
}

void free(Graph *graph) {
  delete st;
  delete[] disc;
  delete[] low;
  delete[] stackMember;
  graph->freeG();
}

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
    g->SCC();
    for (int i = 0; i < students; i++)
        cout << grades[i] << endl;
    free(g);
    return 0;
}