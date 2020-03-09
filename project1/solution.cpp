#include <iostream>
#include <typeinfo>
#include <list>
#include <vector>
#include <stack>
#include <iterator>
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
    
    int _nrNodes;
    vector<Node*> _nodes;

    public:
    Graph(int nrNodes) { _nrNodes = nrNodes; }

    void addNode(Node* node) { _nodes.push_back(node); }
 
    vector<Node*> getNodes() { return _nodes; }

};

/*
// A recursive function that finds and prints strongly connected components using DFS traversal 
// u                --> The vertex to be visited next 
// disc[]           --> Stores discovery times of visited vertices 
// low[]            --> Earliest visited vertex (the vertex with minimum discovery time) that can be reached from subtree rooted with current vertex 
// *st              --> To store all the connected ancestors (could be part of SCC)
// stackMember[]    --> Bit/index array for faster check whether a node is in stack
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

int main(int argc, char* argv[]) {
    int nFriends;
    int nRelationships;
    int grade, from, to;
    if (!scanf("%d,%d", &nFriends, &nRelationships)) {
        printf("Error on scanf\n");
        exit(EXIT_FAILURE);
    }
    if (nFriends < 2 || nRelationships < 1) {
        printf("Error on input\n");
        exit(EXIT_FAILURE);
    }
    Graph *graph = new Graph(nFriends);
    
    // Creates vertices and adds them to graph
    for (int i = 0; i < nFriends; i++) {
        scanf("%d", &grade);
        Vertix* v = new Vertix(grade);
        Node* n = new Node(v);
        graph->addNode(n);
        //cout << n << endl;
    }
    //cout << "-----" << endl;
/*
    for (Node *n: graph->getNodes()){
        cout << n << endl;

    }*/
    list<Node*>::iterator it;
    //Node* node;
    //Vertix* vertix;
    //it = graph->getNodes().begin();
    for (int i = 0; i < nRelationships; i++) {
        scanf("%d %d", &from, &to);
        graph->getNodes().at(from - 1)->addAdj(graph->getNodes().at(to-1)->getVertix());
    }
    return 0;
}
