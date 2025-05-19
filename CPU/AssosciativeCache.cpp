#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <generator>
#include <chrono>
using namespace std;

class Memory {

private:
    int W;
    int B;
    int S;
    int N;
    int M;
    vector< vector<int> > Block;

public:
    Memory() {}

    Memory(int w, int b, int s) {
        this->W = w;
        this->B = b;
        this->S = s;
        this->N = s / b;
        this->M = b / w;
        Block.resize(N, vector<int>(M));
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                Block[i][j] = rand() % 10;
            }
        }
    }

    pair<int, vector<int>> request(int tag, int blockAddress) {
        return { tag,Block[blockAddress] };
    }

    void printData() {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                cout << Block[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }

};

class Cache {

private:
    int W;                          // word Size
    int B;                          // block Size
    int C;                          // cache Size
    int N;                          // number of blocks in cache
    int M;                          // number of words in block
    vector< vector<int> > Block;    // Data Stored
    vector<int> Tag;                // Tag array 
    vector<int> State;              // State array -> 0 = Inv, 1 = Val, 2 = MissPend 

public:
    Cache() {}

    Cache(int w, int b, int c) {
        this->W = w;
        this->B = b;
        this->C = c;
        this->N = c / b;
        this->M = b / w;
        Block.resize(N, vector<int>(M));
        Tag.resize(N, 0);
        State.resize(N, 0);
    }

    int request(int tag) {
        for (int i = 0; i < N; i++) {
            if (Tag[i] == tag) {
                if (State[i] == 1) {
                    // hit
                    return 1;
                }
                else if (State[i] == 2) {
                    // wait
                    return -1;
                }
            }
        }
        int reqInd = -1;
        for (int i = 0; i < N; i++) {
            if (State[i] == 0) {
                reqInd = i;
                break;
            }
        }
        if (reqInd == -1) reqInd = 0;
        // miss
        State[reqInd] = 2;
        Tag[reqInd] = tag;
        return 0;
    }

    void update(int tag, vector<int> block) {
        for (int i = 0; i < N; i++) {
            if (tag == Tag[i] and State[i] == 2) {
                State[i] = 1;
                Block[i] = block;
            }
        }
    }

    void printData() {
        for (int i = 0; i < N; i++) cout << State[i] << " ";
        cout << "   ";
        for (int i = 0; i < N; i++) cout << Tag[i] << " ";
        cout << "   ";
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                cout << Block[i][j] << " ";
            }
            cout << "  ";
        }
        cout << "\n\n";
    }

};

class CPU {

private:
    Memory Mem;
    Cache Cach;
    int ind;
    int N;
    int S;
    int B;
    vector<int> request;

public:
    int hit;
    int miss;
    CPU(int w, int b, int c, int s, vector<int> r) {
        Mem = Memory(w, b, s);
        Cach = Cache(w, b, c);
        ind = 0;
        N = c / b;
        S = s;
        B = b;
        request = r;
        hit = 0;
        miss = 0;
        Cach.printData();
        Mem.printData();
        cout << "Start Exectuion\n\n";
    }

    int getTag(int req) {
        int totBits = log2(S);
        int passBits = log2(B) + log2(N);
        int tagBits = totBits - passBits;
        while (passBits > 0) {
            req >>= 1;
            passBits--;
        }
        int tag = 0;
        int mul = 1;
        while (tagBits > 0) {
            tag += (1 & req) * mul;
            mul <<= 1;
            tagBits--;
        }
        return tag;
    }

    bool processing() {

        if (ind == request.size()) return false;

        // 1. FETCH
        int req = request[ind];
        ind++;
        cout << "REQUEST = " << req << " ----------------------------------------------------------\n\n";

        // 2. DECODE
        int blockAddress = req / B;
        int tag = blockAddress;

        // 3. EXECUTION
        int cachReq = Cach.request(tag);
        if (cachReq == 1) {
            cout << "HIT\n";
            hit++;
            Cach.printData();
        }
        else if (cachReq == 0) {
            cout << "MISS\n";
            miss++;
            Cach.printData();
            pair<int, vector<int>> memResp = Mem.request(tag, blockAddress);
            Cach.update(memResp.first, memResp.second);
            cout << "FILL\n";
            Cach.printData();
        }
        else {
            cout << "WAIT\n";
        }
        cout << "-----------------------------------------------------------------------\n";
        return true;
    }
};

int main() {

    int w = 4, b = 16, c = 64, s = 256;

    int r = 3;

    vector<int> requests = { 36,37,38,128,192,193 };

    CPU cpu = CPU(w, b, c, s, requests);
    while (cpu.processing()) {
        cout << "\n \n";
    }
    cout << cpu.hit << " " << cpu.miss << "\n";
    cout << "Execution Completed\n";


    return 0;
}