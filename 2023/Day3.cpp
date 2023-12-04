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
#include <map>

using namespace std;

using IntT = unsigned long long int;

bool isInside(IntT i, IntT j, std::vector<std::string> const& map) {
    if (i < 0) return false;
    if (i >= map.size()) return false;

    if (j < 0) return false;
    if (j >= map[i].size()) return false;

    return true;
}

bool isSymbol(IntT i, IntT j, std::vector<std::string> const & map) {
    return isInside(i,j,map) && !(std::isdigit(map[i][j]) || map[i][j] == '.');
}

bool isGear(IntT i, IntT j, std::vector<std::string> const& map) {
    return isInside(i, j, map) && (map[i][j] == '*');
}


std::map<std::pair<IntT, IntT>, std::vector<IntT>> gearToNumberMap;

int main3()
{
    const auto rawFile = loadFile("Data/aoc_input_3.txt");

    IntT sum = 0;
    IntT sumPowers = 0;

    
    for (IntT i = 0; i < rawFile.size(); ++i) { 
        bool lastWasNumber = false;
        const auto row = rawFile[i];
        bool hasSymbolNextToIt = false;
        IntT numberStart = 0;
        std::vector<pair<IntT,IntT>> gears;
        for (IntT j = 0; j < row.size(); j++) {
            
            if (std::isdigit(row[j])) {
                hasSymbolNextToIt |= isSymbol(i-1, j-1, rawFile);
                hasSymbolNextToIt |= isSymbol(i-1, j,   rawFile);
                hasSymbolNextToIt |= isSymbol(i-1, j+1, rawFile);
                hasSymbolNextToIt |= isSymbol(i, j - 1, rawFile);
                hasSymbolNextToIt |= isSymbol(i, j, rawFile);
                hasSymbolNextToIt |= isSymbol(i, j + 1, rawFile);
                hasSymbolNextToIt |= isSymbol(i + 1, j - 1, rawFile);
                hasSymbolNextToIt |= isSymbol(i + 1, j, rawFile);
                hasSymbolNextToIt |= isSymbol(i + 1, j + 1, rawFile);

                // Store all gears next to the current number
                if (isGear(i - 1, j - 1, rawFile)) { gears.emplace_back(i - 1, j - 1); }
                if (isGear(i - 1, j, rawFile)) { gears.emplace_back(i - 1, j ); }
                if (isGear(i - 1, j + 1, rawFile)) { gears.emplace_back(i - 1, j + 1); }
                if (isGear(i , j - 1, rawFile)) { gears.emplace_back(i, j - 1); }
                if (isGear(i , j, rawFile)) { gears.emplace_back(i, j ); }
                if (isGear(i , j + 1, rawFile)) { gears.emplace_back(i, j + 1); }
                if (isGear(i + 1, j - 1, rawFile)) { gears.emplace_back(i + 1, j - 1); }
                if (isGear(i + 1, j, rawFile)) { gears.emplace_back(i + 1, j ); }
                if (isGear(i + 1, j + 1, rawFile)) { gears.emplace_back(i + 1, j + 1); }

                if (!lastWasNumber) { numberStart = j; }
                lastWasNumber = true;
            } else {
                // Has a number just finished?
                if (lastWasNumber) {
                    const auto substr = row.substr(numberStart, j - numberStart);
                    if (hasSymbolNextToIt) sum += toInt(substr);
                    lastWasNumber = false;

                    // For each gear, store the number it has next to it
                    for (auto gearPair : gears) gearToNumberMap[gearPair].push_back(toInt(substr));
                    gears.clear();
                }
                numberStart = j;
                hasSymbolNextToIt = false;
            }

        }

        // Has a number just finished?
        if (lastWasNumber) {
            const auto substr = row.substr(numberStart, row.size() - numberStart);
            if (hasSymbolNextToIt) sum += toInt(substr);
            lastWasNumber = false;

            for (auto gearPair : gears) gearToNumberMap[gearPair].push_back(toInt(substr));
            gears.clear();
        }
        numberStart = 0;
        hasSymbolNextToIt = false;


    }
    
    const auto result1 = sum;
    cout << "Total sum of scores for Part I : " << result1 << endl;
    assert(result1 == 530849);

    IntT gearRatioSum = 0;
    for (auto& numberList : gearToNumberMap) {
        auto const it = std::unique(numberList.second.begin(), numberList.second.end());

        const int numNeighbors = std::distance(numberList.second.begin(), it);
        //std::cout << numberList.first.first << " " << numberList.first.second << " " << numNeighbors << ": ";
        //for(int n=0; n< numNeighbors; ++n) cout <<numberList.second[n] << ",";
        //cout << std::endl;
        if (numNeighbors == 2) {
            auto gearRatio = numberList.second[0] * numberList.second[1];
            gearRatioSum += gearRatio;
        }
    }

    // ---- Part 2 ---
    const auto result2 = gearRatioSum;
    cout << "Total sum of scores for Part II: " << result2 << endl;
    assert(result2 == 84900879);

    
    return EXIT_SUCCESS; 
}
