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
        SFOR(i, 0, 64, 2)
            if(c & (1ll<<i)) score1++;
        c = b.first | b.second;
        SFOR(i, 0, 64, 2)
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
int numIterations = 0, maxQSize = 0;

LL encodeCNF(string s) {
    LL ret = 0;
    int in = -1;
    bool neg = false;
    string symbol = "";
    while(in++ < (int)s.size()) {
        if(s[in] == ' ' || s[in] == 0) {
            if(indexMap.find(symbol) == indexMap.end()) {
                int id = indexMap.size();
                indexMap[symbol] = id;
                symbolMap[id] = symbol;
            }
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
    }
    return ret;
}

string decodeCNF(LL c) {
    string ret = "";
    SFOR(i, 0, 64, 2) {
        if(c & (1ll<<i)) {
            if(c & (1ll<<(i+1))) ret += '-';
            ret += symbolMap[i/2] + ' ';
        }
    }
    return ret;
}

void readFile() {
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
    SFOR(i, 0, 64, 2)
        if(!(xr&(1ll<<i)) && (xr&(1ll<<(i+1))))
            ret.push_back((a|b)^((1ll<<(i+1))|(1ll<<i)));
    return ret;
}

bool isResolvable(LL a, LL b) {
    LL xr = a^b;
    bool flag = false;
    SFOR(i, 0, 64, 2)
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
        numIterations++;
        maxQSize = max(maxQSize, (int)q.size());
        if(q.size() > 1e6) break;
        pair<LL, LL> top = q.top();
        q.pop();
        vector<LL> newKB = resolve(top.first, top.second);
        for(LL k : newKB) {
            if(kb.find(k) == kb.end()) {
                int id = kb.size();
                kb[k] = id;
                kb2[id] = k;
                parents[id] = {min(kb[top.first], kb[top.second]), max(kb[top.first], kb[top.second])};
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
    if(parent2 == -2) {
        cout<<id<<". "<<decodeCNF(kb2[id])<<"\t\t[input]\n";
    } else {
        printResult(parent1);
        printResult(parent2);
        cout<<id<<". "<<decodeCNF(kb2[id])<<"\t\t["<<parent1<<", "<<parent2<<"]\n";
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    readFile();
    fillQ();
    if(solve()) {
        cout<<"success!\n";
        printResult(kb[0]);
    }
    else cout<<"Not entailed\n";
    cout<<endl<<"# iterations : "<<numIterations<<endl;
    cout<<"Max queue size : "<<maxQSize<<endl;
}
