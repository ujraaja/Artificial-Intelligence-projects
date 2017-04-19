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
typedef unsigned long long LL;
typedef unsigned long long UL;
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef vector<vector<char>> VVC;
typedef vector<char> VC;
typedef vector<string> VS;
typedef pair<int, int> PI;

int numGoalTests, maxQSize, numStacks, numBlocks;

LL getIndex(VVC stacks) {
    LL ret = 0;
    RREP(i, stacks.size()) {
        ret <<= 4;
        RREP(j, stacks[i].size()) {
            ret <<= 4;
            ret |= stacks[i][j] - 'A' + 1;
        }
    }
    return ret;
}

VVC getStacks(LL index) {
    VVC stacks;
    VC stck;
    while(index) {
        int t = index & 0xF;
        index >>= 4;
        if(t == 0) {
            stacks.push_back(stck);
            stck.clear();
        } else stck.push_back(t-1+'A');
    }
    if(!stck.empty()) stacks.push_back(stck);
    stck.clear();
    while(stacks.size() < numStacks) stacks.push_back(stck);
    return stacks;
}

int getSimpleHeuristic(VVC stacks) {
    int heur = numBlocks;
    REP(i, stacks[0].size())
        if(stacks[0][i] == i+'A') heur--;
    return heur;
}

int getHeur(VVC stacks) {
    int heur = 0, curIndex = 1, pointer = 0;
    while(pointer < stacks[0].size()) {
        if(stacks[0][pointer] == pointer+'A') pointer++;
        else break;
    }
    heur += 2*(stacks[0].size() - pointer);
    FOR(i, 1, stacks.size()) {
        curIndex = 1, pointer = 0;
        if(stacks[i].size()) heur++;
        while(curIndex < stacks[i].size()) {
            if(stacks[i][pointer] < stacks[i][curIndex]) heur+=2;
            else pointer = curIndex, heur++;
            curIndex++;
        }
    }
    return heur;
}

class Node {
    public:
        VVC stacks;
        LL index;
        int depth;
        int heur;
        Node* parent;
        vector<LL> successors() {
            vector<LL> ret;
            REP(i, stacks.size()) {
                if(stacks[i].size() == 0) continue;
                REP(j, stacks.size()) {
                    if(i == j) continue;
                    stacks[j].push_back(stacks[i].back());
                    stacks[i].pop_back();
                    ret.push_back(getIndex(stacks));
                    stacks[i].push_back(stacks[j].back());
                    stacks[j].pop_back();
                }
            }
            return ret;
        }
        vector<Node*> traceback() {
            vector<Node*> vn;
            if(parent != NULL) vn = parent->traceback();
            vn.push_back(this);
            return vn;
        }
} initialState, goalState;

void printStacks(VVC stacks) {
    cout<<endl;
    REP(i, stacks.size()) {
        cout<<i+1<<" | ";
        REP(j, stacks[i].size())
            cout<<stacks[i][j]<<" ";
        cout<<endl;
    }
    cout<<endl;
}

void printPath(vector<Node*> nodes) {
    for(Node* node : nodes)
        printStacks(node->stacks);
}

struct compare {
    bool operator()(Node* a, Node* b) const {
        if(a->heur+a->depth == b->heur+b->depth) return a->heur > b->heur;
        return a->heur+a->depth > b->heur+b->depth;
    }
};
vector<Node*> q;
map<LL, Node*> visitedNodes;

void aStar() {
    maxQSize = numGoalTests = 0;
    q.push_back(&initialState);
    while(!q.empty()) {
        numGoalTests++;
        maxQSize = max(maxQSize, (int) q.size());
        Node* node = q.front();
        pop_heap(q.begin(), q.end(), compare()); q.pop_back();
        if(goalState.index == node->index) {
            cout<<"success!\n";
            printPath(node->traceback());
            cout<<"Depth : "<<node->depth<<endl;
            cout<<"Total Goal Tests : "<<numGoalTests<<endl;
            cout<<"Max Queue Size : "<<maxQSize<<endl;
            return;
        }
        vector<LL> successorList = node->successors();
        for(LL succesorIndex : successorList) {
            if(visitedNodes.find(succesorIndex) == visitedNodes.end()) {
                Node* succesor = new Node();
                succesor->index = succesorIndex;
                succesor->depth = node->depth + 1;
                succesor->stacks = getStacks(succesorIndex);
                succesor->heur = getHeur(succesor->stacks);
                succesor->parent = node;
                visitedNodes[succesorIndex] = succesor;
                q.push_back(succesor); push_heap(q.begin(), q.end(), compare());
            } else {
                Node* oldNode = visitedNodes[succesorIndex];
                if(oldNode->depth > node->depth+1) {
                    oldNode->depth = node->depth + 1;
                    oldNode->parent = node;
                    make_heap(q.begin(), q.end(), compare());
                }
            }
        }
    }
}

#define randomInput
void prepareProblem() {
    #ifdef stdIO
        freopen("input.txt", "r", stdin);
        cin>>numStacks>>numBlocks;
        VC st;
        char line[60];
        cin.getline(line, 60);
        REP(i, numStacks) {
            cin.getline(line, 60);
            st.clear();
            int ind = 0;
            while(line[ind]) {
                if(line[ind] != ' ')
                    st.push_back(line[ind]);
                ind++;
            }
            initialState.stacks.push_back(st);
        }
    #elif defined randomInput
        numStacks = 3;
        numBlocks = 13;
        initialState.stacks.resize(numStacks);
        VC ch;
        REP(i, numBlocks) ch.push_back('A' + i);
        REP(i, numBlocks) {
            int randIndex = rand() % ch.size();
            int randStack = rand() % numStacks;
            initialState.stacks[randStack].push_back(ch[randIndex]);
            ch.erase(ch.begin() + randIndex);
        }
    #elif defined fixedInput
        numStacks = 3;
        numBlocks = 5;
        VC st = {'B'};
        initialState.stacks.push_back(st);
        st.clear();
        st = {'C', 'E'};
        initialState.stacks.push_back(st);
        st.clear();
        st = {'A', 'D'};
        initialState.stacks.push_back(st);
        st.clear();
    #endif
    initialState.depth = 0;
    initialState.heur = getHeur(initialState.stacks);
    initialState.index = getIndex(initialState.stacks);
    VC stck;
    REP(i, numBlocks) stck.push_back(i+'A');
    goalState.stacks.push_back(stck);
    stck.clear();
    FOR(i, 1, numStacks) goalState.stacks.push_back(stck);
    goalState.index = getIndex(goalState.stacks);
}

int main() {
    prepareProblem();
    aStar();
}
