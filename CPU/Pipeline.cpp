#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

using namespace std;

class _PC {
public:
    int pc;
    bool complete;
    bool halt;
    _PC() {
        pc = 0;
        complete = false;
        halt = false;
    }
};

class _IFID {
public:
    int DPC;
    int NPC;
    string IR;
    bool complete;
    _IFID() {
        DPC = 0, NPC = 0;
        IR = "";
        complete = false;
    }
};

class _IDEX {
public:
    int JPC;
    int DPC;
    int NPC;
    int rs1;
    int rs2;
    int SDValue;
    string rdl;
    string fun3;
    string fun7;
    string imm;
    string op;
    bool complete;
    map<string, int> CW;
    _IDEX() {
        JPC = 0, DPC = 0, NPC = 0;
        rs1 = 0, rs2 = 0, SDValue = 0;
        rdl = "", fun3 = "", fun7 = "", imm = "", op = "";
        complete = false;
        CW = {
            {"RegRead",0},
            {"RegWrite",0},
            {"MemRead",0},
            {"MemWrite",0},
            {"Mem2Reg",0},
            {"ALUSrc",0},
            {"ALUOp",0},
            {"Branch",0},
            {"Jump",0}
        };
    }
};

class _EXMO {
public:
    int ALUOUT;
    int SDValue;
    string rdl;
    bool complete;
    map<string, int> CW;
    _EXMO() {
        ALUOUT = 0, SDValue = 0;
        rdl = "";
        complete = false;
        CW = {
            {"RegRead",0},
            {"RegWrite",0},
            {"MemRead",0},
            {"MemWrite",0},
            {"Mem2Reg",0},
            {"ALUSrc",0},
            {"ALUOp",0},
            {"Branch",0},
            {"Jump",0}
        };
    }
};

class _MOWB {
public:
    int LDOUT;
    int ALUOUT;
    string rdl;
    bool complete;
    map<string, int> CW;
    _MOWB() {
        LDOUT = 0, ALUOUT = 0;
        rdl = "";
        complete = false;
        CW = {
            {"RegRead",0},
            {"RegWrite",0},
            {"MemRead",0},
            {"MemWrite",0},
            {"Mem2Reg",0},
            {"ALUSrc",0},
            {"ALUOp",0},
            {"Branch",0},
            {"Jump",0}
        };
    }
};

class Pipeline {
private:
    int instructions;
    vector<string> IM;
    map<string, int> GPR;
    map<int, int> DM;
    map<string, int> stall;
public:
    Pipeline(vector<string> binary) {
        instructions = 0;
        IM = binary;
        GPR = {
            {"00000",0},
            {"00001",0},
            {"00010",0},
            {"00011",0},
            {"00100",0},
            {"00101",0},
            {"00110",0},
            {"00111",0},
            {"01000",0},
            {"01001",0},
            {"01010",0},
            {"01011",0},
            {"01100",0},
            {"01101",0},
            {"01110",0},
            {"01111",0},
            {"10000",0},
            {"10001",0},
            {"10010",0},
            {"10011",0},
            {"10100",0},
            {"10101",0},
            {"10110",0},
            {"10111",0},
            {"11000",0},
            {"11001",0},
            {"11010",0},
            {"11011",0},
            {"11100",0},
            {"11101",0},
            {"11110",0},
            {"11111",0}
        };

        stall = {
            {"00000",0},
            {"00001",0},
            {"00010",0},
            {"00011",0},
            {"00100",0},
            {"00101",0},
            {"00110",0},
            {"00111",0},
            {"01000",0},
            {"01001",0},
            {"01010",0},
            {"01011",0},
            {"01100",0},
            {"01101",0},
            {"01110",0},
            {"01111",0},
            {"10000",0},
            {"10001",0},
            {"10010",0},
            {"10011",0},
            {"10100",0},
            {"10101",0},
            {"10110",0},
            {"10111",0},
            {"11000",0},
            {"11001",0},
            {"11010",0},
            {"11011",0},
            {"11100",0},
            {"11101",0},
            {"11110",0},
            {"11111",0}
        };

        for (int i = 0; i <= 20; i++) DM[i] = 0;
    }

    string extract(string str, int end, int start) {
        if (str == "") return str;
        int s = 31 - end;
        int e = 31 - start;
        string newStr = "";
        for (int i = s; i <= e; i++) newStr.push_back(str[i]);
        return newStr;
    }

