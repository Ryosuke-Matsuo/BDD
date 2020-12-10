#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include <vector>
#include <algorithm>

using namespace std;

struct node {
    int l, r;
    bool exist;
    double power;
};


struct spec {
    double node = 64;
    double power = 100000000;
};


struct node N[64];
int nodesum;
double powersum;
int output[63];

struct spec mnode;
struct spec mpower;

// value = 1 -> -1 
// value = 0 -> -2

void Initialize() {
    for ( int j = 0; j < 6; j++ ) {
        for ( int i = 0; i < pow(2, j); i++ ) {
            int v = pow(2, j) + i;
            N[v].l = v * 2;
            N[v].r = v * 2 + 1;
            N[v].exist = false;
        }
    }
}

void Reduction() {
    int v;

    for ( int j = 5; j > -1; j-- ) {

        for ( int i = 0; i < pow(2, j); i++ ) {
            v = pow(2, j) + i;

            if ( N[v].l == N[v].r ) {
                int p = floor(v / 2);
                if ( v % 2 == 0 ) {
                    N[p].l = N[v].l;
                } else {
                    N[p].r = N[v].l;
                }
            } else {
                for ( int u = pow(2, j); u < v; u++) {
                    if ( N[v].l == N[u].l && N[v].r == N[u].r ) {
                        int p = floor(v / 2);
                        if ( v % 2 == 0 ) {
                            N[p].l = u;
                        } else {
                            N[p].r = u;
                        }
                        break;
                    }
                }
            }
        }
    }
}

void powersearch(int n, double ppower) {
    N[n].power = ppower * 1.25;
    //printf("%d ", list.at(n));

    if ( N[n].exist == false ) {
        nodesum++;
        N[n].exist = true;
    }

    if ( N[n].l == -1 ) {
        //printf("T ");
        powersum = powersum + N[n].power;
    } else if ( N[n].l != -2 ) {
        powersearch(N[n].l, N[n].power);
    } else {
        //printf("F ");
    }

    if ( N[n].r == -1 ) {
        //printf("T ");
        powersum = powersum + N[n].power;
    } else if ( N[n].r != -2 ) {
        powersearch(N[n].r, N[n].power);
    } else {
        //printf("F ");
    }
}

