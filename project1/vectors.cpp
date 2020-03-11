#include <iostream>
#include <list>
#include <vector>
#include <stack>
#define NIL -1
using namespace std;

class Graph {
    int _numberNodes;
    vector<int> _disc;
    vector<int> _low;
    vector<int> _grades; 
    vector<vector<int>> _adj; 
    vector<bool> _stackMember;

    public:
    Graph(int numberNodes) { _numberNodes = numberNodes; }
    
    int getDisc(int index) { return _disc.at(index); }
    int getLow(int index) { return _low.at(index); }
    int getGrade(int index) const { return _grades.at(index); }
    vector<int> getAdj(int index) { return _adj.at(index); }
    bool isStackMember(int index) { return _stackMember.at(index); }
    int getNumberNodes() const { return _numberNodes; }

    void addGrade(int grade) { _grades.push_back(grade); }

    void createVector() { 
        for (int i = 0; i < _numberNodes; i++) {
            vector<int>* v = new vector<int>;
            _adj.push_back(*v);
        }
    }
    void addAdj(int from, int to) { _adj.at(from).push_back(to); }

    void addDisc() { _disc.push_back(NIL); }
    void addLow() { _low.push_back(NIL); }

    void setGrade(int index, int grade) { _grades.at(index) = grade; }
    void setDisc(int index, int disc) { _disc.at(index) = disc; }
    void setLow(int index, int low) { _low.at(index) = low; }
    void changeStackMember(int index) { _stackMember.at(index) ? _stackMember.at(index) = false : _stackMember.at(index) = true; }
    void setStackMember() { _stackMember.push_back(false); }

    void SCCaux(int vertix, stack<int> *st) {
        static int time = 0;
        time++;
        setDisc(vertix, time);
        setLow(vertix, time);
        st->push(vertix);
        changeStackMember(vertix);
        for (int i : getAdj(vertix)) {
            int adjacent = i;
            if (getDisc(adjacent) == NIL) {
                SCCaux(adjacent, st);
                setLow(vertix, min(getLow(vertix), getLow(adjacent)));
                setGrade(vertix, max(getGrade(vertix), getGrade(adjacent)));
            }
            else if (isStackMember(adjacent)) {
                setLow(vertix, min(getLow(vertix), getLow(adjacent)));
                setGrade(vertix, max(getGrade(vertix), getGrade(adjacent)));
            }
        }
        int v;
        if (getLow(vertix) == getDisc(vertix)) { 
            while (st->top() != vertix) { 
                v = (int) st->top(); 
                changeStackMember(v);
                st->pop(); 
            } 
            v = (int) st->top(); 
            changeStackMember(v);
            st->pop();
        }
    }

    friend ostream &operator<<(ostream &o, const Graph &graph) {
        for (int i = 0; i < graph.getNumberNodes(); i++)
            o << graph.getGrade(i) << endl;
        return o;
    }

    void SCC() {
        stack<int>* st = new stack<int>();
        for (int i = 0; i < _numberNodes; i++) {
            if (getDisc(i) == NIL)
                SCCaux(i, st);
        }
    }
};

Graph* graphInit() {
    int students, relationships, grade, from, to;
    
    if (!scanf("%d,%d", &students, &relationships)) { // Scans the first line of input
        printf("Error on scanf\n");
        exit(EXIT_FAILURE);
    }
 
    if (students < 2 || relationships < 1) {
        printf("Error on input\n");
        exit(EXIT_FAILURE);
    }

    Graph* graph = new Graph(students);

    for (int i = 0; i < students; i++) { // Creates vertices and adds them to graph
        scanf("%d", &grade);
        graph->addGrade(grade);
        graph->addLow();
        graph->addDisc();
        graph->setStackMember();
        graph->createVector();
    }
 
    for (int i = 0; i < relationships; i++) { // Adds the relationships to the adjacencies list
        scanf("%d %d", &from, &to);
        graph->addAdj(from-1, to-1);
    }
    return graph;
}

int main() {
    Graph *graph = graphInit();

    graph->SCC();
    cout << *graph;
    return 0;
}