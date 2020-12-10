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
    int variable;
};

struct spec {
    double node = 32;
    double power = 100000000;
};

struct RPnode {
    double all = 32;
    //double second = 2;
    //double third = 4;
};

struct node NC[32];
struct node NS[32];
struct node NCR[32];
struct node NSR[32];

struct node N[32];
int nodesum;
//int Rnodesum;
struct RPnode Rnodesum;
struct RPnode mRnodesum;
double powersum;
double powersumT;
//double powersumF;
int output[31];

struct spec mnode;
struct spec mpower;
struct spec mRnode;
struct spec mCpower;

double mnode_Mpower = 0;

// value = 1 -> -1 
// value = 0 -> -2

void Initialize() {
    for ( int j = 0; j < 5; j++ ) {
        for ( int i = 0; i < pow(2, j); i++ ) {
            int v = pow(2, j) + i;
            N[v].l = v * 2;
            N[v].r = v * 2 + 1;
            N[v].exist = false;
            N[v].variable = j;
        }
    }
}

void Reduction() {
    int v;

    for ( int j = 4; j > -1; j-- ) {

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

    Rnodesum.all++;
    //if ( N[n].variable == 1 ) Rnodesum.second++;
    //if ( N[n].variable == 2 ) Rnodesum.third++;

    if ( N[n].l == -1 ) {
        //printf("T ");
        powersumT = powersumT + N[n].power;
    } else if ( N[n].l == -2 ) {
        //powersumF = powersumF + N[n].power;
    } else {
        powersearch(N[n].l, N[n].power);
    }

    if ( N[n].r == -1 ) {
        //printf("T ");
        powersumT = powersumT + N[n].power;
    } else if ( N[n].r == -2 ) {
        //powersumF = powersumF + N[n].power;
    } else {
        powersearch(N[n].r, N[n].power);
    }
}

void Orderchange(int a, int b, int c, int d, int e) {
    int n;
    N[16].l = output[0];
    n = pow(2, e);
    N[16].r = output[n];
    n = pow(2, d);
    N[17].l = output[n];
    n = pow(2, d) + pow(2, e);
    N[17].r = output[n];
    n = pow(2, c);
    N[18].l = output[n];
    n = pow(2, c) + pow(2, e);
    N[18].r = output[n];
    n = pow(2, c) + pow(2, d);
    N[19].l = output[n];
    n = pow(2, c) + pow(2, d) + pow(2, e);
    N[19].r = output[n];
    n = pow(2, b);
    N[20].l = output[n];
    n = pow(2, b) + pow(2, e);
    N[20].r = output[n];
    n = pow(2, b) + pow(2, d);
    N[21].l = output[n];
    n = pow(2, b) + pow(2, d) + pow(2, e);
    N[21].r = output[n];
    n = pow(2, b) + pow(2, c);
    N[22].l = output[n];
    n = pow(2, b) + pow(2, c) + pow(2, e);
    N[22].r = output[n];
    n = pow(2, b) + pow(2, c) + pow(2, d);
    N[23].l = output[n];
    n = pow(2, b) + pow(2, c) + pow(2, d) + pow(2, e);
    N[23].r = output[n];
    n = pow(2, a);
    N[24].l = output[0];
    n = pow(2, a) + pow(2, e);
    N[24].r = output[n];
    n = pow(2, a) + pow(2, d);
    N[25].l = output[n];
    n = pow(2, a) + pow(2, d) + pow(2, e);
    N[25].r = output[n];
    n = pow(2, a) + pow(2, c);
    N[26].l = output[n];
    n = pow(2, a) + pow(2, c) + pow(2, e);
    N[26].r = output[n];
    n = pow(2, a) + pow(2, c) + pow(2, d);
    N[27].l = output[n];
    n = pow(2, a) + pow(2, c) + pow(2, d) + pow(2, e);
    N[27].r = output[n];
    n = pow(2, a) + pow(2, b);
    N[28].l = output[n];
    n = pow(2, a) + pow(2, b) + pow(2, e);
    N[28].r = output[n];
    n = pow(2, a) + pow(2, b) + pow(2, d);
    N[29].l = output[n];
    n = pow(2, a) + pow(2, b) + pow(2, d) + pow(2, e);
    N[29].r = output[n];
    n = pow(2, a) + pow(2, b) + pow(2, c);
    N[30].l = output[n];
    n = pow(2, a) + pow(2, b) + pow(2, c) + pow(2, e);
    N[30].r = output[n];
    n = pow(2, a) + pow(2, b) + pow(2, c) + pow(2, d);
    N[31].l = output[n];
    n = pow(2, a) + pow(2, b) + pow(2, c) + pow(2, d) + pow(2, e);
    N[31].r = output[n];
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

void Node_check( struct node *Node, struct node *Up ) {
    for ( int i = 0; i < 32; i++ ) {
        if ( Up[i].exist == true ) {
            Node[i].exist = true;
            Node[i].l = Up[i].l;
            Node[i].r = Up[i].r;
            Node[i].variable = Up[i].variable;
        } else {
            Node[i].exist = false;
        }
    }
}

void Spec_check(int a, int b, int c, int d, int e) {

    Initialize();
    powersum = 0;
    powersumT = 0;
    //powersumF = 0;
    nodesum = 0;
    Rnodesum.all = 0;
    //Rnodesum.second = 0;
    //Rnodesum.third = 0;

    Orderchange(a, b, c, d, e);

    Reduction();

    /*
    for ( int i = 1; i < 16; i++ ) {
        cout << i << " " << N[i].l << " " << N[i].r << endl;
    } 
    */

    powersearch(1, 10);
    //powersum = min(powersumT, powersumF);
    powersum = powersumT;
    //cout << powersum << " " << nodesum << endl;

    if ( nodesum < mnode.node ) {
        mnode.node = nodesum;
        mnode.power = powersum;
        Node_check( NC, N );

        mnode_Mpower = powersum;
    }
    if ( nodesum == mnode.node ) {
        if ( powersum < mnode.power ) {
            mnode.node = nodesum;
            mnode.power = powersum;
            Node_check( NC, N );
        }
        if ( powersum > mnode_Mpower ) {
            mnode_Mpower = powersum;
        }
    }

    if ( Rnodesum.all < mRnodesum.all ) {
        mRnodesum.all = Rnodesum.all;
        mRnode.power = powersum;
        Node_check( NCR, N );
    } else if ( Rnodesum.all == mRnodesum.all && powersum < mRnode.power ) {
        mRnodesum.all = Rnodesum.all;
        mRnode.power = powersum;
        Node_check( NCR, N );
    }

    /*
    if ( Rnodesum.second < mRnodesum.second ) {
        mRnodesum.second = Rnodesum.second;
        mRnode.power = powersum;
    } else if ( Rnodesum.second == mRnodesum.second && powersum < mRnode.power ) {
        mRnodesum.second = Rnodesum.second;
        mRnode.power = powersum;
    }
    */
    
    /*
    if ( Rnodesum.third < mRnodesum.third ) {
        mRnodesum.third = Rnodesum.third;
        mRnode.power = powersum;
    } else if ( Rnodesum.third == mRnodesum.third && powersum < mRnode.power ) {
        mRnodesum.third = Rnodesum.third;
        mRnode.power = powersum;
    }
    */
    
    /*
    if ( Rnodesum.second < mRnodesum.second ) {
        mRnodesum.second = Rnodesum.second;
        mRnodesum.third = Rnodesum.third;
        mRnode.power = powersum;
    } else if ( Rnodesum.second == mRnodesum.second ) {
        if ( Rnodesum.third < mRnodesum.third ) {
            mRnodesum.second = Rnodesum.second;
            mRnodesum.third = Rnodesum.third;
            mRnode.power = powersum;
        } else if ( Rnodesum.third == mRnodesum.third && powersum < mRnode.power ) {
            mRnodesum.second = Rnodesum.second;
            mRnodesum.third = Rnodesum.third;
            mRnode.power = powersum;
        }
    }
    */

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
    int Rcounter = 0;
    int Ccounter = 0;
    double powerdif = 0;
    int nodedif = 0;
    double nodedifsum = 0;
    double nodedifsumpercent = 0;
    double mnodepowerdif = 0;
    double powerdifsum = 0;
    double powerdifsumpercent = 0;

    double Rpowerdif = 0;
    double Rpowerdifsum = 0;
    double Rpowerdifsumpercent = 0;
    double Rpowerdifori = 0;

    double Cpowerdif = 0;
    double Cpowerdifsum = 0;
    double Cpowerdifsumpercent = 0;
    double Cpowerdifori = 0;

    double mnodeMpowerCount = 0;
    double mnodeMpowersum = 0;
    double mnodeMpowersumper = 0;
    double mnodeMpowerdif = 0;
    double mnodeMpowerdifori = 0;

    int outputdif[32];
    srand((unsigned int)time(NULL));

    int cycle = 100000;

    for ( int i = 0; i < cycle; i++ ) {
            vector<int> nums { 0, 1, 2, 3, 4 };
        for ( int i = 0; i < 32; i++ ) {
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
            Spec_check(nums[0], nums[1], nums[2], nums[3], nums[4]);
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
                //Node_check( NS, NC );
                for ( int i = 0; i < 32; i++ ) {
                    outputdif[i] = output[i];
                }
            }
        }

        if ( mnode_Mpower != mpower.power ) {
            mnodeMpowerCount++;
            mnodeMpowersum = mnodeMpowersum + mnode_Mpower - mpower.power;
            mnodeMpowersumper = mnodeMpowersumper + ( mnode_Mpower - mpower.power ) / mnode_Mpower;
            if ( mnodeMpowerdif < mnode_Mpower - mpower.power ) {
                mnodeMpowerdif = mnode_Mpower - mpower.power;
                mnodeMpowerdifori = mnode_Mpower;
            }
        }

        if ( mRnode.power != mpower.power ) {
            Rcounter++;
            Rpowerdifsum = Rpowerdifsum + mRnode.power - mpower.power;
            Rpowerdifsumpercent = Rpowerdifsumpercent + ( mRnode.power - mpower.power ) / mRnode.power;
            if ( Rpowerdif < mRnode.power - mpower.power ) {
                Rpowerdif = mRnode.power - mpower.power;
                Rpowerdifori = mRnode.power;
            }
        }

        /*
        if ( mnode.power > mRnode.power ) mCpower.power = mRnode.power;
        else mCpower.power = mnode.power;

        if ( mCpower.power != mpower.power ) {
            Ccounter++;
            Cpowerdifsum = Cpowerdifsum + mCpower.power - mpower.power;
            Cpowerdifsumpercent = Cpowerdifsumpercent + ( mCpower.power - mpower.power ) / mCpower.power;
            if ( Cpowerdif < mCpower.power - mpower.power ) {
                Cpowerdif = mCpower.power - mpower.power;
            }
        }
        */

        if ( mnode.power < mRnode.power ) {
            Ccounter++;
            Cpowerdifsum = Cpowerdifsum + mRnode.power - mnode.power;
            Cpowerdifsumpercent = Cpowerdifsumpercent + ( mRnode.power - mnode.power ) / mRnode.power;
            if ( Cpowerdif < mRnode.power - mnode.power ) {
                Cpowerdif = mRnode.power - mnode.power;
                Cpowerdifori = mRnode.power;
                Node_check( NS, NC );
                Node_check( NSR, NCR );
            }
        }

        mnode.node = 32;
        mnode.power = 100000;
        mpower.node = 32;
        mpower.power = 100000;
        mRnode.node = 32;
        mRnode.power = 100000;
        mRnodesum.all = 32;

        mnode_Mpower = 100000;
        //mRnodesum.second = 2;
        //mRnodesum.third = 4;
    }

    cout << endl << "5-input function" << endl;
    cout << "Iteration " << cycle << endl; 
    cout << "                       " << "Count      " << "Ave. power dif.        " << "Max power dif. " << endl;
    cout << "min node min power     " << counter << "      " << powerdifsum / counter << "  " 
    << powerdifsumpercent / counter * 100 << "%      " << powerdif << "  " << powerdif / mnodepowerdif * 100 << "%" << endl;

    cout << "min node Max power     " << mnodeMpowerCount << "      " << mnodeMpowersum / mnodeMpowerCount << "  " 
    << mnodeMpowersumper / mnodeMpowerCount * 100 << "%      " << mnodeMpowerdif << "  " 
    << mnodeMpowerdif / mnodeMpowerdifori * 100 << "%" << endl;

    cout << "min RPnode min power   " << Rcounter << "       " << Rpowerdifsum / Rcounter << "  " 
    << Rpowerdifsumpercent / Rcounter * 100 << "%      " << Rpowerdif << "  " << Rpowerdif / Rpowerdifori * 100 << "%" << endl;

    cout << "min RPnode > min node  " << Ccounter << "       " << Cpowerdifsum / Ccounter << "  " 
    << Cpowerdifsumpercent / Ccounter * 100 << "%      " << Cpowerdif << "  " << Cpowerdif / Cpowerdifori * 100 << "%" << endl;
    
    /*
    cout << "Node" << endl;
    cout << nodedifsum / counter << " " << nodedifsumpercent / counter * 100 << "%" << endl;
    cout << "Best case" << endl;
    cout << "Powerdif mnodepower nodedif" << endl;
    cout << powerdif << " " << mnodepowerdif << " " << nodedif << endl;
    for ( int i = 0; i < 32; i++ ) {
        if ( outputdif[i] == -1 )
        cout << "1 ";
        else cout << "0 ";
    }
    cout << endl;
    cout << Rpowerdif << " " << Cpowerdif << " " << mnodeMpowerdif << endl;
    */

    /*
    cout << "BDD node min" << endl;
    for ( int i = 0; i < 32; i++ ) {
        if ( NS[i].exist == true ) {
        cout << i << " " << NS[i].l << " " << NS[i].r << " " << NS[i].variable << endl;
        }
    }

    cout << "RPBDD node min" << endl;
    for ( int i = 0; i < 32; i++ ) {
        if ( NSR[i].exist == true ) {
        cout << i << " " << NSR[i].l << " " << NSR[i].r << " " << NSR[i].variable << endl;
        }
    }
    */

    return 0;
}