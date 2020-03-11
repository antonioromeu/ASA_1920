#include <iostream>
#include <list>
#include <vector>
#include <stack>
#define NIL -1
using namespace std;

class Vertix {
    int _grade, _disc, _low, _vecIndex;
    bool _stackMember;

    public:
    Vertix(int grade, int index) {
        _grade = grade;
        _disc = NIL;
        _low = NIL;
        _stackMember = false;
        _vecIndex = index;
    }

    void setGrade(int grade) { _grade = grade; }

    void setDisc(int disc) { _disc = disc; }

    void setLow(int low) { _low = low; }

    void setStackMember(bool b) { _stackMember = b; }

    int getGrade() const { return _grade; }

    int getDisc() { return _disc; }

    int getLow() { return _low; }
    
    int isStackMember() { return _stackMember; }

    int getIndex() { return _vecIndex; }
};

class Node {
    Vertix* _vertix;
    list<Vertix*> _adj;


    public:
    Node(Vertix* vertix) { _vertix = vertix; }

    void setVertix(Vertix* vertix) { _vertix = vertix; }

    void addAdj(Vertix* vertix) { _adj.push_back(vertix); }

    Vertix* getVertix() { return _vertix; }

    list<Vertix*> getAdj() { return _adj; }
};

class Graph {
    int _numberNodes;
    vector<Node*> _nodes;

    public:
    Graph(int numberNodes) { _numberNodes = numberNodes; }

    void addNode(Node* node) { _nodes.push_back(node); }
 
    vector<Node*> getNodes() const { return _nodes; }

    friend ostream &operator<<(ostream &o, const Graph &graph) {
        for (Node *n : graph.getNodes())
            o << n->getVertix()->getGrade() << endl;
        return o;
    }

    void SCCaux(Vertix* vertix, stack<Vertix*> st[]) {
        Node* node =_nodes.at(vertix->getIndex());
        static int time = 0;
        time++;
        vertix->setDisc(time);
        vertix->setLow(time);
        st->push(vertix);
        vertix->setStackMember(true);
        for (Vertix* i : node->getAdj()) {
            Vertix* adjacent = i;
            if (adjacent->getDisc() == NIL) {
                SCCaux(adjacent, st);
                vertix->setLow(min(vertix->getLow(), adjacent->getLow()));
                //vertix->setGrade(max(vertix->getGrade(), adjacent->getGrade()));
            }
            else if (adjacent->isStackMember()) {
                vertix->setLow(min(vertix->getLow(), adjacent->getLow()));
                //vertix->setGrade(max(vertix->getGrade(), adjacent->getGrade()));
            }
        }
        vector<int> nSCC;
        Vertix* v;
        if (vertix->getLow() == vertix->getDisc()) { 
            while (st->top()->getIndex() != vertix->getIndex()) { 
                v = (Vertix*) st->top(); 
                v->setStackMember(false);
                nSCC.push_back(v->getIndex());
                st->pop(); 
            } 
            v = (Vertix*) st->top(); 
            v->setStackMember(false);
            nSCC.push_back(v->getIndex());
            st->pop(); 
        }
    }

    void SCC() {
        stack<Vertix*>* st = new stack<Vertix*>();
        for (int i = 0; i < (int) _nodes.size(); i++) {
            Vertix* vertix = _nodes[i]->getVertix();
            if (vertix->getDisc() == NIL)
                SCCaux(vertix, st);
        }
    }
};

Graph* graphInit() {
    int students, relationships, grade, from, to;
    Graph *graph = new Graph(students);
    
    if (!scanf("%d,%d", &students, &relationships)) { // Scans the first line of input
        printf("Error on scanf\n");
        exit(EXIT_FAILURE);
    }
 
    if (students < 2 || relationships < 1) {
        printf("Error on input\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < students; i++) { // Creates vertices and adds them to graph
        scanf("%d", &grade);
        Vertix* v = new Vertix(grade, i);
        Node* n = new Node(v);
        graph->addNode(n);
    }

    for (int i = 0; i < relationships; i++) { // Adds the relationships to the adjacencies list
        scanf("%d %d", &from, &to);
        graph->getNodes().at(from-1)->addAdj(graph->getNodes().at(to-1)->getVertix());
    }
    return graph;
}

int main() {
    Graph *graph = graphInit();
    graph->SCC();
    cout << *graph;
    return 0;
}