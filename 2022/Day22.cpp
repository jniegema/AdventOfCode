#include "StringToVector.h"
#include "FileIO.h"

#include <iostream>
#include <iterator>
#include <algorithm>
#include <ranges>
#include <string_view>
#include <array>
#include <numeric>
#include <cassert>
#include <map>
#include <cstdio>
#include <string>

using namespace std;
using IntT = ptrdiff_t;

using MapT = vector<vector<int>>;

void wrapPos(MapT const& m, IntT& xPos, IntT& yPos, IntT const dir) {
    if (dir == 0) {
        IntT x = 0;
        while (m[yPos][x] == 0) ++x;
        xPos = x;
        return;
    }

    if (dir == 2) {
        IntT x = m[yPos].size() - 1;
        while (m[yPos][x] == 0) --x;
        xPos = x;
        return;
    }

    if (dir == 1) {
        IntT y = 0;
        while (m[y][xPos] == 0) ++y;
        yPos = y;
        return;
    }

    if (dir == 3) {
        IntT y = m.size() - 1;
        while (m[y][xPos] == 0) --y;
        yPos = y;
        return;
    }
}

const int side = 50;

void wrapPos2(MapT const& m, IntT& xPos, IntT& yPos, IntT& dir) {
    if (dir == 0) {
        if ((yPos >= 0  ) && (yPos < 50))  { yPos = 149 - yPos; xPos =  99; dir = 2; return; }  /// Face 3 -> 6
        if ((yPos >= 50) && (yPos < 100))  { xPos = yPos + 50; yPos = 49;   dir = 3; return; }  /// Face 5 -> 4
        if ((yPos >= 100) && (yPos < 150)) { yPos = 149 - yPos; xPos = 149; dir = 2; return; }  /// Face 6 -> 3
        if ((yPos >= 150) && (yPos < 200)) { xPos = yPos -100; yPos = 149;  dir = 3; return; }  /// Face 8 -> 7
        abort();
    }

    if (dir == 2) {
        if ((yPos >=   0) && (yPos <  50)) { yPos = 149 - yPos; xPos = 0;  dir = 0; return; }   /// Face 14 -> 11
        if ((yPos >=  50) && (yPos < 100)) { xPos = yPos - 50; yPos = 100; dir = 1; return; }   /// Face 13 -> 12
        if ((yPos >= 100) && (yPos < 150)) { yPos = 149 - yPos; xPos = 50; dir = 0; return; }   /// Face 11 -> 14
        if ((yPos >= 150) && (yPos < 200)) { xPos = yPos - 100; yPos = 0;  dir = 1; return; }   /// Face 10 -> 1
        abort();
    }

    if (dir == 1) {
        if ((xPos >= 0)   && (xPos < 50))  { xPos = xPos + 100; yPos = 0;  dir = 1; return; }   /// Face 9-> 2
        if ((xPos >= 50)  && (xPos < 100)) { yPos = xPos + 100; xPos = 49; dir = 2; return; }   /// Face 7 -> 8
        if ((xPos >= 100) && (xPos < 150)) { yPos = xPos -  50; xPos = 99; dir = 2; return; }   /// Face 4 -> 5
        abort();
    }

    if (dir == 3) {
        if ((xPos >=   0) && (xPos <  50)) { yPos = xPos +  50; xPos = 50;  dir = 0; return; }   /// Face 12->13
        if ((xPos >=  50) && (xPos < 100)) { yPos = xPos + 100; xPos = 0;   dir = 0; return; }   /// Face 1 -> 10
        if ((xPos >= 100) && (xPos < 150)) { xPos = xPos - 100; yPos = 199; dir = 3; return; }   /// Face 2 -> 9
        abort();
    }

    abort();
}

void printMap(MapT const& m) {

    for (int i = 0; i < ssize(m); ++i) {
        for (int j = 0; j < ssize(m[i]); ++j) {
            auto mm = m[i][j];
            if (mm == 0) std::cout << ' ';
            if (mm == 1) std::cout << '.';
            if (mm == 2) std::cout << '#';
            if (mm == 3) std::cout << '>';
            if (mm == 4) std::cout << 'v';
            if (mm == 5) std::cout << '<';
            if (mm == 6) std::cout << '^';
        }
        std::cout << std::endl;
    }
}

