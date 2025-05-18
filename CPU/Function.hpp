#pragma once

#include <iostream>
#include <string>
#include <map>
using namespace std;

class Function {
private:
    map<string, string> funOpcode;
public:
    Function();
    string getType(string& fun);
};