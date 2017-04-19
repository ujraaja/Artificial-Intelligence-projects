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

int numIterations;
//pets, colors, nations, drinks, chocs
int assignment[5][5]; //represents the values for each house from left to right

bool consistencyCheck() {
    numIterations++;
    //same value can't exist for 2 houses
    REP(i, 5) FOR(j, i+1, 5) REP(k, 5) if(assignment[i][k] != -1 && assignment[i][k] == assignment[j][k]) return false;
    //Englishman lives in the red house
    REP(i, 5) {
        if(assignment[i][2] != -1 && assignment[i][1] != -1)
            if(assignment[i][2] == 0 && assignment[i][1] != 0 || assignment[i][2] != 0 && assignment[i][1] == 0)
                return false;
    }
    //Spaniard owns the dog
    REP(i, 5) {
        if(assignment[i][2] != -1 && assignment[i][0] != -1)
            if(assignment[i][2] == 1 && assignment[i][0] != 0 || assignment[i][2] != 1 && assignment[i][0] == 0)
                return false;
    }
    //Norwegian lives in the first house on the left
    if(assignment[0][2] != -1 && assignment[0][2] != 2) return false;
    FOR(i, 1, 5) if(assignment[i][2] == 2) return false;
    //green house is immediately to the right of the ivory house
    if(assignment[4][1] == 2 || assignment[0][1] == 1) return false;
    REP(i, 4) {
        if(assignment[i][1] != -1 && assignment[i+1][1] != -1)
            if(assignment[i][1] == 2 && assignment[i+1][1] != 1 || assignment[i][1] != 2 && assignment[i+1][1] == 1)
                return false;
    }
    //man who eats Hershey bars lives in the house next to the man with the fox
    REP(i, 5) {
        if(assignment[i][4] == 0) {
            int n1Pet, n2Pet;
            if(i == 0) n1Pet = n2Pet = assignment[i+1][0];
            else if(i == 4) n1Pet = n2Pet = assignment[i-1][0];
            else n1Pet = assignment[i-1][0], n2Pet = assignment[i+1][0];
            if(n1Pet != -1 && n2Pet != -1 && n1Pet != 1 && n2Pet != 1) return false;
        }
    }
    //Kits Kats are eaten in the yellow house
    REP(i, 4) {
        if(assignment[i][4] != -1 && assignment[i][1] != -1)
            if(assignment[i][4] == 1 && assignment[i][1] != 3 || assignment[i][4] != 1 && assignment[i][1] == 3)
                return false;
    }
    //Norwegian lives next to the blue house
    REP(i, 5) {
        if(assignment[i][2] == 2) {
            int n1Color, n2Color;
            if(i == 0) n1Color = n2Color = assignment[i+1][1];
            else if(i == 4) n1Color = n2Color = assignment[i-1][1];
            else n1Color = assignment[i-1][1], n2Color = assignment[i+1][1];
            if(n1Color != -1 && n2Color != -1 && n1Color != 4 && n2Color != 4) return false;
        }
    }
    //Smarties eater owns snails
    REP(i, 5) {
        if(assignment[i][4] != -1 && assignment[i][0] != -1)
            if(assignment[i][4] == 2 && assignment[i][0] != 2 || assignment[i][4] != 2 && assignment[i][0] == 2)
                return false;
    }
    //Snickers eater drinks orange juice
    REP(i, 5) {
        if(assignment[i][4] != -1 && assignment[i][3] != -1)
            if(assignment[i][4] == 3 && assignment[i][3] != 4 || assignment[i][4] != 3 && assignment[i][3] == 4)
                return false;
    }
    //Ukranian drinks tea
    REP(i, 5) {
        if(assignment[i][2] != -1 && assignment[i][3] != -1)
            if(assignment[i][2] == 3 && assignment[i][3] != 0 || assignment[i][2] != 3 && assignment[i][3] == 0)
                return false;
    }
    //Japanese person eats Milky Ways
    REP(i, 5) {
        if(assignment[i][2] != -1 && assignment[i][4] != -1)
            if(assignment[i][2] == 4 && assignment[i][4] != 4 || assignment[i][2] != 4 && assignment[i][4] == 4)
                return false;
    }
    //Kit Kats are eaten in a house next to the house where the horse is kept
    REP(i, 5) {
        if(assignment[i][4] == 1) {
            int n1Pet, n2Pet;
            if(i == 0) n1Pet = n2Pet = assignment[i+1][0];
            else if(i == 4) n1Pet = n2Pet = assignment[i-1][0];
            else n1Pet = assignment[i-1][0], n2Pet = assignment[i+1][0];
            if(n1Pet != -1 && n2Pet != -1 && n1Pet != 3 && n2Pet != 3) return false;
        }
    }
    //Coffee is drunk in the green house
    REP(i, 5) {
        if(assignment[i][3] != -1 && assignment[i][1] != -1)
            if(assignment[i][3] == 2 && assignment[i][1] != 1 || assignment[i][3] != 2 && assignment[i][1] == 1)
                return false;
    }
    //Milk is drunk in the middle house
    REP(i, 5) {
        if(i != 2 && assignment[i][3] == 1 || i == 2 && assignment[i][3] != -1 && assignment[i][3] != 1)
            return false;
    }
    return true;
}

bool solve() {
    bool flag = true;
    REP(i, 5) REP(j, 5) if(assignment[i][j] == -1) flag = false;
    if(flag) return flag;
    REP(i, 5) REP(j, 5) {
        if(assignment[i][j] == -1) {
            REP(k, 5) {
                assignment[i][j] = k;
                if(consistencyCheck()) {
                    flag = solve();
                    if(flag) return flag;
                }
                assignment[i][j] = -1;
            }
        }
        if(assignment[i][j] == -1) return false;
    }
}

map<int, string> maps[5];   //pets, colors, nations, drinks, chocs
void populateMaps() {
    maps[0][0] = "dog";
    maps[0][1] = "fox";
    maps[0][2] = "snail";
    maps[0][3] = "horse";
    maps[0][4] = "zebra";
    maps[1][0] = "red";
    maps[1][1] = "green";
    maps[1][2] = "ivory";
    maps[1][3] = "yellow";
    maps[1][4] = "blue";
    maps[2][0] = "Englishman";
    maps[2][1] = "Spaniard";
    maps[2][2] = "Norwegian";
    maps[2][3] = "Ukrainian";
    maps[2][4] = "Japanese";
    maps[3][0] = "tea";
    maps[3][1] = "milk";
    maps[3][2] = "coffee";
    maps[3][3] = "water";
    maps[3][4] = "juice";
    maps[4][0] = "Hershey";
    maps[4][1] = "Kit Kat";
    maps[4][2] = "Smarties";
    maps[4][3] = "Snickers";
    maps[4][4] = "Milky Way";
}
//pets, colors, nations, drinks, chocs

int main() {
    SET(assignment);
    populateMaps();
    numIterations = 0;
    if(solve()) {
        cout<<"Found the following mapping!\n";
        cout<<"\t\tPET\t\tCOLOR\t\tNATIONALITY\t\tDRINK\t\tCHOCOLATE\n";
        cout<<"--------------------------------------------------------------------------------------------------\n";
        REP(i, 5) {
            cout<<"House#"<<i+1<<"\t\t";
            REP(j, 5) cout<<maps[j][assignment[i][j]]<<"\t\t";
            cout<<endl;
        }
        cout<<"Number of iterations: "<<numIterations<<endl;
    } else {
        cout<<"Sorry couldn't find a mapping\n";
    }
}
