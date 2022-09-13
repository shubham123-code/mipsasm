#include "stdc++.h"
#include <fstream>

using namespace std;

ifstream myfile;
string file;
ofstream output;

void read_file(){
    if(!myfile){
        cout<<"file is empty";
        return;
    }
    stringstream buffer;
    buffer<<myfile.rdbuf();
    file = buffer.str();
    cout<<file<<"\n";
}

void get_line(string s){

}


int main(int argc, char* argv[]){
    myfile.open(argv[1]);
    output.open("output.txt");
    read_file();
    
    myfile.close();
}