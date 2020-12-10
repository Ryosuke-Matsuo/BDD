#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;

int main() {

    int Cid;
    int lut;

    //Cid = 7552;
    scanf("%d", &Cid);
    scanf("%d", &lut);

    FILE *outputfile;
    char outfilename[50];
    sprintf(outfilename, "reverse_func-class_make.plt");

    outputfile = fopen(outfilename, "w");
    fprintf(outputfile, "set terminal png\n");
    fprintf(outputfile, "set output 'graph/reverse_compare_function.png'\n");
    fprintf(outputfile, "set xl \"function\"\n");
    fprintf(outputfile, "set yl \"Power [uW]\"\n");
    fprintf(outputfile, "set key left top\n");
    fprintf(outputfile, "plot \"reverse/c%d_lut%d_function_class.txt\" using 1:3 with points pointsize 1 title \"Normal\", \"reverse/c%d_lut%d_function_class.txt\" using 1:5 with points pointsize 1 title \"Reverse\"\n", Cid, lut, Cid, lut);
    fprintf(outputfile, "set terminal postscript enhanced color\n");
    fprintf(outputfile, "set output 'graph/reverse_compare_function.eps'\n");
    fprintf(outputfile, "set size 0.6,0.6\n");
    fprintf(outputfile, "set xl \"function\"\n");
    fprintf(outputfile, "set yl \"Power [uW]\"\n");
    fprintf(outputfile, "set key left top\n");
    fprintf(outputfile, "plot \"reverse/c%d_lut%d_function_class.txt\" using 1:3 with points pointsize 1 title \"Normal\", \"reverse/c%d_lut%d_function_class.txt\" using 1:5 with points pointsize 1 title \"Reverse\"\n", Cid, lut, Cid, lut);
}