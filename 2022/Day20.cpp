#include "StringToVector.h"
#include "FileIO.h"

//#include <boost/graph/graph_traits.hpp>
//#include <boost/graph/adjacency_list.hpp>
//#include <boost/graph/dijkstra_shortest_paths.hpp>
//#include <boost/multiprecision/cpp_IntT.hpp>
//#include <boost/spirit/include/qi_parse.hpp>
//#include <boost/spirit/include/qi.hpp>
//
// #include <Eigen/Dense>

#include <iostream>
//#include <iterator>
#include <algorithm>
//#include <ranges>
//#include <string_view>
//#include <array>
#include <numeric>
//#include <cassert>
//#include <map>
//#include <unordered_map>
//#include <unordered_set>
//#include <set>
//#include <iostream>
//#include <vector>
//#include <queue>
//#include <stack>
#include <list>


using namespace std;

using IntT = ptrdiff_t;


void mixList(std::vector<IntT> const & vals, std::vector<IntT>& pos) {
    const IntT N = ssize(pos);
    for (IntT i = 0; i < N; ++i) {

        const auto curit = find(pos.begin(), pos.end(), i);
        const IntT curpos = distance(pos.begin(), curit);
        const IntT move_by = vals[i];

        IntT newPos = (curpos + move_by) % (N - 1);
        while (newPos <= 0) newPos += (N - 1);

        pos.erase(curit);
        pos.insert(pos.begin() + newPos, i);

        //for (IntT j = 0; j < N; ++j) {
        //    std::cout << il[j] << " ";
        //}std::cout << endl;
        //for (IntT j = 0; j < N; ++j) {
        //    std::cout << vals[pos[j]] << " ";
        //}std::cout << endl;
    }


}


int main20()
{

    std::vector<IntT> l2;
    const auto rawFile = loadFile("aoc_input_20.txt");

    IntT idx = 0;
    for (auto const curRow : rawFile) {
        l2.push_back(stoi(curRow));
        idx++;
    }

    IntT N = ssize(l2);
    std::vector<IntT> il(N);
    std::iota(il.begin(), il.end(), 0);

    mixList(l2, il);

    // Reordered list
    std::vector<IntT> rl;
    for (IntT j = 0; j < N; ++j) {  rl.push_back(l2[il[j]]);  };

    IntT zeropos = distance(rl.begin(), find(rl.begin(), rl.end(), 0));
    IntT zeropos1 = (zeropos + 1000) % N;
    IntT zeropos2 = (zeropos + 2000) % N;
    IntT zeropos3 = (zeropos + 3000) % N;

    IntT result1 = rl[zeropos1] + rl[zeropos2] + rl[zeropos3];
    std::cout << "Result for Part I : " << result1 << std::endl;




    std::iota(il.begin(), il.end(), 0);
    for (IntT j = 0; j < N; ++j) {  l2[j]*= 811589153; };


    for (int k = 0; k < 10; ++k) {
        mixList(l2, il);
        //for (IntT j = 0; j < N; ++j) {
        //    std::cout << l2[il[j]] << " ";
        //}std::cout << endl;
    }

    // Reordered list
    for (IntT j = 0; j < N; ++j) { rl[j] = l2[il[j]]; };

    IntT zeroposb = distance(rl.begin(), find(rl.begin(), rl.end(), 0));
    IntT zeropos1b = (zeroposb + 1000) % N;
    IntT zeropos2b = (zeroposb + 2000) % N;
    IntT zeropos3b = (zeroposb + 3000) % N;

    IntT result2 = rl[zeropos1b] + rl[zeropos2b] + rl[zeropos3b];

    std::cout << "Result for Part II : " << result2 << std::endl;

    return EXIT_SUCCESS;
}

