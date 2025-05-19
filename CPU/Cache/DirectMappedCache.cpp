#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
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

    pair<pair<int, int>, vector<int>> request(int tag, int blockAddress) {
        return { {tag,blockAddress},Block[blockAddress] };
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

    int request(int reqInd, int tag) {
        if (Tag[reqInd] == tag) {
            if (State[reqInd] == 1) {
                // hit
                return 1;
            }
            else if (State[reqInd] == 2) {
                // wait
                return -1;
            }
        }
        // miss
        State[reqInd] = 2;
        Tag[reqInd] = tag;
        return 0;
    }

    void update(int tag, int blockAddress, vector<int> block) {
        int reqInd = blockAddress % N;
        if (tag == Tag[reqInd] and State[reqInd] == 2) {
            State[reqInd] = 1;
            Block[reqInd] = block;
        }
    }

    void printData() {
        for (int i = 0; i < N; i++) {
            cout << State[i] << "  " << Tag[i] << "  ";
            for (int j = 0; j < M; j++) {
                cout << Block[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }

};

class CPU {

private:
    Memory Mem;
    Cache Cach;
    int ind;
    int N;
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
        B = b;
        request = r;
        hit = 0;
        miss = 0;
        Cach.printData();
        Mem.printData();
        cout << "Start Exectuion\n\n";
    }

    bool processing() {

        if (ind == request.size()) return false;

        // 1. FETCH
        int req = request[ind];
        ind++;
        cout << "REQUEST = " << req << " ----------------------------------------------------------\n\n";

        // 2. DECODE
        int blockAddress = req / B;
        int reqInd = blockAddress % N;
        int tag = blockAddress / N;

        // 3. EXECUTION
        int cachReq = Cach.request(reqInd, tag);
        if (cachReq == 1) {
            cout << "HIT\n";
            hit++;
            Cach.printData();
        }
        else if (cachReq == 0) {
            cout << "MISS\n";
            miss++;
            Cach.printData();
            pair<pair<int, int>, vector<int>> memResp = Mem.request(tag, blockAddress);
            Cach.update(memResp.first.first, memResp.first.second, memResp.second);
            cout << "FILL\n";
            Cach.printData();
        }
        else {
            cout << "WAIT\n";
        }
        cout << "------------------------------------------------------------------------\n";
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