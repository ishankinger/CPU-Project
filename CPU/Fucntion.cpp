#include "Function.hpp"

Function::Function() {
    funOpcode = {
        {"ADD","R_type"},
        {"SUB","R_type"},
        {"MUL","R_type"},
        {"DIV","R_type"},
        {"REM","R_type"},
        {"AND","R_type"},
        {"OR","R_type"},
        {"XOR","R_type"},
        {"SLL","R_type"},
        {"SRL","R_type"},
        {"SRA","R_type"},
        {"SLT","R_type"},
        {"SLTU","R_type"},

        {"ADDI","I_type"},
        {"XORI","I_type"},
        {"ORI","I_type"},
        {"ANDI","I_type"},
        {"SLTI","I_type"},

        {"SLLI","I_type2"},
        {"SRLI","I_type2"},
        {"SRAI","I_type2"},

        {"JALR","I_type3"},

        {"LW","L_type"},
        {"LD","L_type"},
        {"LH","L_type"},
        {"LB","L_type"},
        {"LWU","L_type"},
        {"LHU","L_type"},
        {"LBU","L_type"},

        {"SW","S_type"},
        {"SB","S_type"},
        {"SH","S_type"},
        {"SD","S_type"},

        {"BEQ","B_type"},
        {"BNE","B_type"},
        {"BLT","B_type"},
        {"BGE","B_type"},
        {"BLTU","B_type"},
        {"BGEU","B_type"},

        {"JAL","J_type"},

        {"LUI","U_type"}
    };
}

string Function::getType(string& fun) {
    if (funOpcode.find(fun) != funOpcode.end()) {
        return funOpcode[fun];
    }
    return "NULL";
}