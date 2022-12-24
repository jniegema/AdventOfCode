
#include "StringToVector.h"
#include "FileIO.h"

#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <algorithm> // forcopy
#include <ranges>
#include <string_view>

#include <string>

#include <cassert>

using namespace std;

using IntervalT = pair<int, int>;

IntervalT parseInterval(string const& s) {
    const auto s1 = s.substr(0, s.find("-"));
    const auto s2 = s.substr(s.find("-") + 1);
    return make_pair(stoi(s1),stoi(s2));
}

bool contains(IntervalT i1, IntervalT i2) {
    return ((((i1.first) <= (i2.first)) && ((i1.second) >= (i2.second))) ||
            (((i1.first) >= (i2.first)) && ((i1.second) <= (i2.second))));
}

bool overlap(IntervalT i1, IntervalT i2) {
    return ((i1.second >= i2.first) && (i1.first <= i2.second));
}

int main4()
{
    const auto listOfItems = loadFile("aoc_input_4.txt");

    vector<pair<IntervalT, IntervalT>> listIntPairs;
    transform(begin(listOfItems), end(listOfItems), back_inserter(listIntPairs), [](auto const& s) {const auto s1 = s.substr(0, s.find(","));
                                                                                                    const auto s2 = s.substr(s.find(",")+1);
                                                                                                    return make_pair(parseInterval(s1), parseInterval(s2)); });

    const auto result1 = ranges::count_if(listIntPairs, [](auto const pp) {return contains(pp.first,pp.second); });
    cout << "Total sum of scores for Part I : " << result1 <<endl;
    assert(result1 == 569);

    const auto result2 = ranges::count_if(listIntPairs, [](auto const pp) {return overlap(pp.first, pp.second); });

    cout << "Total sum of scores for Part II: " << result2  <<endl;
    assert(result2 == 936);

    return EXIT_SUCCESS;
}