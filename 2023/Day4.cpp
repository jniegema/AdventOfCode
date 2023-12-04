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

int main()
{
    const auto rawFile = loadFile("Data/aoc_input_4.txt");

    IntT sum1 = 0;
    IntT sum2 = 0;

    std::vector<int> numberOfCopies(rawFile.size(), 1);
    for (IntT i = 0; i < rawFile.size(); ++i) { 
        auto row = removeBefore(rawFile[i], ": ");

        const auto pair = stringToVector(row, '|');
        auto ll1 = stringToVectorOfInt(pair[0], ' ');
        auto ll2 = stringToVectorOfInt(pair[1], ' ');

        sr::sort(ll1);
        sr::sort(ll2);

        vector<int> out{};
        sr::set_intersection(ll1, ll2, std::back_inserter(out));

        const IntT len = out.size();
        if( len>0) sum1 += pow(2, len - 1);

        for (int j = 0; j < len; ++j) {
            const auto newIdx = i + j + 1;
            if (newIdx < numberOfCopies.size()) numberOfCopies[newIdx] += numberOfCopies[i];
        }
    }
    
    const auto result1 = sum1;
    cout << "Total sum of scores for Part I : " << result1 << endl;
    assert(result1 == 24542);

    // ---- Part 2 ---
    const auto result2 = reduce(begin(numberOfCopies),end(numberOfCopies));
    cout << "Total sum of scores for Part II: " << result2 << endl;
    assert(result2 == 8736438);

    
    return EXIT_SUCCESS; 
}
