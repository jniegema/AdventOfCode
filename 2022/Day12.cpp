#include "StringToVector.h"
#include "FileIO.h"

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/multiprecision/cpp_int.hpp>

#include <iostream>
#include <iterator>
#include <algorithm>
#include <ranges>
#include <string_view>
#include <array>
#include <numeric>
#include <cassert>
#include <map>

using IntT = std::ptrdiff_t;

struct Pos { IntT x; IntT y; };

using MapT = std::vector<std::vector<int>>;


bool isLegal(MapT const& map, Pos oldPos, Pos newPos) {
    const auto Nx = std::ssize(map);
    const auto Ny = std::ssize(map[0]);
    const auto x1 = oldPos.x;
    const auto y1 = oldPos.y;
    const auto x2 = newPos.x;
    const auto y2 = newPos.y;

    if ((x2 < 0) || (x2 >= Nx) || (y2 < 0) || (y2 >= Ny)) return false;
    if (map[x2][y2] > (map[x1][y1] + 1)) return false;

    return true;
}

using namespace boost;
using GraphT = adjacency_list<listS, vecS, directedS, no_property, property<edge_weight_t, int>>;
using VertexT = graph_traits<GraphT>::vertex_descriptor;

IntT nodeID(IntT x, IntT y, IntT Ny) {
    return x * Ny + y;
}

Pos nodeIDtoCoord(IntT id, IntT Ny) {
    IntT y = (id % Ny);
    IntT x = (id - y) / Ny;
    return { x,y };
}

GraphT buildGraph(MapT const& map) {
    using Edge = std::pair<IntT, IntT>;
    const auto Nx = std::ssize(map);
    const auto Ny = std::ssize(map[0]);
    const auto num_nodes = Nx * Ny;

    std::vector<Edge> edges;
    for (IntT x = 0; x < Nx; ++x) {
        for (IntT y = 0; y < Ny; ++y) {

            const auto nodeId = nodeID(x, y, Ny);
            const Pos curPos = { x,y };
            if (isLegal(map, curPos, { x + 1, y })) { edges.emplace_back(nodeId, nodeID(x + 1, y, Ny)); }
            if (isLegal(map, curPos, { x - 1, y })) { edges.emplace_back(nodeId, nodeID(x - 1, y, Ny)); }
            if (isLegal(map, curPos, { x, y + 1 })) { edges.emplace_back(nodeId, nodeID(x, y + 1, Ny)); }
            if (isLegal(map, curPos, { x, y - 1 })) { edges.emplace_back(nodeId, nodeID(x, y - 1, Ny)); }
        }
    }

    std::vector<int> weights(edges.size(), 1);

    return GraphT(edges.begin(), edges.end(), weights.begin(), (unsigned)num_nodes);
}

auto findPathLength(GraphT const & g, MapT const& map, Pos start, Pos goal, IntT maxLength = std::numeric_limits<IntT>::max() ) {

    const auto Nx = std::ssize(map);
    const auto Ny = std::ssize(map[0]);
    const auto numVertices = Nx * Ny;

    auto startId = nodeID(start.x, start.y, Ny);
    auto goalId = nodeID(goal.x, goal.y, Ny);
    VertexT startV = vertex(startId , g);
    VertexT goalV = vertex(goalId, g);

    std::vector<VertexT> directions(numVertices);

    dijkstra_shortest_paths(g, startV, predecessor_map(directions.data()));

    VertexT p = goalV;
    std::vector<VertexT> path;
    while (p != startV)
    {
        path.push_back(p);
        p = directions[p];

        //auto const coord = nodeIDtoCoord(p, Ny);
  //      std::cout << p << " (" << coord.x << " " << coord.y << ")" << '\n';
        if (std::ssize(path) > maxLength) break;  //< Can't be (much) larger than the solution for part I to account for disconnected graphs. Would be more elegant to detect cycles here ... but whatever!
    }

    return std::ssize(path);
}

int main12()
{
    const auto rawFile = loadFile("aoc_input_12.txt");

    MapT map;

    Pos goal  = { -1,-1 };
    Pos start = { -1,-1 };

    const auto Nx = std::ssize(rawFile);
    for(int x=0; x<Nx;++x) {
        auto const& curRow = rawFile[x];

        const auto Ny = std::ssize(curRow);
        std::vector<int> mapRow;

        for (int y = 0; y < Ny; ++y) {

            const char c= curRow[y];
            if (std::islower(c)) { mapRow.push_back(c-'a'); }
            if (c == 'S') { start = {x,y};  mapRow.push_back(0); }
            if (c == 'E') {
                goal = { x, y };  mapRow.push_back(26);
        }
        }
        map.push_back(mapRow);

    }
    
    GraphT g = buildGraph(map);

    
    const auto result1 = findPathLength(g, map, start, goal);
    std::cout << "Result for Part I : " << result1 << std::endl;


    /// For each starting point with height 0:
    std::vector<IntT> L;
    for (IntT x = 0; x < Nx; ++x) {
        IntT Ny = std::ssize(map[x]);
        for (IntT y = 0; y < Ny; ++y) {
            if (map[x][y] == 0) {
               // std::cout << "Start at " << x << " " << y << " ";
                const auto r = findPathLength(g, map, {x,y}, goal, result1);
                L.push_back(r);
         //       std::cout << r << std::endl;
            }
        }
    }


    const auto minIt = std::min_element(L.cbegin(), L.cend());
    std::cout << "Result for Part II : " << *minIt << std::endl;

    return EXIT_SUCCESS;
}