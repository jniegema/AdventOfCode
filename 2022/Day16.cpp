#include "StringToVector.h"
#include "FileIO.h"

//#include <boost/graph/graph_traits.hpp>
//#include <boost/graph/adjacency_list.hpp>
//#include <boost/graph/dijkstra_shortest_paths.hpp>
//#include <boost/multiprecision/cpp_int.hpp>
//#include <boost/spirit/include/qi_parse.hpp>
//#include <boost/spirit/include/qi.hpp>
//
//#include <Eigen/Dense>

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


#include <chrono>
using namespace std::chrono;


using namespace std;

using IntT = ptrdiff_t;


//struct Valve {
//    std::string name;
//    int         flow;
//
//    std::vector<std::string> connections;
//};

struct Valve {
    int name;
    int flow;

    std::vector<std::pair<int,int>> connections;
};

std::vector<Valve> mapOfValves;
std::vector<Valve> mapOfValves2; //< Simplified


int maxTime = 30;

//void visitValve(std::string const& prevVal, std::string const &curVal, int curTime, int totalFlow, std::set<std::string>& openValves) {
//
//    auto const& V = mapOfValves.at(curVal);
//   // cout << "T=" << curTime << " - Enter valve " << curVal << " Total flow " << totalFlow << std::endl;
//
//    if (totalFlow > maxTotalFime) {
//        std::cout << "New max : " << totalFlow << std::endl;
//        maxTotalFime = totalFlow;
//    }
//    // Time is up or all valves are open (nothing we can do)
//    if (( curTime == maxTime) || (openValves.size() == numValves)) {
//        return;
//    }
//
//    // Option 1: Open the current valve if it isn't already:
//    if ((V.flow>0) && (!openValves.contains(curVal))) {
//        openValves.insert(curVal);
//    //    cout << "T=" << curTime << " - Open valve " << curVal << " with flux " << V.flow <<std::endl;
//        visitValve(curVal,curVal, curTime+1, totalFlow + V.flow * (maxTime - curTime-1), openValves);
//        openValves.erase(curVal);
//    }
//
//    // Option 2: Visit other rooms:
//    auto const & c = V.connections;
//    for (auto const & newVal : c) {
//        if (newVal == prevVal) continue;
//
//        visitValve(curVal,newVal, curTime + 1, totalFlow, openValves);
//    }
//}


int numValves = 0;
int maxTotalFlux1 = 0;
void visitValve(std::unordered_set<int>& prevVal, int const& curVal, int curTime, int totalFlow, std::unordered_set<int>& openValves) {

    auto const& V = mapOfValves2[curVal];
    // cout << "T=" << curTime << " - Enter valve " << curVal << " Total flow " << totalFlow << std::endl;

    if (totalFlow > maxTotalFlux1) {
        std::cout << "New max : " << totalFlow << std::endl;
        maxTotalFlux1 = totalFlow;
    }
    // Time is up or all valves are open (nothing we can do)
    if ((curTime == maxTime) || (openValves.size() == numValves)) {
        return;
    }

    // Option 1: Open the current valve if it isn't already:
    if ((V.flow > 0) && (!openValves.contains(curVal))) {
        openValves.insert(curVal);
        //    cout << "T=" << curTime << " - Open valve " << curVal << " with flux " << V.flow <<std::endl;
        std::unordered_set<int> empty;
        visitValve(empty, curVal, curTime + 1, totalFlow + V.flow * (maxTime - curTime - 1), openValves);
        openValves.erase(curVal);
    }

    // Option 2: Visit other rooms:
    auto const& c = V.connections;
    for (auto const& newVal : c) {
        if (prevVal.contains(newVal.first)) continue;
        
        prevVal.insert(curVal);
        visitValve(prevVal, newVal.first, curTime + newVal.second, totalFlow, openValves);
        prevVal.erase(curVal);
    }
}




inline bool contains(std::vector<int> C, int e) {
    return find(C.begin(), C.end(), e) != C.end();
}


inline bool contains(std::unordered_set<int> C, int e) {
    return C.contains(e);
}

int getEstimateOfRemainingFlow(std::unordered_set<int> const& openValves, int curTime) {
    int result = 0;
    for (auto const& v : mapOfValves) {
        if (!contains(openValves, v.name)) result += (maxTime - curTime - 1) * v.flow;
    }
    return result;
}


