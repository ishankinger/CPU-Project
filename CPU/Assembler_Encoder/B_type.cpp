#include "B_type.hpp"

B_type::B_type() {
    opcode = "1100011";
    func3 = {
        {"BEQ","000"},
        {"BNE","001"},
        {"BLT","100"},
        {"BGE","101"},
        {"BLTU","110"},
        {"BGEU","111"}
    };
}

string B_type::intToImm(int& num) {
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

int B_type::setValues(string& ins, int& ptr, string& rs1Bin, int& imm, string& rs2Bin) {
    removeSpace(ins, ptr);
    string rs1 = extractString(ins, ptr);
    removeSpace(ins, ptr);
    string rs2 = extractString(ins, ptr);
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
    if (getRegister(rs1) == "NULL" or getRegister(rs2) == "NULL") {
        cout << "Wrong Register Used\n";
        return 1;
    }
    if (imm < -2048 or imm > 2047) {
        cout << "Immediate out of range\n";
        return 1;
    }
    rs2Bin = getRegister(rs2);
    rs1Bin = getRegister(rs1);
    return 0;
}

string B_type::getBinary(string& rs1, string& rs2, int& immediate, string& function) {
    string encoded = "";
    if (immediate >= 0) encoded += '0';
    else encoded += '1';
    string imm = intToImm(immediate);
    for (int i = 1; i < 7; i++) encoded.push_back(imm[i]);
    encoded += rs2 + rs1 + func3[function];
    for (int i = 7; i < 11; i++) encoded.push_back(imm[i]);
    encoded.push_back(imm[11]);
    encoded += opcode;
    return encoded;
}