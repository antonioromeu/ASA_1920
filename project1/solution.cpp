#include <iostream>
#include <list>
#include <vector>
#include <stack>
#define NIL -1
using namespace std;

class Vertix {
    int _grade, _disc, _low;
    bool _stackMember;

    public:
    Vertix(int grade) {
        _grade = grade;
        _disc = NIL;
        _low = NIL;
        _stackMember = false;
    }

    void setGrade(int grade) { _grade = grade; }

    void setDisc(int disc) { _disc = disc; }

    void setLow(int low) { _low = low; }

    void setStackMember(bool b) { _stackMember = b; }

    int getGrade() { return _grade; }

    int getDisc() { return _disc; }

    int getLow() { return _low; }
    
    int getStackMember() { return _stackMember; }
};

class Node {
    Vertix* _vertix;
    list<Vertix*> _adj;

    public:
    Node(Vertix* vertix) { _vertix = vertix; }

    void setVertix(Vertix* vertix) { _vertix = vertix; }

    void addAdj(Vertix* vertix) { _adj.push_back(vertix); }

    Vertix* getVertix() const { return _vertix; }

    list<Vertix*> getAdj() { return _adj; }

    friend ostream &operator<<(ostream &o, const Node &node) {
        o << "nota " << node.getVertix()->getGrade() << endl;
        return o;
    }
};

class Graph {
    int _numberNodes;
    vector<Node*> _nodes;

    public:
    Graph(int numberNodes) { _numberNodes = numberNodes; }

    void addNode(Node* node) { _nodes.push_back(node); }
 
    vector<Node*> getNodes() { return _nodes; }
};

int main() {
    int students, relationships, grade, from, to;
    
    if (!scanf("%d,%d", &students, &relationships)) { // Scans the first line of input
        printf("Error on scanf\n");
        exit(EXIT_FAILURE);
    }
 
    if (students < 2 || relationships < 1) {
        printf("Error on input\n");
        exit(EXIT_FAILURE);
    }

    Graph *graph = new Graph(students);

    for (int i = 0; i < students; i++) { // Creates vertices and adds them to graph
        scanf("%d", &grade);
        Vertix* v = new Vertix(grade);
        Node* n = new Node(v);
        graph->addNode(n);
    }

    for (int i = 0; i < relationships; i++) { // Adds the relationships to the adjacencies list
        scanf("%d %d", &from, &to);
        graph->getNodes().at(from-1)->addAdj(graph->getNodes().at(to-1)->getVertix());
    }

    return 0;
}

/*
// A recursive function that finds and prints strongly connected components using DFS traversal 
void Graph::SCCUtil(int u, int disc[], int low[], stack<int> *st, bool stackMember[]) {
    // A static variable is used for simplicity, we can avoid use of static variable by passing a pointer.
    static int time = 0;

    // Initialize discovery time and low value 
    disc[u] = low[u] = ++time;
    st->push(u);
    stackMember[u] = true; 

    // Go through all vertices adjacent to this 
    list<int>::iterator i; 
    for (i = adj[u].begin(); i != adj[u].end(); ++i) {
        int v = *i;  // v is current adjacent of 'u'
        
        // If v is not visited yet, then recur for it 
        if (disc[v] == -1) { 
            SCCUtil(v, disc, low, st, stackMember); 

            // Check if the subtree rooted with 'v' has a connection to one of the ancestors of 'u' 
            // Case 1 (per above discussion on Disc and Low value) 
            low[u] = min(low[u], low[v]); 
        }
  
        // Update low value of 'u' only of 'v' is still in stack (i.e. it's a back edge, not cross edge). 
        // Case 2 (per above discussion on Disc and Low value)
        else if (stackMember[v] == true)
            low[u] = min(low[u], disc[v]);
    }

    // head node found, pop the stack and print an SCC 
    int w = 0;  // To store stack extracted vertices 
    if (low[u] == disc[u]) {
        while (st->top() != u) {
            w = (int) st->top();
            cout << w << " ";
            stackMember[w] = false;
            st->pop();
        }
        w = (int) st->top();
        cout << w << "\n";
        stackMember[w] = false;
        st->pop();
    }
}

// The function to do DFS traversal. It uses SCCUtil()
void Graph::SCC() {
    int *disc = new int[V];
    int *low = new int[V];
    bool *stackMember = new bool[V];
    stack<int> *st = new stack<int>();

    // Initialize disc and low, and stackMember arrays
    for (int i = 0; i < V; i++) {
        disc[i] = NIL;
        low[i] = NIL;
        stackMember[i] = false;
    }

    // Call the recursive helper function to find strongly
    // connected components in DFS tree with vertex 'i'
    for (int i = 0; i < V; i++)
        if (disc[i] == NIL)
            SCCUtil(i, disc, low, st, stackMember);
}
*/
