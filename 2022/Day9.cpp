#include "StringToVector.h"
#include "FileIO.h"

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <iostream>
#include <iterator>
#include <algorithm>
#include <ranges>
#include <string_view>
#include <array>
#include <string>
#include <numeric>
#include <cassert>
#include <map>



struct Pos {
    int x;
    int y;

    auto operator<=>(const Pos&) const = default;
};

void updateNextKnot(int xH, int yH, int& xT, int& yT) {

    // If they are still touching, we do nothing
    if (abs(xH - xT) <= 1 && abs(yH - yT) <= 1) return;

    if (yH > yT) { yT++; }
    if (yH < yT) { yT--; }
    if (xH > xT) { xT++; }
    if (xH < xT) { xT--; }
} 

void updateKnots(std::vector<Pos>& knots) {
    for (int j = 0; j < (std::ssize(knots) - 1); ++j) {
        updateNextKnot(knots[j].x, knots[j].y, knots[j + 1].x, knots[j + 1].y);
    }
}

auto numUniqueTailPos(const int numKnots, std::vector<std::string> const & instructions) {

    std::vector<Pos> knots(numKnots, { 0,0 });

    std::vector<Pos> listOfTailPos;
    listOfTailPos.push_back(knots.back());

    for (auto const& curRow : instructions) {
        const auto tokens = stringToVector(curRow, ' ');
        const char instruction = tokens[0][0];
        const auto count = std::stoi(tokens[1]);
        
        for (int i = 0; i < count; ++i) {
            if (instruction == 'U') knots[0].y += 1;
            if (instruction == 'D') knots[0].y -= 1;
            if (instruction == 'L') knots[0].x -= 1;
            if (instruction == 'R') knots[0].x += 1;

            updateKnots(knots);
            listOfTailPos.push_back(knots.back());
        }
    }

    std::sort(listOfTailPos.begin(), listOfTailPos.end());
    auto const lastIter = std::unique(listOfTailPos.begin(), listOfTailPos.end());

    return std::distance(listOfTailPos.begin(), lastIter);
}

int main9()
{
    const auto rawFile = loadFile("aoc_input_9.txt");

    const auto result1 = numUniqueTailPos(2, rawFile);
    assert(result1 == 6098);
    std::cout << "Result for Part I : " << result1 << std::endl;
     
    const auto result2 = numUniqueTailPos(10, rawFile);
    assert(result2 == 2597);
    std::cout << "Result for Part II : " << result2 << std::endl;

    return EXIT_SUCCESS;
}