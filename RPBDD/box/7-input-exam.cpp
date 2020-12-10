#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include <vector>
#include <algorithm>

using namespace std;

//#define N 7;

struct node {
    int l, r;
    bool exist;
    double power;
};


struct spec {
    double node = 128;
    double power = 1000000000000;
};


struct node N[128];
int nodesum;
double powersum;
int output[127];

struct spec mnode;
struct spec mpower;

// value = 1 -> -1 
// value = 0 -> -2

void Initialize() {
    for ( int j = 0; j < 7; j++ ) {
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

    for ( int j = 6; j > -1; j-- ) {

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

void Orderchange(int a, int b, int c, int d, int e, int f, int g) {
    
    for ( int i = 0; i < 128; i++ ) {
        int xa = 0, xb = 0, xc = 0, xd = 0, xe = 0, xf = 0, xg = 0;
        int num = i;
        
        if ( num > 64 ) xa = 1;
        num = num % 64;
        if ( num > 32 ) xb = 1;
        num = num % 32;
        if ( num > 16 ) xc = 1;
        num = num % 16;
        if ( num > 8 ) xd = 1;
        num = num % 8;
        if ( num > 4 ) xe = 1;
        num = num % 4;
        if ( num > 2 ) xf = 1;
        num = num % 2;
        if ( num == 1 ) xg = 1;

        int nodenum;
        nodenum = 64 + floor(i / 2);
        int n = xa * pow(2, a) + xb * pow(2, b) + xc * pow(2, c) + xd * pow(2, d) + xe * pow(2, e) + xf * pow(2, f) + xg * pow(2, g);
        
        if ( i % 2 == 0 ) N[nodenum].l = output[n];
        else N[nodenum].r = output[n];
    }
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

void Spec_check(int a, int b, int c, int d, int e, int f, int g) {

    Initialize();
    powersum = 0;
    nodesum = 0;

    Orderchange(a, b, c, d, e, f, g);

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
    //int outputdif[128];
    srand((unsigned int)time(NULL));

    for ( int i = 0; i < 100; i++ ) {
            vector<int> nums { 0, 1, 2, 3, 4, 5, 6 };
        for ( int i = 0; i < 128; i++ ) {
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
            Spec_check(nums[0], nums[1], nums[2], nums[3], nums[4], nums[5], nums[6]);
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
                /*
                for ( int i = 0; i < 128; i++ ) {
                    outputdif[i] = output[i];
                }
                */
            }
        }

        mnode.node = 128;
        mnode.power = 1000000000;
        mpower.node = 128;
        mpower.power = 100000000;
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
    /*
    for ( int i = 0; i < 128; i++ ) {
        if ( outputdif[i] == -1 )
        cout << "1 ";
        else cout << "0 ";
    }
    */
    cout << endl;

    return 0;
}