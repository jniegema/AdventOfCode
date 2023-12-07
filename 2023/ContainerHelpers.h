#pragma once

#include <iostream>
#include <ranges>
#include <algorithm>

namespace sr = std::ranges;
namespace sv = std::views;

void print(auto const& view)
{
    std::cout << "{ ";
    for (const auto element : view)  std::cout << element << ' ';
    std::cout << "} ";
};

void printSplitView(auto outer_view)
{
    std::cout << "split_view[" << std::ranges::distance(outer_view) << "]:  ";
    for (auto const& inner_view : outer_view) print(inner_view);
};

/**
 * Returns true if all elements in a container/range are unique
 **/
template<typename C>
bool isAllUnique(C b) {
    sr::sort(b);
    return (sr::adjacent_find(b) == b.end());
}