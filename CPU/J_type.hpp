#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "Registers.hpp"
#include "File.hpp"
using namespace std;

class J_type : public Registers, public Trimming {
private:
    string opcode;
public:
    J_type();
    string intToImm(int& num);
    int setValues(string& ins, int& ptr, string& rdBin, int& imm);
    string getBinary(string& rd, int& immediate);
};