#include "R_type.hpp"

R_type::R_type() {
    opcode = "0110011";
    func7 = {
        {"ADD", "0000000"},
        {"SUB", "0100000"},
        {"MUL", "0000001"},
        {"DIV", "0000001"},
        {"REM", "0000001"},
        {"AND", "0000000"},
        {"OR",  "0000000"},
        {"XOR", "0000000"},
        {"SLL", "0000000"},
        {"SRL", "0000000"},
        {"SRA", "0100000"},
        {"SLT", "0000000"},
        {"SLTU","0000000"}
    };
    func3 = {
        {"ADD", "000"},
        {"SUB", "000"},
        {"MUL", "000"},
        {"DIV", "100"},
        {"REM", "110"},
        {"AND", "111"},
        {"OR",  "110"},
        {"XOR", "100"},
        {"SLL", "001"},
        {"SRL", "101"},
        {"SRA", "101"},
        {"SLT", "010"},
        {"SLTU","011"}
    };
}

int R_type::setValues(string& ins, int& ptr, string& rs1Bin, string& rs2Bin, string& rdBin) {
    removeSpace(ins, ptr);
    string rd = extractString(ins, ptr);
    removeSpace(ins, ptr);
    string rs1 = extractString(ins, ptr);
    removeSpace(ins, ptr);
    string rs2 = extractString(ins, ptr);
    if (extraCode(ins, ptr)) {
        return 1;
    }
    if (getRegister(rs1) == "NULL" or getRegister(rs2) == "NULL" or getRegister(rd) == "NULL") {
        cout << "Wrong Register Used\n";
        return 1;
    }
    rdBin = getRegister(rd);
    rs1Bin = getRegister(rs1);
    rs2Bin = getRegister(rs2);
    return 0;
}

string R_type::getBinary(string& rs1, string& rs2, string& rd, string& function) {
    string encoded = func7[function] + rs2 + rs1 + func3[function] + rd + opcode;
    return encoded;
}