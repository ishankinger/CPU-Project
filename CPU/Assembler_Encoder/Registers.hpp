#pragma once

#include <iostream>
#include <string>
#include <map>
using namespace std;

class Registers {
private:
    map<string, string> regBin;
public:
    Registers();
    string getRegister(string& reg);
};