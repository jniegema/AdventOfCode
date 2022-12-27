#pragma once

#include <iostream>
#include <ranges>

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

