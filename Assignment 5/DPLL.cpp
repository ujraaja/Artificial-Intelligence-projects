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

map<string, int> indexMap;
map<int, string> symbolMap;
map<LL, int> kb;
map<int, LL> kb2;
int numIterations = 0;

LL encodeCNF(string s) {
    LL ret = 0;
    int in = -1;
    bool neg = false;
    string symbol = "";
    while(in++ < (int)s.size()) {
        if(s[in] == '-') {
            assert(symbol == "");
            neg = true;
        } else if((s[in] >= 'a' && s[in] <= 'z') || (s[in] >= 'A' && s[in] <= 'Z') || (s[in] >= '0' && s[in] <= '9')) {
            symbol += s[in];
        } else if(symbol.length() != 0 && ((symbol[0] >= 'a' && symbol[0] <= 'z') || (symbol[0] >= 'A' && symbol[0] <= 'Z') || (symbol[0] >= '0' && symbol[0] <= '9'))) {
            if(indexMap.find(symbol) == indexMap.end()) {
                int id = indexMap.size();
                indexMap[symbol] = id;
                symbolMap[id] = symbol;
            }
            ret |= 1ll<<(2*indexMap[symbol]);
            if(neg) ret |= 1ll<<(2*indexMap[symbol]+1);
            symbol = "";
            neg = false;
        }
    }
    return ret;
}

void readFile() {
    bool prevLineEmpty = false;
    string line;
    while(true) {
        getline(cin, line);
        if(line.length() <= 1) {
            if(prevLineEmpty) return;
            prevLineEmpty = true;
        } else {
            prevLineEmpty = false;
            if(line[0] != '#' && line[0] != ' ') {
                int id = kb.size();
                LL index = encodeCNF(line);
                kb[index] = id;
                kb2[id] = index;
            }
        }
    }
}

LL truthValues = -1;
void printModel() {
    cout<<"model = {";
    REP(i, symbolMap.size()) {
        if((3 & (truthValues>>(i*2))) != 3)
            cout<<symbolMap[i]<<": "<<((3 & (truthValues>>(i*2)))==0?"false":"true")<<", ";
    }
    cout<<"}\n";
}

bool isSatisfiable() {
    REP(i, kb2.size()) {
        LL cnf = kb2[i];
        bool flag = false;
        REP(j, symbolMap.size()) {
            int truthValue = (3 & (truthValues>>(j*2))), cnfValue = (3 & (cnf>>(j*2)));
            if(truthValue == 3) flag = true;
            else if(truthValue == 0 && cnfValue == 3) flag = true;
            else if(truthValue == 1 && cnfValue == 1) flag = true;
        }
        if(!flag) {
            cout<<"Backtracking\n";
            return false;
        }
    }
    return true;
}

int pureSymbol() {
    REP(i, symbolMap.size()) {
        if((3&(truthValues>>2*i)) != 3) continue;
        bool flag = true;
        int neg = -1;
        REP(j, kb2.size()) {
            if(3&(kb2[j]>>2*i)) {
                bool dontCare = false;
                REP(k, symbolMap.size()) {
                    int currentValue = 3&(kb2[j]>>2*k);
                    if(currentValue != 0 && (currentValue == 3? 0:1) == (3&(truthValues>>2*k)))
                        dontCare = true, k = MOD;
                }
                if(!dontCare)
                    if(neg == -1) neg = (3&(kb2[j]>>2*i));
                    else if(neg != (3&(kb2[j]>>2*i))) flag = false, j = MOD;
            }
        }
        if(flag) {
            cout<<"Pure Symbol : "<<symbolMap[i]<<" = "<<(neg == 3? "false" : "true")<<endl;
            return ((i<<2)|neg);
        }
    }
    return -1;
}

void printClause(int index) {
    LL cnf = kb2[index];
    REP(j, symbolMap.size()) {
        int neg = 3&(cnf>>2*j);
        if(neg) {
            if(neg == 3) cout<<"-";
            cout<<symbolMap[j]<<" ";
        }
    }
}

int unitClause() {
    REP(i, kb2.size()) {
        int ret = -1;
        REP(j, symbolMap.size()) {
            int currentValue = (3&(kb2[i]>>2*j)), truthValue = (3&(truthValues>>2*j));
            if(currentValue && truthValue == 3) {
                if(ret == -1) ret = ((j<<2)|currentValue);
                else ret = -1, j = MOD;
            } else if(currentValue && truthValue == (currentValue == 3? 0:1)) ret = -1, j = MOD;
        }
        if(ret != -1) {
            cout<<"Unit Clause on {";
            printClause(i);
            cout<<"}: "<<symbolMap[ret>>2]<<" = "<<((ret&3) == 3? "false" : "true")<<endl;
            return ret;
        }
    }
    return -1;
}

//DPLL
bool solve() {
    numIterations++;
    printModel();
    int index = -1;
    REP(i, symbolMap.size())
        if((3&(truthValues>>2*i)) == 3) index = i;
    if(index == -1) return isSatisfiable();
    if(!isSatisfiable()) return false;
    int ret = pureSymbol();
    if(ret == -1) ret = unitClause();
    if(ret != -1) {
        LL value = 3&ret;
        if(value == 3) value = 0;
        index = ret>>2;
        truthValues &= ~(3ll<<2*index);
        truthValues |= value<<2*index;
        if(solve()) return true;
        truthValues |= 3ll<<2*index;
        return false;
    }
    truthValues &= ~(3ll<<2*index);
    cout<<"Trying : "<<symbolMap[index]<<" = false"<<endl;
    if(isSatisfiable() && solve()) return true;
    truthValues |= 1ll<<2*index;
    cout<<"Trying : "<<symbolMap[index]<<" = true"<<endl;
    if(isSatisfiable() && solve()) return true;
    truthValues |= 3ll<<2*index;
    return false;
}

int main() {
    freopen("input.txt", "r", stdin);
    readFile();
    if(solve()) {
        cout<<"\n\nSatisfiable!\n\nTrue props:\n";
        REP(i, symbolMap.size())
            if((3&(truthValues>>2*i)) == 1)
                cout<<symbolMap[i]<<endl;
    }
    else cout<<"\n\nNot satisfiable\n";
    cout<<endl<<"# iterations : "<<numIterations<<endl;
}
