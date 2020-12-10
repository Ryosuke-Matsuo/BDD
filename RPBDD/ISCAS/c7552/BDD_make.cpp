#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <fstream>
#include <queue>
#include <time.h>

using namespace std;

static const int X = 2;
static const int null = 0;
static const int MAX_node = 20000;

struct vertex {
    int low, high;
    int index;
    int val;
    int id;
    bool mark;
    double power;
};

char op; // operation of target function
int N; // the number of inputs
//int id = N * 2 + 3; // node id
int id;
double powersum;
double minpower = 10000000000000;
vector<vector<int>> pQ(20); // this is used for power cal
int iteration = 0;

int func[500][10]; // logic function formulation, 0 -> 0, 1 -> 1, '-' -> 2,

struct vertex V[MAX_node];
int T[MAX_node][MAX_node]; // reduction of array is possible
pair<int, int> Tlist[MAX_node];
int Tlistnum = 0;

vector<vector<int>> vlist(20);

void Traverse( struct vertex v ) {
    printf("%d %d %d %d %d\n", v.id, v.index, v.low, v.high, v.val);

    if ( v.index == N + 1 ) return;
    Traverse(V[v.low]);
    Traverse(V[v.high]);
}

void push_vlist( struct vertex &v ) {
    if ( v.mark == true ) return;

    int n = v.index;
    v.mark = true;
    vlist[n].push_back(v.id);

    if ( n == N + 1 ) return;
    push_vlist(V[v.low]);
    push_vlist(V[v.high]);
}

int Reduce( struct vertex &v ) {
    for ( int i = 0; i < N + 2; i++ ) {
        vlist[i].clear();
    }
    push_vlist(v);
    int nextid = 0;
    for ( int i = N + 1; i > 0; i-- ) {
        map<int, int> Q; // using stiring has larger time complexity than int ?

        for ( int j = 0; j < vlist[i].size(); j++ ) {
            int uid = vlist[i][j];
            if ( V[uid].index == N + 1 ) {
              int key = V[uid].val;
              if ( Q.find(key) == Q.end() ) {
                Q.insert(make_pair(key, uid)); 
                } else {
                    V[uid].id = Q[key];
                }
            } else {
                    int lowid = V[uid].low;
                    int highid = V[uid].high;
                    V[uid].low = V[lowid].id;
                    V[uid].high = V[highid].id;

                    if ( V[uid].low == V[uid].high ) {
                        V[uid].id = V[uid].low;
                    } else {
                        int key = V[uid].low * MAX_node + V[uid].high; 
                        // express key as one box. When the number of V[] is large, incorrect
                        if ( Q.find(key) == Q.end() ) {
                            Q.insert(make_pair(key, uid));
                        } else {
                            V[uid].id = Q[key];
                    }
                }
            }
        }
    }
    return 0;
}

int Apply_step( struct vertex &v1, struct vertex &v2 ) {

    if ( T[v1.id][v2.id] != null ) return T[v1.id][v2.id];

    struct vertex &u = V[id];
    u.id = id++;
    u.mark = false;
    u.power = 0;
    T[v1.id][v2.id] = u.id;
    Tlist[Tlistnum].first = v1.id;
    Tlist[Tlistnum].second = v2.id;
    Tlistnum++;

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
            vlow1 = V[v1.low]; vhigh1 = V[v1.high];
        } else {
            vlow1 = V[v1.id]; vhigh1 = V[v1.id];
        }
        if ( v2.index == u.index ) {
            vlow2 = V[v2.low]; vhigh2 = V[v2.high];
        } else {
            vlow2 = V[v2.id]; vhigh2 = V[v2.id];
        }
        u.low = Apply_step(vlow1, vlow2);
        u.high = Apply_step(vhigh1, vhigh2);
    }
    return u.id;
}

int Apply( vertex &v1, vertex &v2 ) {

    for ( int i = 0; i < Tlistnum; i++ ) {
        T[Tlist[i].first][Tlist[i].second] = null;
    } //  Initialization of T[][]

    Tlistnum = 0;

    return Apply_step(v1, v2);

}

void push_pQ( struct vertex &v ) {
    if ( v.mark == false ) return;

    int n = v.index;
    v.mark = false;
    pQ[n].push_back(v.id);
    //printf("%d\n", v.id);

    if ( n == N + 1 ) return;
    push_pQ(V[v.low]);
    push_pQ(V[v.high]);
}

