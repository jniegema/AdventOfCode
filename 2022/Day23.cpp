#include "StringToVector.h"
#include "FileIO.h"

#include <iostream>
#include <iterator>
#include <algorithm>
#include <ranges>
#include <string_view>
#include <array>
#include <numeric>
#include <cassert>
#include <map>
#include <set>
#include <cstdio>
#include <string>


using namespace std;
using IntT = ptrdiff_t;

using MapT = vector<pair<IntT, IntT>>;
using SetT = set<pair<IntT, IntT>>;
using MMapT = multimap<pair<IntT, IntT>, IntT>;



pair<int, int> iterate(MapT m, SetT s, int maxRounds) {
    auto isFree = [&](int x, int y) {return !(s.contains(make_pair(x, y))); };

    MMapT proposedMoves;

    int round = 0;
    while (round < maxRounds) {


        for (int i = 0; i < ssize(m); ++i) {
            auto const curPos = m[i];
            auto x = curPos.first;
            auto y = curPos.second;

            // If all around are free
            auto const N = isFree(x, y - 1);
            auto const NE = isFree(x + 1, y - 1);
            auto const NW = isFree(x - 1, y - 1);
            auto const E = isFree(x + 1, y);
            auto const W = isFree(x - 1, y);
            auto const S = isFree(x, y + 1);
            auto const SE = isFree(x + 1, y + 1);
            auto const SW = isFree(x - 1, y + 1);

            if (N && NE && NW && E && W && S && SE && SW) {
                proposedMoves.insert(pair(pair(x, y), i));
                continue;
            }

            if (round % 4 == 0) {
                if (N && NE && NW) { proposedMoves.insert(pair(pair(x, y - 1), i)); continue; }
                if (S && SE && SW) { proposedMoves.insert(pair(pair(x, y + 1), i)); continue; }
                if (W && NW && SW) { proposedMoves.insert(pair(pair(x - 1, y), i)); continue; }
                if (E && NE && SE) { proposedMoves.insert(pair(pair(x + 1, y), i)); continue; }
            }
            if (round % 4 == 1) {
                if (S && SE && SW) { proposedMoves.insert(pair(pair(x, y + 1), i)); continue; }
                if (W && NW && SW) { proposedMoves.insert(pair(pair(x - 1, y), i)); continue; }
                if (E && NE && SE) { proposedMoves.insert(pair(pair(x + 1, y), i)); continue; }
                if (N && NE && NW) { proposedMoves.insert(pair(pair(x, y - 1), i)); continue; }
            }
            if (round % 4 == 2) {
                if (W && NW && SW) { proposedMoves.insert(pair(pair(x - 1, y), i)); continue; }
                if (E && NE && SE) { proposedMoves.insert(pair(pair(x + 1, y), i)); continue; }
                if (N && NE && NW) { proposedMoves.insert(pair(pair(x, y - 1), i)); continue; }
                if (S && SE && SW) { proposedMoves.insert(pair(pair(x, y + 1), i)); continue; }
            }
            if (round % 4 == 3) {
                if (E && NE && SE) { proposedMoves.insert(pair(pair(x + 1, y), i)); continue; }
                if (N && NE && NW) { proposedMoves.insert(pair(pair(x, y - 1), i)); continue; }
                if (S && SE && SW) { proposedMoves.insert(pair(pair(x, y + 1), i)); continue; }
                if (W && NW && SW) { proposedMoves.insert(pair(pair(x - 1, y), i)); continue; }
            }
        }

        // Execute moves
        int movesExecuted = 0;
        for (auto const& curMove : proposedMoves) {
            auto const id = curMove.second;
            auto const& target = curMove.first;
            if (proposedMoves.count(target) > 1) continue; // Ignore duplicates
            if (m[id] != target) {
                movesExecuted++;
                s.erase(m[id]); s.insert(target);
            }
            m[id] = target;

        }

        proposedMoves.clear();
       // std::cout << round << " " << movesExecuted << std::endl;
        round++;
        if (movesExecuted == 0) break;
    }

    // Find min/max x,y
    IntT minX = 100000, minY = 100000, maxX = -100000, maxY = -100000;

    for (IntT i = 0; i < ssize(m); ++i) {
        minX = min(minX, m[i].first);
        maxX = max(maxX, m[i].first);
        minY = min(minY, m[i].second);
        maxY = max(maxY, m[i].second);
    }
    const auto area = (maxX - minX + 1) * (maxY - minY + 1);
    const auto freeArea = area - ssize(m);
    return pair(freeArea, round);
}

int main23()
{
    auto rawFile = loadFile("aoc_input_23.txt");

    MapT m;
    SetT s;
    for (int i = 0; i < rawFile.size(); ++i) {
        for (int j = 0; j < rawFile[i].size(); ++j) {
            if (rawFile[i][j] == '#') {
                m.emplace_back(j, i);
                s.insert(make_pair(j, i));
            }
        }
    }

    const auto [result1, totalRounds1] = iterate(m, s, 10);
    assert(result1 == 4208);
    std::cout << "Result for Part I : " << result1 << std::endl;

    const auto [freeArea2, result2] = iterate(m, s, 100000);
    assert(result2 == 1016);
    std::cout << "Result for Part II : " << result2 << std::endl;

    return EXIT_SUCCESS;
}