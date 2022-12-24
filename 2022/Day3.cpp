
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

int evalCharList(std::vector<char> const& list) {
    //std::vector<int> valueList;
    //std::transform(list.cbegin(), list.cend(), std::back_inserter(valueList), [](char c) { int val = c - 65; return (val < 26) ? (val + 27) : (val - 31); });
    //return std::accumulate(valueList.cbegin(), valueList.cend(), 0);

    return std::transform_reduce(list.cbegin(), list.cend(), 0, std::plus{}, [](char c) { int val = c - 65; return (val < 26) ? (val + 27) : (val - 31); });
}

void main3()
{
    std::ifstream file("aoc_input_3.txt");
    std::vector<std::string> listOfItems(std::istream_iterator<std::string>(file), {});

    //std::vector<std::pair<std::string, std::string>> listOfItemsInCompartments;
    //std::transform(listOfItems.cbegin(), listOfItems.cend(), std::back_inserter(listOfItemsInCompartments), [](auto const& s) { const auto N = s.length()/2;
    //                                                                                                                            auto p = std::make_pair(s.substr(0, N), s.substr(N));
    //                                                                                                                            std::sort(p.first.begin(), p.first.end());
    //                                                                                                                            std::sort(p.second.begin(), p.second.end());
    //                                                                                                                            return p; });

    // Find the common letter for each half by splitting the strings, sorting each of them and then calling std::set_intersection
    std::vector<char> commonLetter;
    std::transform(listOfItems.cbegin(), listOfItems.cend(), std::back_inserter(commonLetter), [](auto const& s) { const auto N = s.length()/2;
                                                                                                                   auto str1 = s.substr(0, N);
                                                                                                                   std::sort(str1.begin(), str1.end());
                                                                                                                   auto str2 = s.substr(N);
                                                                                                                   std::sort(str2.begin(), str2.end());
                                                                                                                   std::vector<char> result;
                                                                                                                   std::set_intersection(str1.begin(), str1.end(), str2.begin(), str2.end(), std::back_inserter(result));
                                                                                                                   return result[0]; });

    std::cout << "Total sum of scores for Part I : " << evalCharList(commonLetter)<< std::endl;


    // ---- Part 2 ---
    std::vector<std::string> sortedListOfItems;
    std::transform(listOfItems.cbegin(), listOfItems.cend(), std::back_inserter(sortedListOfItems), [](auto s) { std::sort(s.begin(), s.end()); return s; });

    std::vector<char> groupBadge;
    const auto N = sortedListOfItems.size();
    for(int i = 0; i < N; i+=3) {
        std::vector<char> result;
        std::set_intersection(sortedListOfItems[i].cbegin(), sortedListOfItems[i].cend(), sortedListOfItems[i+1].cbegin(), sortedListOfItems[i+1].cend(), std::back_inserter(result));
        std::vector<char> result2;
        std::set_intersection(result.cbegin(), result.cend(), sortedListOfItems[i + 2].cbegin(), sortedListOfItems[i + 2].cend(), std::back_inserter(result2));
        groupBadge.push_back(result2[0]);
    }

    std::cout << "Total sum of scores for Part II: " << evalCharList(groupBadge) << std::endl;
}