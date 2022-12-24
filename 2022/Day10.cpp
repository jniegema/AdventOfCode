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
#include <numeric>
#include <cassert>
#include <map>


int main10()
{
    const auto rawFile = loadFile("aoc_input_10.txt");


    std::vector<int> XX;

    int X = 1;
    XX.push_back(X); //< Counting cycles from one, so we add one for padding so that the index matches the cycle!
    int cycle = 1;
    for (auto const& curRow : rawFile) {
        const auto tokens = stringToVector(curRow, ' ');

        const auto instruction = tokens[0];

        if (instruction == "noop") {
            //std::cout << cycle << " " << X << std::endl;
            XX.push_back(X);
            cycle++;
        }
        if (instruction == "addx") {
            const auto V = std::stoi(tokens[1]);
            XX.push_back(X);
            XX.push_back(X);
            cycle +=2;
            X += V;
        }
    }

    auto const result = 20*XX[20] + 60*XX[60] + 100*XX[100] + 140*XX[140] + 180*XX[180] + 220*XX[220];
    std::cout << "Result for Part I : " << result << std::endl;


    std::cout << "Result for Part II : " << std::endl;
    for (int i = 1; i <= (6*40); ++i) {

        if (abs(XX[i] - ((i%40)-1 )) <= 1) {
            std::cout << '#';
        }
        else {
            std::cout << '.';
        }

            //std::cout << std::endl;
        if ((i % 40) == 0) { std::cout << std::endl; };
    }

    return EXIT_SUCCESS;
}