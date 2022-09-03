#include "/Users/maple/Downloads/stdc++.h/stdc++.h"
#include <fstream>
using namespace std;

stringstream buffer;
ifstream myfile("sum_of_n_natural_nums.txt");
string file;
void readfile(){
    if(!myfile){
        cout<<"File is empty !!\n";
    }
    buffer<<myfile.rdbuf();
    file = buffer.str();
    
}
map<string,int>m;

vector<string>tokens;

void get_tokens(){
    string temp;
    int f=0;
    for(int i=0;i<file.size();i++){
        if(file[i]=='"'){
            if(!f)f=1;
            else f=0;
        }
        if(!f&&(file[i]==' '||file[i]=='\n')){
            if(temp.size()){
                tokens.push_back(temp);
                cout<<temp<<"\n";
            }
            temp.clear();
            continue;
        }
        temp.push_back(file[i]);
    }
}
// unordered_map<string,int> arguments={
//     {"add",1}
// };


int main(){
    ifstream myfile("sum_of_n_natural_nums.txt");
    readfile();
    get_tokens();
    if(!file.size()){
        cout<<"File is empty\n";
        return 0;
    }

    return 0;
}