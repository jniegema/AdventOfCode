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

int getSumPartI(const std::vector<std::string>& rawFile) {

    std::vector<int> numbers;
    for (const auto row : rawFile) {

        std::string curDigits;
        for (int i = 0; i < row.size(); ++i) {
            std::string row2 = row.substr(i);

            if (std::isdigit(row2[0])) { curDigits.push_back(row2[0]); }
        }

        auto const two_digit_num = toInt(std::string(1, curDigits.front()) + curDigits.back());
        numbers.push_back(two_digit_num);
    }

    return std::reduce(numbers.begin(), numbers.end());
}

int getSumPartII(const std::vector<std::string>& rawFile) {

    std::vector<int> numbers;
    for (const auto row : rawFile) {

        std::string curDigits;
        for (int i = 0; i < row.size(); ++i) {
            std::string row2 = row.substr(i);

            if (std::isdigit(row2[0])) { curDigits.push_back(row2[0]); }
            if (row2.starts_with("one"))   { curDigits.push_back('1');}
            if (row2.starts_with("two"))   { curDigits.push_back('2');}
            if (row2.starts_with("three")) { curDigits.push_back('3');}
            if (row2.starts_with("four"))  { curDigits.push_back('4');}
            if (row2.starts_with("five"))  { curDigits.push_back('5');}
            if (row2.starts_with("six"))   { curDigits.push_back('6');}
            if (row2.starts_with("seven")) { curDigits.push_back('7');}
            if (row2.starts_with("eight")) { curDigits.push_back('8');}
            if (row2.starts_with("nine"))  { curDigits.push_back('9');}
        }

        auto const two_digit_num = toInt(std::string(1, curDigits.front()) + curDigits.back());
        numbers.push_back(two_digit_num);
    }

    return std::reduce(numbers.begin(), numbers.end());
}

int main1()
{
    const auto rawFile = loadFile("Data/aoc_input_1.txt");
        
    const auto result1 = getSumPartI(rawFile);
    cout << "Total sum of scores for Part I : " << result1 << endl;
    assert(result1 == 56397);


    // ---- Part 2 ---
    const auto result2 = getSumPartII(rawFile);
    cout << "Total sum of scores for Part II: " << result2 << endl;
    assert(result2 == 55701);


    return EXIT_SUCCESS; 
}
