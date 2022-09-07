

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
map<string,int> commands={{"ldc",0}, {"adc",1}, {"ldl",2}, {"stl",3}, {"ldnl",4}, {"stnl",5}, {"add",6}, {"sub",7}, {"shl",8}, {"shr",9}, {"adj",10}, {"a2sp",11}, {"sp2a",12}, {"call",13}, {"return",14}, {"brz",15}, {"brlz",16}, {"br",17}, {"HALT",18}};


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
int checkerr(){
    for(int i=0;i<tokens.size();i++){
        if(tokens[i].back()==':'||tokens[i][0]=='$'){
            continue;
        }
        if(!commands.count(tokens[i])){
            return 0;
        }
    }
}
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