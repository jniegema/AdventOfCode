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
    const auto rawFile = loadFile("Data/aoc_input1.txt");


    std::vector<long long int> v1, v2;
    for (auto line : rawFile) {
        auto const curV = stringToVectorOfInt(line, ' ');
        v1.push_back(curV[0]);
        v2.push_back(curV[1]);
    }

    sr::sort(v1);
    sr::sort(v2);

    auto sum3 = std::transform_reduce(v1.cbegin(), v1.cend(), v2.cbegin(), 0L, std::plus{}, [](auto x, auto y) { return std::abs(x-y); });

    const auto result1 = sum3;
    cout << "Total sum of scores for Part I : " << result1 << endl;
//    assert(result1 == 56397);

    // Determine how many integers match a target value.
    long long int sum2 = 0;
    for (const auto target : v1)
    {
        const int num_items = std::count(v2.cbegin(), v2.cend(), target);
        sum2 += num_items * target;
    }

    // ---- Part 2 ---
    //const auto result2 = getSumPartII(rawFile);
    cout << "Total sum of scores for Part II: " << sum2 << endl;
    //assert(result2 == 55701);


    return EXIT_SUCCESS; 
}
