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

using namespace std;


using lint = long long int;

vector<pair<lint, lint>> d;
vector<pair<lint, lint>> e;
vector<pair<lint, lint>> f;

int main9()
{
    const auto rawFile = loadFile("Data/aoc_input9.txt");

    auto const s = rawFile[0];

    lint cnt = 0;
    lint N = s.size();
    for (int i = 0; i < N; i+=2) {
        d.emplace_back(s[i] - '0', cnt);
        e.emplace_back(s[i+1] - '0', -1); //< Free space!
        f.emplace_back(s[i] - '0', cnt);
        f.emplace_back(s[i + 1] - '0', -1); //< Free space!

        cnt++;
    }
    if (N < (d.size() + e.size())) { e.pop_back(); f.pop_back(); }

    int insertPos = 1;
    int ePos = 0;
    //while ( (ePos < e.size()) && (insertPos<d.size())) {
    //    auto const id = d.back().second;
    //    auto const num = d.back().first;
    //    auto const numEmpty = e[ePos].first;

    //    if (numEmpty > num) {
    //        d.pop_back();
    //        d.insert(next(d.begin(), insertPos), make_pair(num,id));
    //        e[ePos].first -= num;
    //        insertPos++;
    //    } else {
    //        d.insert(next(d.begin(), insertPos), make_pair(numEmpty, id));
    //        d.back().first -= numEmpty;
    //        if (d.back().first == 0) { d.pop_back(); }
    //        ePos++;
    //        insertPos += 2;
    //    }
    //}


    // Try each file once:
    for (int fPos = (f.size() - 1); fPos > 1; fPos--) {
        auto const id = f[fPos].second;
        auto const num = f[fPos].first;
        if (id == -1) continue;
        // Try each empty position
        for (int fPos2 = 0; fPos2 < fPos; fPos2++) {
            auto const id2 = f[fPos2].second;
            auto const num2 = f[fPos2].first;

            if ((id2 == -1) && (num2 >= num)) {
                f[fPos].second = -1; //< Mark as empty
                assert(f[fPos - 1].second == -1);
                //f[fPos - 1].first += num;
                //f.erase(next(f.begin(), fPos));
                //if (fPos < f.size()) {
                //    f[fPos - 1].first += f[fPos].first;
                //    f.erase(next(f.begin(), fPos));
                //}

                f[fPos2].first -= num; //< Reduce the empty spots
                f.insert(next(f.begin(), fPos2), make_pair(num, id));
                break;
            }
        }
    }
    //while ((ePos < e.size()) && (insertPos < d.size())) {
    //    auto const id = d[dPos].second;
    //    auto const num = d[dPos].first;
    //    auto const numEmpty = e[ePos].first;

    //    if (numEmpty >= num) {
    //        d.erase(next(d.begin(), dPos));
    //        d.insert(next(d.begin(), insertPos), make_pair(num, id));
    //        e[ePos].first -= num;
    //        if (e[ePos].first == 0) {
    //            ePos++; insertPos++;
    //        }
    //        insertPos++;
    //        dPos = d.size() - 1;
    //    } else {
    //        dPos--;
    //    }
    //    
    //    //else {
    //    //    d.insert(next(d.begin(), insertPos), make_pair(numEmpty, id));
    //    //    d.back().first -= numEmpty;
    //    //    if (d.back().first == 0) { d.pop_back(); }
    //    //    ePos++;
    //    //    insertPos += 2;
    //    //}
    //}







    // Compute checksum:
    int a = 0;
    lint checksum = 0;
    for (auto const x : d) {
        const auto b = a + x.first - 1;
        if (x.second > 0) {
            const auto xx = (b >= a) ? (x.second * (b - a + 1) * (a + b) / 2) : 0;
            checksum += xx;
        }
        a = b+1;
    }



    const auto result1 = checksum;
    cout << "Total sum of scores for Part I : " << result1 << endl;
//    assert(result1 == 56397);


        // Compute checksum  2:
    int a2 = 0;
    lint checksum2 = 0;
    for (auto const x : f) {
        const auto b = a2 + x.first - 1;
        if (x.second > 0) {
            const auto xx = (b >= a2) ? (x.second * (b - a2 + 1) * (a2 + b) / 2) : 0;
            checksum2 += xx;
        }
        a2 = b + 1;
    }

    // ---- Part 2 ---
    const auto result2 = checksum2;
    cout << "Total sum of scores for Part II: " << result2 << endl;
    //assert(result2 == 55701);


    return EXIT_SUCCESS; 
}
