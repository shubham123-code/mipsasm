/*****************************************************************************
TITLE: Assembler C Program																																
AUTHOR:   		Shubham Ghodke
ROLL NUMBER:	2001CS68
Declaration of Authorship
This .cpp file, asm.cpp, is part of the assignment of CS321 at the 
department of Computer Science and Engineering, IIT Patna . 
*****************************************************************************/

#include "stdc++.h"
//#include <bits/stdc++.h>

#include <fstream>
using namespace std;

stringstream buffer;
ifstream myfile;
ofstream o_file;
ofstream log_file("asm_log.txt");
ofstream listing;
string file;
void readfile()
{
    if (!myfile)
    {
        cout << "File is empty !!\n";
    }
    buffer << myfile.rdbuf();

    file = buffer.str();
}

map<string, int> commands;
map<string, int> label;
map<string, int> label_data;
vector<string> lines;
int errs = 0;

// separate the input file into lines and stores them in a vector of strings
void trim()
{
    string newFile;
    int f = 0;
    int cnt = 0;
    for (int i = 0; i < file.size(); i++)
    {
        if (file[i] == ';')
        {
            f = 1;
            continue;
        }
    
        if (file[i] == '\n' && f == 1 )
        {
            f = 0;
            if(newFile.back()!=':')newFile.push_back('\n');
            cnt++;
            continue;
        }
        if (f)
            continue;
        if (file[i] == '\n' && newFile.back() == '\n')
            continue;
        if (newFile.back() == ':' && (file[i] == ' ' || file[i] == '\n'||file[i]=='\t'))
            continue;
        if (newFile.back() == ':')
            newFile.push_back(' ');
        if (file[i] == '\n')
            cnt++;
        
        newFile.push_back(file[i]);
    }
    cout<<newFile;
    file = newFile;
}
void get_line()
{
    string temp;
    int f = 0;
    int r = 0;
    for (int i = 0; i < file.size(); i++)
    {
        if (file[i] == '\n' && r == 1)
        {
            r = 0;
            continue;
        }
        if (r)
            continue;
        if (file[i] == '\n' || file[i] == ';')
        {
            if (file[i] == ';')
                r = 1;
            if (temp.size())
            {
                reverse(temp.begin(),temp.end());
                while(temp.back()==' ')temp.pop_back();
                reverse(temp.begin(),temp.end());
                lines.push_back(temp);
                
            }
            temp.clear();
            continue;
        }
        temp.push_back(file[i]);
    }
    if (temp.size())
    {
        reverse(temp.begin(),temp.end());
        while(temp.back()==' ')temp.pop_back();
        reverse(temp.begin(),temp.end());
        lines.push_back(temp);
    }
}

// separates tokens from an input line
vector<string> get_tokens(string line)
{
    string temp;
    int f = 0;
    vector<string> tokens;
    for (int i = 0; i < line.size(); i++)
    {
        if (!f && (line[i] == ' ' || line[i] == ':'|| line[i]=='\t'))
        {
            if (line[i] == ':')
                temp.push_back(':');
            if (temp.size())
            {
                tokens.push_back(temp);
                
            }
            temp.clear();
            continue;
        }
        temp.push_back(line[i]);
    }
    tokens.push_back(temp);
    return tokens;
}

// stores label address into a map
void set_labels()
{
    int pc = 0;

    for (int i = 0; i < lines.size(); i++)
    {

        vector<string> tokens = get_tokens(lines[i]);
        if (tokens.back().size() == 0)
            tokens.pop_back();
        int c = 0;
        string s;
        for (int i = 0; i < tokens.size(); i++)
        {
            if (tokens[i] == "data"||tokens[i]== "SET")
            {
                if (i != 0)
                {
                    stringstream val_s(tokens[i + 1]);
                    int val;
                    val_s >> val;
                    label_data[tokens[i - 1]] = pc;
                    break;
                }
            }
        }
        if (tokens[c].back() == ':')
        {
            tokens[c].pop_back();
            if (label.find(tokens[c]) != label.end())
            {
                log_file << "duplicate label found: " << tokens[c] << " (line " << i + 1 << ")\n";
                errs = 1;
            }
            label[tokens[c]] = pc;
        }

        if (tokens.size() >= 1 && tokens.back().back() != ':')
            pc++;
    }
}

