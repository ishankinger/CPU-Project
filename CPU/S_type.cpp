#include "S_type.hpp"

S_type::S_type() {
    opcode = "0100011";
    func3 = {
        {"SW","010"},
        {"SB","000"},
        {"SH","001"},
        {"SD","011"}
    };
}

string S_type::intToImm(int& num) {
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

int S_type::setValues(string& ins, int& ptr, string& rs1Bin, int& imm, string& rs2Bin) {
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
    removeSpace(ins, ptr);
    string rs1 = extractString(ins, ptr);
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

string S_type::getBinary(string& rs1, string& rs2, int& immediate, string& function) {
    string imm = intToImm(immediate);
    string encoded = "";
    for (int i = 0; i < 7; i++) encoded.push_back(imm[i]);
    encoded += rs2 + rs1 + func3[function];
    for (int i = 7; i < 12; i++) encoded.push_back(imm[i]);
    encoded += opcode;
    return encoded;
}