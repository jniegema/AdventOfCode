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
#include <map>

using namespace std;

using IntT = long long int;

int main6()
{
    const auto rawFile = loadFile("Data/aoc_input_6.txt");

    IntT sum1 = 1;
    IntT sum2 = 0;

    auto time = removeBefore(rawFile[0], ": ");
    auto timeList = std::vector<IntT>({ 53897698 }); // stringToVectorOfInt(time, ' ');

    auto dist = removeBefore(rawFile[1], ": ");
    auto distList = std::vector<IntT>({ 313109012141201 }); stringToVectorOfInt(dist, ' ');
    
    for (IntT i = 0; i < distList.size(); ++i) {
        auto const t = timeList[i];
        auto const d = distList[i];

        //std::vector<IntT> totalDist;
        //for (IntT tp = 0; tp < t; ++tp) {
        //    totalDist.push_back((t - tp) * tp);
        //}
        //IntT numBetter = std::count_if(begin(totalDist), end(totalDist), [d](auto x) {return x > d; });
        //sum1 *= numBetter;

        double cut1 = (-t + sqrt(double(t) * t - 4. * d)) / -2.;
        double cut2 = (-t - sqrt(double(t) * t - 4. * d)) / -2.;

        IntT cut1i = IntT(cut1);
        IntT cut2i = IntT(cut2- 0.000000000001);

        sum2 = cut2i - cut1i;
    }

    std::vector<IntT> locList;

    const auto result1 = sum1;
    cout << "Total sum of scores for Part I : " << result1 << endl;
    //assert(result1 == 32314248

    

    // ---- Part 2 ---
    const auto result2 = sum2;
    cout << "Total sum of scores for Part II: " << result2 << endl;
    //assert(result2 == 79874951);

    
    return EXIT_SUCCESS; 
}
