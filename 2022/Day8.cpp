#include "StringToVector.h"
#include "FileIO.h"

#include <iostream>
#include <iterator>
#include <algorithm>
#include <ranges>
#include <string_view>
#include <array>
#include <string>
#include <numeric>
#include <cassert>
#include <map>


bool isVisible(std::vector<std::vector<int>> matrix, int x, int y) {

    const int Nx = (int)matrix.size();
    const int Ny = (int)matrix[x].size();
 
    bool visFromLeft = true;
    for (int i = (x - 1); i >= 0; i--) {
        if (matrix[i][y] >= matrix[x][y]) visFromLeft = false;
    }
   
    bool visFromRight = true;
    for (int i = x + 1; i < Nx; i++) {
        if (matrix[i][y] >= matrix[x][y]) visFromRight = false;
    }

    bool visFromBottom = true;
    for (int j = (y - 1); j >= 0; j--) {
        if (matrix[x][j] >= matrix[x][y]) visFromBottom = false;
    }
    
    bool visFromTop = true;
    for (int j = y + 1; j < Ny; j++) {
        if (matrix[x][j] >= matrix[x][y]) visFromTop = false;
    }

    return (visFromLeft || visFromRight || visFromBottom || visFromTop);
}

int ViewingScore(std::vector<std::vector<int>> matrix, int x, int y) {

    const int Nx = (int)matrix.size();
    const int Ny = (int)matrix[x].size();

    int visFromLeft = 0;
    for (int i = (x - 1); i >= 0; i--) {
        visFromLeft++;
        if (matrix[i][y] >= matrix[x][y]) break;
    }

    int visFromRight = 0;
    for (int i = x + 1; i < Nx; i++) {
        visFromRight++;
        if (matrix[i][y] >= matrix[x][y]) break;
    }

    int visFromBottom = 0;
    for (int j = (y - 1); j >= 0; j--) {
        visFromBottom++;
        if (matrix[x][j] >= matrix[x][y])break;
    }
    
    int visFromTop = 0;
    for (int j = y + 1; j < Ny; j++) {
        visFromTop++;
        if (matrix[x][j] >= matrix[x][y]) break;
    }

    return (visFromLeft * visFromRight * visFromBottom * visFromTop);
}


void main8()
{
    auto rawFile = loadFile("aoc_input_8.txt");

    std::vector<std::vector<int>> matrix;
    for (int j = 0; j < rawFile.size();++j) {
        matrix.push_back(std::vector<int>());
        for (int i = 0; i < rawFile[j].size();++i) {
            matrix[j].push_back(rawFile[j][i]-'0');
        }
    }
    

    int sum = 0;
    for (int j = 0; j < rawFile.size(); ++j) {
        for (int i = 0; i < rawFile[j].size(); ++i) {
            if( isVisible(matrix,i,j)) { sum++;}
                
        }
    }
    std::cout << "Result for Part I : " << sum << std::endl;


 //   std::cout << 2 << " " << 1 << " " << matrix[1][2] << " " << ViewingScore(matrix, 1, 2) << std::endl;

    int maxScore = 0;
    for (int j = 0; j < rawFile.size(); ++j) {
        for (int i = 0; i < rawFile[j].size(); ++i) {
            auto const score = ViewingScore(matrix, i, j);
  //          std::cout << i << " " << j << " " << matrix[i][j] << " " << score << std::endl;

            maxScore = std::max(maxScore, score);

        }
    }
    std::cout << "Result for Part II : " << maxScore << std::endl;
}