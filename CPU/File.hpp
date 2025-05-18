#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

class Trimming {
public:
    void removeSpace(string& ins, int& ptr);
    string extractString(string& ins, int& ptr);
    int extraCode(string& ins, int& ptr);
};

class File {
public:
    vector<string> readFile();
    void writeFile(vector<string> binary);
};