//bfs is not suitable for power calculation
void power_cal( struct vertex &v ) {
    push_pQ(v);
    v.power = 10;

    for ( int i = 1; i < N + 1; i++ ) {
        for ( int j = 0; j < pQ[i].size(); j++ ) {
            int node = pQ[i][j];
            struct vertex u = V[node];
            V[u.low].power += u.power * 1.25;
            V[u.high].power += u.power * 1.25;
            //printf("%d\n", u.id);
            iteration++;
        }
        pQ[i].clear();
    }

    int term;
    term = pQ[N + 1][0];
    if ( V[term].val == 1 ) {
        powersum = V[term].power;
    }
    term = pQ[N + 1][1];
    if ( V[term].val == 1 ) {
        powersum = V[term].power;
    }
    pQ[N + 1].clear();
}

// time complexity of power_cal_red is O(V), where V is the number of nodes in RPBDD
void power_cal_red( int n, double ppower ) {
    V[n].power = ppower * 1.25;
    int lowid = V[n].low;
    int highid = V[n].high;
    //printf("%d ", list.at(n));
    iteration++;

    if ( V[lowid].val == 1 ) {
        //printf("T ");
        powersum = powersum + V[n].power;
    } else if ( V[lowid].val != 0 ) {
        power_cal_red(lowid, V[n].power);
    }

    if ( V[highid].val == 1 ) {
        //printf("T ");
        powersum = powersum + V[n].power;
    } else if ( V[highid].val != 0 ) {
        power_cal_red(highid, V[n].power);
    }
}

int main() {

    clock_t start = clock();

    string infilename;
    cin >> infilename;
    ifstream ifs(infilename);
    string str;
    if (ifs.fail())
    {
        cerr << "失敗" << endl;
        return -1;
    }
    int line = -1;
    //int variable = -1;
    N = -1;
    while (getline(ifs, str)) {
        if ( line == -1 ) {
            for ( int i = 0; i < str.length(); i++ ) {
                if ( str[i] == ' ' ) N++;
            }
        }

        for ( int i = 0; i < N; i++ ) {
            if ( str[i] == '0' ) func[line][i] = 0;
            if ( str[i] == '1' ) func[line][i] = 1;
            if ( str[i] == '-' ) func[line][i] = 2;
        }
        line++;
    }

    for ( int i = 0; i < 1000; i++ ) {
        Tlist[i].first = null;
        Tlist[i].second = null;
    }

    // V[0] is empty in order to use null
    V[1].id = 1; V[1].index = N + 1; V[1].low = null; V[1].high = null; V[1].val = 0; // 0-terminal
    V[2].id = 2; V[2].index = N + 1; V[2].low = null; V[2].high = null; V[2].val = 1; // 1-terminal

    for ( int i = 1; i < N + 1 ; i++ ) {
        int num = 2 * i + 1;
        V[num].id = num; V[num].index = i; V[num].low = 1; V[num].high = 2; V[num].val = X; // true node
        num = 2 * i + 2;
        V[num].id = num; V[num].index = i; V[num].low = 2; V[num].high = 1; V[num].val = X; // false node
    } // primary input node,  

    vector<int> nums;
    for ( int i = 0; i < N; i++ ) {
        nums.push_back(i);
    } 

    int g;
    int count = 0;


    do {
        int f, h;
        id = N * 2 + 3;

        g = 1;
        for ( int i = 0; i < line; i++ ) {
            f = 2;
            op = '*';
            for ( int j = 0; j < N; j++ ) {
                if ( func[i][j] == 0 ) {
                    int nodeval = 2 * nums[j] + 4;
                    f = Apply(V[f], V[nodeval]);
                    Reduce(V[f]);
                }
                if ( func[i][j] == 1 ) {
                    int nodeval = 2 * nums[j] + 3;
                    f = Apply(V[f], V[nodeval]);
                    Reduce(V[f]);
                }
            }
            op = '+';
            g = Apply(V[g], V[f]);
            Reduce(V[g]);
        }

        powersum = 0;

        //power_cal(V[g]);
        power_cal_red(g, 10);

        printf("%d  %lf\n", ++count, powersum);
        minpower = min(minpower, powersum);

        //if ( nums[0] == 2 && nums[1] == 3 && nums[2] == 0 && nums[3] == 1 )
        //Traverse(V[g]);

    } while (next_permutation(nums.begin(), nums.end()));

    clock_t end = clock();
    const double time = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1;

    printf("min power : %lf\ntime : %lf[s]\niteration : %d\n", minpower, time, iteration);
    return 0;
}