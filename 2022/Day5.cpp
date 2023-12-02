#include "StringToVector.h"
#include "FileIO.h"

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

using namespace std;
namespace sr = std::ranges;

void printStack(vector<vector<char>> const & stack) {

    for (int i = 0; i < stack.size(); ++i) {
        cout << i << ": ";
        for (int j = 0; j < stack[i].size(); ++j) {
            cout << stack[i][j] << ", ";
        }
        cout << endl;
    }
}


int main5()
{
    vector<string> rawStacks = loadFile("Data/aoc_input_5a.txt");
    vector<vector<char>> stacks;
    for (auto const rawStack : rawStacks) {
        stacks.push_back(stringToVectorOfChar(rawStack,','));
    }

    vector<string> procedure = loadFile("Data/aoc_input_5b.txt");
    vector<vector<int>> listOfProcedures;
    for (auto const proc : procedure) {
        listOfProcedures.push_back(stringToVectorOfInt(proc, ','));
    }

    vector<vector<char>> stacks1 = stacks;
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

    cout << "Result for Part I : ";
    sr::for_each(stacks1, [](auto const& c) {cout << c.back(); });
    cout << endl;


    vector<vector<char>> stacks2 = stacks;

    for (auto const& curProc : listOfProcedures) {
        const int num = curProc[0];
        const int from = curProc[1] - 1;
        const int to = curProc[2] - 1;

        stacks2[to].insert(stacks2[to].end(), stacks2[from].end()-num, stacks2[from].end());
        stacks2[from].erase(stacks2[from].end() - num, stacks2[from].end());
 //       printStack(stacks2);
 //       cout << "---" << endl;
    }

    cout << "Result for Part II : ";
    sr::for_each(stacks2, [](auto const& c) {cout << c.back(); });
    cout << endl;

    return EXIT_SUCCESS;
}