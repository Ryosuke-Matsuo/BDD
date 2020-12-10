#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
#include <vector>

using namespace std;

struct node {
    int l, r;
    bool exist;
    double power;
};

struct node N[8];
int nodesum;
int output[7];

// value = 1 -> -1 
// value = 0 -> -2

void Initialize() {
    for ( int j = 0; j < 3; j++ ) {
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

    for ( int j = 2; j > -1; j-- ) {

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

void powersearch(int n, double ppower, double &powersum) {
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
        powersearch(N[n].l, N[n].power, powersum);
    } else {
        //printf("F ");
    }

    if ( N[n].r == -1 ) {
        //printf("T ");
        powersum = powersum + N[n].power;
    } else if ( N[n].r != -2 ) {
        powersearch(N[n].r, N[n].power, powersum);
    } else {
        //printf("F ");
    }
}

void Orderchange(int a, int b, int c) {
    int n;
    N[4].l = output[0];
    n = pow(2, c);
    N[4].r = output[n];
    n = pow(2, b);
    N[5].l = output[n];
    n = pow(2, b) + pow(2, c);
    N[5].r = output[n];
    n = pow(2, a);
    N[6].l = output[n];
    n = pow(2, a) + pow(2, c);
    N[6].r = output[n];
    n = pow(2, a) + pow(2, b);
    N[7].l = output[n];
    N[7].r = output[7];
}

int main() {
    
    for ( int i = 0; i < 8; i++ ) {
        cout << i << "-th Output value :";
        cin >> output[i];
    } 

    double powersum = 0;

    Initialize();

    Orderchange(2, 1, 0);

    Reduction();

    for ( int i = 1; i < 8; i++ ) {
        cout << i << " " << N[i].l << " " << N[i].r << endl;
    } 

    powersearch(1, 10, powersum);
    cout << powersum << " " << nodesum << endl;

    

    Initialize();
    powersum = 0;
    nodesum = 0;

    Orderchange(0, 1, 2);

    Reduction();

    for ( int i = 1; i < 8; i++ ) {
        cout << i << " " << N[i].l << " " << N[i].r << endl;
    } 

    powersearch(1, 10, powersum);
    cout << powersum << " " << nodesum << endl;

    Initialize();
    powersum = 0;
    nodesum = 0;

    Orderchange(0, 2, 1);

    Reduction();

    for ( int i = 1; i < 8; i++ ) {
        cout << i << " " << N[i].l << " " << N[i].r << endl;
    } 

    powersearch(1, 10, powersum);
    cout << powersum << " " << nodesum << endl;
    

    return 0;
}