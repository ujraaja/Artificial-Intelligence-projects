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

#define SD(i) scanf("%d", &(i))
#define SDD(i, j) scanf("%d%d", &(i), &(j))
#define SDDD(i, j, k) scanf("%d%d%d", &(i), &(j), &(k))
#define SDDDD(i, j, k, l) scanf("%d%d%d%d", &(i), &(j), &(k), &(l))
#define SL(i) scanf("%lld", &(i))
#define SC(i) scanf(" %c", &(i))
#define SS(i) scanf(" %s", &(i))
#define PD(i) printf("%d", i)
#define PL(i) printf("%lld", i)
#define PC(i) printf("%c", i)
#define PS(i) printf("%s", i)
#define PN printf("\n");

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

class compare {
public:
    bool operator() (pair<LL, LL> a, pair<LL, LL> b) {
        LL c = a.first | a.second;
        int score1 = 0, score2 = 0;
        SFOR(i, 0, 62, 2)
            if(c & (1ll<<i)) score1++;
        c = b.first | b.second;
        SFOR(i, 0, 62, 2)
            if(c & (1ll<<i)) score2++;
        return score1 > score2;
    }
};

map<string, int> indexMap;
map<int, string> symbolMap;
map<LL, int> kb;
map<int, LL> kb2;
map<int, PI> parents;
priority_queue<pair<LL, LL>, vector<pair<LL, LL>>, compare> q;

LL encodeCNF(string s) {
    LL ret = 0;
    int in = 0;
    bool neg = false;
    string symbol = "";
    while(in < s.size()) {
        if(s[in] == ' ') {
            ret |= 1ll<<(2*indexMap[symbol]);
            if(neg) ret |= 1ll<<(2*indexMap[symbol]+1);
            symbol = "";
            neg = false;
        } else if(s[in] == '-') {
            assert(symbol == "");
            neg = true;
        } else {
            symbol += s[in];
        }
        in++;
    }
    ret |= 1ll<<(2*indexMap[symbol]);
    if(neg) ret |= 1ll<<(2*indexMap[symbol]+1);
    return ret;
}

string decodeCNF(LL c) {
    string ret = "";
    SFOR(i, 0, 62, 2) {
        if(c & (1ll<<i)) {
            if(c & (1ll<<(i+1))) ret += '-';
            ret += symbolMap[i/2] + ' ';
        }
    }
    return ret;
}

void fillSymbols() {
    symbolMap[0] = "C1Y";
    symbolMap[1] = "C1W";
    symbolMap[2] = "C1B";
    symbolMap[3] = "C2Y";
    symbolMap[4] = "C2W";
    symbolMap[5] = "C2B";
    symbolMap[6] = "C3Y";
    symbolMap[7] = "C3W";
    symbolMap[8] = "C3B";
    symbolMap[9] = "O1Y";
    symbolMap[10] = "O1W";
    symbolMap[11] = "O2Y";
    symbolMap[12] = "O2W";
    symbolMap[13] = "O3Y";
    symbolMap[14] = "O3W";
    symbolMap[15] = "L1Y";
    symbolMap[16] = "L1W";
    symbolMap[17] = "L1B";
    symbolMap[18] = "L2Y";
    symbolMap[19] = "L2W";
    symbolMap[20] = "L2B";
    symbolMap[21] = "L3Y";
    symbolMap[22] = "L3W";
    symbolMap[23] = "L3B";

    for(auto pr : symbolMap)
        indexMap[pr.second] = pr.first;
}

