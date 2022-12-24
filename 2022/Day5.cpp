#include "StringToVector.h"

#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <string_view>
#include <array>
#include <string>
#include <numeric>
#include <cassert>

void printStack(std::vector<std::vector<char>> const & stack) {

    for (int i = 0; i < stack.size(); ++i) {
        std::cout << i << ": ";
        for (int j = 0; j < stack[i].size(); ++j) {
            std::cout << stack[i][j] << ", ";
        }
        std::cout << std::endl;
    }
}


void main5()
{
    std::ifstream file1("aoc_input_5a.txt");
    std::vector<std::string> rawStacks(std::istream_iterator<std::string>(file1), {});
    std::vector<std::vector<char>> stacks;
    for (auto const rawStack : rawStacks) {
        stacks.push_back(stringToVectorOfChar(rawStack,','));
    }


    std::ifstream file2("aoc_input_5b.txt");
    std::vector<std::string> procedure(std::istream_iterator<std::string>(file2), {});
    std::vector<std::vector<int>> listOfProcedures;
    for (auto const proc : procedure) {
        listOfProcedures.push_back(stringToVectorOfInt(proc, ','));
    }

    std::vector<std::vector<char>> stacks1 = stacks;
    for (auto const & curProc : listOfProcedures) {
        const int num = curProc[0];
        const int from = curProc[1]-1;
        const int to = curProc[2]-1;

        for (int i = 0; i < num; ++i) {
            const auto item = stacks1[from].back();
            stacks1[from].pop_back();
            stacks1[to].push_back(item);
        }
    }

    std::cout << "Result for Part I : ";
    std::for_each(stacks1.cbegin(), stacks1.cend(), [](auto const& c) {std::cout << c.back(); });
    std::cout << std::endl;


    std::vector<std::vector<char>> stacks2 = stacks;
    //printStack(stacks2);
    for (auto const& curProc : listOfProcedures) {
        const int num = curProc[0];
        const int from = curProc[1] - 1;
        const int to = curProc[2] - 1;

 //       std::cout << "Procedure: " << num << " from " << from << " to " << to << std::endl;

        stacks2[to].insert(stacks2[to].end(), stacks2[from].end()-num, stacks2[from].end());
        stacks2[from].erase(stacks2[from].end() - num, stacks2[from].end());
 //       printStack(stacks2);
 //       std::cout << "---" << std::endl;
    }

    std::cout << "Result for Part II : ";
    std::for_each(stacks2.cbegin(), stacks2.cend(), [](auto const& c) {std::cout << c.back(); });
    std::cout << std::endl;
}