    string extractB(string str, int end1, int start1, int end2, int start2) {
        if (str == "") return str;
        string str1 = "";
        int s1 = 31 - end1, e1 = 31 - start1;
        for (int i = s1; i <= e1; i++) str1.push_back(str[i]);
        string str2 = "";
        int s2 = 31 - end2, e2 = 31 - start2;
        for (int i = s2; i <= e2; i++) str2.push_back(str[i]);

        string real = "";
        real += str1[0];
        real += str2[str2.size() - 1];
        for (int i = 1; i < str1.size(); i++) real.push_back(str1[i]);
        for (int i = 0; i < str2.size() - 1; i++) real.push_back(str2[i]);
        for (int i = 0; i < real.size() - 1; i++) real[i] = real[i + 1];
        real[real.size() - 1] = '0';

        return real;
    }

    string extractS(string str, int end1, int start1, int end2, int start2) {
        if (str == "") return str;
        string str1 = "";
        int s1 = 31 - end1, e1 = 31 - start1;
        for (int i = s1; i <= e1; i++) str1.push_back(str[i]);
        string str2 = "";
        int s2 = 31 - end2, e2 = 31 - start2;
        for (int i = s2; i <= e2; i++) str2.push_back(str[i]);

        string real = "";
        for (int i = 0; i < str1.size(); i++) real.push_back(str1[i]);
        for (int i = 0; i < str2.size(); i++) real.push_back(str2[i]);
        return real;
    }

    int immediate(string str) {
        if (str == "") return 0;
        int inc = 1;
        int num = 0;
        for (int i = str.size() - 1; i >= 1; i--) {
            num += (str[i] - '0') * inc;
            inc *= 2;
        }
        if (str[0] == '1') num -= inc;
        return num;
    }

    int signExtended(string str) {
        if (str == "") return 0;
        string num = "";
        num.push_back(str[0]);
        for (int i = 12; i <= 19; i++) num.push_back(str[i]);
        num.push_back(str[11]);
        for (int i = 1; i <= 10; i++) num.push_back(str[i]);
        for (int i = 0; i < num.size() - 1; i++) num[i] = num[i + 1];
        num[num.size() - 1] = '0';
        return immediate(num);
    }

    map<string, int> controller(string str) {
        map<string, int> cw = {
            {"RegRead",0},
            {"RegWrite",0},
            {"MemRead",0},
            {"MemWrite",0},
            {"Mem2Reg",0},
            {"ALUSrc",0},
            {"ALUOp",0},
            {"Branch",0},
            {"Jump",0}
        };

        if (str == "") return cw;

        if (str == "0110011") {
            cw["RegRead"] = 1;
            cw["RegWrite"] = 1;
        }
        else if (str == "0010011") {
            cw["RegRead"] = 1;
            cw["RegWrite"] = 1;
            cw["ALUSrc"] = 1;
        }
        else if (str == "0000011") {
            cw["RegRead"] = 1;
            cw["RegWrite"] = 1;
            cw["ALUSrc"] = 1;
            cw["MemRead"] = 1;
            cw["Mem2Reg"] = 1;
        }
        else if (str == "0100011") {
            cw["RegRead"] = 1;
            cw["ALUSrc"] = 1;
            cw["MemWrite"] = 1;
        }
        else if (str == "1100011") {
            cw["RegRead"] = 1;
            cw["Branch"] = 1;
        }
        else if (str == "1101111") {
            cw["RegRead"] = 1;
            cw["RegWrite"] = 1;
            cw["Jump"] = 1;
        }
        else if (str == "1100111") {
            cw["RegRead"] = 1;
            cw["RegWrite"] = 1;
            cw["ALUSrc"] = 1;
            cw["Jump"] = 1;
        }

        return cw;
    }

