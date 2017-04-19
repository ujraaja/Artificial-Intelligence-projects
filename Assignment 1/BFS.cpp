#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <queue>
#include <cassert>
using namespace std;

#define FOR(i,a,b) for(int i=a;i<b;++i)
#define SFOR(i,a,b,c) for(int i=a;i<b;i+=c)
#define REP(i,n) FOR(i,0,n)
#define RFOR(i,a,b) for(int i=a;i>=b;i--)
#define RREP(i,n) RFOR(i,n-1,0)
#define ECH(it, v) for(auto it=v.begin();it!=v.end();++it)
#define ALL(x) (x).begin(),(x).end()
#define SRT(x) sort(ALL(x))
#define CLR(x) memset(x,0,sizeof(x))
#define SET(x) memset(x,-1,sizeof(x))
#define MOD 1000000007
typedef unsigned int UI;
typedef long long LL;
typedef unsigned long long UL;
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef vector<string> VS;
typedef pair<int, int> PI;

#define MAX 275

bool edge[MAX][MAX], isVisited[MAX];
int numVertices, numEdges, iterations, maxFrontierSize, verticesVisited;

class Node {
    public:
        vector<int> v;
        Node* parent;
        int depth;
        float heur;
        Node() {}
        Node(vector<int> I) {
            v = I;
        }
        Node(vector<int> I, Node* p) {
            v = I;
            parent = p;
        }
        vector<Node*> successors();
        vector<Node*> traceback() {
            vector<Node*> vn;
            if(parent != NULL) vn = parent->traceback();
            vn.push_back(this);
            return vn;
        }
} node[MAX];

vector<Node*> Node::successors() {
    vector<Node*> s;
    REP(i, MAX)
        if(edge[i][v[0]])
            s.push_back(&node[i]);
    return s;
}

//BFS
Node* Search(Node* initial_state, Node* goal) {
    cout<<"Number of Vertices : "<<numVertices<<endl;
    cout<<"Number of Edges : "<<numEdges<<endl;
    cout<<"Start=("<<initial_state->v[1]<<", "<<initial_state->v[2]<<"), Goal=(";
    cout<<goal->v[1]<<", "<<goal->v[2]<<"), Vertices: "<<initial_state->v[0]<<" and "<<goal->v[0]<<endl;
    queue<Node*> frontier;
    frontier.push(initial_state);
    initial_state->depth = 0;
    initial_state->parent = NULL;
    isVisited[initial_state->v[0]] = true;
    iterations = 0;
    verticesVisited = 1;
    maxFrontierSize = frontier.size();
    while (!frontier.empty()) {
        iterations++;
        Node* current = frontier.front();
        frontier.pop();
        cout<<"iter="<<iterations<<", frontier="<<frontier.size()<<", popped="<<current->v[0]<<" (";
        cout<<current->v[1]<<", "<<current->v[2]<<"), depth="<<current->depth<<endl;
        if(current == goal)
            return goal;
        vector<Node*> successorList = current->successors();
        for(Node* nod : successorList) {
            int nodeID = nod->v[0];
            if(!isVisited[nodeID]) {
                nod->depth = current->depth + 1;
                nod->parent = current;
                frontier.push(nod);
                isVisited[nodeID] = true;
                verticesVisited++;
                cout<<"pushed "<<nod->v[0]<<" ("<<nod->v[1]<<", "<<nod->v[2]<<")\n";
            }
        }
        maxFrontierSize = max(maxFrontierSize, (int) frontier.size());
    }
    return NULL;
}

void solve() {
    cin>>numVertices;
    REP(i, numVertices) {
        int id, x, y;
        cin>>id>>x>>y;
        node[id].v = {id, x, y};
    }
    CLR(edge);
    CLR(isVisited);
    cin>>numEdges;
    REP(i, numEdges) {
        int id, v1, v2;
        cin>>id>>v1>>v2;
        edge[v1][v2] = edge[v2][v1] = true;
    }
    int id1, id2;
    cin>>id1>>id2;
    Node* goal = Search(&node[id1], &node[id2]);
    cout<<"---PATH---\n";
    vector<Node*> path = goal->traceback();
    REP(i, path.size()) {
        cout<<"Vertex : "<<path[i]->v[0]<<" ("<<path[i]->v[1]<<", "<<path[i]->v[2]<<")\n";
    }
    cout<<"------BFS------\n";
    if(goal == NULL) {
        cout<<"No path\n";
        return;
    }
    cout<<"Total Iterations : "<<iterations<<endl;
    cout<<"Max Frontier Size : "<<maxFrontierSize<<endl;
    cout<<"Vertices Visited : "<<verticesVisited<<" / "<<numVertices<<endl;
    cout<<"Path Length : "<<goal->depth<<endl;


    cout<<iterations<<endl;
    cout<<maxFrontierSize<<endl;
    cout<<verticesVisited<<endl;
    cout<<goal->depth<<endl;
}

int main() {
    freopen("input.txt", "r", stdin);
    solve();
}
