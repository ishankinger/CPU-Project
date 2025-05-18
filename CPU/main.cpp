#pragma once

#include "Pipeline.cpp"
#include "Instructions.hpp"

int main() {
    File fl;
    vector<string> instruction = fl.readFile();
    vector<string> binary;
    Instruction ins;
    for (int i = 0; i < instruction.size(); i++) {
        if (ins.setIns(instruction[i])) return -1;
        string bin = ins.getBin();
        cout << bin << "\n";
        if (bin == "NULL") return -1;
        else binary.push_back(bin);
    }
    cout << "\nCode Assembled\n";

    _PC* PC = new _PC();
    _IFID* IFID = new _IFID();
    _IDEX* IDEX = new _IDEX();
    _EXMO* EXMO = new _EXMO();
    _MOWB* MOWB = new _MOWB();

    int cycles = 0;

    Pipeline P(binary);
    while (true) {
        cycles++;
        if (P.registerWrite(MOWB, EXMO)) break;
        P.memoryAccess(EXMO, MOWB, IDEX);
        P.instExecution(IDEX, EXMO, PC, IFID);
        if (P.instDecode(IFID, IDEX, PC)) continue;
        P.instFetch(PC, IFID);
    }

    cout << "Number of cycles -> " << cycles << "\n";
    P.printValues();

    return 0;
}