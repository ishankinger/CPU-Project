#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <generator>
#include <chrono>
using namespace std;

class Cache {

private:
    long long W;                                  // word Size
    long long B;                                  // block Size
    long long C;                                  // cache Size
    long long N;                                  // number of blocks in cache
    long long M;                                  // number of words in block
    long long WS;                                 // number of ways (number of blocks in each set)
    long long S;                                  // number of sets
    vector< vector<long long> > Tag;              // Tag array 
    vector< vector<long long> > State;            // State array -> 0 = Inv, 1 = Val, 2 = MissPend 

public:
    Cache() {}

    Cache(long long w, long long b, long long c, long long ws) {
        W = w;
        B = b;
        C = c;
        N = c / b;
        M = b / w;
        WS = ws;
        S = N / ws;
        Tag.resize(S, vector<long long>(WS, 0));
        State.resize(S, vector<long long>(WS, 0));
    }

    long long request(long long reqInd, long long tag, long long read) {
        for (int i = 0; i < WS; i++) {
            if (Tag[reqInd][i] == tag) {
                if (State[reqInd][i] == 1) {
                    // cout << "HIT\n";
                    if (read == 0) {
                        // cout << "UPDATE IN CACHE\n";
                    }
                    return 1;
                }
                else if (State[reqInd][i] == 2) {
                    // cout << "WAIT\n";
                    return -1;
                }
            }
        }
        // cout << "MISS\n";
        long long randInd = rand() % WS;
        // cout << "Random Index -> " << randInd << "\n";
        State[reqInd][randInd] = 2;
        Tag[reqInd][randInd] = tag;
        return 0;
    }

    void update(long long tag, long long blockAddress) {
        long long reqInd = blockAddress % S;
        for (int i = 0; i < WS; i++) {
            if (tag == Tag[reqInd][i] and State[reqInd][i] == 2) {
                State[reqInd][i] = 1;
            }
        }
    }
};

class CPU {

private:
    Cache Cach;
    long long ind;
    long long N;
    long long B;
    long long WS;
    long long S;
    vector<pair<long long, long long>> request;

public:
    long long hit;
    long long miss;
    CPU(long long w, long long b, long long c, long long ws, vector<pair<long long, long long>> r) {
        Cach = Cache(w, b, c, ws);
        ind = 0;
        N = c / b;
        B = b;
        WS = ws;
        S = N / ws;
        request = r;
        hit = 0;
        miss = 0;
        cout << "Start Exectuion\n\n";
    }

    bool processing() {

        if (ind == request.size()) return false;

        // 1. FETCH
        long long req = request[ind].first;
        long long read = request[ind].second;
        ind++;
        string r = read == 0 ? "write" : "read";
        // cout << "REQUEST = " << req << " " << r  << " ------------------------------------------------------\n";

        // 2. DECODE
        long long blockAddress = req / B;
        long long reqInd = blockAddress % S;
        long long tag = blockAddress / S;

        // 3. EXECUTION
        long long cachReq = Cach.request(reqInd, tag, read);
        if (cachReq == 1) {
            hit++;
            if (read == 0) {
                // cout << "UPDATE IN MEMORY\n";
            }
        }
        else if (cachReq == 0) {
            miss++;
            if (read == 0) {
                // cout << "UPDATE IN MEMORY\n";
            }
            Cach.update(tag, blockAddress);
            // cout << "FILL IN THE CACHE MEMORY\n";
        }
        else {
            // cout << "WAIT\n";
        }
        // cout << "\n";
        return true;
    }
};

int main() {

    //long long w = 4, b = 64, c = 32768, ws = 8;

    //long long a = (1LL) << 40;
    //long long seed = chrono::system_clock::now().time_since_epoch().count();
    ////default_random_engine generator(seed);
    ////normal_distribution<double> distribution(a / 4, 16500);

    //vector<pair<long long, long long>> requests;
    //for (int i = 0; i < 1e7; i++) {
    //    requests.push_back({ distribution(generator),(rand() % 2) });
    //}

    //// auto start_time = std::chrono::high_resolution_clock::now();
    //// auto duration = std::chrono::seconds(100);



    //CPU cpu = CPU(w, b, c, ws, requests);
    //while (cpu.processing()) {
    //    // cout << "\n \n";
    //}
    //cout << cpu.hit << " " << cpu.miss << "\n";
    //cout << "Execution Completed\n";


    return 0;
}