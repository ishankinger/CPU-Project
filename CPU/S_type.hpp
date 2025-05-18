#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "Registers.hpp"
#include "File.hpp"
using namespace std;

class S_type : public Registers, public Trimming {
private:
    string opcode;
    map<string, string> func3;
public:
    S_type();
    string intToImm(int& num);
    int setValues(string& ins, int& ptr, string& rs1Bin, int& imm, string& rs2Bin);
    string getBinary(string& rs1, string& rs2, int& immediate, string& function);
};