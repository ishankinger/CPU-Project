#include "File.hpp"

void Trimming::removeSpace(string& ins, int& ptr) {
    while (ptr < ins.size() and (ins[ptr] == ' ' or ins[ptr] == ',' or ins[ptr] == '(' or ins[ptr] == ')'))
        ptr++;
}

string Trimming::extractString(string& ins, int& ptr) {
    string s = "";
    while (ptr < ins.size() and (ins[ptr] != ' ' and ins[ptr] != ',' and ins[ptr] != '(' and ins[ptr] != ')'))
        s.push_back(ins[ptr++]);
    return s;
}

int Trimming::extraCode(string& ins, int& ptr) {
    while (ptr < ins.size() and ins[ptr] == ' ') ptr++;
    if (ptr < ins.size() and ins[ptr] == ')') {
        ptr++;
        while (ptr < ins.size() and ins[ptr] == ' ') ptr++;
    }
    if (ptr != ins.size()) {
        cout << "Error Extra Code\n";
        return 1;
    }
    return 0;
}

vector<string> File::readFile() {
    ifstream fin;
    fin.open("assemblyCode.s");
    vector<string> instructions;
    char line[100];
    while (fin) {
        fin.getline(line, 100);
        string s = line;
        for (int i = 0; i < s.size(); i++) {
            if (s[i] != ' ') {
                instructions.push_back(s);
                break;
            }
        }
    }
    fin.close();
    return instructions;
}

void File::writeFile(vector<string> binary) {
    ofstream fout;
    fout.open("binaryCode.txt");
    for (int i = 0; i < binary.size(); i++) {
        fout << binary[i] << "\n";
    }
    fout.close();
}