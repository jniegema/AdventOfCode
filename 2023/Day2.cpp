#include "FileIO.h"
#include "ContainerHelpers.h"
#include "StringToVector.h"

#include <iostream>
#include <algorithm>
#include <ranges>
#include <numeric>
#include <cassert>
#include <cctype>
#include <regex>

using namespace std;

using IntT = unsigned long long int;

const IntT maxRed = 12;
const IntT maxGreen = 13;
const IntT maxBlue = 14;

struct Draw {
    Draw() : red(0), green(0), blue(0) {}
    IntT red;
    IntT green;
    IntT blue;

    IntT power() { return red * green * blue; }
};

bool isValid(Draw const & draw) {
    return (draw.red <= maxRed) && (draw.green <= maxGreen) && (draw.blue <= maxBlue);
}

int main2()
{
    const auto rawFile = loadFile("Data/aoc_input_2.txt");

    IntT sum = 0;
    IntT sumPowers = 0;

    for (IntT i = 0; i < rawFile.size(); ++i) {
        const auto row = rawFile[i];
        auto items = stringToVector(row, ';');
        removeBefore(items[0], ": ");

        std::vector<Draw> listOfDraws;
        Draw maxDraw;
        for (const auto word : items) {
            auto pair = stringToVector(word, ',');
            
            Draw curDraw;
            for (const auto pr : pair) {
                auto game_result = stringToVector(pr, ' ');

                if (game_result[2] == "red") curDraw.red = toInt(game_result[1]);
                if (game_result[2] == "green") curDraw.green = toInt(game_result[1]);
                if (game_result[2] == "blue") curDraw.blue = toInt(game_result[1]);
                //std::cout << game_result[2] << " " << toIntT(game_result[1]) << std::endl;

            }
            listOfDraws.push_back(curDraw);
            maxDraw.red   = std::max(maxDraw.red,   curDraw.red);
            maxDraw.green = std::max(maxDraw.green, curDraw.green);
            maxDraw.blue  = std::max(maxDraw.blue,  curDraw.blue);
        }
        const bool isPossible = std::all_of(begin(listOfDraws), end(listOfDraws), isValid);
        if (isPossible) sum += (i + 1);

        sumPowers += maxDraw.power();
    }
    
    const auto result1 = sum;
    cout << "Total sum of scores for Part I : " << result1 << endl;
    assert(result1 == 2683);



    // ---- Part 2 ---
    const auto result2 = sumPowers;
    cout << "Total sum of scores for Part II: " << result2 << endl;
    assert(result2 == 49710);


    return EXIT_SUCCESS; 
}
