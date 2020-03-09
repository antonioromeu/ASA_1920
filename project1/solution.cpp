#include <iostream>
#include <typeinfo>
#include <list>
#include <stack>
#include <iterator>
#define NIL -1
using namespace std;

typedef struct node {
    int grade, disc, low;
    bool stackMember;
    struct node *next;
    //int nSCC;
} *Node;

class Graph {
    int nrNodes;
    list<Node> adj;
    //void SCCUtil(int u, int disc[], int low[], stack<int> *st, bool stackMember[]); 
 
    public:
    Graph(int V);
    void addGrade(int v, int grade);
    void addEdge(int v, Node w); 
    //void SCC();                     // Prints strongly connected components 
};

Graph::Graph(int nrNodes) {
    this->nrNodes = nrNodes;
    adj = new list<Node>[nrNodes];
    //for (list<Node>::iterator it = adj->begin(); it != adj->end(); ++it) {
    for (int i = 0; i < nrNodes; i++) {
        Node node = (Node) malloc (sizeof(struct node));
        node->grade = NIL;
        node->disc = NIL;
        node->low = NIL;
        node->stackMember = false;
        node->next = NULL;
        /*adj[i] = node;
        adj[i]->grade = NIL;
        adj[i]->disc = NIL;
        adj[i]->low = NIL;
        adj[i]->stackMember = false;
        //(*it)->nSCC = NIL;
        */
    }
}

void Graph::addGrade(int v, int grade) {
    adj[v]->grade = NIL;
}

void Graph::addEdge(int v, Node w) {
    adj[v].push_back(w);
}

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
    int *grades;
    if (!scanf("%d,%d", &nFriends, &nRelationships)) {
        printf("Error on scanf\n");
        exit(EXIT_FAILURE);
    }
    if (nFriends < 2 || nRelationships < 1) {
        printf("Error on input\n");
        exit(EXIT_FAILURE);
    }
    grades = (int*) malloc(sizeof(int) * nFriends);
    for (int i = 0; i < nFriends; i++) {
        scanf("%d", &grades[i]);
    }
    Graph *graph = new Graph(nFriends);
    
    return 0;
}
