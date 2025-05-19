#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include "File.hpp"
#include "Function.hpp"
#include "R_type.hpp"
#include "I_type.hpp"
#include "I_type2.hpp"
#include "I_type3.hpp"
#include "L_type.hpp"
#include "S_type.hpp"
#include "B_type.hpp"
#include "J_type.hpp"
#include "U_type.hpp"
using namespace std;

class Instruction : public Trimming, public Function {
private:
    int ptr;
    string ins;
    string type;
    string function;
    string rs1;
    string rs2;
    string rd;
    int imm;
public:
    int setIns(string);
    string getBin();
};