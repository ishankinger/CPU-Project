#include "Instructions.hpp"

int Instruction::setIns(string ins) {
    this->ins = ins;
    ptr = 0;
    removeSpace(ins, ptr);
    function = extractString(ins, ptr);
    for (int i = 0; i < function.size(); i++) {
        function[i] = toupper(function[i]);
    }
    if (getType(function) == "NULL") {
        cout << "Wrong Function Used\n";
        return 1;
    }
    type = getType(function);
    return 0;
}

string Instruction::getBin() {
    if (type == "R_type") {
        R_type rtype;
        if (rtype.setValues(ins, ptr, rs1, rs2, rd)) return "NULL";
        return rtype.getBinary(rs1, rs2, rd, function);
    }
    else if (type == "I_type") {
        I_type itype;
        if (itype.setValues(ins, ptr, rs1, imm, rd)) return "NULL";
        return itype.getBinary(rs1, imm, rd, function);
    }
    else if (type == "I_type2") {
        I_type2 itype2;
        if (itype2.setValues(ins, ptr, rs1, imm, rd)) return "NULL";
        return itype2.getBinary(rs1, imm, rd, function);
    }
    else if (type == "I_type3") {
        I_type3 itype3;
        if (itype3.setValues(ins, ptr, rs1, imm, rd)) return "NULL";
        return itype3.getBinary(rs1, imm, rd, function);
    }
    else if (type == "L_type") {
        L_type ltype;
        if (ltype.setValues(ins, ptr, rs1, imm, rd)) return "NULL";
        return ltype.getBinary(rs1, imm, rd, function);
    }
    else if (type == "S_type") {
        S_type stype;
        if (stype.setValues(ins, ptr, rs1, imm, rs2)) return "NULL";
        return stype.getBinary(rs1, rs2, imm, function);
    }
    else if (type == "B_type") {
        B_type btype;
        if (btype.setValues(ins, ptr, rs1, imm, rs2)) return "NULL";
        return btype.getBinary(rs1, rs2, imm, function);
    }
    else if (type == "J_type") {
        J_type jtype;
        if (jtype.setValues(ins, ptr, rd, imm)) return "NULL";
        return jtype.getBinary(rd, imm);
    }
    else if (type == "U_type") {
        U_type utype;
        if (utype.setValues(ins, ptr, rd, imm)) return "NULL";
        return utype.getBinary(rd, imm);
    }

    return "NULL";
}
