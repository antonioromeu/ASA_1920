#include <iostream>
#include <vector>
#include <chrono>
#include <stack>
#define NIL -1
#define max(a, b) (a < b ? b : a)
#define min(a, b) (a < b ? a : b)
using namespace std;

stack<int> *st = new stack<int>();
stack<int> *stSCCs = new stack<int>();//////////////////
vector<bool> stackMember;
vector<int> low; 

int nSCCs = 0;
int *studentsSCC; //eh preciso inicializar com tamanho n students
vector<int> gradesSCC;
vector<vector<int>> adjSCCmatrix;

class Graph {
    int _vertices;
    vector<int> _disc;
    vector<int> _grades;
    vector<int> *_adj;

    public:
    Graph(int vertices) {
        _vertices = vertices;
        _disc = vector<int>(vertices, NIL);
        _adj = new vector<int>[vertices];
        _grades = *(new vector<int>[vertices]);
    }
    Graph() {
        _vertices = nSCCs;
        _disc = vector<int>(_vertices, 0);
        _adj = new vector<int>[_vertices];
        _grades = gradesSCC;
    }

    void addGrade(int grade) { _grades.push_back(grade); }
    void setGrade(int index, int grade) { _grades[index] = grade;  }
    void setAdj(int index, int adj) { _adj[index].push_back(adj); }
    void setAdjs(int index, vector<int> adjs) {_adj[index] = adjs; }
    void setDisc(int index, int disc) { _disc[index] = disc; }
    int getVertices() { return _vertices; }
    int getDisc(int i) { return _disc[i]; }
    int getGrade(int index) { return _grades[index]; }
    vector<int> getAdjList(int index) { return _adj[index]; }
    vector<int> getGrades() { return _grades; }

    void freeG() { delete[] _adj; }
};


void DFSaux(Graph *graph, int vertice) {
    graph->setDisc(vertice, 1);
    for (int i = 0; i < (int) graph->getAdjList(vertice).size(); i++) {
        int adjacent = graph->getAdjList(vertice)[i];
        if (!graph->getDisc(adjacent))
            DFSaux(graph, adjacent);
        graph->setGrade(vertice, max(graph->getGrade(vertice), graph->getGrade(adjacent)));
    }
}

vector<int> DFS(Graph *graph) {
    for (int i = 0; i < graph->getVertices(); i++) 
        if (!graph->getDisc(i))
            DFSaux(graph, i);
    return graph->getGrades();
}

int SCCaux(Graph *graph, int vertice, int time_) {
    int nAdjSCCs = 0 ;
    low[vertice] = ++time_;
    graph->setDisc(vertice, time_);
    stackMember[vertice] = true;
    st->push(vertice);
    for (int i = 0; i < (int) graph->getAdjList(vertice).size(); i++) {
        int adjacent = graph->getAdjList(vertice)[i];
        if (graph->getDisc(adjacent) == NIL) {

            nAdjSCCs += SCCaux(graph, adjacent, time_);

            low[vertice] = min(low[vertice], low[adjacent]);
        }
        else if (stackMember[adjacent]) {
            low[vertice] = min(low[vertice], graph->getDisc(adjacent));
        }
        graph->setGrade(vertice, max(graph->getGrade(vertice), graph->getGrade(adjacent)));
    }
    
    if (low[vertice] == graph->getDisc(vertice)) { 
        int v;
        while (st->top() != vertice) { 
            v = (int) st->top(); 
            studentsSCC[v] = nSCCs;
            stackMember[v] = false;
            st->pop();
        } 
        v = (int) st->top();
        studentsSCC[v] = nSCCs;
        stackMember[v] = false;
        st->pop();
        gradesSCC.push_back(graph->getGrade(v)); //colocar na lista de SCCs a "grade" do novo SCC

        vector<int> adjSCCs; //criar um vetor com os SCCs adjacentes ao novo SCC
        while (nAdjSCCs > 0) { 
            adjSCCs.push_back(stSCCs->top()); //colocar os SCCs nesse vetor
            stSCCs->pop(); //retira-los da pilha
            nAdjSCCs--;
        }
        adjSCCmatrix.push_back(adjSCCs); //colocar na matrix de adjacencias o vetor de SCCs adjacentes ao novo vetor
        stSCCs->push(nSCCs); // colocar o novo SCC na pilha de SCC recem criados

        nAdjSCCs++;
        nSCCs++;
    }
    return nAdjSCCs;
}

void SCC(Graph *graph) {
    static int time_ = 0;
    int vertices = graph->getVertices();
    for (int i = 0; i < vertices; i++) {
        if (graph->getDisc(i) == NIL)
            SCCaux(graph, i, time_);
    }
}

Graph* compactGraphInit() {
    Graph *graph = new Graph();
    for (int i = 0; i < nSCCs; i++) {
        graph->setAdjs(i, adjSCCmatrix[i]);
    }
    return graph;
}

Graph* graphInit(int vertices, int relationships) {
    int grade, from, to;
    Graph* graph = new Graph(vertices);
        
    for (int i = 0; i < vertices; i++) {
        scanf("%d", &grade);
        graph->addGrade(grade);
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

    studentsSCC = new int[students];
    low = vector<int>(students, NIL);

    Graph *graph = graphInit(students, relationships); //

    stackMember = vector<bool>(students, false);
    SCC(graph);
    graph->freeG();

    Graph *compressedGraph = compactGraphInit();
    vector<int> gradesSCC = DFS(compressedGraph);
    compressedGraph->freeG();

    for (int i = 0; i < students; i++) {
        cout << gradesSCC[studentsSCC[i]] << endl;
    }

    auto end = chrono::steady_clock::now();

    auto diff = end - start;
    //cout << chrono::duration <double, milli> (diff).count() << " ms" << endl;
    return 0;
}