#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

static const int X = 2;
static const int null = 0;

struct vertex {
    int low, high;
    int index;
    int val;
    int id;
    bool mark;
};

char op; // operation of target function
int N = 3; // the number of inputs
int id = N * 2 + 3; // node id
int order = 0; // BDD order number, separate nodes from another order BDD

struct vertex V[100000][10000];
int T[100][100];

vector<vector<int>> vlist(N + 2);

void Traverse( struct vertex v ) {
    printf("%d %d %d %d %d\n", v.id, v.index, v.low, v.high, v.val);

    if ( v.index == N + 1 ) return;
    Traverse(V[order][v.low]);
    Traverse(V[order][v.high]);
}

void push_vlist( struct vertex v ) {
    if ( v.mark == true ) return;

    int n = v.index;
    vlist[n].push_back(v.id);

    if ( n == N + 1 ) return;
    push_vlist(V[order][v.low]);
    push_vlist(V[order][v.high]);
}

int Reduce( struct vertex v ) {
    for ( int i = 0; i < N + 2; i++ ) {
        vlist[i].clear();
    }
    push_vlist(v);
    int nextid = 0;
    for ( int i = N + 1; i > 0; i-- ) {
        map<int, int> Q; // using stiring has larger time complexity than int ?

        for ( int j = 0; j < vlist[i].size(); j++ ) {
            int uid = vlist[i][j];
            if ( V[order][uid].index == N + 1 ) {
              int key = V[order][uid].val;
              if ( Q.find(key) == Q.end() ) {
                Q.insert(make_pair(key, uid)); 
                } else {
                    V[order][uid].id = Q[key];
                }
            } else {
                    int lowid = V[order][uid].low;
                    int highid = V[order][uid].high;
                    V[order][uid].low = V[order][lowid].id;
                    V[order][uid].high = V[order][highid].id;

                    if ( V[order][uid].low == V[order][uid].high ) {
                        V[order][uid].id = V[order][uid].low;
                    } else {
                        int key = V[order][uid].low * 10000 + V[order][uid].high; 
                        // express key as one box. When the number of V[] is large, incorrect
                        if ( Q.find(key) == Q.end() ) {
                            Q.insert(make_pair(key, uid));
                        } else {
                            V[order][uid].id = Q[key];
                    }
                }
            }
        }
    }
    return 0;
}

int Apply_step( struct vertex v1, struct vertex v2 ) {

    if ( T[v1.id][v2.id] != null ) return T[v1.id][v2.id];

    struct vertex &u = V[order][id];
    u.id = id++;
    u.mark = false;
    T[v1.id][v2.id] = u.id;

    if ( op == '+' ) {
        if ( v1.val == 1 | v2.val == 1 ) u.val = 1;
        else if ( v1.val == 0 && v2.val == 0 ) u.val = 0;
        else u.val = X;
    }
    if ( op == '*' ) {
        if ( v1.val == 0 | v2.val == 0 ) u.val = 0;
        else if ( v1.val == 1 && v2.val == 1 ) u.val = 1;
        else u.val = X;
    }

    if ( u.val != X ) {
        u.index = N + 1; u.low = null; u.high = null;
    } else {
        u.index = min(v1.index, v2.index);
        struct vertex vlow1;
        struct vertex vlow2; 
        struct vertex vhigh1;
        struct vertex vhigh2;

        if ( v1.index == u.index ) {
            vlow1 = V[order][v1.low]; vhigh1 = V[order][v1.high];
        } else {
            vlow1 = V[order][v1.id]; vhigh1 = V[order][v1.id];
        }
        if ( v2.index == u.index ) {
            vlow2 = V[order][v2.low]; vhigh2 = V[order][v2.high];
        } else {
            vlow2 = V[order][v2.id]; vhigh2 = V[order][v2.id];
        }
        u.low = Apply_step(vlow1, vlow2);
        u.high = Apply_step(vhigh1, vhigh2);
    }
    return u.id;
}

int Apply( vertex v1, vertex v2 ) {
    for ( int i = 0; i < 100; i++ ) {
        for ( int j = 0; j < 100; j++ ) {
            T[i][j] = null;
        }
    }

    return Apply_step(v1, v2);

}

int main() {
    int f, g, h;

    // V[0] is empty in order to use null
    V[order][1].id = 1; V[order][1].index = 4; V[order][1].low = null; V[order][1].high = null; V[order][1].val = 0; // 0-terminal
    V[order][2].id = 2; V[order][2].index = 4; V[order][2].low = null; V[order][2].high = null; V[order][2].val = 1; // 1-terminal

    for ( int i = 1; i < N + 1 ; i++ ) {
        int num = 2 * i + 1;
        V[order][num].id = num; V[order][num].index = i; V[order][num].low = 1; V[order][num].high = 2; V[order][num].val = X;
        num = 2 * i + 2;
        V[order][num].id = num; V[order][num].index = i; V[order][num].low = 2; V[order][num].high = 1; V[order][num].val = X;
    } // primary input node,    

    op = '+';
    f = Apply(V[order][1 * 2 + 2], V[order][3 * 2 + 2]);
    cout << f << endl;
    Reduce(V[order][f]);

    op = '*';
    g = Apply(V[order][2 * 2 + 1], V[order][3 * 2 + 1]);
    cout << g << endl;
    Reduce(V[order][g]);

    op = '+';
    h = Apply(V[order][f], V[order][g]);
    cout << h << endl;
    Reduce(V[order][h]);

    /*
    V[3].id = 3; V[3].index = 1; V[3].low = 2; V[3].high = 4; V[3].val = X;
    V[4].id = 4; V[4].index = 3; V[4].low = 2; V[4].high = 1; V[4].val = X;
    V[5].id = 5; V[5].index = 2; V[5].low = 1; V[5].high = 6; V[5].val = X;
    V[6].id = 6; V[6].index = 3; V[6].low = 1; V[6].high = 2; V[6].val = X;
    

    op = '+';
    id = 7;
    cout << Apply(V[3], V[5]) << endl;

    V[id].id = id; V[id].index = 2; V[id].low = 1; V[id].high = 2; V[id].val = X;
    id++;
    V[id].id = id; V[id].index = 3; V[id].low = 1; V[id].high = 2; V[id].val = X;
    id++;

    op = '*';
    cout << Apply(V[15], V[16]) << endl;

    //cout << V[16].index << endl;
    */

    for ( int i = 1; i < 30; i++ ) {
        printf("%d %d %d %d %d\n", V[order][i].id, V[order][i].index, V[order][i].low, V[order][i].high, V[order][i].val);
    }
    //int i = 11;
    //printf("%d %d %d %d %d\n", V[i].id, V[i].index, V[i].low, V[i].high, V[i].val);

    //Reduce(V[7]);
    Traverse(V[order][19]);

    return 0;
}