// 2020/12/9  written 
// confirm that this work properly by comparing the result from CUDD
// increase the number of nodes reused in building BDD based on SOP
// calculate power and node of both normal and reverse sharing circuit.
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <fstream>
#include <queue>
#include <time.h>

using namespace std;

// variables for permutation
int perm_a[20];
int reverse_a[20];
int perm_c[20];
int perm_o[20];
int perm_s;
int perm_j;
int perm_q;

static const int X = 2;
static const int null = 0;
static const int MAX_node = 20000; // the number of vertex required for constructing first BDD
static const double MAX_powersum = 100000000;
static const int MAX_nodesum = 1000000;

struct vertex {
    int low, high;
    int index;
    int val;
    int id;
    bool mark;
    double power;
    int level;
    int reverse_exist; // 0: dont exist;  1 or 2: exist
    int reverse_id; // node id of the reverse node, in power cal, when we visit node where reverse_exist = 1, we cal power added to children nodes.
    // when we visit node where reverse_exist = 2, we dont add power to children.
};

struct vertex initialnode; // dammy node for indicating level 1 node

char op; // operation of target function
int N; // the number of inputs
//int id = N * 2 + 3; // node id
int id;
queue<int> rep; // show the V[] that can be replaced by new node
double powersum;
//double minpower = MAX_powersum;
double Avenodesum;
double Avepower;
int nodesum;
int longestpath;
int pre_nodesum;
int cur_nodesum;
int iteration = 0;
int allnode = 0;
int shuffle_count = 0;

struct spec {
    double power = MAX_powersum;
    double maxpower = 0;
    int node = MAX_nodesum;
    double avenode;
    double avepower;
    int maxpowerdelay;
    double averagepowerdelay;
};

struct delayspec {
    double average;
    int firstorder;
    int reverseorder;
    int mpower;
    int mnodeminpower;
    int mnodemaxpower;
    double mnodeave;
};

struct delayspec delay;

struct spec mpower;
struct spec mnode;
struct spec firstorder;
struct spec reverseorder;
struct spec Norm;
struct spec Rev;
int minpowerO[20];

int func[500][10]; // logic function formulation, 0 -> 0, 1 -> 1, '-' -> 2,

struct vertex V[MAX_node];
int T[MAX_node][MAX_node]; // reduction of array is possible
pair<int, int> Tlist[MAX_node];
int Tlistnum = 0;

vector<vector<int>> vlist(15); // node list that is not reduced
vector<vector<int>> pQ(15); // node list that is reduced
set<int> borderlist;
int border_x;
int border_y;

vector<int> dispose;
vector<int> dispose2;

int make_id() {
    int newid;
    if ( rep.empty() == true ) {
        newid = id++;
        return newid;
        
    } else {
        newid = rep.front();
        rep.pop();
        return newid;
    }
    
}

void dispose_insert() {
    for ( int i = 0; i < dispose.size(); i++ ) {
        rep.push(dispose[i]);
    }
    dispose.clear();
}

void dispose2_insert() {
    for ( auto it = dispose.begin(); it != dispose.end(); it++ ) {
        rep.push(*it);
    }
    dispose.clear();
    for ( auto it = dispose2.begin(); it != dispose2.end(); it++ ) {
        rep.push(*it);
    }
    dispose2.clear();
}

void dispose2_make() {
    for ( auto it = dispose.begin(); it != dispose.end(); it++ ) {
        dispose2.push_back(*it);
    }
    dispose.clear();
}

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
    dispose.push_back(v.id);
    push_vlist(V[v.low]);
    push_vlist(V[v.high]);
}

int reversenode = 0;
vector<pair<int, int>> Revlist;

