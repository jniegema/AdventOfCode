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

bool isSafe(std::vector<long long int> const& v) {
    const bool allPos = std::all_of(std::next(v.begin()), v.end(), [](auto x) {return x > 0; });
    const bool allNeg = std::all_of(std::next(v.begin()), v.end(), [](auto x) {return x < 0; });
    const bool bounded = std::all_of(std::next(v.begin()), v.end(), [](auto x) {return abs(x) > 0 && abs(x) < 4; });
    return ((allPos || allNeg) && bounded);
}

int main()
{
    const auto rawFile = loadFile("Data/aoc_input2.txt");


    std::vector<long long int> v1, v2;
    int isSafe1 = 0;
    int isSafe2 = 0;
    std::vector<long long int> v3;
    for (auto line : rawFile) {
        auto const curV = stringToVectorOfInt(line, ' ');

        bool isIncreasing = true;
        bool isDecreasing = true;
        
        v3.clear();
        std::adjacent_difference(curV.begin(), curV.end(), std::back_inserter(v3));

        long long int numLevels = curV.size()-1;
        const long long int numPos = numLevels - std::count_if(std::next(v3.begin()), v3.end(), [](auto x) {return x > 0; });
        const long long int numNeg = numLevels - std::count_if(std::next(v3.begin()), v3.end(), [](auto x) {return x < 0; });
        const long long int numBounded = numLevels - std::count_if(std::next(v3.begin()), v3.end(), [](auto x) {return abs(x) > 0 && abs(x) < 4; });

        if (isSafe(v3)) {
            isSafe1++;
            isSafe2++;
        } else {
            if (numPos == 1) {
                // Find which one and check if dropping it helps:
                auto itr = std::find_if(std::next(v3.begin()), v3.end(), [](auto x) {return x <= 0; });
                auto pos = std::distance(v3.begin(), itr)-1;
                
                v1 = curV;
                v1.erase(v1.begin()+pos);
                v3.clear();
                std::adjacent_difference(v1.begin(), v1.end(), std::back_inserter(v3));
                if (isSafe(v3)) { isSafe2++; continue; }

                v1 = curV;
                v1.erase(v1.begin() + pos+1);
                v3.clear();
                std::adjacent_difference(v1.begin(), v1.end(), std::back_inserter(v3));
                if (isSafe(v3)) { isSafe2++; continue; }
            }

            if (numNeg == 1) {
                // Find which one and check if dropping it helps:
                auto itr = std::find_if(std::next(v3.begin()), v3.end(), [](auto x) {return x >= 0; });
                auto pos = std::distance(v3.begin(), itr)-1;
                
                v1 = curV;
                v1.erase(v1.begin() + pos);
                v3.clear();
                std::adjacent_difference(v1.begin(), v1.end(), std::back_inserter(v3));
                if (isSafe(v3))  { isSafe2++; continue;
                }

                v1 = curV;
                v1.erase(v1.begin() + pos+1);
                v3.clear();
                std::adjacent_difference(v1.begin(), v1.end(), std::back_inserter(v3));
                if (isSafe(v3)) {
                    isSafe2++; continue;
                }
            }

            if (numBounded == 1) {
                // Find which one and check if dropping it helps:
                auto itr = std::find_if(std::next(v3.begin()), v3.end(), [](auto x) {return abs(x) <= 0 || abs(x) > 3; });
                auto pos = std::distance(v3.begin(), itr)-1;
                
                // Two options, we could drop the first or the second one
                v1 = curV;
                v1.erase(v1.begin() + pos);
                v3.clear();
                std::adjacent_difference(v1.begin(), v1.end(), std::back_inserter(v3));
                if (isSafe(v3))  {isSafe2++; continue;
                }

                v1 = curV;
                v1.erase(v1.begin() + pos+1);
                v3.clear();
                std::adjacent_difference(v1.begin(), v1.end(), std::back_inserter(v3));
                if (isSafe(v3)) {
                    isSafe2++;
                    continue;
                }

            }
        }

    }

    const auto result1 = isSafe1;
    cout << "Total sum of scores for Part I : " << result1 << endl;
//    assert(result1 == 56397);


    // ---- Part 2 ---
    //const auto result2 = getSumPartII(rawFile);
    cout << "Total sum of scores for Part II: " << isSafe2 << endl;
    //assert(result2 == 55701);


    return EXIT_SUCCESS; 
}
