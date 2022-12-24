#include "StringToVector.h"
#include "FileIO.h"

//#include <boost/graph/graph_traits.hpp>
//#include <boost/graph/adjacency_list.hpp>
//#include <boost/graph/dijkstra_shortest_paths.hpp>
//#include <boost/multiprecision/cpp_int.hpp>
//#include <boost/spirit/include/qi_parse.hpp>
//#include <boost/spirit/include/qi.hpp>
//
// #include <Eigen/Dense>

#include <iostream>
#include <iterator>
#include <algorithm>
#include <ranges>
#include <string_view>
#include <array>
#include <numeric>
#include <cassert>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include<iostream>
#include<vector>
#include<queue>
#include<stack>

#include <unsupported/Eigen/CXX11/Tensor>

#include <chrono>
using namespace std::chrono;


using namespace std;

using IntT = ptrdiff_t;
using A = std::array<IntT, 3>;


int main18()
{
    const auto rawFile = loadFile("aoc_input_18.txt");

    std::set<A> ll;

    IntT large = numeric_limits<IntT>::max();
    IntT minX = large, minY = large, minZ = large, maxX = -large, maxY = -large, maxZ = -large;

    for (auto const curRow : rawFile) {
        auto const r = stringToVectorOfInt(curRow, ',');

        const IntT x = r[0];
        const IntT y = r[1];
        const IntT z = r[2];
        minX = min(minX, x - 1);
        minY = min(minY, y - 1);
        minZ = min(minZ, z - 1);
        maxX = max(maxX, x + 1);
        maxY = max(maxY, y + 1);
        maxZ = max(maxZ, z + 1);

        ll.insert({ r[0], r[1], r[2] });
    }
    

    auto const instructions = rawFile[0];
    int surfaces = 0;
    for(auto const [x,y,z] : ll) {
        if (!(ll.contains({ x - 1, y, z }))) { surfaces++; }
        if (!(ll.contains({ x + 1, y, z }))) { surfaces++; }
        if (!(ll.contains({ x, y + 1, z }))) { surfaces++; }
        if (!(ll.contains({ x, y - 1, z }))) { surfaces++; }
        if (!(ll.contains({ x, y, z + 1 }))) { surfaces++; }
        if (!(ll.contains({ x, y, z - 1 }))) { surfaces++; }
    }

    const IntT result1 = surfaces;
    std::cout << "Result for Part I : " << result1 << std::endl;


    /// Do a bfs to find the outside surface:
    IntT surfaces2 = 0;
    queue<A> q;
    set<A> visted;
    q.push({ minX,minY,minZ }); //< Need to start somewhere outside of the drop
    while (!q.empty())
    {
        const auto [x, y, z] = q.front();
        q.pop();
        if (visted.contains({ x,y,z })) continue;
        visted.insert({ x,y,z });
        
        if (x > minX) { if (ll.contains({ x - 1, y, z })) { surfaces2++; } else { q.push({ x - 1, y, z }); } }
        if (x < maxX) { if (ll.contains({ x + 1, y, z })) { surfaces2++; } else { q.push({ x + 1, y, z }); } }
        if (y > minY) { if (ll.contains({ x, y - 1, z })) { surfaces2++; } else { q.push({ x, y - 1, z }); } }
        if (y < maxY) { if (ll.contains({ x, y + 1, z })) { surfaces2++; } else { q.push({ x, y + 1, z }); } }
        if (z > minZ) { if (ll.contains({ x, y, z - 1 })) { surfaces2++; } else { q.push({ x, y, z - 1 }); } }
        if (z < maxZ) { if (ll.contains({ x, y, z + 1 })) { surfaces2++; } else { q.push({ x, y, z + 1 }); } }
    }

    const IntT result2 = surfaces2;
    std::cout << "Result for Part II : " << result2 << std::endl;

    return EXIT_SUCCESS;
}




