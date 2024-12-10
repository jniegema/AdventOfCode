#include "FileIO.h"
#include "ContainerHelpers.h"
#include "StringToVector.h"

#include <iostream>
#include <algorithm>
#include <ranges>
#include <numeric>
#include <cassert>
#include <cctype>
#include <regex>
#include <algorithm>    // std::find

using namespace std;


using lint = long long int;


lint concat(lint a, lint b) {
    
    int n = 10;
    while ( (b / n) > 0) { n*=10; }

    return a * n + b;
}

std::vector<lint> possibleCombinations2(vector<lint> const & v) {
    std::vector<lint> results;
    std::vector<lint> results2;
    
    results.push_back(v[0]);
    for (auto curV = next(v.begin()); curV != v.end(); curV++) {
        auto const vv = *curV;
        for (const auto r : results) {
            results2.push_back(r + vv);
            results2.push_back(r * vv);
            results2.push_back(concat(r,vv));
        }
        swap(results, results2);
        results2.clear();
    }
    return results;
}


std::vector<lint> possibleCombinations(vector<lint> const& v) {
    std::vector<lint> results;

    results.push_back(v[0]);
    for (auto curV = next(v.begin()); curV != v.end(); curV++) {
        auto const vv = *curV;
        for (auto& r : results) {
            r += vv;
        }
        auto const sz = results.size();
        for (lint i = 0; i < sz; ++i) {
            auto r = results[i];
            results.push_back((r - vv) * vv);
        }
    }
    return results;
}


int main7()
{
    const auto rawFile = loadFile("Data/aoc_input7.txt");

    lint sum1 = 0;
    lint sum2 = 0;
    for (auto line : rawFile) {
        auto const curV = stringToVector(line, ':');
        auto const tv = std::stoll(curV[0]);
        auto const cv = stringToVectorOfInt(curV[1], ' ');

        auto const res = possibleCombinations(cv);
        if (find(res.begin(), res.end(), tv) != res.end()) {
            sum1 += tv;
        }

        auto const res2 = possibleCombinations2(cv);
        if (find(res2.begin(), res2.end(), tv) != res2.end()) {
            sum2 += tv;
        }
    }


    const auto result1 = sum1;
    cout << "Total sum of scores for Part I : " << result1 << endl;
//    assert(result1 == 56397);


    // ---- Part 2 ---
    const auto result2 = sum2;
    cout << "Total sum of scores for Part II: " << result2 << endl;
    //assert(result2 == 55701);


    return EXIT_SUCCESS; 
}
