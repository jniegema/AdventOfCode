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
#include <vector>
#include <stack>
#include <set>
#include <map>

using namespace std;


using lint = long long int;
using pint = pair<int, int>;
using mapt = vector<vector<int>>;

int main8()
{
    map<char, std::vector<pint>> antennas;

    const auto rawFile = loadFile("Data/aoc_input8.txt");

    auto const s = rawFile[0];

    lint rows = rawFile.size();
    lint cols = s.size();

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            auto ch = rawFile[r][c];

            if (ch != '.') {
                antennas[ch].emplace_back(r, c);
            }

        }
    }

    // Compute anti-nodes
    set<pint> antinodes;
    set<pint> antinodes2;

    for (auto const [f, clist] : antennas) {

        auto const N = clist.size();
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < i; ++j) {
                auto const dx = clist[i].first - clist[j].first;
                auto const dy = clist[i].second - clist[j].second;
                auto const dd = gcd(dx, dy);

                for (int k = -100; k < 100; ++k) {
                    auto const x1 = clist[i].first + k*dx/dd;
                    auto const y1 = clist[i].second + k * dy / dd;
                    if ((x1 >= 0) && (y1 >= 0) && (x1 < cols) && (y1 < rows)) {
                        antinodes2.emplace(x1, y1);
                    }

                }

                auto const x1 = clist[i].first + dx;
                auto const y1 = clist[i].second + dy;
                if ((x1 >= 0) && (y1 >= 0) && (x1 < cols) && (y1 < rows)) {
                        antinodes.emplace(x1, y1);
                }



                auto const x2 = clist[j].first - dx;
                auto const y2 = clist[j].second - dy;
                if ((x2 >= 0) && (y2 >= 0) && (x2 < cols) && (y2 < rows)) {
                    antinodes.emplace(x2, y2);
                }
            }
        }


    }



    const auto result1 = antinodes.size();
    cout << "Total sum of scores for Part I : " << result1 << endl;
    //    assert(result1 == 56397);

        // ---- Part 2 ---
    const auto result2 = antinodes2.size();
    cout << "Total sum of scores for Part II: " << result2 << endl;
    //assert(result2 == 55701);


    return EXIT_SUCCESS;
}