    int ALU(string op, string fun3, string fun7, int rs1, int rs2, int pc, bool& ALUFlag) {
        int result = 0;
        if (op == "") return result;

        if (op == "0110011") {
            if (fun7 == "0000000") {
                if (fun3 == "000") result = rs1 + rs2;
                else if (fun3 == "111") result = rs1 & rs2;
                else if (fun3 == "110") result = rs1 | rs2;
                else if (fun3 == "100") result = rs1 ^ rs2;
                else if (fun3 == "001") result = rs1 << rs2;
                else if (fun3 == "101") result = rs1 >> rs2;
                else if (fun3 == "010") result = rs1 > rs2;
            }
            else if (fun7 == "0100000") {
                if (fun3 == "000") result = rs1 - rs2;
            }
            else if (fun7 == "0000001") {
                if (fun3 == "000") result = rs1 * rs2;
                else if (fun3 == "100") result = rs1 / rs2;
                else if (fun3 == "110") result = rs1 % rs2;
            }
        }
        else if (op == "0010011") {
            if (fun3 == "000") result = rs1 + rs2;
            else if (fun3 == "100") return rs1 ^ rs2;
            else if (fun3 == "110") return rs1 | rs2;
            else if (fun3 == "111") return rs1 & rs2;
            else if (fun3 == "010") return rs1 > rs2;
        }
        else if (op == "0000011") {
            result = rs1 + rs2;                                // -----------------------------------------
        }
        else if (op == "0100011") {
            result = rs1 + rs2;                                 // -----------------------------------------
        }
        else if (op == "1100011") {
            if (fun3 == "000") ALUFlag = (rs1 == rs2);
            else if (fun3 == "001") ALUFlag = (rs1 != rs2);
            else if (fun3 == "100") ALUFlag = (rs1 < rs2);
            else if (fun3 == "101") ALUFlag = (rs1 >= rs2);
        }
        else if (op == "1101111") {
            return pc;
        }
        else if (op == "1100111") {
            return pc;
        }

        return result;
    }

    bool hold(string rs1, string rs2, string op) {
        if (op == "0110011") {
            return stall[rs2] or stall[rs1];
        }
        else if (op == "0010011") {
            return stall[rs1];
        }
        else if (op == "0000011") {
            return stall[rs1];
        }
        else if (op == "0100011") {
            return stall[rs2] or stall[rs1];
        }
        else if (op == "1100011") {
            return stall[rs2] or stall[rs1];
        }
        else if (op == "1100111") {
            return stall[rs1];
        }
        else return false;
    }

    void writeReg(string rdl, string op) {
        if (op == "0110011") {
            stall[rdl]++;
        }
        else if (op == "0010011") {
            stall[rdl]++;
        }
        else if (op == "0000011") {
            stall[rdl]++;
        }
        else if (op == "1100111") {
            stall[rdl]++;
        }
    }

    void flush1(_IFID* IFID) {
        IFID->DPC = 0, IFID->NPC = 0;
        IFID->IR = "";
        IFID->complete = false;
    }

    void flush2(_IDEX* IDEX) {
        IDEX->JPC = 0, IDEX->DPC = 0, IDEX->NPC = 0;
        IDEX->rs1 = 0, IDEX->rs2 = 0, IDEX->SDValue = 0;
        IDEX->rdl = "", IDEX->fun3 = "", IDEX->fun7 = "", IDEX->imm = "", IDEX->op = "";
        IDEX->complete = false;
        IDEX->CW = {
            {"RegRead",0},
            {"RegWrite",0},
            {"MemRead",0},
            {"MemWrite",0},
            {"Mem2Reg",0},
            {"ALUSrc",0},
            {"ALUOp",0},
            {"Branch",0},
            {"Jump",0}
        };
    }

    void instFetch(_PC* PC, _IFID* IFID) {
        if (PC->complete) return;
        if (PC->halt) {
            flush1(IFID);
            return;
        }
        if ((PC->pc) / 4 >= IM.size()) {                  // -----------------------------
            PC->complete = true;
            return;
        }
        IFID->IR = IM[(PC->pc) / 4];
        IFID->DPC = PC->pc;
        IFID->NPC = PC->pc + 4;
        string op = extract(IFID->IR, 6, 0);
        if (op == "1100011" or op == "1100111" or op == "1101111") PC->halt = true;
        else PC->pc = PC->pc + 4;                       // -----------------------
        if ((PC->pc) / 4 >= IM.size()) PC->complete = true; // ----------------------
    }

