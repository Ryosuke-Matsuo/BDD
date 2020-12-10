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
    double node = 16;
    double power = 100000000;
};

struct node N[16];
int nodesum;
double powersum;
int output[15];

struct spec mnode;
struct spec mpower;

// value = 1 -> -1 
// value = 0 -> -2

void Initialize() {
    for ( int j = 0; j < 4; j++ ) {
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

    for ( int j = 3; j > -1; j-- ) {

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

void Orderchange(int a, int b, int c, int d) {
    int n;
    N[8].l = output[0];
    n = pow(2, d);
    N[8].r = output[n];
    n = pow(2, c);
    N[9].l = output[n];
    n = pow(2, c) + pow(2, d);
    N[9].r = output[n];
    n = pow(2, b);
    N[10].l = output[n];
    n = pow(2, b) + pow(2, d);
    N[10].r = output[n];
    n = pow(2, b) + pow(2, c);
    N[11].l = output[n];
    n = pow(2, b) + pow(2, c) + pow(2, d);
    N[11].r = output[n];
    n = pow(2, a);
    N[12].l = output[n];
    n = pow(2, a) + pow(2, d);
    N[12].r = output[n];
    n = pow(2, a) + pow(2, c);
    N[13].l = output[n];
    n = pow(2, a) + pow(2, c) + pow(2, d);
    N[13].r = output[n];
    n = pow(2, a) + pow(2, b);
    N[14].l = output[n];
    n = pow(2, a) + pow(2, b) + pow(2, d);
    N[14].r = output[n];
    n = pow(2, a) + pow(2, b) + pow(2, c);
    N[15].l = output[n];
    N[15].r = output[15];
}

void Spec_check(int a, int b, int c, int d) {

    Initialize();
    powersum = 0;
    nodesum = 0;

    Orderchange(a, b, c, d);

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
    int nodedif = 0;
    double nodedifsum = 0;
    double nodedifsumpercent = 0;
    double mnodepowerdif = 0;
    double powerdifsum = 0;
    double powerdifsumpercent = 0;
    int outputdif[16];
    srand((unsigned int)time(NULL));
    
    for ( int i = 0; i < 100000; i++ ) {
            vector<int> nums { 0, 1, 2, 3 };
        for ( int i = 0; i < 16; i++ ) {
            output[i] = rand() % 2 - 2;

            /*
            if ( output[i] == -1 ) {
            cout << "1 ";
            } else {
                cout << "0 ";
            }
            */
        } 
        //cout << endl;
        printf("%d\n", i);
        
        do {
            Spec_check(nums[0], nums[1], nums[2], nums[3]);
        } while (next_permutation(nums.begin(), nums.end()));
        
        //cout << "Minimum Node " << mnode.power << " " << mnode.node << endl;
        //cout << "Minimum Power " << mpower.power << " " << mpower.node << endl;

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
                for ( int i = 0; i < 16; i++ ) {
                    outputdif[i] = output[i];
                }
            }
        }

        mnode.node = 16;
        mnode.power = 100000;
        mpower.node = 16;
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
    for ( int i = 0; i < 16; i++ ) {
        if ( outputdif[i] == -1 )
        cout << "1 ";
        else cout << "0 ";
    }
    cout << endl;

    return 0;
}