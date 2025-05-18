#include "J_type.hpp"

J_type::J_type() {
    opcode = "1101111";
}

string J_type::intToImm(int& num) {
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
    int len = 21 - binary.size();
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

int J_type::setValues(string& ins, int& ptr, string& rdBin, int& imm) {
    removeSpace(ins, ptr);
    string rd = extractString(ins, ptr);
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
    if (getRegister(rd) == "NULL") {
        cout << "Wrong Register Used\n";
        return 1;
    }
    if (imm < -1048576 or imm > 1048574) {
        cout << "Immediate out of range\n";
        return 1;
    }
    rdBin = getRegister(rd);
    return 0;
}


string J_type::getBinary(string& rd, int& immediate) {
    string encoded = "";
    string imm = intToImm(immediate);
    encoded.push_back(imm[0]);
    for (int i = 10; i <= 19; i++) encoded.push_back(imm[i]);
    encoded.push_back(imm[9]);
    for (int i = 1; i <= 8; i++) encoded.push_back(imm[i]);
    encoded += rd + opcode;
    return encoded;
}