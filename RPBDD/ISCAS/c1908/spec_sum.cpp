// 9/13 writen
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

struct spec {
    double node;
    double avenode;
    double power;
    double avepower;
    double maxpower;
    bool visit = false;
};

struct spec average;
struct spec order;
struct spec reorder;
struct spec mpower;
struct spec mnode;
double timesum;
bool active = false;
bool inputcheck = false;
int line = 0;
int circuit = 0;

double double_make( string str, int start ) {
    string quantity = str.substr(start);
    //printf("%lf\n", stod(quantity));
    return stod(quantity);
}

int int_make( string str, int start ) {
    string quantity = str.substr(start);
    //printf("%d\n", stoi(quantity));
    return stoi(quantity);
}

void time_sum( string str, int start ) {
    string quantity = str.substr(start);
    //printf("%lf\n", stod(quantity));
    timesum += stod(quantity);
}

int main() {

    string infilename;
    infilename = "result.txt";
    ifstream ifs(infilename);
    string str;
    if (ifs.fail())
    {
        cerr << "失敗" << endl;
        return -1;
    }

    string input;
    cin >> input;

    while (getline(ifs, str)) {
        if ( str[0] == 'S' ) {
            inputcheck = true;
            line = 0;
            active = false;
        }

        if ( inputcheck == true ) {
            if ( line == 2 ) {
                if ( str[0] == input[0] && str[1] == input[1] ) {
                    active = true;
                    inputcheck = false;
                    circuit++;
                } else {
                    inputcheck = false;
                    //cout << "ok" << endl;
                }
            }
            line++;
            //cout << line << endl;
        }

        if ( active == true ) {
        if ( str[0] == '-') {
            if ( str[5] == '-' ) average.visit = true;
            if ( str[5] == 'O' ) order.visit = true;
            if ( str[5] == 'R' ) reorder.visit = true;
            if ( str[5] == 'M' ) {
                if ( str[13] == 'p' ) mpower.visit = true;
                if ( str[13] == 'n' ) mnode.visit = true;
            }
        } else {
            if ( average.visit == true ) {
                if ( str[0] == 'S' ) average.visit = false;
                if ( str[0] == 'T' ) {
                    time_sum(str, 7);
                }
                if ( str[0] == 'A' ) {
                    if ( str[8] == 'n' ) {
                        average.avenode += double_make(str, 16);
                    }
                    if ( str[8] == 'p' ) {
                        average.power += double_make(str, 16);
                        average.visit = false;
                    }
                }
            }
            if ( order.visit == true ) {
                if ( str[0] == 'N' ) {
                    order.node += double_make(str, 7);
                }
                if ( str[0] == 'P' ) {
                    order.power += double_make(str, 8);
                    order.visit = false;
                }
            }
            if ( reorder.visit == true ) {
                if ( str[0] == 'N' ) {
                    reorder.node += double_make(str, 7);
                }
                if ( str[0] == 'P' ) {
                    reorder.power += double_make(str, 8);
                    reorder.visit = false;
                }
            }
            if ( mpower.visit == true ) {
                if ( str[0] == 'N' ) {
                    mpower.node += double_make(str, 7);
                }
                if ( str[0] == 'P' ) {
                    mpower.power += double_make(str, 8);
                    mpower.visit = false;
                }
            }
            if ( mnode.visit == true ) {
                if ( str[0] == 'N' ) {
                    mnode.node += double_make(str, 7);
                }
                if ( str[0] == 'M' ) {
                    if ( str[1] == 'i' ) {
                        mnode.power += double_make(str, 12);
                    } else {
                        mnode.maxpower += double_make(str, 12);
                    }
                }
                if ( str[0] == 'A' ) {
                    mnode.avepower += double_make(str, 16);
                    mnode.visit = false;
                }
            }
        }
        }
    }

    printf("\nNumber of circuits : %d\n", circuit);
    printf("----------RESULT----------\n");
    printf("Time : %lf[s]\nAverage nodes : %lf\nAverage power : %lf[mW]\n\n", timesum / circuit, average.avenode / circuit , average.power / 1000 / circuit);
    printf("-----Order from frequenct variables-----\n");
    printf("Node : %lf\nPower : %lf[mW]\n\n", order.node / circuit, order.power / 1000 / circuit);
    printf("-----Reverse order from frequent variables-----\n");
    printf("Node : %lf\nPower : %lf[mW]\n\n", reorder.node / circuit, reorder.power / 1000 / circuit);
    printf("-----Minimum power result-----\n");
    printf("Node : %lf\nPower : %lf[mW]\n\n", mpower.node / circuit, mpower.power / 1000 / circuit );
    printf("-----Minimum node result------\n");
    printf("Node : %lf\nMin power : %lf[mW]\nMax power : %lf[mW]\nAverage power : %lf[mW]\n\n", mnode.node / circuit, mnode.power / 1000 / circuit, mnode.maxpower / 1000 / circuit, mnode.avepower / 1000 / circuit);
    printf("------------------------------\n");

    return 0;
}