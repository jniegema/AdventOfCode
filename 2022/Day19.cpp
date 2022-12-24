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


struct robots {
    int oreR;
    int clayR;
    int obR;
    int geoR;
    auto operator <=>(robots const&) const = default;
};
robots operator+(robots const& a, robots const& b) {
    return { a.oreR + b.oreR, a.clayR + b.clayR,a.obR + b.obR, a.geoR + b.geoR };
}

struct resources {
    int ore;
    int clay;
    int ob;
    int geo;
    auto operator <=>(resources const&) const = default;
};

resources operator+(resources const& a, robots const& b) {
    return { a.ore + b.oreR, a.clay + b.clayR,a.ob + b.obR, a.geo + b.geoR };
}

resources operator+(resources const& a, resources const& b) {
    return { a.ore + b.ore, a.clay + b.clay,a.ob + b.ob, a.geo + b.geo };
}


struct plan {
    int oreRobotCost;
    int clayRobotCost;
    int obRobotCost1;
    int obRobotCost2;
    int geoRobotCost1;
    int geoRobotCost2;
    auto operator <=>(plan const& a) const = default;
};

vector<plan> pl;


int globalMaxGeo = 0;
int maxT2 = 24;


std::map<std::tuple<int, resources, robots>, int> maxGeoCache;

int dfs(plan const& p, int time, resources const& res, robots const& rob) {

    auto it = maxGeoCache.find(std::make_tuple(time, res, rob));
    if (it != maxGeoCache.end()) return it->second;

    if (res.geo > globalMaxGeo) {
        globalMaxGeo = max(globalMaxGeo, res.geo);
    }

    if (time == (maxT2 - 1)) {
        globalMaxGeo = max(globalMaxGeo, res.geo + rob.geoR);
        return res.geo + rob.geoR;
    }

    const int timeLeft = maxT2 - time;

    // Purely theoretical, assuming we produce another geode robot in each remaining step
    const int possibleGeo = res.geo + timeLeft * rob.geoR + (timeLeft - 1) * timeLeft / 2;
    if (possibleGeo < globalMaxGeo) {
        return possibleGeo;  //< If there is no hope, quit!
    }


    int maxGeo = 0;

    /// Various options
    auto const [ore, clay, ob, geo] = res;

    /// If we can buy an geo-bot, we always will
    if (ore >= p.geoRobotCost1 && ob >= p.geoRobotCost2) {
        const resources newRes = { ore - p.geoRobotCost1, clay, ob - p.geoRobotCost2, geo };
        const robots newRob = rob + robots({ 0,0,0,1 });
        maxGeo = max(maxGeo, dfs(p, time + 1, newRes + rob, newRob));
    }
    else {

        if ((timeLeft >= 3) && (rob.obR < p.geoRobotCost2) && (ore >= p.obRobotCost1) && (clay >= p.obRobotCost2)) {
            const resources newRes = { ore - p.obRobotCost1, clay - p.obRobotCost2, ob, geo };
            const robots newRob = rob + robots({ 0,0,1,0 });
            maxGeo = max(maxGeo, dfs(p, time + 1, newRes + rob, newRob));
        }
        //       else {

        if ((timeLeft >= 4) && (rob.clayR < p.obRobotCost2) && (ore >= p.clayRobotCost)) {    ///  && (res.clay<(p.obRobotCost2) )
            const resources newRes = { ore - p.clayRobotCost, clay, ob, geo };
            const robots newRob = rob + robots({ 0,1,0,0 });
            maxGeo = max(maxGeo, dfs(p, time + 1, newRes + rob, newRob));
        }
        const IntT maxOre = max(max(p.oreRobotCost, p.clayRobotCost), max(p.obRobotCost1, p.geoRobotCost2));

        if ((timeLeft >= 3) && (rob.oreR < maxOre) && ore >= p.oreRobotCost) {
            const resources newRes = { ore - p.oreRobotCost, clay, ob, geo };
            const robots newRob = rob + robots({ 1,0,0,0 });
            maxGeo = max(maxGeo, dfs(p, time + 1, newRes + rob, newRob));
        }

        // Or, we could do nothing:
        maxGeo = max(maxGeo, dfs(p, time + 1, res + rob, rob));
        //       }
    }
    maxGeoCache[std::make_tuple(time, res, rob)] = maxGeo;

    return maxGeo;
}

int main19()
{
    const auto rawFile = loadFile("aoc_input_19.txt");

    for (auto const curRow : rawFile) {
        //Blueprint 1: Each ore robot costs 3 ore. Each clay robot costs 4 ore. Each obsidian robot costs 3 ore and 10 clay. Each geode robot costs 2 ore and 7 obsidian.

        auto const r = stringToVector(curRow, ' ');
        int oreRobotCost = stoi(r[6]);
        int clayRobotCost = stoi(r[12]);
        int obRobotCost1 = stoi(r[18]);
        int obRobotCost2 = stoi(r[21]);
        int geoRobotCost1 = stoi(r[27]);
        int geoRobotCost2 = stoi(r[30]);

        pl.push_back({ oreRobotCost, clayRobotCost, obRobotCost1, obRobotCost2, geoRobotCost1, geoRobotCost2 });
    }


    const robots    initRob = { 1,0,0,0 };
    const resources initRes = { 0,0,0,0 };

    maxT2 = 24;
    int sum = 0;
    for (int i = 1; i <= ssize(pl); ++i) {
        std::cout << "Plan #" << i << std::endl;
        maxGeoCache.clear();
        globalMaxGeo = 0;

        const auto start = high_resolution_clock::now();
        auto maxOb = dfs(pl[i - 1], 0, initRes, initRob);
        const auto stop = high_resolution_clock::now();
        const auto duration = duration_cast<milliseconds>(stop - start);
        std::cout << "-> Result: " << maxOb << " " << (i * maxOb) << "  Time taken by function: " << duration.count() / 1000. << " seconds" << endl;

        sum += (i * maxOb);

    }
    IntT result1 = sum;
    std::cout << "Result for Part I : " << result1 << std::endl;


    maxT2 = 32;
    int product = 1;
    for (int i = 1; i <= min(ssize(pl), 3ll); ++i) {
        std::cout << "Plan #" << i << std::endl;
        maxGeoCache.clear();
        globalMaxGeo = 0;

        const auto start = high_resolution_clock::now();
        auto maxOb = dfs(pl[i - 1], 0, initRes, initRob);
        const auto stop = high_resolution_clock::now();
        const auto duration = duration_cast<milliseconds>(stop - start);
        std::cout << "-> Result: " << maxOb << " " << (i * maxOb) << "  Time taken by function: " << duration.count() / 1000. << " seconds" << endl;

        product *= maxOb;

    }


    IntT result2 = product;
    std::cout << "Result for Part II : " << result2 << std::endl;

    return EXIT_SUCCESS;
}




