#include "FileIO.h"
#include "ContainerHelpers.h"

#include <iostream>
#include <algorithm>
#include <ranges>
#include <numeric>
#include <cassert>

using namespace std;

int toInt(string const& s) { return stoi(s); }

int main1()
{
    const auto rawFile = loadFile("Data/aoc_input_1.txt");

    ranges::split_view split{ rawFile, "" }; //< Split on empty strings
    printSplitView(split);

    auto lst = split | views::transform([](auto const& c) { const auto p = c | views::transform(toInt);  return accumulate(begin(p), end(p), 0); });

    const auto result1 = ranges::max(lst);
    assert(result1 == 70374);
    cout << "Result for Part I: " << result1 << endl;


    auto lst2 = vector(begin(lst), end(lst));
    ranges::partial_sort(lst2, begin(lst2) + 3, greater{});
    const auto result2 = accumulate(begin(lst2), begin(lst2) + 3, 0);
    
    cout << "Result for Part II: " << result2 << endl;
    assert(result2 == 204610);

    return EXIT_SUCCESS; 
}
