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
#include <numeric>
using namespace std;

using IntT = long long int;

bool any_not_zero(vector<IntT> x) {
    return any_of(begin(x),end(x), [](auto y) {return y != 0; });
}

int main9()
{
    const auto rawFile = loadFile("Data/aoc_input_9.txt");

    string rl = rawFile[0];

    IntT sum1 = 0;
    IntT sum2 = 0;
    for (auto row : rawFile) {
        auto new_row = stringToVectorOfInt(row,' ');
        vector<IntT> lastNum;
        vector<IntT> firstNum;
        while (any_not_zero(new_row)) {
            std::vector<IntT> row2;
            lastNum.push_back(new_row.back());
            firstNum.push_back(new_row.front());
            adjacent_difference(begin(new_row), end(new_row), back_inserter(row2));
            new_row.clear();
            new_row.insert(new_row.begin(),next(begin(row2)), end(row2));
        }
        sum1 += reduce(begin(lastNum), end(lastNum));
        IntT v = 0;
        for (IntT j = firstNum.size()-1; j >= 0; j--) {
            v = firstNum[j] - v;
        }
        sum2 += v;
    }

    const auto result1 = sum1;
    cout << "Total sum of scores for Part I : " << result1 << endl;
    //assert(result1 == 17141);

    // Part II


    const auto result2 = sum2;
    cout << "Total sum of scores for Part II: " << result2 << endl;
    //assert(result2 == 10818234074807);
    
     return EXIT_SUCCESS; 
}

