

#include "stdc++.hpp"
//#include <bits/stdc++.h>
#include <fstream>
using namespace std;

stringstream buffer;
ifstream myfile;
ofstream o_file("output.o");
ofstream log_file("asm_log.txt");
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
            if(tokens[c]=="labels:"){
                cout<<"label declaration 'label:' not allowed. Choose a different name"<<" (line "<<i+1<<")\n";
            }
            tokens[c].pop_back();
            cout<<pc<<"\n";
            if(label.find(tokens[c])!=label.end()){
                log_file<<"duplicate label found: "<<tokens[c]<<" (line "<<i+1<<")\n";
            }
            label[tokens[c]]=pc;
        }
        pc++;
    }
}
int main(int argc, char* argv[]){
    myfile.open(argv[1]);
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
        int cnt=0;
        string s1;
        while(c<tokens.size()){
            if(tokens[c].back()==' ')tokens[c].pop_back();
            if(cnt==1){
                int val=0;
                if(tokens[c].back()>='a'){
                    if(label.find(tokens[c])==label.end()){
                        log_file<<"invalid label: "<<tokens[c]<<" (line "<<i+1<<")\n";
                    }
                    val=label[tokens[c]];
                }
                else{
                    stringstream val_s(tokens[c]);
                    val_s>>val;

                }
                for(int j=23;j>=0;j--){
                    s.push_back('0'+(val>>j&1));
                }
                c++;
                continue;
            }
            if(commands.find(tokens[c])==commands.end()){
                log_file<<"invalid command: "<<tokens[c]<<" (line "<<i+1<<")\n";
            }
            int op=commands[tokens[c]];
            for(int j=7;j>=0;j--){
                s1.push_back('0'+(op>>j&1));
            }
            cnt++;
            c++;
        }
        if(s.size()==0){
            s=string(24,'0');
        }
        s.append(s1);
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