void Orderchange(int a, int b, int c, int d, int e, int f) {
    int n;
    N[32].l = output[0];
    n = pow(2, f);
    N[32].r = output[n];
    n = pow(2, e);
    N[33].l = output[n];
    n = pow(2, e) + pow(2, f);
    N[33].r = output[n];
    n = pow(2, d);
    N[34].l = output[n];
    n = pow(2, d) + pow(2, f);
    N[34].r = output[n];
    n = pow(2, d) + pow(2, e);
    N[35].l = output[n];
    n = pow(2, d) + pow(2, e) + pow(2, f);
    N[35].r = output[n];
    n = pow(2, c);
    N[36].l = output[n];
    n = pow(2, c) + pow(2, f);
    N[36].r = output[n];
    n = pow(2, c) + pow(2, e);
    N[37].l = output[n];
    n = pow(2, c) + pow(2, e) + pow(2, f);
    N[37].r = output[n];
    n = pow(2, c) + pow(2, d);
    N[38].l = output[n];
    n = pow(2, c) + pow(2, d) + pow(2, f);
    N[38].r = output[n];
    n = pow(2, c) + pow(2, d) + pow(2, e);
    N[39].l = output[n];
    n = pow(2, c) + pow(2, d) + pow(2, e) + pow(2, f);
    N[39].r = output[n];
    n = pow(2, b);
    N[40].l = output[0];
    n = pow(2, b) + pow(2, f);
    N[40].r = output[n];
    n = pow(2, b) + pow(2, e);
    N[41].l = output[n];
    n = pow(2, b) + pow(2, e) + pow(2, f);
    N[41].r = output[n];
    n = pow(2, b) + pow(2, d);
    N[42].l = output[n];
    n = pow(2, b) + pow(2, d) + pow(2, f);
    N[42].r = output[n];
    n = pow(2, b) + pow(2, d) + pow(2, e);
    N[43].l = output[n];
    n = pow(2, b) + pow(2, d) + pow(2, e) + pow(2, f);
    N[43].r = output[n];
    n = pow(2, b) + pow(2, c);
    N[44].l = output[n];
    n = pow(2, b) + pow(2, c) + pow(2, f);
    N[44].r = output[n];
    n = pow(2, b) + pow(2, c) + pow(2, e);
    N[45].l = output[n];
    n = pow(2, b) + pow(2, c) + pow(2, e) + pow(2, f);
    N[45].r = output[n];
    n = pow(2, b) + pow(2, c) + pow(2, d);
    N[46].l = output[n];
    n = pow(2, b) + pow(2, c) + pow(2, d) + pow(2, f);
    N[46].r = output[n];
    n = pow(2, b) + pow(2, c) + pow(2, d) + pow(2, e);
    N[47].l = output[n];
    n = pow(2, b) + pow(2, c) + pow(2, d) + pow(2, e) + pow(2, f);
    N[47].r = output[n];
    n = pow(2, a);
    N[48].l = output[n];
    n = pow(2, a) + pow(2, f);
    N[48].r = output[n];
    n = pow(2, a) + pow(2, e);
    N[49].l = output[n];
    n = pow(2, a) + pow(2, e) + pow(2, f);
    N[49].r = output[n];
    n = pow(2, a) + pow(2, d);
    N[50].l = output[n];
    n = pow(2, a) + pow(2, d) + pow(2, f);
    N[50].r = output[n];
    n = pow(2, a) + pow(2, d) + pow(2, e);
    N[51].l = output[n];
    n = pow(2, a) + pow(2, d) + pow(2, e) + pow(2, f);
    N[51].r = output[n];
    n = pow(2, a) + pow(2, c);
    N[52].l = output[n];
    n = pow(2, a) + pow(2, c) + pow(2, f);
    N[52].r = output[n];
    n = pow(2, a) + pow(2, c) + pow(2, e);
    N[53].l = output[n];
    n = pow(2, a) + pow(2, c) + pow(2, e) + pow(2, f);
    N[53].r = output[n];
    n = pow(2, a) + pow(2, c) + pow(2, d);
    N[54].l = output[n];
    n = pow(2, a) + pow(2, c) + pow(2, d) + pow(2, f);
    N[54].r = output[n];
    n = pow(2, a) + pow(2, c) + pow(2, d) + pow(2, e);
    N[55].l = output[n];
    n = pow(2, a) + pow(2, c) + pow(2, d) + pow(2, e) + pow(2, f);
    N[55].r = output[n];
    n = pow(2, a) + pow(2, b);
    N[56].l = output[0];
    n = pow(2, a) + pow(2, b) + pow(2, f);
    N[56].r = output[n];
    n = pow(2, a) + pow(2, b) + pow(2, e);
    N[57].l = output[n];
    n = pow(2, a) + pow(2, b) + pow(2, e) + pow(2, f);
    N[57].r = output[n];
    n = pow(2, a) + pow(2, b) + pow(2, d);
    N[58].l = output[n];
    n = pow(2, a) + pow(2, b) + pow(2, d) + pow(2, f);
    N[58].r = output[n];
    n = pow(2, a) + pow(2, b) + pow(2, d) + pow(2, e);
    N[59].l = output[n];
    n = pow(2, a) + pow(2, b) + pow(2, d) + pow(2, e) + pow(2, f);
    N[59].r = output[n];
    n = pow(2, a) + pow(2, b) + pow(2, c);
    N[60].l = output[n];
    n = pow(2, a) + pow(2, b) + pow(2, c) + pow(2, f);
    N[60].r = output[n];
    n = pow(2, a) + pow(2, b) + pow(2, c) + pow(2, e);
    N[61].l = output[n];
    n = pow(2, a) + pow(2, b) + pow(2, c) + pow(2, e) + pow(2, f);
    N[61].r = output[n];
    n = pow(2, a) + pow(2, b) + pow(2, c) + pow(2, d);
    N[62].l = output[n];
    n = pow(2, a) + pow(2, b) + pow(2, c) + pow(2, d) + pow(2, f);
    N[62].r = output[n];
    n = pow(2, a) + pow(2, b) + pow(2, c) + pow(2, d) + pow(2, e);
    N[63].l = output[n];
    n = pow(2, a) + pow(2, b) + pow(2, c) + pow(2, d) + pow(2, e) + pow(2, f);
    N[63].r = output[n];
}

