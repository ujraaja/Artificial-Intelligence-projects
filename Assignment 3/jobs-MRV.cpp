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
set<int> remValues[8];   //represents the domain of each job
int numIterations;

void updateRemValues() {
    REP(i, 8) remValues[i].clear();
    int jobsPerPerson[4];
    CLR(jobsPerPerson);
    REP(i, 8) if(person[i] != -1) jobsPerPerson[person[i]]++;
    if(person[0] == -1) {
        //chef by female
        if(jobsPerPerson[0] < 2) remValues[0].insert(0);
        if(jobsPerPerson[1] < 2) remValues[0].insert(1);
        //Roberta is not chef
        remValues[0].erase(0);
        //chef and police are different
        REP(i, 4) if(person[4] == i) remValues[0].erase(i);
    }
    if(person[1] == -1) {
        //no restriction on guard
        REP(i, 4) if(jobsPerPerson[i] < 2) remValues[1].insert(i);
    }
    if(person[2] == -1) {
        //nurse by male
        if(jobsPerPerson[2] < 2) remValues[2].insert(2);
        if(jobsPerPerson[3] < 2) remValues[2].insert(3);
        //Pete is not nurse
        remValues[2].erase(3);
    }
    if(person[3] == -1) {
        //clerk by male
        if(jobsPerPerson[2] < 2) remValues[3].insert(2);
        if(jobsPerPerson[3] < 2) remValues[3].insert(3);
    }
    if(person[4] == -1) {
        REP(i, 4) if(jobsPerPerson[i] < 2) remValues[4].insert(i);
        //Pete is not police
        remValues[4].erase(3);
        //Roberta is not police
        remValues[4].erase(0);
        //chef and police are different
        REP(i, 4) if(person[0] == i) remValues[4].erase(i);
    }
    if(person[5] == -1) {
        REP(i, 4) if(jobsPerPerson[i] < 2) remValues[5].insert(i);
        //Pete is not teacher
        remValues[5].erase(3);
    }
    if(person[6] == -1) {
        //no restriction on actor
        REP(i, 4) if(jobsPerPerson[i] < 2) remValues[6].insert(i);
    }
    if(person[7] == -1) {
        REP(i, 4) if(jobsPerPerson[i] < 2) remValues[7].insert(i);
        //Roberta is not boxer
        remValues[7].erase(0);
    }
}

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
    updateRemValues();
    int mrv = -1, mrValue = 5;
    REP(i, 8) if(person[i] == -1 && mrValue > remValues[i].size()) mrv = i, mrValue = remValues[i].size();
    set<int> rvbk = remValues[mrv];
    for(int j : rvbk) {
        person[mrv] = j;
        if(consistencyCheck()) {
            flag = solve();
            if(flag) return flag;
        }
        person[mrv] = -1;
    }
    return false;
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