// binary string to hex convertor
string bin_hex(string s)
{
    string k;
    for (int i = 0; i < s.size(); i += 4)
    {
        int t = 8 * (s[i] - '0') + 4 * (s[i + 1] - '0') + 2 * (s[i + 2] - '0') + s[i + 3] - '0';
        if (t < 10)
        {
            k.push_back('0' + t);
        }
        else
        {
            k.push_back('a' + t - 10);
        }
    }
    return k;
}
// main function
int main(int argc, char *argv[])
{
    // setting names of the output files
    myfile.open(argv[1]);
    char out_name[100];
    strcpy(out_name, argv[1]);
    int sz = strlen(out_name);
    out_name[sz - 2] = '\0';
    out_name[sz - 3] = 'o';

    o_file.open(out_name, ios::out | ios::binary);
    out_name[sz - 3] = 'l';
    listing.open(out_name);
    readfile();
    trim();
    // storing instruction codes for each instructions into a map
    commands["ldc"] = 0;
    commands["adc"] = 1;
    commands["ldl"] = 2;
    commands["stl"] = 3;
    commands["ldnl"] = 4;
    commands["stnl"] = 5;
    commands["add"] = 6;
    commands["sub"] = 7;
    commands["shl"] = 8;
    commands["shr"] = 9;
    commands["adj"] = 10;
    commands["a2sp"] = 11;
    commands["sp2a"] = 12;
    commands["call"] = 13;
    commands["return"] = 14;
    commands["brz"] = 15;
    commands["brlz"] = 16;
    commands["br"] = 17;
    commands["HALT"] = 18;
    commands["data"] = 19;
    commands["SET"] = 20;

    // initialising the process
    get_line();
    set_labels();
    if(errs){
        if (errs)
        {
            cout<<"errors in file check the log file to the errors.\n";
            o_file.clear();
            listing.clear();
            
        }
    }
    int pc = 0;
    vector<string> obj;

    for (int i = 0; i < lines.size(); i++)
    {
        vector<string> tokens = get_tokens(lines[i]);
        if (tokens.back().size() == 0)
            tokens.pop_back();
        if (tokens.size() == 1 && tokens[i].back() == ':')
        {
            continue;
        }
        int c = 0;
        
        // cout << "\n";
        string s;
        // increase iterator incase encounter a label
        if (tokens[c].back() == ':')
        {
            c++;
            
        }
        for(auto u:tokens){
            cout<<u<<"|";
        }
        int cnt = 0;
        string s1;
        int branch=0;
        if(tokens[c]=="data"||tokens[c]=="SET"){
            int val=stoi(tokens[c+1],nullptr,10);
            for(int i=31;i>=0;i--){
                s1.push_back('0'+(val>>i&1));
            }
            branch=1;
        }
        // processing the input string
        while (c < tokens.size()&&!branch)
        {
            while (tokens[c].back() == ' '||tokens[c].back()=='\t')
                tokens[c].pop_back();
            if (cnt == 1)
            {
                int val = 0;
                if (label_data.find(tokens[c] + ":") != label_data.end()||label.find(tokens[c]) != label.end())
                {
                    // separate block for data operation
                    if (label_data.find(tokens[c] + ":") != label_data.end())
                    {
                        // cout << tokens[c] << "\n";
                        val = label_data[tokens[c] + ":"];
                        // cout << val << "\n";
                    }
                    else
                    {
                        // incase no data operation
                        if (label.find(tokens[c]) == label.end())
                        {
                            log_file << "invalid label: " << tokens[c] << " (line " << i + 1 << ")\n";
                            errs = 1;
                        }
                        val = label[tokens[c]] - pc - 1;
                    }
                }
                else
                {
                    //converting opcode to integer

                    if(tokens[c].size()<2||tokens[c][1]!='x'){
                        int f=0;
                        for(auto u:tokens[c]){
                            if(u-'0'>9){
                                f=1;
                            }
                        }
                        if(!f)val=stoull(tokens[c],nullptr,10);
                        else{
                            log_file<<"input is not an integer: "<<tokens[c]<<"\n";
                        }
                    }
                    else {
                        val = stoull(tokens[c],nullptr,16);
                    }
                    
                }
                for (int j = 23; j >= 0; j--)
                {
                    s.push_back('0' + (val >> j & 1));
                }
                c++;
                continue;
            }
            if (cnt == 1)
            {
                log_file << "Extra operand found (line " << i + 1 << ")\n";
            }
            if (commands.find(tokens[c]) == commands.end())
            {
                log_file << "invalid command: " << tokens[c] << " (line " << i + 1 << ")\n";
                errs = 1;
            }
            int op = commands[tokens[c]];
            for (int j = 7; j >= 0; j--)
            {
                s1.push_back('0' + (op >> j & 1));
            }
            cnt++;
            c++;
        }
        // converting string to asm file
        if (s.size() == 0&&s1.size()==8)
        {
            s = string(24, '0');
        }
        string value=s;
        s.append(s1);
        string pc1;
        string temp;
        for (int i = 7; i >= 0; i--)
        {
            if (pc >> i & 1)
            {
                temp.push_back('1');
            }
            else
            {
                temp.push_back('0');
            }
        }
        if(!errs){
            pc1 = bin_hex(temp);
            int sz = pc1.size();
            pc1 = string(8 - sz, '0') + pc1;
            if(tokens[0].back()==':'){
                listing<<pc1<<" "<<tokens[0]<<"\n";
            }
            string lis;
            if(!branch)lis = pc1 + " " + bin_hex(s) + " " + tokens[cnt-1] + " 0x"  + bin_hex(value);
            else lis=pc1 + " " + bin_hex(s) + " data 0x" + bin_hex(s); 
            listing << lis << "\n";
            int t = stoul(s, nullptr, 2);
            o_file.write((char *)&t, sizeof(t));
        }
       
        pc++;
    }
    if (errs)
    {
        cout<<"errors in file check the log file to the errors.\n";
        o_file.clear();
        listing.clear();
    }
    else{
        log_file<<"no errors found.\n";
    }
    o_file.close();
    log_file.close();
    listing.close();
    if (!file.size())
    {
        cout << "File is empty\n";
        return 0;
    }

    return 0;
}