void populateConstraints() {
    vector<string> constraints;
    constraints.push_back("C1Y C2Y C3Y");
    constraints.push_back("C1W C2W C3W");
    constraints.push_back("C1B C2B C3B");

    constraints.push_back("C1Y C1W C1B");
    constraints.push_back("C2Y C2W C2B");
    constraints.push_back("C3Y C3W C3B");

    constraints.push_back("-C1Y -C2Y");
    constraints.push_back("-C1Y -C3Y");
    constraints.push_back("-C1W -C2W");
    constraints.push_back("-C1W -C3W");
    constraints.push_back("-C1B -C2B");
    constraints.push_back("-C1B -C3B");
    constraints.push_back("-C2Y -C3Y");
    constraints.push_back("-C2W -C3W");
    constraints.push_back("-C2B -C3B");

    constraints.push_back("-O1Y C1Y C1B");
    constraints.push_back("-O1W C1W C1B");
    constraints.push_back("-O2Y C2Y C2B");
    constraints.push_back("-O2W C2W C2B");
    constraints.push_back("-O3Y C3Y C3B");
    constraints.push_back("-O3W C3W C3B");

    constraints.push_back("-L1Y -C1Y");
    constraints.push_back("-L1W -C1W");
    constraints.push_back("-L1B -C1B");
    constraints.push_back("-L2Y -C2Y");
    constraints.push_back("-L2W -C2W");
    constraints.push_back("-L2B -C2B");
    constraints.push_back("-L3Y -C3Y");
    constraints.push_back("-L3W -C3W");
    constraints.push_back("-L3B -C3B");

    for(auto con : constraints) {
        int id = kb.size();
        LL index = encodeCNF(con);
        kb[index] = id;
        kb2[id] = index;
        parents[id] = {-1, -1};
    }
}

void readFacts() {
    bool prevLineEmpty = false;
    string line;
    while(true) {
        getline(cin, line);
        if(line.length() == 0) {
            if(prevLineEmpty) return;
            prevLineEmpty = true;
        } else {
            prevLineEmpty = false;
            if(line[0] != '#' && line[0] != ' ') {
                int id = kb.size();
                LL index = encodeCNF(line);
                kb[index] = id;
                kb2[id] = index;
                parents[id] = {-2, -2};
            }
        }
    }
}

vector<LL> resolve(LL a, LL b) {
    vector<LL> ret;
    LL xr = a^b;
    SFOR(i, 0, 62, 2)
        if(!(xr&(1ll<<i)) && (xr&(1ll<<(i+1))))
            ret.push_back((a|b)^((1ll<<(i+1))|(1ll<<i)));
    return ret;
}

bool isResolvable(LL a, LL b) {
    LL xr = a^b;
    bool flag = false;
    SFOR(i, 0, 62, 2)
        if(!(xr&(1ll<<i)) && (xr&(1ll<<(i+1))))
            if(flag) return false;
            else flag = true;
    return flag;
}

void fillQ() {
    REP(i, kb2.size())
        FOR(j, i+1, kb2.size()) {
            if(isResolvable(kb2[i], kb2[j]))
                q.push({kb2[i], kb2[j]});
        }
}

bool solve() {
    while(!q.empty()) {
        pair<LL, LL> top = q.top();
        q.pop();
        vector<LL> newKB = resolve(top.first, top.second);
        for(LL k : newKB) {
            if(kb.find(k) == kb.end()) {
                int id = kb.size();
                kb[k] = id;
                kb2[id] = k;
                parents[id] = {kb[top.first], kb[top.second]};
                if(k == 0) return true;
                REP(i, id) {
                    if(isResolvable(k, kb2[i]))
                        q.push({k, kb2[i]});
                }
            }
        }
    }
    return false;
}

set<int> printedIDs;
void printResult(int id) {
    if(printedIDs.find(id) != printedIDs.end()) return;
    int parent1 = parents[id].first, parent2 = parents[id].second;
    printedIDs.insert(id);
    if(parent1 == -1) {
        cout<<id<<". "<<decodeCNF(kb2[id])<<"\t[initial constraints]\n";
    } else if(parent2 == -2) {
        cout<<id<<". "<<decodeCNF(kb2[id])<<"\t[input]\n";
    } else {
        printResult(parent1);
        printResult(parent2);
        cout<<id<<". "<<decodeCNF(kb2[id])<<"\t["<<parent1<<", "<<parent2<<"]\n";
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    fillSymbols();
    populateConstraints();
    readFacts();
    fillQ();
    if(solve()) {
        cout<<"success!\n";
        printResult(kb[0]);
    }
    else cout<<"Not entailed\n";
}