    int instDecode(_IFID* IFID, _IDEX* IDEX, _PC* PC) {
        if (IFID->complete) return 0;
        if (hold(extract(IFID->IR, 19, 15), extract(IFID->IR, 24, 20), extract(IFID->IR, 6, 0))) {
            flush2(IDEX);
            return 1;
        }
        if (IFID->IR != "") instructions++;
        writeReg(extract(IFID->IR, 11, 7), extract(IFID->IR, 6, 0));
        IDEX->JPC = IFID->DPC + signExtended(extract(IFID->IR, 31, 12));      // --------------------------------------
        IDEX->DPC = IFID->DPC;
        IDEX->NPC = IFID->NPC;
        IDEX->imm = extractB(IFID->IR, 31, 25, 11, 7);
        IDEX->fun3 = extract(IFID->IR, 14, 12);
        IDEX->fun7 = extract(IFID->IR, 31, 25);
        IDEX->rdl = extract(IFID->IR, 11, 7);
        IDEX->SDValue = (IFID->IR != "") ? GPR[extract(IFID->IR, 24, 20)] : 0;
        IDEX->op = extract(IFID->IR, 6, 0);
        IDEX->CW = controller(extract(IFID->IR, 6, 0));
        if (IDEX->CW["RegRead"]) IDEX->rs1 = GPR[extract(IFID->IR, 19, 15)];
        if (IDEX->CW["ALUSrc"]) {
            if (IDEX->CW["RegRead"]) {
                if (IDEX->op == "0100011") IDEX->rs2 = immediate(extractS(IFID->IR, 31, 25, 11, 7));
                else IDEX->rs2 = immediate(extract(IFID->IR, 31, 20));
            }
        }
        else {
            if (IDEX->CW["RegRead"]) IDEX->rs2 = GPR[extract(IFID->IR, 24, 20)];
        }
        if (PC->complete) IFID->complete = true;
        return 0;
    }

    void instExecution(_IDEX* IDEX, _EXMO* EXMO, _PC* PC, _IFID* IFID) {
        if (IDEX->complete) return;
        bool ALUFlag = true;
        EXMO->ALUOUT = ALU(IDEX->op, IDEX->fun3, IDEX->fun7, IDEX->rs1, IDEX->rs2, IDEX->NPC, ALUFlag);
        EXMO->CW = IDEX->CW;
        EXMO->SDValue = IDEX->SDValue;
        EXMO->rdl = IDEX->rdl;
        if (IDEX->op == "1100011" or IDEX->op == "1100111" or IDEX->op == "1101111") {
            PC->halt = false;
            if (IDEX->CW["Branch"] and ALUFlag) PC->pc = IDEX->DPC + immediate(IDEX->imm);
            else if (IDEX->CW["Jump"]) {                                                        // -----------------------------
                if (IDEX->op == "1100111") PC->pc = IDEX->rs1 + IDEX->rs2;                    // --------------------------------
                else PC->pc = IDEX->JPC;
            }
            else PC->pc = IDEX->DPC + 4;                                                      // -------------------------------
        }
        if (IFID->complete) IDEX->complete = true;
    }

    void memoryAccess(_EXMO* EXMO, _MOWB* MOWB, _IDEX* IDEX) {
        if (EXMO->complete) return;
        if (EXMO->CW["MemWrite"]) DM[(EXMO->ALUOUT) / 4] = EXMO->SDValue;
        if (EXMO->CW["MemRead"]) MOWB->LDOUT = DM[(EXMO->ALUOUT) / 4];
        MOWB->ALUOUT = EXMO->ALUOUT;
        MOWB->CW = EXMO->CW;
        MOWB->rdl = EXMO->rdl;
        if (IDEX->complete) EXMO->complete = true;
    }

    int registerWrite(_MOWB* MOWB, _EXMO* EXMO) {
        if (MOWB->complete) return 1;
        if (MOWB->CW["RegWrite"]) {
            if (stall[MOWB->rdl]) stall[MOWB->rdl]--;
            if (MOWB->CW["Mem2Reg"]) GPR[MOWB->rdl] = MOWB->LDOUT;
            else GPR[MOWB->rdl] = MOWB->ALUOUT;
        }
        if (EXMO->complete) MOWB->complete = true;
        return 0;
    }

    void printValues() {

        cout << "Number of Instructions -> " << instructions << "\n";

        cout << "-------------------- Register Values --------------------\n";
        for (auto gpr : GPR) {
            cout << gpr.first << "     " << gpr.second << "\n";
        }
        cout << "\n\n\n";

        cout << "----------------------- Memory ---------------------------\n";
        for (auto dm : DM) {
            cout << dm.first << "      " << dm.second << "\n";
        }
    }
};