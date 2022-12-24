// AdventOfCode2022_Cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "StringToVector.h"
#include "FileIO.h"

#include <iostream>
#include <algorithm>
#include <ranges>
#include <numeric>
#include <cassert>

using namespace std;

//void main()
//{
//    std::ifstream file("aoc_input_1.txt");
// 
//    std::string str2;
//    std::vector<std::vector<int>> listOfLists;
//
//    std::vector<int> curList;
//
//    while (std::getline(file, str2))
//    {
//        if (!str2.empty()) {
//            curList.push_back(std::stoi(str2));
//        }
//        else {
//            listOfLists.emplace_back(curList);
//            curList.clear();
//        }
//    }
//
//    std::vector<int> totalEnergy;
//    totalEnergy.resize(listOfLists.size());
//
//    std::transform(listOfLists.cbegin(), listOfLists.cend(),
//        totalEnergy.begin(), // write to the same location
//        [](auto const& l) { return std::accumulate(l.cbegin(),l.cend(),0); });
//
//    auto largestElement = std::max_element(totalEnergy.begin(), totalEnergy.end());
//
//    const auto result1 = *largestElement;
//    assert(result1 == 70374);
//    std::cout << "Result for Part I: " << result1 << std::endl;
//
//    // --- Part II ---
//    std::partial_sort(totalEnergy.begin(), totalEnergy.begin()+3, totalEnergy.end(), std::greater{});
//    const auto result2 = std::accumulate(totalEnergy.begin(), totalEnergy.begin() + 3, 0);
//    std::cout << "Result for Part II: " << result2 << std::endl;
//    assert(result2 == 204610);
//
//}

void print(auto const& view)
{
    // `view` is of std::views::lazy_split_view::__outer_iterator::value_type
    cout << "{ ";
    for (const auto element : view)  cout << element << ' ';
    cout << "} ";
};

int toInt(string const& s) { return stoi(s); }

int main1()
{
    const auto rawFile = loadFile("Data/aoc_input_1.txt");

    ranges::split_view outer_view{ rawFile, "" }; //< Split on empty strings
    //std::cout << "splits[" << std::ranges::distance(outer_view) << "]:  ";
    //for (auto const& inner_view : outer_view)
    //    print(inner_view);

    auto lst = outer_view | views::transform([](auto const& c) { const auto p = c | views::transform(toInt);  return accumulate(begin(p), end(p), 0); });

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
