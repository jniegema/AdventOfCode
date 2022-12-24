#include "StringToVector.h"
#include "FileIO.h"

//#include <boost/graph/graph_traits.hpp>
//#include <boost/graph/adjacency_list.hpp>
//#include <boost/graph/dijkstra_shortest_paths.hpp>
//#include <boost/multiprecision/cpp_int.hpp>
//#include <boost/spirit/include/qi_parse.hpp>
//#include <boost/spirit/include/qi.hpp>
//
#include <Eigen/Dense>

#include <iostream>
#include <iterator>
#include <algorithm>
#include <ranges>
#include <string_view>
#include <array>
#include <numeric>
#include <cassert>
#include <map>

using namespace std;

using IntT = ptrdiff_t;
using MapT = Eigen::Matrix<int,Eigen::Dynamic, Eigen::Dynamic>;

struct Pos { int x; int y; };

void  drawLine(int xStart, int yStart, int xEnd, int yEnd, MapT& map) {

    int dx = xEnd - xStart;
    int dy = yEnd - yStart;
    int N = std::max(abs(dx), abs(dy));
    dx = dx / N;
    dy = dy / N;
    for (int i = 0; i <= N; ++i) {
        map(xStart + i * dx, yStart + i * dy) = 1;
    }
}

void fillMap(std::vector < std::vector<Pos> > listOfLines, int minX, MapT& map) {

    for (auto curLine : listOfLines) {
        for (int i = 0; i < curLine.size() - 1; ++i) {
            const auto startPos = curLine[i];
            const auto endPos = curLine[i+1];
            drawLine(startPos.x - minX, startPos.y, endPos.x - minX, endPos.y, map);
        }
    }

}

bool dropSand(int minX, MapT& map) {

    int sX = 500 - minX;
    int sY = 0 ;
    if (map(sX, sY) != 0) return false;

    const int maxY = map.cols()-1;
    
    while (sY < maxY) {

        if (map(sX  ,sY+1) == 0) { sY++; continue; }
        if (map(sX-1,sY+1) == 0) { sX--; sY++; continue; }
        if (map(sX+1,sY+1) == 0) { sX++; sY++; continue; }

        map(sX,sY) = 2;
        return true;
    }
    return false;
}

void printMap(MapT& map) {
    const int Nx = map.rows();
    const int Ny = map.cols();
    
    for (int j = 0; j < Ny; ++j) {
        for (int i = 0; i < Nx; ++i) {
            std::cout << map(i,j);
        }
        std::cout << '\n';
    }
    std::cout << std::endl;
}


int main14()
{
    const auto rawFile = loadFile("aoc_input_14.txt");
    std::vector < std::vector<Pos> > listOfLines;
    
    int minX=10000, maxX = 0,maxY = 0;
    for (auto curRow : rawFile) {
        replaceStringInPlace(curRow, " -> ", ";");
        const auto s = stringToVector(curRow, ';');

        std::vector<Pos> listOfPos;
        std::transform(s.cbegin(), s.cend(), std::back_inserter(listOfPos), [](auto ss) { auto a = stringToVector(ss, ',');  return Pos{ stoi(a[0]), stoi(a[1]) }; });

        for (auto curPos : listOfPos) {
            minX = std::min(minX, curPos.x);
            maxX = std::max(maxX, curPos.x);
            maxY = std::max(maxY, curPos.y);
        }

        listOfLines.push_back(listOfPos);
    }
    const auto numY = maxY + 3;
    const auto numX = maxX - minX + 2* numY;

    minX -= numY;

    MapT map = MapT::Zero(numX,numY);
    fillMap(listOfLines, minX , map);

    printMap(map);
    

    int cnt = 0;
    while (dropSand(minX, map)) {
        cnt++;
       // if ((cnt % 20) == 0) { 
         //   printMap(map); 
      //  }

    }

    const auto result1 = cnt;
    std::cout << "Result for Part I : " << result1 << std::endl;

    // Reset map
    map = MapT::Zero(numX, numY);
    fillMap(listOfLines, minX, map);

    for (int x = 0; x < numX; ++x) {
        map(x, numY - 1) = 3;
    }
    printMap(map);
   
    
    cnt = 0;
    while (dropSand(minX, map)) {
        cnt++;
        // if ((cnt % 20) == 0) { 
    //    printMap(map); 
        //  }

    }


    const auto result2 = cnt;
    std::cout << "Result for Part II : " << result2 << std::endl;

    return EXIT_SUCCESS;
}