#include "I_type.hpp"

I_type::I_type() {
    opcode = "0010011";
    func3 = {
        {"ADDI","000"},
        {"XORI","100"},
        {"ORI","110"},
        {"ANDI","111"},
        {"SLTI","010"}
    };
}

string I_type::intToImm(int& num) {
    string binary;
    bool neg = false;
    if (num < 0) {
        neg = true;
        num *= -1;
    }
    while (num > 0) {
        binary = (num % 2 == 0 ? "0" : "1") + binary;
        num /= 2;
    }
    int len = 12 - binary.size();
    string pref = "";
    for (int i = 0; i < len; i++) pref += "0";
    string numBin = binary.empty() ? pref : pref + binary;
    if (neg) {
        for (int i = 0; i < numBin.size(); i++) numBin[i] ^= 1;
        int ptr = numBin.size() - 1;
        while (ptr > 0 and numBin[ptr] == '1') {
            numBin[ptr] = '0';
            ptr--;
        }
        numBin[ptr] = '1';
    }
    return numBin;
}

int I_type::setValues(string& ins, int& ptr, string& rs1Bin, int& imm, string& rdBin) {
    removeSpace(ins, ptr);
    string rd = extractString(ins, ptr);
    removeSpace(ins, ptr);
    string rs1 = extractString(ins, ptr);
    removeSpace(ins, ptr);
    try {
        imm = stoi(extractString(ins, ptr));
    }
    catch (invalid_argument) {
        cout << "Error in immediate\n";
        return 1;
    }
    catch (out_of_range) {
        cout << "Immediate out of range\n";
        return 1;
    }
    if (extraCode(ins, ptr)) {
        return 1;
    }
    if (getRegister(rs1) == "NULL" or getRegister(rd) == "NULL") {
        cout << "Wrong Register Used\n";
        return 1;
    }
    if (imm < -2048 or imm > 2047) {
        cout << "Immediate out of range\n";
        return 1;
    }
    rdBin = getRegister(rd);
    rs1Bin = getRegister(rs1);
    return 0;
}

string I_type::getBinary(string& rs1, int& immediate, string& rd, string& function) {
    string encoded = intToImm(immediate) + rs1 + func3[function] + rd + opcode;
    return encoded;
}