/*
void Ordermem(spec T, int a, int b, int c, int d, int e) {
    T.order.clear();
    T.order.push_back(a);
    T.order.push_back(b);
    T.order.push_back(c);
    T.order.push_back(d);
    T.order.push_back(e);
}
*/

void Spec_check(int a, int b, int c, int d, int e, int f) {

    Initialize();
    powersum = 0;
    nodesum = 0;

    Orderchange(a, b, c, d, e, f);

    Reduction();

    /*
    for ( int i = 1; i < 16; i++ ) {
        cout << i << " " << N[i].l << " " << N[i].r << endl;
    } 
    */

    powersearch(1, 10);
    //cout << powersum << " " << nodesum << endl;

    if ( nodesum < mnode.node ) {
        mnode.node = nodesum;
        mnode.power = powersum;
    } else if ( nodesum == mnode.node && powersum < mnode.power ) {
        mnode.node = nodesum;
        mnode.power = powersum;
    }

    if ( powersum < mpower.power ) {
        mpower.power = powersum;
        mpower.node = nodesum;
    } else if ( powersum == mpower.power && nodesum < mpower.node ) {
        mpower.power = powersum;
        mpower.node = nodesum;
    }
}

int main() {

    int counter = 0;
    double powerdif = 0;
    double powerdifsumpercent = 0;
    int nodedif = 0;
    double nodedifsum = 0;
    double nodedifsumpercent = 0;
    double mnodepowerdif = 0;
    double powerdifsum = 0;
    int outputdif[64];
    srand((unsigned int)time(NULL));

    for ( int i = 0; i < 100000; i++ ) {
            vector<int> nums { 0, 1, 2, 3, 4, 5 };
        for ( int i = 0; i < 64; i++ ) {
            output[i] = rand() % 2 - 2;

            /*
            if ( output[i] == -1 ) {
            cout << "1 ";
            } else {
                cout << "0 ";
            }*/
        }
        //cout << endl;
        printf("%d\n", i);
        
        do {
            Spec_check(nums[0], nums[1], nums[2], nums[3], nums[4], nums[5]);
        } while (next_permutation(nums.begin(), nums.end()));
        
        /*
        cout << "Minimum Node " << mnode.power << " " << mnode.node << endl;
        cout << "Minimum Power " << mpower.power << " " << mpower.node << endl;
        */

        if ( mnode.power != mpower.power ) {
            counter++;
            powerdifsum = powerdifsum + mnode.power - mpower.power;
            powerdifsumpercent = powerdifsumpercent + ( mnode.power - mpower.power ) / mnode.power;
            nodedifsum = nodedifsum + mpower.node - mnode.node;
            nodedifsumpercent = nodedifsumpercent + ( mpower.node - mnode.node ) / mpower.node;
            if ( powerdif < mnode.power - mpower.power ) { 
                powerdif = mnode.power - mpower.power;
                mnodepowerdif = mnode.power;
                nodedif = mpower.node - mnode.node;
                for ( int i = 0; i < 64; i++ ) {
                    outputdif[i] = output[i];
                }
            }
        }

        mnode.node = 64;
        mnode.power = 100000;
        mpower.node = 64;
        mpower.power = 100000;
    }

    cout << endl;
    cout << "Count " << counter << endl;
    cout << "Average" << endl;
    cout << "Power" << endl;
    cout << powerdifsum / counter << " " << powerdifsumpercent / counter * 100 << "%" << endl;
    cout << "Node" << endl;
    cout << nodedifsum / counter << " " << nodedifsumpercent / counter * 100 << "%" << endl;
    cout << "Best case" << endl;
    cout << "Powerdif mnodepower nodedif" << endl;
    cout << powerdif << " " << mnodepowerdif << " " << nodedif << endl;
    for ( int i = 0; i < 64; i++ ) {
        if ( outputdif[i] == -1 )
        cout << "1 ";
        else cout << "0 ";
    }
    cout << endl;

    return 0;
}