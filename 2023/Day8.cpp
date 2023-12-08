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

int main()
{
    const auto rawFile = loadFile("Data/aoc_input_8.txt");

    string rl = rawFile[0];

    map<string, pair<string,string> > nodeList;
    map<string, pair<string, IntT> > toNextZ;

    for (int i = 2; i < rawFile.size(); ++i ) {
        const auto split = stringToVector(rawFile[i], '=');
        const auto key = trimString(split[0], " ");

        const auto rlsplit = stringToVector(split[1], ',');
        const auto str1 = trimString(rlsplit[0], " ()");
        const auto str2 = trimString(rlsplit[1], " ()");
        nodeList.emplace(key, make_pair(str1, str2));
    }

    string curStr = "AAA";
    IntT cnt = 0;
    while (curStr != "ZZZ") {
        auto const curInstr = cnt % rl.size();
        if (rl[curInstr] == 'L')  curStr = nodeList[curStr].first;  else  curStr = nodeList[curStr].second;
        cnt++;
    }
    
    const auto result1 = cnt;
    cout << "Total sum of scores for Part I : " << result1 << endl;
    assert(result1 == 17141);

    // Part II

    // Look for cycles from nodes ending on 'A' to nodes ending in 'Z' ... and from nodes ending in 'Z' to nodes ending in 'Z'
    vector<string> curPos;
    for (auto node : nodeList) {
        if (node.first[2] == 'A') curPos.emplace_back(node.first); //< Store all starting nodes for the general solution

        if ((node.first[2] == 'A') || (node.first[2] == 'Z')) {
            string  curStr = node.first;
            cnt = 1;
            if (rl[cnt] == 'L')  curStr = nodeList[curStr].first;  else  curStr = nodeList[curStr].second;

            while (curStr[2] != 'Z') {
                auto const curInstr = cnt % rl.size();
                if (rl[curInstr] == 'L')  curStr = nodeList[curStr].first;  else  curStr = nodeList[curStr].second;
                cnt++;
            }
            toNextZ[node.first] = make_pair(curStr, cnt);
        }
    }

    // Just looking at the data we see that we have very basic cycles and the solution will just be
    // the lowest common multiple between the cycles from 'A' to 'Z'.
    IntT solution2 = 1;
    for (auto node : toNextZ) {
        if (node.first[2] == 'A') solution2 = lcm(solution2, node.second.second);
    }

    // Not sure how long it would take, but if we didn't have those simple cycles we could still try to work on the reduced map

    const auto result2 = solution2;
    cout << "Total sum of scores for Part II: " << result2 << endl;
    assert(result2 == 10818234074807);
    
     return EXIT_SUCCESS; 
}

