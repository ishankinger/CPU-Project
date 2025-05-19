#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
using namespace std;

// ADD write to memory operations also
// change the write back strategy to write throough when we are writting and missing in the cache


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

    void update(int blockAddress) {
        Block[blockAddress] = { 1,1,1,1 };
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
    int W;                                  // word Size
    int B;                                  // block Size
    int C;                                  // cache Size
    int N;                                  // number of blocks in cache
    int M;                                  // number of words in block
    int WS;                                 // number of ways (number of blocks in each set)
    int S;                                  // number of sets
    vector< vector< vector<int> > > Block;  // Data Stored
    vector< vector<int> > Tag;              // Tag array 
    vector< vector<int> > State;            // State array -> 0 = Inv, 1 = Val, 2 = MissPend 

public:
    Cache() {}

    Cache(int w, int b, int c, int ws) {
        W = w;
        B = b;
        C = c;
        N = c / b;
        M = b / w;
        WS = ws;
        S = N / ws;
        Block.resize(S, vector<vector<int>>(WS, vector<int>(M)));
        Tag.resize(S, vector<int>(WS, 0));
        State.resize(S, vector<int>(WS, 0));
    }

    int request(int reqInd, int tag, int read) {
        vector< vector<int> > set = Block[reqInd];
        vector<int> setTag = Tag[reqInd];
        vector<int> setState = State[reqInd];
        for (int i = 0; i < WS; i++) {
            if (setTag[i] == tag) {
                if (setState[i] == 1) {
                    cout << "HIT\n";
                    if (read == 0) {
                        cout << "UPDATE IN CACHE\n";
                        set[i] = { 1,1,1,1 };
                        Block[reqInd] = set;
                        printData();
                    }
                    return 1;
                }
                else if (setState[i] == 2) {
                    cout << "WAIT\n";
                    return -1;
                }
            }
        }
        cout << "MISS\n";
        int randInd = rand() % WS;
        cout << "Random Index -> " << randInd << "\n";
        setState[randInd] = 2;
        setTag[randInd] = tag;
        State[reqInd] = setState;
        Tag[reqInd] = setTag;
        Block[reqInd] = set;
        return 0;
    }

    void update(int tag, int blockAddress, vector<int> block) {
        int reqInd = blockAddress % S;
        vector< vector<int> > set = Block[reqInd];
        vector<int> setTag = Tag[reqInd];
        vector<int> setState = State[reqInd];
        for (int i = 0; i < WS; i++) {
            if (tag == setTag[i] and setState[i] == 2) {
                setState[i] = 1;
                set[i] = block;
            }
        }
        Block[reqInd] = set;
        State[reqInd] = setState;
        Tag[reqInd] = setTag;
    }

    void printData() {
        for (int i = 0; i < S; i++) {
            for (int j = 0; j < WS; j++) cout << State[i][j] << "  ";
            cout << "  ";
            for (int j = 0; j < WS; j++) cout << Tag[i][j] << "  ";
            cout << "  ";
            for (int j = 0; j < WS; j++) {
                for (int k = 0; k < M; k++) cout << Block[i][j][k] << " ";
                cout << " ";
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
    int WS;
    int S;
    vector<pair<int, int>> request;

public:
    int hit;
    int miss;
    CPU(int w, int b, int c, int s, int ws, vector<pair<int, int>> r) {
        Mem = Memory(w, b, s);
        Cach = Cache(w, b, c, ws);
        ind = 0;
        N = c / b;
        B = b;
        WS = ws;
        S = N / ws;
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
        int req = request[ind].first;
        int read = request[ind].second;
        ind++;
        string r = read == 0 ? "write" : "read";
        cout << "REQUEST = " << req << " " << r << " ------------------------------------------------------\n\n";

        // 2. DECODE
        int blockAddress = req / B;
        int reqInd = blockAddress % S;
        int tag = blockAddress / S;

        // 3. EXECUTION
        int cachReq = Cach.request(reqInd, tag, read);
        if (cachReq == 1) {
            hit++;
            if (read == 0) {
                cout << "UPDATE IN MEMORY\n";
                Mem.update(blockAddress);
                Mem.printData();
            }
            else Cach.printData();
        }
        else if (cachReq == 0) {
            miss++;
            Cach.printData();
            if (read == 0) {
                cout << "UPDATE IN MEMORY\n";
                Mem.update(blockAddress);
                Mem.printData();
            }
            pair<pair<int, int>, vector<int>> memResp = Mem.request(tag, blockAddress);
            Cach.update(memResp.first.first, memResp.first.second, memResp.second);
            cout << "FILL IN CACHE FROM MEMORY\n";
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

    int w = 4, b = 16, c = 64, s = 256, ws = 2;

    int r = 6;

    vector<pair<int, int>> requests = { {36,1},{37,1},{38,0},{128,1},{130,0},{192,0},{193,1} };

    CPU cpu = CPU(w, b, c, s, ws, requests);
    while (cpu.processing()) {
        cout << "\n \n";
    }

    cout << cpu.hit << " " << cpu.miss << "\n";
    cout << "Execution Completed\n";

    return 0;
}