int Reduce( struct vertex &v ) {
    push_vlist(v);
    for ( int i = N + 1; i > 0; i-- ) {
        map<long long, int> Q; // using stiring has larger time complexity than int ?

        for ( int j = 0; j < vlist[i].size(); j++ ) {
            int uid = vlist[i][j];
            if ( V[uid].index == N + 1 ) {
              int key = V[uid].val;
              if ( Q.find(key) == Q.end() ) {
                Q.insert(make_pair(key, uid)); 
                } else {
                    V[uid].id = Q[key];
                    //rep.push(uid);
                }
            } else {
                int lowid = V[uid].low;
                int highid = V[uid].high;
                V[uid].low = V[lowid].id;
                V[uid].high = V[highid].id;
                if ( V[uid].low == V[uid].high ) {
                    V[uid].id = V[uid].low;
                    //rep.push(uid);
                } else {
                    long long key = V[uid].low * MAX_node + V[uid].high; 
                    // express key as one box. When the number of V[] is large, dose not work correctly
                    if ( Q.find(key) == Q.end() ) {
                        Q.insert(make_pair(key, uid));
                        V[uid].reverse_exist = 0;

                        key = V[uid].high * MAX_node + V[uid].low;
                        if ( Q.find(key) != Q.end() ) {
                            reversenode++;
                            Revlist.push_back( make_pair( Q[key], V[uid].id ));
                            int revid = Q[key];
                            V[revid].reverse_exist = 1; V[uid].reverse_exist = 2;
                            V[revid].reverse_id = V[uid].id;
                        }
                    } else {
                        V[uid].id = Q[key];
                        //rep.push(uid);
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

//vector<int> dispose;

int Apply_step( struct vertex &v1, struct vertex &v2 ) {

    if ( T[v1.id][v2.id] != null ) return T[v1.id][v2.id];

    //int newid = make_id();
    //struct vertex &u = V[newid];
    //u.id = newid;
    int newid = make_id();
    struct vertex &u = V[newid];
    u.id = newid;
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

    if ( n == N + 1 ) return;

    nodesum++;
    push_pQ(V[v.low]);
    push_pQ(V[v.high]);
}

//bfs is not suitable for power calculation
void power_cal_pQ_make( struct vertex &v ) {
    push_pQ(v);
    v.power = 10;

    for ( int i = 1; i < N + 1; i++ ) {
        for ( int j = 0; j < pQ[i].size(); j++ ) {
            int node = pQ[i][j];
            struct vertex u = V[node];
            if ( u.reverse_exist == 0 ) {
                V[u.low].power += u.power * 1.25;
                V[u.high].power += u.power * 1.25;
                iteration++;
            } else if ( u.reverse_exist == 1 ) {
                double power = max(u.power, V[u.reverse_id].power);
                V[u.low].power += power * 1.25;
                V[u.high].power += power * 1.25;
            }
        }
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
}

void power_cal_border_make( struct vertex &v ) {
    for ( int i = 1; i < N + 2; i++ ) {
        for ( auto it = pQ[i].begin(); it != pQ[i].end(); it++ ) {
            V[*it].power = 0;
        }
    }

    v.power = 10;

    for ( int i = 1; i < N + 1; i++ ) {
        for ( int j = 0; j < pQ[i].size(); j++ ) {
            int node = pQ[i][j];
            struct vertex u = V[node];
            if ( u.reverse_exist == 0 ) {
                V[u.low].power += u.power * 1.25;
                V[u.high].power += u.power * 1.25;
                iteration++;
            } else if ( u.reverse_exist == 1 ) {
                double power = max(u.power, V[u.reverse_id].power);
                V[u.low].power += power * 1.25;
                V[u.high].power += power * 1.25;
            }

            if ( u.index != border_x && u.index != border_y ) {
                int lowidlevel = V[u.low].index;
                int highidlevel = V[u.high].index;
                if ( lowidlevel == border_x | lowidlevel == border_y | highidlevel == border_x | highidlevel == border_y ) {
                    borderlist.insert(u.id);
                }
            }
        }
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
}

// time complexity of power_cal_red is O(V), where V is the number of nodes in RPBDD
// search for boundary nodes between upper and lower

int node_cal_norm( struct vertex &v ) {
    int amount = -2;
    for ( int i = 1; i < N + 2; i++ ) {
        for ( auto it = pQ[i].begin(); it != pQ[i].end(); it++ ) {
            V[*it].power = 0;
            amount++;
        }
    }
    return amount;
}

double power_cal_norm( struct vertex &v ) {
    v.power = 10;

    for ( int i = 1; i < N + 1; i++ ) {
        for ( int j = 0; j < pQ[i].size(); j++ ) {
            int node = pQ[i][j];
            struct vertex u = V[node];
            V[u.low].power += u.power * 1.25;
            V[u.high].power += u.power * 1.25;
        }
    }

    int term;
    term = pQ[N + 1][0];
    if ( V[term].val == 1 ) {
        return V[term].power;
    }
    term = pQ[N + 1][1];
    if ( V[term].val == 1 ) {
        return V[term].power;
    }
}

double power_cal_for_norm( int n, double ppower ) {
    if ( V[n].val == 1 ) {
        return V[n].power;
    }
    if ( V[n].val == 0 ) {
        return 0;
    }

    V[n].power = ppower * 1.25;
    int lowid = V[n].low;
    int highid = V[n].high;
    return power_cal_for_norm(lowid, V[n].power) + power_cal_for_norm(highid, V[n].power);

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

        map<long long, int> Q; // using stiring has larger time complexity than int ?

        for ( int j = 0; j < vlist[i].size(); j++ ) {
            int uid = vlist[i][j];
                    int lowid = V[uid].low;
                    int highid = V[uid].high;
                    V[uid].low = V[lowid].id; // make children id same as the original node id
                    V[uid].high = V[highid].id; // redandant V[] can be reused

                    if ( V[uid].low == V[uid].high ) {
                        V[uid].id = V[uid].low;
                        rep.push(uid);
                    } else {
                        long long key = V[uid].low * MAX_node + V[uid].high; 
                        // express key as one box. When the number of V[] is large, incorrect
                        if ( Q.find(key) == Q.end() ) {
                            Q.insert(make_pair(key, uid));
                            pQ[i].push_back(uid);
                            cur_nodesum++; // count current nodesum
                            V[uid].reverse_exist = 0;

                            key = V[uid].high * MAX_node + V[uid].low;
                            if ( Q.find(key) != Q.end() ) {
                                int revid = Q[key];
                                reversenode++;
                                V[revid].reverse_exist = 1; V[uid].reverse_exist = 2;
                                V[revid].reverse_id = V[uid].id;
                            }
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
    for ( auto it = borderlist.begin(); it != borderlist.end(); it++ ) {
        int uid = *it;
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
    if ( x > y ) {
        swap(x, y); // x is to be less than y
    }

    if ( x == 1 ) {
        node_swap(initialnode, x, y);
    } else {
        for ( auto it = borderlist.begin(); it != borderlist.end(); it++ ) {
            int node = *it;
            node_swap(V[node], x, y);
        }
    }

    for ( int i = 0; i < pQ[x].size(); i++ ) {
        rep.push(pQ[x][i]);
    }
    for ( int i = 0; i < pQ[y].size(); i++ ) {
        rep.push(pQ[y][i]);
    }
    pre_nodesum = - reversenode;
    cur_nodesum = 0;

    pre_nodesum += pQ[x].size(); // count pre_nodesum
    pre_nodesum += pQ[y].size();
    pQ[x].clear(); // nodes in previous BDD
    pQ[y].clear();
    reversenode = 0;

    adjacent_Reduce(x, y); // elements are inserted to pQ[x], pQ[y]
    nodesum = nodesum + cur_nodesum - pre_nodesum - reversenode;
}

bool visitok = true;
int N_fact = 1;

void factorial() {
    for ( int i = 2; i < N + 1; i++ ) {
        N_fact *= i;
    }
}

void spec_check( double powersum, int nodesum );

void permute() {
    while ( shuffle_count < N_fact) {
        if ( visitok == true ) {
            
            /*
            for ( int i = 1; i < N + 1; i++ ) {
                printf("%d ", perm_a[i]);
            }
            printf("\n");
            */
                        

            perm_j = N;
            perm_s = 0;
        }

        perm_q = perm_c[perm_j] + perm_o[perm_j];
        if ( perm_q < 0 ) {
            perm_o[perm_j] = -perm_o[perm_j];
            perm_j--;
            visitok = false;
        } else if ( perm_q == perm_j ) {
            if ( perm_j == 1 ) return;
            perm_s++;
            perm_o[perm_j] = -perm_o[perm_j];
            perm_j--;
            visitok = false;
        } else {
            int x = perm_j - perm_c[perm_j] + perm_s;
            int y = perm_j - perm_q + perm_s;
            perm_c[perm_j] = perm_q;
            visitok = true;

            powersum = 0;
            border_x = x; 
            border_y = y;
            power_cal_border_make(V[initialnode.low]); // make borderlist
            //power_cal_red_ad(initialnode.low, 10, 0);

            shuffle_count++;
            bool reverseok = true;
            for ( int i = 1; i < N + 1; i++ ) {
                if ( perm_a[i] != reverse_a[i] ) {
                    reverseok = false;
                    break;
                }
            }
            if ( reverseok == true ) {
                reverseorder.power = powersum;
                reverseorder.node = nodesum;
            }
            if ( shuffle_count == 1 ) {
                firstorder.power = powersum;
                firstorder.node = nodesum;
            }

            spec_check( powersum, nodesum );

            adjacent_swap(x, y);  // construct next BDD

            borderlist.clear(); // clear borderlist  
            swap(perm_a[x], perm_a[y]);
        }
    }
}

int mnode_count = 0;

void order_mem() {
    for ( int i = 0; i < N + 1; i++ ) {
        minpowerO[i] = perm_a[i];
    }
}

void spec_check( double powersum, int nodesum ) {
    if ( powersum < Rev.power ) {
        Rev.power = powersum;
        Rev.node = nodesum;
        order_mem();
    } else if ( powersum == Rev.power && nodesum < Rev.node ) {
        Rev.node = nodesum;
        order_mem();
    }

    int Cnode = node_cal_norm(V[initialnode.low]);
    double Cpower = power_cal_norm(V[initialnode.low]);
    if ( Cpower < Norm.power ) {
        Norm.power = Cpower;
        Norm.node = Cnode;
    } else if ( Cpower == Norm.power && Cnode < Norm.node ) {
        Norm.node = Cnode;
    }
}

void one_input_result() {
    reverseorder.node = 1; 
    reverseorder.power = 12.5;
    firstorder.node = 1; 
    firstorder.power = 12.5;
    minpowerO[1] = 1;
    Rev.node = 1; 
    Rev.power = 12.5;
}

void print_pQ() {
    int number = 1;
    for ( int i = 1; i < N + 2; i++ ) {
        for ( auto it = pQ[i].begin(); it != pQ[i].end(); it++ ) {
            struct vertex v = V[*it];
            printf("%d %d %d %d %d %d %d %d\n", number++, v.id, v.index, v.low, v.high, v.val, v.reverse_exist, v.reverse_id);
        }
    }
}

void print_Revlist() {
    for ( auto it = Revlist.begin(); it != Revlist.end(); it++ ) {
        printf("%d %d\n", it->first, it->second);
    }
}

int main() {

    clock_t start = clock();

    int Cid;
    int lut;
    int subcircuit;
    //scanf("%d", &Cid);
    //scanf("%d", &lut);
    scanf("%d", &subcircuit);
    Cid = 7552;
    lut = 10;
    char infilename[50];
    sprintf(infilename, "c%d/lut%d/sub_%d.blif", Cid, lut, subcircuit);

    ifstream ifs(infilename);
    string str;
    if (ifs.fail())
    {
        cerr << "失敗" << endl;
        return -1;
    }
    int line = -1;
    N = -1;
    vector<bool> Exist;
    while (getline(ifs, str)) {
        if ( line == -1 ) {
            for ( int i = 0; i < str.length(); i++ ) {
                if ( str[i] == ' ' ) N++;
            }
        }
        for ( int i = 0; i < N; i++ ) {
            Exist.push_back(false);
        }

        for ( int i = 0; i < N; i++ ) {
            if ( str[i] == '0' ) {
                func[line][i] = 0;
                Exist[i] = true;
            }
            if ( str[i] == '1' ) {
                func[line][i] = 1;
                Exist[i] = true;
            }
            if ( str[i] == '-' ) func[line][i] = 2;
        }
        line++;
    }
    // correct the failure in blif.file
    int correct = 0;
    for ( int non = 0; non < N; non++ ) {
        if ( Exist[non] == false ) {
            for ( int i = 0; i < line; i++ ) {
                for ( int j = non - correct; j < N; j++ ) {
                    func[i][j] = func[i][j + 1];
                }
            }
            N--;
            correct++;
        }
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
                dispose_insert();
                Reduce(V[f]);
            }
            if ( func[i][j] == 1 ) {
                int nodeval = 2 * nums[j] + 3;
                f = Apply(V[f], V[nodeval]);
                dispose_insert();
                Reduce(V[f]);
            }
        }
        op = '+';
        g = Apply(V[g], V[f]);
        dispose2_insert();
        reversenode = 0; // initialize information of Reverse nodes
        Revlist.clear();
        Reduce(V[g]);
        dispose2_make();
    }
    powersum = 0;
    nodesum -= reversenode;
    power_cal_pQ_make(V[g]); // push elements into the pQ
    //printf("%d %d %lf\n", reversenode, nodesum, powersum);
    //print_pQ();
    //print_Revlist();

    initialnode.low = g; // dammy node for adjacent swap of level 1 and 2S
    for ( int i = 1; i < N + 1; i++ ) {
    perm_c[i] = 0;
    perm_o[i] = 1;
    }
    for ( int i = 1; i < N + 1; i++ ) {
        perm_a[i] = i;
    }
    for ( int i = 1; i < N + 1; i++ ) {
        reverse_a[N + 1 - i] = i;
    }

    
    factorial();
    permute();
    powersum = 0;
    power_cal_border_make(V[initialnode.low]); // make borderlist
    //power_cal_red_ad(initialnode.low, 10, 0);

    shuffle_count++;
    //printf("%d %d %lf\n\n", shuffle_count, nodesum, powersum);
    Avenodesum += nodesum;
    Avepower += powersum;
    spec_check(powersum, nodesum);
    
    if ( N == 2 ) {
        reverseorder.power = powersum;
        reverseorder.node = nodesum;
    }
    if ( N == 1) {
    one_input_result();
    }
    

    clock_t end = clock();
    const double time = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1;
    
    
    printf("\n%d-input logic function\n", N);
    

    FILE *outputfile;
    char outfilename[50];
    sprintf(outfilename, "result/reverse/c%d_lut%d_reverse.txt", Cid, lut);
    if ( subcircuit == 1 ) {
        outputfile = fopen(outfilename, "w");
        fclose(outputfile);
    }
    outputfile = fopen(outfilename, "a");
    fprintf(outputfile, "%d %d %lf\n", N, Rev.node, Rev.power);
    printf("%d %d %lf\n", N, Rev.node, Rev.power);

    sprintf(outfilename, "result/reverse/c%d_lut%d_normal.txt", Cid, lut);
    if ( subcircuit == 1 ) {
        outputfile = fopen(outfilename, "w");
        fclose(outputfile);
    }
    outputfile = fopen(outfilename, "a");
    fprintf(outputfile, "%d %d %lf\n", N, Norm.node, Norm.power);
    printf("%d %d %lf\n", N, Norm.node, Norm.power);

    // NOTATION
    // Normal BDD and Rev. share BDD have a difference in the number of nodes in variable order that minimizes the power consumption.

    return 0;
}
