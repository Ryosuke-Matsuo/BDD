#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <fstream>
#include <queue>

using namespace std;

struct spec {
    int node = 0;
    double power = 0;
    int id;
    int input;
};

struct spec Rev;
struct spec normal;

bool compare_power( const spec& left, const spec& right ) {
    return left.power == right.power ? left.node < right.node : left.power < right.power;
}

static const double INF = 1000000000;
struct spec N[1000];
struct spec R[1000];
struct spec minpower;
struct spec minnode;

int main() {

    int Cid;
    int lut;
    int numberC;
    //scanf("%d", &Cid);
    //scanf("%d", &lut);
    //scanf("%d", &numberC);
    Cid = 7552;
    lut = 10;
    //numberC = 769;
    int border = 9;

    char infilename[50];
    sprintf(infilename, "result/reverse/c%d_lut%d_reverse.txt", Cid, lut);

    ifstream ifs(infilename);
    string str;
    if (ifs.fail())
    {
        cerr << "can not read" << endl;
        return -1;
    }

    int line = 0;
    while (getline(ifs, str)) {
        int start = 0;
        int end;
        string numstr;

        for ( int i = 0; i < str.size(); i++ ) {
            if ( str[i] == ' ' ) {
                end = i;
                break;
            }
        }
        numstr = str.substr(start, end - start);
        int input = stoi(numstr);

        start = end + 1;
        for ( int i = start; i < str.size(); i++ ) {
            if ( str[i] == ' ' ) {
                end = i;
                break;
            }
        }
        numstr = str.substr(start, end - start);
        int node = stoi(numstr);

        start = end + 1;
        for ( int i = start; i < str.size(); i++ ) {
            if ( str[i] == '\n' ) {
                end = i;
                break;
            }
        }
        numstr = str.substr(start, end - start);
        double power = stod(numstr);

        if ( input >=  border ) {
        Rev.node += node;
        Rev.power += power;
        R[line].node = node;
        R[line].power = power;
        R[line].id = line;
        line++;
        }
    }

    sprintf(infilename, "result/reverse/c%d_lut%d_normal.txt", Cid, lut);

    ifstream ifs2(infilename);
    str;
    if (ifs2.fail())
    {
        cerr << "can not read" << endl;
        return -1;
    }

    line = 0;
    while (getline(ifs2, str)) {
        int start = 0;
        int end;
        string numstr;

        for ( int i = 0; i < str.size(); i++ ) {
            if ( str[i] == ' ' ) {
                end = i;
                break;
            }
        }
        numstr = str.substr(start, end - start);
        int input = stoi(numstr);

        start = end + 1;
        for ( int i = start; i < str.size(); i++ ) {
            if ( str[i] == ' ' ) {
                end = i;
                break;
            }
        }
        numstr = str.substr(start, end - start);
        int node = stoi(numstr);

        start = end + 1;
        for ( int i = start; i < str.size(); i++ ) {
            if ( str[i] == '\n' ) {
                end = i;
                break;
            }
        }
        numstr = str.substr(start, end - start);
        double power = stod(numstr);

        if ( input >= border ) {
        normal.node += node;
        normal.power += power;
        N[line].node = node;
        N[line].power = power;
        N[line].id = line;
        line++;
        }
    }

    FILE *outputfile;
    char outfilename[50];
    sprintf(outfilename, "result/reverse/c%d_lut%d_function_class.txt", Cid, lut);
    outputfile = fopen(outfilename, "w");
    fclose(outputfile);
    outputfile = fopen(outfilename, "a");

    sort(N, N + line, compare_power);
    for ( int i = 0; i < line; i++ ) {
        int num = N[i].id;
        printf("%d %d %lf %d %lf\n", i, N[i].node, N[i].power, R[num].node, R[num].power);
        fprintf(outputfile, "%d %d %lf %d %lf\n", i, N[i].node, N[i].power, R[num].node, R[num].power);
    }
    fclose(outputfile);


    printf("Reverse : %lf\n", Rev.power);
    printf("Normal : %lf\n", normal.power);

    return 0;
}