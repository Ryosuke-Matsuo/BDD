#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    int Cid;
    int lut;
    scanf("%d", &Cid);
    scanf("%d", &lut);
    char infilename[50];
    sprintf(infilename, "c%d/lut%d/iscas_c%d_lut%d.blif", Cid, lut, Cid, lut);

    ifstream ifs(infilename);
    string str;
    if (ifs.fail())
    {
        cerr << "失敗" << endl;
        return -1;
    }

    int count = -3;  //use for naming file
    bool backs = false;
    
    while (getline(ifs, str))
    {
      if ( str[0] == '.' ) {
          count++;
          printf("0");
      }
    

      if ( count > 0 ) {
        if ( str[0] == '.' ) {
            int length = str.length();

            printf("%d", count);
            
            for ( int i = 6; i < length; i++ ) {
                if ( str[i] == '\\' ) backs = true; 
            }
            if ( backs == false ) {
                char filename[50];
                sprintf(filename, "c%d/lut%d/sub_%d.blif", Cid, lut, count);
                ofstream writing_file;
                writing_file.open(filename, ios::out);
                writing_file << str << endl;
            } else {
                string command;
                for ( int i = 0; i < length - 2; i++ ) {
                    command += str[i];
                }
                char filename[50];
                sprintf(filename, "c%d/lut%d/sub_%d.blif", Cid, lut, count);
                ofstream writing_file;
                writing_file.open(filename, ios::out);
                writing_file << command;
            }
        } else if ( backs == true ) {
            int length = str.length();
            string command;
            for ( int i = 1; i < length; i++ ) {
                command += str[i];
            }
                char filename[50];
                sprintf(filename, "c%d/lut%d/sub_%d.blif", Cid, lut, count);
                ofstream writing_file;
                writing_file.open(filename, ios::app);
                writing_file << str << endl;
                backs = false;
        } else {
                char filename[50];
                sprintf(filename, "c%d/lut%d/sub_%d.blif", Cid, lut, count);
                ofstream writing_file;
                writing_file.open(filename, ios::app);
        writing_file << str << endl;
        }
        }
    }
    return 0;
}