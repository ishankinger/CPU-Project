#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "Registers.hpp"
#include "File.hpp"
using namespace std;

class I_type2 : public Registers, public Trimming {
private:
    string opcode;
    map<string, string> func3;
    map<string, string> func7;
public:
    I_type2();
    string intToImm(int& num);
    int setValues(string& ins1, int& ptr, string& rs1Bin, int& imm, string& rdBin);
    string getBinary(string& rs1, int& immediate, string& rd, string& function);
};