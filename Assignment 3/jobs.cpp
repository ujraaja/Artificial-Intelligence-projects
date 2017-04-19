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
#define REP(i,n) FOR(i,0,n)
#define CLR(x) memset(x,0,sizeof(x))
#define SET(x) memset(x,-1,sizeof(x))

int person[8]; //represents person for each job
int numIterations;

bool consistencyCheck() {
    numIterations++;
    int jobsPerPerson[4];
    CLR(jobsPerPerson);
    REP(i, 8) if(person[i] != -1) jobsPerPerson[person[i]]++;
    REP(i, 4) if(jobsPerPerson[i] > 2) return false;    //check each person holds 2 jobs max
    if(person[2] != -1 && person[2] < 2) return false; //nurse by male
    if(person[0] > 1) return false; //chef by female
    if(person[3] != -1 && person[3] < 2) return false; //clerk by male
    if(person[7] == 0) return false;    //Roberta is not boxer
    if(person[2] == 3) return false;    //Pete is not nurse
    if(person[5] == 3) return false;    //Pete is not teacher
    if(person[4] == 3) return false;    //Pete is not police
    if(person[0] == 0) return false;    //Roberta is not chef
    if(person[4] == 0) return false;    //Roberta is not police
    if(person[0] == person[4]) return false;    //chef and police are different
    return true;
}

bool solve() {
    bool flag = true;
    REP(i, 8) if(person[i] == -1) flag = false;
    if(flag) return flag;
    REP(i, 8) {
        if(person[i] == -1) {
            REP(j, 4) {
                person[i] = j;
                if(consistencyCheck()) {
                    flag = solve();
                    if(flag) return flag;
                    person[i] = -1;
                } else person[i] = -1;
            }
            if(person[i] == -1) return false;
        }
    }
}

map<int, string> jobs, persons;
void populateMaps() {
    jobs[0] = "chef";
    jobs[1] = "guard";
    jobs[2] = "nurse";
    jobs[3] = "clerk";
    jobs[4] = "police";
    jobs[5] = "teacher";
    jobs[6] = "actor";
    jobs[7] = "boxer";
    persons[0] = "Roberta";
    persons[1] = "Thelma";
    persons[2] = "Steve";
    persons[3] = "Pete";
}

int main() {
    populateMaps();
    REP(i, 8) person[i] = -1;
    numIterations = 0;
    if(solve()) {
        cout<<"Found the following mapping!\n";
        REP(i, 8)
            cout<<jobs[i]<<" is held by "<<persons[person[i]]<<endl;
        cout<<"Number of iterations: "<<numIterations<<endl;
    } else {
        cout<<"Sorry couldn't find a mapping\n";
    }
}
