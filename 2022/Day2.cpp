
#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <string_view>

#include <string>
#include <cassert>
#include <numeric>


int charToInt(char curChar) {
    switch (curChar)
    {
        case 'A' :
        case 'X' : return 1;
        
        case 'B' :
        case 'Y' : return 2;
        
        case 'C' :
        case 'Z' : return 3;
    }
    throw std::runtime_error("Invalid Input!");
}

int scoreGame(std::pair<int, int> const & game) {
    const auto a = game.first;
    const auto b = game.second;

    if ((a == b)) return 3;
    
    if ((a == 1) && (b == 2)) return 6; // Paper wins over Rock
    if ((a == 1) && (b == 3)) return 0; // Scissors loses to Rock
    
    if ((a == 2) && (b == 1)) return 0;
    if ((a == 2) && (b == 3)) return 6;
    
    if ((a == 3) && (b == 1)) return 6;
    if ((a == 3) && (b == 2)) return 0;

    throw std::runtime_error("Invalid input!");
}

int totalScore1(std::pair<int, int> const& game) {
    return scoreGame(game) + game.second;
}


int findWhatToPlay(std::pair<int, int> const& game) {
    const auto a = game.first;
    const auto b = game.second;

    if (b==2) return a;  //< To draw, we return the same as them

    if ((a == 1) && (b == 1)) return 3; // If A has Rock and we need to lose, we play Scissors
    if ((a == 1) && (b == 3)) return 2; // If A has Rock and we need to win, we play Paper

    if ((a == 2) && (b == 1)) return 1; // If A has Paper and we need to lose, we play Rock
    if ((a == 2) && (b == 3)) return 3; // If A has Paper and we need to win, we play Scissors

    if ((a == 3) && (b == 1)) return 2; // If A has Scissors and we need to lose, we play Paper
    if ((a == 3) && (b == 3)) return 1; // If A has Scissors and we need to win, we play Rock

    throw std::runtime_error("Invalid input!");
}

int totalScore2(std::pair<int, int> const& game) {
    return findWhatToPlay(game) + (game.second-1)*3;
}

void main2()
{
    std::ifstream file("Data/aoc_input_2.txt");
    std::vector<std::pair<int,int>> listOfGames;

    std::string str;
    while (std::getline(file, str))
    {
        listOfGames.emplace_back(charToInt(str[0]), charToInt(str[2]));
    }

    std::vector<int> scores1;
    std::transform(listOfGames.cbegin(), listOfGames.cend(), std::back_inserter(scores1), &totalScore1);

    const auto sumOfScores = std::accumulate(scores1.cbegin(), scores1.cend(), 0);
    assert(sumOfScores == 13268);
    std::cout << "Result for Part I: " << sumOfScores << std::endl;


    // --- Part II ---
    std::vector<int> scores2;
    std::transform(listOfGames.cbegin(), listOfGames.cend(), std::back_inserter(scores2), &totalScore2);

    const auto sumOfScores2 = std::accumulate(scores2.cbegin(), scores2.cend(), 0);
    assert(sumOfScores2 == 15508);
    std::cout << "Result for Part II: " << sumOfScores2 << std::endl;
}