void visitValveB(std::vector<int>& prevVal, int const& curVal, int curTime, int totalFlow, std::vector<int>& openValves) {

    auto const& V = mapOfValves2[curVal];
    // cout << "T=" << curTime << " - Enter valve " << curVal << " Total flow " << totalFlow << std::endl;

    if (totalFlow > maxTotalFlux1) {
        std::cout << "New max : " << totalFlow << std::endl;
        maxTotalFlux1 = totalFlow;
    }

    // Time is up or all valves are open (nothing we can do)
    if ((curTime >= maxTime) || (openValves.size() == numValves)) {
        return;
    }

    // Option 1: Open the current valve if it isn't already:
    if ((V.flow > 0) && (!contains(openValves, curVal)) ) {

        openValves.push_back(curVal);
        //    cout << "T=" << curTime << " - Open valve " << curVal << " with flux " << V.flow <<std::endl;
        std::vector<int> empty;
        visitValveB(empty, curVal, curTime + 1, totalFlow + V.flow * (maxTime - curTime - 1), openValves);
        openValves.pop_back();
    }

    // Option 2: Visit other rooms:
    auto const& c = V.connections;
    for (auto const& newVal : c) {
        if (contains(prevVal, newVal.first)) continue;

        //prevVal.insert(curVal);
        prevVal.push_back(curVal);
        visitValveB(prevVal, newVal.first, curTime + newVal.second, totalFlow, openValves);
        prevVal.pop_back();
    }
}





std::vector<int> maxFlow(maxTime, 0);



int maxTotalFlux2 = 0;
void visitValve2(std::vector<int>& prevVal1, int curVal1, std::vector<int>& prevVal2, int curVal2,  int curTime, int totalFlow, std::unordered_set<int>& openValves) {

    auto const& V1 = mapOfValves[curVal1];
    auto const& V2 = mapOfValves[curVal2];
    // cout << "T=" << curTime << " - Enter valve " << curVal << " Total flow " << totalFlow << std::endl;

    if (totalFlow > maxTotalFlux2) {
        std::cout << "New max : " << totalFlow << std::endl;
        maxTotalFlux2 = totalFlow;
    }

    // Time is up or all valves are open (nothing we can do)
    if ((curTime >= maxTime) || (openValves.size() == numValves)) {
        return;
    }

    // Compute an estimate of what we still could accomplish if all remaining valves were opened right now:
    if (curTime > 10) {
        const int upperLimit = totalFlow + getEstimateOfRemainingFlow(openValves, curTime);
        if (upperLimit < maxTotalFlux2) return;
    } 

    // Option 1: Both are in separate rooms
    if (curVal1 != curVal2) {

        // Both open their respective valves:
        if ((V1.flow > 0) && (V2.flow > 0) && (!contains(openValves,curVal1))  && (!contains(openValves,curVal2))) {
            openValves.insert(curVal1);
            openValves.insert(curVal2);
            //    cout << "T=" << curTime << " - Open valve " << curVal << " with flux " << V.flow <<std::endl;
            std::vector<int> prev1;
            std::vector<int> prev2;
            visitValve2(prev1, curVal1, prev2, curVal2, curTime + 1, totalFlow + (V1.flow * (maxTime - curTime - 1)) + (V2.flow * (maxTime - curTime - 1)), openValves);
            openValves.erase(curVal2);
            openValves.erase(curVal1);
        }

        //Only one opens the valve, other moves on
        if ((V1.flow > 0) && (!contains(openValves,curVal1))) {
            auto const& c2 = V2.connections;
            for (auto const& newVal2 : c2) {
                if(contains(prevVal1, newVal2.first)) continue;
                if(contains(prevVal2, newVal2.first)) continue;

                openValves.insert(curVal1);
                std::vector<int> prev1;
                prevVal2.push_back(curVal2);
                visitValve2(prev1, curVal1, prevVal2, newVal2.first, curTime + 1, totalFlow + V1.flow * (maxTime - curTime - 1), openValves);
                prevVal2.pop_back();
                openValves.erase(curVal1);
            }
        }

        //Only other one opens the valve, first moves on
        if ((V2.flow > 0) && (!contains(openValves,curVal2))) {
            auto const& c1 = V1.connections;
            for (auto const& newVal1 : c1) {
                if (contains(prevVal1, newVal1.first)) continue;
                if (contains(prevVal2, newVal1.first)) continue;
                openValves.insert(curVal2);
                std::vector<int> prev2;
                prevVal1.push_back(curVal1);
                visitValve2(prevVal1, newVal1.first, prev2, curVal2, curTime + 1, totalFlow + V2.flow * (maxTime - curTime - 1), openValves);
                prevVal1.pop_back();
                openValves.erase(curVal2);
            }
        }
        
        auto const& c1 = V1.connections;
        auto const& c2 = V2.connections;
        for (auto const& newVal1 : c1) {
            if (contains(prevVal1, newVal1.first)) continue;
            if (contains(prevVal2, newVal1.first)) continue;
            for (auto const& newVal2 : c2) {
                if (contains(prevVal1, newVal2.first)) continue;
                if (contains(prevVal2, newVal2.first)) continue;
               
                prevVal1.push_back(curVal1);
                prevVal2.push_back(curVal2);
                visitValve2(prevVal1, newVal1.first, prevVal2, newVal2.first, curTime + 1, totalFlow , openValves);
                prevVal1.pop_back();
                prevVal2.pop_back();
            }
        }


    } else {

        auto const& c = V2.connections;
        // Both are in the same room, one opens the valve, the other continues on (does not matter who)
        if ((V1.flow > 0) && (!contains(openValves,curVal1))) {
            
            for (auto const& newVal2 : c) {
                if (contains(prevVal1, newVal2.first)) continue;
                if (contains(prevVal2, newVal2.first)) continue;
                
                openValves.insert(curVal1);
                std::vector<int> prev1;
                prevVal2.push_back(curVal2);
                visitValve2(prev1, curVal1, prevVal2, newVal2.first, curTime + 1, totalFlow + V1.flow * (maxTime - curTime - 1), openValves);
                prevVal2.pop_back();
                openValves.erase(curVal1);
            }
        }

        // Both are in the same room, and each continue on (does not matter who goes where, so avoid duplicates?)
        const int maxI = c.size();
        for (int i = 0; i < maxI; ++i) {
            auto const & newVal1 = c[i];
            if (contains(prevVal1, newVal1.first)) continue;
            if (contains(prevVal2, newVal1.first)) continue;
            for (int j = 0; j <= i; ++j) {
                auto const& newVal2 = c[j];
                if (contains(prevVal1, newVal2.first)) continue;
                if (contains(prevVal2, newVal2.first)) continue;
                prevVal1.push_back(curVal1);
                prevVal2.push_back(curVal2);
                visitValve2(prevVal1, newVal1.first, prevVal2, newVal2.first, curTime + 1, totalFlow, openValves);
                prevVal1.pop_back();
                prevVal2.pop_back();
            }
        }
    }
}


