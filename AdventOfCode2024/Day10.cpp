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

using namespace std;


using lint = long long int;
using pint = pair<int, int>;
using mapt = vector<vector<int>>;

vector<pint> start;
mapt map;


int findPaths(mapt const & map, pint start ) {
    stack<tuple<int,int,int>> s;
    set<pint> goals;
    const auto rows = map.size();
    const auto cols = map[0].size();
    int numHikes = 0;
    s.push(make_tuple(start.first, start.second, 0));
    while (!s.empty()) {
        auto [r, c, v] = s.top();
        s.pop();
        if( (v == 9) && !goals.contains(make_pair(r,c)) ) {
            numHikes++;
            goals.emplace(r, c);
            continue;
        }
        if ((r > 0) && (map[r-1][c] == (v + 1))) s.push(make_tuple(r- 1, c, v + 1));
        if ((c > 0) && (map[r][c-1] == (v + 1))) s.push(make_tuple(r, c-1, v + 1));
        if ((r < (rows-1)) && (map[r + 1][c] == (v + 1))) s.push(make_tuple(r + 1, c, v + 1));
        if ((c < (cols-1)) && (map[r][c+1] == (v + 1))) s.push(make_tuple(r, c + 1, v + 1));
    }

    return goals.size();
}

int findRating(mapt const& map, pint start) {
    stack<tuple<int, int, int>> s;
//    set<pint> goals;
    const auto rows = map.size();
    const auto cols = map[0].size();
    int numHikes = 0;
    s.push(make_tuple(start.first, start.second, 0));
    while (!s.empty()) {
        auto [r, c, v] = s.top();
        s.pop();
        if ((v == 9) ) {
            numHikes++;
            continue;
        }
        if ((r > 0) && (map[r - 1][c] == (v + 1))) s.push(make_tuple(r - 1, c, v + 1));
        if ((c > 0) && (map[r][c - 1] == (v + 1))) s.push(make_tuple(r, c - 1, v + 1));
        if ((r < (rows - 1)) && (map[r + 1][c] == (v + 1))) s.push(make_tuple(r + 1, c, v + 1));
        if ((c < (cols - 1)) && (map[r][c + 1] == (v + 1))) s.push(make_tuple(r, c + 1, v + 1));
    }

    return numHikes;
}

int main10()
{
    const auto rawFile = loadFile("Data/aoc_input10.txt");

    auto const s = rawFile[0];

    lint rows = rawFile.size();
    lint cols = s.size();
    
    for (int r = 0; r < rows; r++) {
        map.emplace_back(cols, -1);
        for (int c = 0; c < cols; c++) {
            map[r][c] = (rawFile[r][c] - '0');
            if (map[r][c] == 0) start.emplace_back(r, c);
        }
    }
    
    lint totalHikes = 0;
    lint totalRating = 0;
    for (const auto st : start)
    {
        const auto numPaths = findPaths(map, st);
        totalHikes += numPaths;
        totalRating += findRating(map, st);
    }


    const auto result1 = totalHikes;
    cout << "Total sum of scores for Part I : " << result1 << endl;
//    assert(result1 == 56397);

    // ---- Part 2 ---
    const auto result2 = 0;
    cout << "Total sum of scores for Part II: " << result2 << endl;
    //assert(result2 == 55701);


    return EXIT_SUCCESS; 
}
