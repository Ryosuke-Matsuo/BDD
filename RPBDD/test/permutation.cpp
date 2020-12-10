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

// variables for permutation
int perm_a[20];
int perm_c[20];
int perm_o[20];
int perm_s;
int perm_j;
int perm_q;

static const int X = 2;
static const int null = 0;

struct vertex {
    int low, high;
    int index;
    int val;
    int id;
    bool mark;
    double power;
};

struct vertex initialnode; // dammy node for indicating level 1 node

char op; // operation of target function
int N; // the number of inputs
//int id = N * 2 + 3; // node id
int id;
queue<int> rep; // show the V[] that can be replaced by new node
double powersum;
double minpower = 10000000000000;
int iteration = 0;
int allnode = 0;
int shuffle_count = 0;

int func[100][10]; // logic function formulation, 0 -> 0, 1 -> 1, '-' -> 2,

struct vertex V[1000000000];
int T[10000][10000]; // reduction of array is possible
pair<int, int> Tlist[10000];
int Tlistnum = 0;

vector<vector<int>> vlist(10); // node list that is not reduced
vector<vector<int>> pQ(10); // node list that is reduced

int make_id() {
    int newid;
    if ( rep.empty() == true ) {
        newid = id++;
        //if ( shuffle_count == 11 )
        //printf("ok%d\n", newid);
        return newid;
    } else {
        newid = rep.front();
        rep.pop();
        //if (shuffle_count == 11 )
        //printf("%d\n", newid);
        return newid;
    }
}