int main16()
{
    const auto rawFile = loadFile("aoc_input_16.txt");

    std::unordered_map<std::string, int> mapId;

    numValves = 0;
    int curValveId = 0;

    mapOfValves.resize(rawFile.size());
    for (auto const& curRow : rawFile) {

        auto const list = stringToVector(curRow, ';');
        auto const A = stringToVector(list[0], ' ');
        auto B = stringToVector(list[1], ',');
        B[0] = stringToVector(B[0], ' ').back();
        auto const A2 = stringToVector(A[4], '=');

        auto const name = A[1];

        mapId[name] = curValveId;
        curValveId++;
    }

    curValveId = 0;
    for (auto const& curRow : rawFile) {

        //
        auto const list = stringToVector(curRow, ';');
        auto const A = stringToVector(list[0], ' ');
        auto B = stringToVector(list[1], ',');
        B[0] = stringToVector(B[0], ' ').back();
        auto const A2 = stringToVector(A[4], '=');

        auto const name = A[1];

        auto const flux = stoi(A2[1]);
        if (flux > 0) { numValves++; }
        
        std::vector<std::pair<int,int>> connections;
        for (auto const & curName : B) {
            connections.emplace_back(mapId[curName],1);
        }

        auto V = Valve(curValveId, flux, connections);
        mapOfValves[curValveId] = V;

        curValveId++;
    }
    
    
    for (auto& v : mapOfValves) {

        auto& c = v.connections;
        std::sort(c.begin(), c.end(), [&](auto a, auto b) {return mapOfValves[a.first].flow > mapOfValves[b.first].flow; });
    }



  //  /// Simplify Graph

    int startId = mapId["AA"];
    mapOfValves2 = mapOfValves;
    for (int id = 0; id < ssize(mapOfValves2); ++id ) {
        
        auto& V = mapOfValves2[id];
        if (id == startId) continue;
        if (V.flow == 0) {

            auto const& cc = V.connections;
            for (auto const& c : cc) {

                auto& c2 = mapOfValves2[c.first].connections;
                c2.erase(std::remove(c2.begin(), c2.end(), make_pair(id, c.second)), c2.end());

                for (auto const& d : cc) {
                    if (d.first == c.first) continue;
                    c2.emplace_back(make_pair(d.first, d.second + c.second));
                }
            }
            V.connections.clear();
        }
    }







    std::vector<int> openValves;
    std::vector<int> prevPos;

    maxTime = 30;
    maxTotalFlux1;

    const auto start = high_resolution_clock::now();
    visitValveB(prevPos, startId, 0, 0, openValves);
    const auto stop = high_resolution_clock::now();
    const auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Time taken by function: " << duration.count()/1000. << " seconds" << endl;

    IntT result1 = maxTotalFlux1;
    std::cout << "Result for Part I : " << result1 << std::endl;



    maxTime = 26;
    maxTotalFlux2 = 0;
    std::unordered_set<int> openValves2;
    std::vector<int> prevPos1;
    std::vector<int> prevPos2;
    const auto start2 = high_resolution_clock::now();
    visitValve2(prevPos1, startId, prevPos2, startId, 0, 0, openValves2);
    const auto stop2 = high_resolution_clock::now();
    const auto duration2 = duration_cast<milliseconds>(stop2 - start2);
    cout << "Time taken by function: " << duration2.count() / 1000. << " seconds" << endl;

    IntT result2 = maxTotalFlux2;
    std::cout << "Result for Part II : " << result2 << std::endl;

    return EXIT_SUCCESS;
}