int main22()
{
    auto rawFile = loadFile("aoc_input_22.txt");

    const auto instructions = rawFile.back();
    rawFile.pop_back();
    rawFile.pop_back();

    const IntT M = ssize(rawFile);
    const IntT N = ranges::max(rawFile | views::transform(ranges::ssize));

    std::vector<std::vector<int>> m;
    for (auto const& curLine : rawFile) {
        m.push_back(std::vector<int>());
        auto& curM = m.back();
        for (auto const curChar : curLine) {

            if (curChar == ' ') curM.push_back(0);
            if (curChar == '.') curM.push_back(1);
            if (curChar == '#') curM.push_back(2);
        }


        while (curM.size() < N) curM.push_back(0);

    }

    // Alternative (shorter/more elegant) option: Keep list of strings!
    //auto m = rawFile;
    //for (auto& curM : m) {  while (curM.size() < N) curM.push_back(' '); }


    // Starting coordinate:
    IntT startX = 0;
    IntT startY = 0;
    IntT dir = 0;
    while (m[0][startX] != 1) ++startX;


    // Split the instructions
    std::vector<int> i;
    int lpos = 0;
    int rpos = 0;
    for (rpos = 0; rpos < instructions.size(); ++rpos) {
        auto c = instructions[rpos];
        if (isdigit(c)) continue;

        i.push_back(stoi(instructions.substr(lpos, rpos - lpos)));
        lpos = rpos + 1;
        if (c == 'R') i.push_back(-1);
        if (c == 'L') i.push_back(-2);
    }
    if (rpos != lpos) i.push_back(stoi(instructions.substr(lpos, rpos - lpos)));




    IntT xPos = startX;
    IntT yPos = startY;
    for (auto curI : i) {
        //std::cout << "Cur pos: " << xPos << " " << yPos << " " << dir << std::endl;
        if (curI == -1) {
            dir = (dir + 1) % 4;
            continue;
        }
        if (curI == -2) {
            dir = (dir + 3) % 4;
            continue;
        }

        for (int k = 0; k < curI; ++k) {
            IntT newX = xPos, newY = yPos;
            if (dir == 0) newX++;
            if (dir == 1) newY++;
            if (dir == 2) newX--;
            if (dir == 3) newY--;

            if ((newY < 0) || (newY == M) || (newX < 0) || (newX == (m[newY].size())) || m[newY][newX] == 0) { wrapPos(m, newX, newY, dir); }// warp around, check if there is a wall there!
            if (m[newY][newX] == 2) break;
            xPos = newX;
            yPos = newY;
//            std::cout << "  New pos: " << xPos << " " << yPos << " " << dir << std::endl;
        }
    }

    const auto result1 = 4 * (xPos + 1) + 1000 * (yPos + 1) + dir;
    assert(result1 == 89224);
    std::cout << "Result for Part I : " << result1 << std::endl;



    xPos = startX;
    yPos = startY;
    dir = 0;
    for (auto curI : i) {
        //if (dir == 0) m[yPos][xPos] = 3;
        //if (dir == 1) m[yPos][xPos] = 4;
        //if (dir == 2) m[yPos][xPos] = 5;
        //if (dir == 3) m[yPos][xPos] = 6;
      //  printMap(m);
        if (curI == -1) {
            dir = (dir + 1) % 4;
            continue;
        }
        if (curI == -2) {
            dir = (dir + 3) % 4;
            continue;
        }

        for (int k = 0; k < curI; ++k) {
            IntT newX = xPos, newY = yPos, newDir = dir;
            if (dir == 0) newX++;
            if (dir == 1) newY++;
            if (dir == 2) newX--;
            if (dir == 3) newY--;

            if ((newY < 0) || (newY == M) || (newX < 0) || (newX == (m[newY].size())) || m[newY][newX] == 0) { wrapPos2(m, newX, newY, newDir); }// warp around, check if there is a wall there!
            if (m[newY][newX] == 2) break;
            xPos = newX;
            yPos = newY;
            dir = newDir;
            //if (dir == 0) m[yPos][xPos] = 3;
            //if (dir == 1) m[yPos][xPos] = 4;
            //if (dir == 2) m[yPos][xPos] = 5;
            //if (dir == 3) m[yPos][xPos] = 6;
    //        std::cout << "  New pos: " << xPos << " " << yPos << " " << dir << std::endl;
        }
    }

    const auto result2 = 4 * (xPos + 1) + 1000 * (yPos + 1) + dir;
    assert(result2 == 136182);

    std::cout << "Result for Part II : " << result2 << std::endl;


    return EXIT_SUCCESS;
}