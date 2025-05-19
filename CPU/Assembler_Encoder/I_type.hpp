#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "Registers.hpp"
#include "File.hpp"
using namespace std;

class I_type : public Registers, public Trimming {
private:
    string opcode;
    map<string, string> func3;
public:
    I_type();
    string intToImm(int& num);
    int setValues(string& ins, int& ptr, string& rs1Bin, int& imm, string& rdBin);
    string getBinary(string& rs1, int& immediate, string& rd, string& function);
};