
#include "StringToVector.h"
#include "FileIO.h"

#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <algorithm> // for std::copy
#include <ranges>
#include <string_view>

#include <string>
#include <numeric>

#include <cassert>

using namespace std;
namespace sr = std::ranges;

int evalCharList(vector<char> const& list) {
    return transform_reduce(list.cbegin(), list.cend(), 0, plus{}, [](char c) { int val = c - 65; return (val < 26) ? (val + 27) : (val - 31); });
}

int main3()
{
    const auto listOfItems = loadFile("Data/aoc_input_3.txt");

    // Find the common letter for each half by splitting the strings, sorting each of them and then calling set_intersection
    vector<char> commonLetter;
    sr::transform(listOfItems,back_inserter(commonLetter), [](auto const& s) { const auto N = s.length()/2;
                                                                               auto str1 = s.substr(0, N);
                                                                               sr::sort(str1);
                                                                               auto str2 = s.substr(N);
                                                                               sr::sort(str2);
                                                                               vector<char> result;
                                                                               sr::set_intersection(str1, str2, back_inserter(result));
                                                                               return result[0]; });
    const auto result1 = evalCharList(commonLetter);
    cout << "Total sum of scores for Part I : " << result1 << endl;
    assert(result1 == 8394);
    

    // ---- Part 2 ---
    vector<string> sortedListOfItems;
    sr::transform(listOfItems, back_inserter(sortedListOfItems), [](auto s) { sr::sort(s); return s; });

    vector<char> groupBadge;
    const auto N = sortedListOfItems.size();
    for(int i = 0; i < N; i+=3) {
        vector<char> result;
        sr::set_intersection(sortedListOfItems[i], sortedListOfItems[i+1], back_inserter(result));
        vector<char> result2;
        sr::set_intersection(result, sortedListOfItems[i + 2],back_inserter(result2));
        groupBadge.push_back(result2[0]);
    }

    const auto result2 = evalCharList(groupBadge);
    cout << "Total sum of scores for Part II: " << result2 << endl;
    assert(result2 == 2413);

    return EXIT_SUCCESS;
}