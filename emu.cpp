

#include "/Users/maple/Downloads/stdc++.h/stdc++.h"
//#include <bits/stdc++.h>
#include <fstream>
using namespace std;

stringstream buffer;
ifstream myfile("bubble.txt");
string file;
void readfile(){
    if(!myfile){
        cout<<"File is empty !!\n";
    }
    buffer<<myfile.rdbuf();

    file = buffer.str();
    
}

map<string,int> commands;
map<string,int>label;
vector<string>lines;

void get_line(){
    string temp;
    int f=0;
    int r=0;
    for(int i=0;i<file.size();i++){
        if(file[i]=='\n'&&r==1){
            r=0;
            continue;
        }
        if(r)continue;
        if(file[i]=='\n'||file[i]==';'){
            if(file[i]==';')r=1;
            if(temp.size()){
                lines.push_back(temp);
                //cout<<temp<<"\n";
            }
            temp.clear();
            continue;
        }
        temp.push_back(file[i]);
    }
}

vector<string> get_tokens(string line){
    string temp;
    int f=0;
    vector<string>tokens;
    for(int i=0;i<line.size();i++){
        if(!f&&(line[i]==' ')){
            if(temp.size()){
                tokens.push_back(temp);
                //cout<<temp<<" ";
            }
            temp.clear();
            continue;
        }
        temp.push_back(line[i]);
    }
    tokens.push_back(temp);
    return tokens;
}
int checkerr(vector<string> tokens){
    return 1;
}
void set_labels(){
    int pc=0;
    for(int i=0;i<lines.size();i++){
        vector<string> tokens=get_tokens(lines[i]);
        if(tokens.back().size()==0)tokens.pop_back();
        int c=0;
        string s;
        if(tokens[c].back()==':'){
            tokens[c].pop_back();
            cout<<pc<<"\n";
            label[tokens[c]]=pc;
        }
        pc++;
    }
}
int main(){
    ifstream myfile("sum_of_n_natural_nums.txt");
    readfile();
    commands["ldc"]=0;
    commands["adc"]=1;
    commands["ldl"]=2;
    commands["stl"]=3;
    commands["ldnl"]=4;
    commands["stnl"]=5;
    commands["add"]=6;
    commands["sub"]=7;
    commands["shl"]=8;
    commands["shr"]=9;
    commands["adj"]=10;
    commands["a2sp"]=11;
    commands["sp2a"]=12;
    commands["call"]=13;
    commands["return"]=14;
    commands["brz"]=15;
    commands["brlz"]=16;
    commands["br"]=17;
    commands["HALT"]=18;
    get_line();
    set_labels();
    int pc=1;
    vector<string>obj;
    ofstream o_file("bubble.o");
    for(int i=0;i<lines.size();i++){
        vector<string> tokens=get_tokens(lines[i]);
        if(tokens.back().size()==0)tokens.pop_back();
        
        int c=0;
        
        for(auto u:tokens){
            cout<<u<<"|";
        }
        cout<<"\n";
        string s;
        if(tokens[c].back()==':'){
            c++;
        }
        int val=pc;
        for(int j=7;j>=0;j--){
            s.push_back('0'+(val>>j&1));
        }            
        int cnt=0;
        while(c<tokens.size()){
            if(tokens[c].back()==' ')tokens[c].pop_back();
            if(cnt==1){
                int val=0;
                if(tokens[c].back()>='a'){
                    cout<<label[tokens[c]]<<" ";
                    val=label[tokens[c]];
                }
                else{
                    stringstream val_s(tokens[c]);
                        val_s>>val;
                        
                }
                for(int j=7;j>=0;j--){
                    s.push_back('0'+(val>>j&1));
                }
                c++;
                continue;
            }
            int op=commands[tokens[c]];
            for(int j=7;j>=0;j--){
                s.push_back('0'+(op>>j&1));
            }
            cnt++;
            c++;
        }
        pc++;
        o_file<<s<<"\n";
        cout<<s<<"\n";
        
    }
    o_file.close();
    if(!file.size()){
        cout<<"File is empty\n";
        return 0;
    }

    return 0;
}
