

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
                lines.push_back(temp);
                // cout<<temp<<"\n";
            }
            temp.clear();
            continue;
        }
        temp.push_back(file[i]);
    }
    if (temp.size())
    {
        lines.push_back(temp);
    }
}

vector<string> get_tokens(string line)
{
    string temp;
    int f = 0;
    vector<string> tokens;
    for (int i = 0; i < line.size(); i++)
    {
        if (!f && (line[i] == ' ' || line[i] == ':'))
        {
            if (line[i] == ':')
                temp.push_back(':');
            if (temp.size())
            {
                tokens.push_back(temp);
                // cout<<temp<<" ";
            }
            temp.clear();
            continue;
        }
        temp.push_back(line[i]);
    }
    tokens.push_back(temp);
    return tokens;
}

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
            if (tokens[i] == "data")
            {
                if (i != 0)
                {
                    stringstream val_s(tokens[i + 1]);
                    int val;
                    val_s >> val;
                    label_data[tokens[i - 1]] = val;
                    pc++;
                    continue;
                }
            }
        }
        if (tokens[c].back() == ':')
        {
            if (tokens[c] == "label:")
            {
                log_file << "label declaration 'label:' not allowed. Choose a different name"
                         << " (line " << i + 1 << ")\n";
                errs = 1;
            }
            tokens[c].pop_back();
            if (label.find(tokens[c]) != label.end())
            {
                log_file << "duplicate label found: " << tokens[c] << " (line " << i + 1 << ")\n";
                errs = 1;
            }
            label[tokens[c]] = pc + 1;
        }
        pc++;
    }
}
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
int main(int argc, char *argv[])
{
    myfile.open(argv[1]);
    char out_name[100];
    strcpy(out_name, argv[1]);
    int sz = strlen(out_name);
    out_name[sz - 2] = '\0';
    out_name[sz - 3] = 'o';

    o_file.open(out_name);
    out_name[sz - 3] = 'l';
    listing.open(out_name);
    readfile();
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
    get_line();
    set_labels();
    int pc = 1;
    vector<string> obj;

    for (int i = 0; i < lines.size(); i++)
    {
        vector<string> tokens = get_tokens(lines[i]);
        if (tokens.back().size() == 0)
            tokens.pop_back();

        int c = 0;

        for (auto u : tokens)
        {
            cout << u << "|";
        }
        cout << "\n";
        string s;
        if (tokens[c].back() == ':')
        {
            c++;
        }
        int cnt = 0;
        string s1;
        while (c < tokens.size())
        {
            while (tokens[c].back() == ' ')
                tokens[c].pop_back();
            if (cnt == 1)
            {
                int val = 0;
                if (tokens[c].back() >= 'a')
                {
                    if (label_data.find(tokens[c] + ":") != label_data.end())
                    {
                        cout << tokens[c] << "\n";
                        val = label_data[tokens[c] + ":"];
                        cout << val << "\n";
                    }
                    else
                    {
                        if (label.find(tokens[c]) == label.end())
                        {
                            log_file << "invalid label: " << tokens[c] << " (line " << i + 1 << ")\n";
                            errs = 1;
                        }
                        val = label[tokens[c]];
                    }
                }
                else
                {
                    stringstream val_s(tokens[c]);
                    val_s >> val;
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

        if (s.size() == 0)
        {
            s = string(24, '0');
        }
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
        pc1 = bin_hex(temp);
        int sz = pc1.size();
        pc1 = string(8 - sz, '0') + pc1;
        string lis = pc1 + " " + bin_hex(s) + " " + lines[i];
        listing << lis << "\n";
        o_file << s << "\n";
        cout << s << "\n";
        pc++;
    }
    if (errs)
    {
        o_file.clear();
        listing.clear();
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