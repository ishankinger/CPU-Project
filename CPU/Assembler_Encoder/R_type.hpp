#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "Registers.hpp"
#include "File.hpp"
using namespace std;

class R_type : public Registers, public Trimming {
private:
    string opcode;
    map<string, string> func7;
    map<string, string> func3;
public:
    R_type();
    int setValues(string& ins, int& ptr, string& rs1Bin, string& rs2Bin, string& rdBin);
    string getBinary(string& rs1, string& rs2, string& rd, string& function);
};