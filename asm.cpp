#include "/Users/maple/Downloads/stdc++.h/stdc++.h"
#include <fstream>
using namespace std;

int main(){
    ifstream myfile("sum_of_n_natural_nums.txt");
    stringstream buffer;
    buffer<<myfile.rdbuf();
    cout<<buffer.str();
    return 0;
}