void Traverse( struct vertex v ) {
    printf("%d %d %d %d %d\n", v.id, v.index, v.low, v.high, v.val);

    if ( v.index == N + 1 ) return;
    /*
    if ( shuffle_count == 11 ) {
    if ( v.index == 2 ) return;
    }
    */
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
    push_vlist(v);
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
                        int key = V[uid].low * 10000 + V[uid].high; 
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

    for ( int i = 0; i < N + 2; i++ ) {
        vlist[i].clear();
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
    allnode++;

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
        //pQ[i].clear();
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
    //pQ[N + 1].clear();
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

pair<int, int> id_pair;

void cal_lowid( int t, int x, int y ) {
    struct vertex ynode = V[t];
    if ( ynode.index == x ) {
        if ( V[ynode.low].index == y ) {
            id_pair.first = V[ynode.low].low;
        } else {
            id_pair.first = ynode.low;
        }
        if ( V[ynode.high].index == y ) {
            id_pair.second = V[ynode.high].low;
        } else {
            id_pair.second = ynode.high;
        }
    }
}

void cal_highid( int t, int x, int y ) {
    struct vertex ynode = V[t];
    if ( ynode.index == x ) {
        if ( V[ynode.low].index == y ) {
            id_pair.first = V[ynode.low].high;
        } else {
            id_pair.first = ynode.low;
        }
        if ( V[ynode.high].index == y ) {
            id_pair.second = V[ynode.high].high;
        } else {
            id_pair.second = ynode.high;
        }
    }
}

void node_swap( struct vertex &v, int x, int y ) {
    int newid;
    // v is (x - 1) level node
    int t = v.low; // t is y level node
    //struct vertex ynode = V[t];

    if ( V[t].index == x | V[t].index == y ) {

    newid = make_id();
    v.low = newid;
    vlist[x].push_back(newid);
    struct vertex &lowu = V[newid]; // create new x level node 
    lowu.id = newid;
    lowu.mark = false;
    lowu.power = 0;
    lowu.index = x;
    lowu.val = X;

    if ( V[t].index == y ) {
        lowu.low = V[t].low;
        lowu.high = V[t].high;
    } else {

    cal_lowid(t, x, y);
    int lowid = id_pair.first;
    int highid = id_pair.second;

    if ( lowid == highid ) {
        lowu.low = lowid;
    } else {
        newid = make_id();
        lowu.low = newid; // create low tree
        V[newid].id = newid;
        V[newid].mark = false;
        V[newid].power = 0;
        V[newid].index = y;
        V[newid].val = X;
        V[newid].low = lowid;
        V[newid].high = highid;
        vlist[y].push_back(newid);
    }

    cal_highid(t, x, y);
    lowid = id_pair.first;
    highid = id_pair.second;

    if ( lowid == highid ) {
        lowu.high = lowid;
    } else {
        newid = make_id();
        lowu.high = newid; // create high tree
        V[newid].id = newid;
        V[newid].mark = false;
        V[newid].power = 0;
        V[newid].index = y;
        V[newid].val = X;
        V[newid].low = lowid;
        V[newid].high = highid;
        vlist[y].push_back(newid);
    }

    if ( lowu.low == lowu.high ) {
        v.low = lowu.low;
    } // if lowu node is redandant, it is eliminated
    }
    }

    if ( x == 1 ) return; // if x = 1, high edge of variable 1 node is not created

    t = v.high; // t is y level node
    //ynode = V[t];

    if ( V[t].index == x | V[t].index == y ) {

    newid = make_id();
    v.high = newid;
    vlist[x].push_back(newid);
    struct vertex &highu = V[newid]; // create new x level node 
    highu.id = newid;
    highu.mark = false;
    highu.power = 0;
    highu.index = x;
    highu.val = X;

    if ( V[t].index == y ) {
        highu.low = V[t].low;
        highu.high = V[t].high;
    } else {

    cal_lowid(t, x, y);
    int lowid = id_pair.first;
    int highid = id_pair.second;

    if ( lowid == highid ) {
        highu.low = lowid;
    } else {
        newid = make_id();
        highu.low = newid; // create low tree
        V[newid].id = newid;
        V[newid].mark = false;
        V[newid].power = 0;
        V[newid].index = y;
        V[newid].val = X;
        V[newid].low = lowid;
        V[newid].high = highid;
        vlist[y].push_back(newid);
    }

    cal_highid(t, x, y);
    lowid = id_pair.first;
    highid = id_pair.second;

    if ( lowid == highid ) {
        highu.high = lowid;
    } else {
        newid = make_id();
        highu.high = newid; // create high tree
        V[newid].id = newid;
        V[newid].mark = false;
        V[newid].power = 0;
        V[newid].index = y;
        V[newid].val = X;
        V[newid].low = lowid;
        V[newid].high = highid;
        vlist[y].push_back(newid);
    }

    if ( highu.low == highu.high ) {
        v.high = highu.low;
    }
    }
    }

}

void adjacent_Reduce( int x, int y ) {
    for ( int i = y; i >= x; i-- ) {

        map<int, int> Q; // using stiring has larger time complexity than int ?

        for ( int j = 0; j < vlist[i].size(); j++ ) {
            int uid = vlist[i][j];
                    int lowid = V[uid].low;
                    int highid = V[uid].high;
                    V[uid].low = V[lowid].id;
                    V[uid].high = V[highid].id;

                    if ( V[uid].low == V[uid].high ) {
                        V[uid].id = V[uid].low;
                    } else {
                        int key = V[uid].low * 10000 + V[uid].high; 
                        // express key as one box. When the number of V[] is large, incorrect
                        if ( Q.find(key) == Q.end() ) {
                            Q.insert(make_pair(key, uid));
                            pQ[i].push_back(uid);
                        } else {
                            V[uid].id = Q[key];
                            rep.push(uid);
                    }
                }
            
        }
    }

    int i = x - 1;
    if ( i == 0 ) {
        initialnode.low = pQ[1][0];
    } else {
    for ( int j = 0; j < pQ[i].size(); j++ ) {
        int uid = pQ[i][j];
        int lowid = V[uid].low;
        int highid = V[uid].high;
        V[uid].low = V[lowid].id;
        V[uid].high = V[highid].id;
    }
    }

    vlist[x].clear();
    vlist[y].clear();
}

void adjacent_swap( int x, int y ) {
    if ( x > y ) swap(x, y); // x is to be less than y

    /* reuse the memory
    for ( int i = 0; i < pQ[x].size(); i++ ) {
        rep.push_back(pQ[x][i]);
    }
    for ( int i = 0; i < pQ[y].size(); i++ ) {
        rep.push_back(pQ[y][i]);
    }
    */

    /*
    if ( shuffle_count == 11 ) {
        while ( rep.empty() == false ) {
            printf("%d ", rep.front());
            rep.pop();
        }
        printf("\n");
    }
    */

    if ( x == 1 ) {
        node_swap(initialnode, x, y);
    } else {
        for ( int i = 0; i < pQ[x - 1].size(); i++ ) {
            int node = pQ[x - 1][i];
            node_swap(V[node], x, y);
        }
    }

    for ( int i = 0; i < pQ[x].size(); i++ ) {
        rep.push(pQ[x][i]);
    }
    for ( int i = 0; i < pQ[y].size(); i++ ) {
        rep.push(pQ[y][i]);
    }

    /*
    if ( shuffle_count == 10 ) {
        while ( rep.empty() == false ) {
            printf("%d ", rep.front());
            rep.pop();
        }
        printf("\n");
    }
    */

    pQ[x].clear();
    pQ[y].clear();

    adjacent_Reduce(x, y); // elements are inserted to pQ[x], pQ[y]
}

void perm_ready();

void perm_visit() {
    for ( int i = 1; i < N + 1; i++ ) {
        printf("%d ", perm_a[i]);
    }
    printf("\n");
    /*
    for ( int i = 1; i < N + 1; i++ ) {
        printf("%d ", perm_c[i]);
    }
    printf("\n");
    */

    perm_j = N;
    perm_s = 0;
    //if ( Count == 2 ) return;

    perm_ready();
}

void perm_ready() {
    perm_q = perm_c[perm_j] + perm_o[perm_j];
    //printf("%d %d %d\n", q, perm_c[j], perm_o[j]);
    if ( perm_q < 0 ) {
        perm_o[perm_j] = -perm_o[perm_j];
        perm_j--;
        perm_ready();
    } else if ( perm_q == perm_j ) {
        if ( perm_j == 1 ) return;
        perm_s++;
        perm_o[perm_j] = -perm_o[perm_j];
        perm_j--;
        perm_ready();
    } else {
        int x = perm_j - perm_c[perm_j] + perm_s;
        int y = perm_j - perm_q + perm_s;
        //printf("%d %d %d\n", x, y, j);
        swap(perm_a[x], perm_a[y]);
        perm_c[perm_j] = perm_q;

        adjacent_swap(x, y);
        powersum = 0;
        //power_cal_red(initialnode.low, 10);
        //if ( shuffle_count == 5 )
        Traverse(V[initialnode.low]);
        printf("%d  %lf\n", ++shuffle_count, powersum);
        minpower = min(minpower, powersum);
        

        perm_visit();
    }
}

int main() {
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

    clock_t start = clock();

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

        power_cal(V[g]);
        //power_cal_red(g, 10);

        printf("%d  %lf\n", ++shuffle_count, powersum);
        minpower = min(minpower, powersum);

        //Traverse(V[g]);

        initialnode.low = g; // dammy node for adjacent swap of level 1 and 2S

        /*
        adjacent_swap(2, 3);
        adjacent_swap(1, 2);
        adjacent_swap(1, 2);
        adjacent_swap(2, 3);
        adjacent_swap(3, 4);
        Traverse(V[initialnode.low]);

        powersum = 0;
        power_cal_red(initialnode.low, 10);
        printf("%d  %lf\n", ++shuffle_count, powersum);
        */

        for ( int i = 1; i < N + 1; i++ ) {
        perm_c[i] = 0;
        perm_o[i] = 1;
        }

        for ( int i = 1; i < N + 1; i++ ) {
            perm_a[i] = i;
        }

        perm_visit();

        //adjacent_swap(2, 3);
        //Traverse(V[initialnode.low]);


    clock_t end = clock();
    const double time = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000.0;

    printf("min power : %lf\ntime : %lf[ms]\nthe total number of nodes : %d\npower calculation iteration : %d\n", minpower, time, allnode, iteration);

    //perm_visit();

    return 0;
}
