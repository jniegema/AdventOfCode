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

struct PlantMap {

    PlantMap(IntT in_dstStart, IntT in_srcStart, IntT in_length) : dstStart(in_dstStart), srcStart(in_srcStart), length(in_length) {}
    IntT srcStart;
    IntT dstStart;
    IntT length;


    IntT minSrc() const { return srcStart; }
    IntT maxSrc() const { return srcStart+length-1; }
    IntT getOffset() const { return (dstStart - srcStart); }

    bool isIn(IntT src) const {
        return (src >= srcStart) && (src < (srcStart + length));
    }

    IntT performMap(IntT src) const {
        return src + (dstStart - srcStart);
    }

    bool operator < (PlantMap const& b) const { return srcStart < b.srcStart; }

};


IntT plantMap(std::vector<PlantMap> const& mp, IntT src) {
    for (auto const & curMp : mp) {
        const bool isIn = curMp.isIn(src);
        if (isIn) return curMp.performMap(src);
    }
    return src;
}

int main5()
{
    const auto rawFile = loadFile("Data/aoc_input_5.txt");

    IntT sum1 = 0;
    IntT sum2 = 0;

    auto seeds = removeBefore(rawFile[0], ": ");
    auto seedList = stringToVectorOfInt(seeds, ' ');

    std::vector<PlantMap> seedToSoil, soilToFertilizer, fertilizerToWater, waterToLight, lightToTemperature, temperatureToHumidity, humidityToLocation;

    for (IntT i = 1; i < rawFile.size(); ++i) {
        auto const row = rawFile[i];

        if (row.starts_with("seed-to-soil")) {
            ++i; auto next_row = rawFile[i];
            while (next_row != "") {
                auto ll1 = stringToVectorOfInt(next_row, ' ');
                seedToSoil.emplace_back(ll1[0], ll1[1], ll1[2]);
                ++i; next_row = rawFile[i];
            }
        }

        if (row.starts_with("soil-to-fertilizer")) {
            ++i; auto next_row = rawFile[i];
            while (next_row != "") {
                auto ll1 = stringToVectorOfInt(next_row, ' ');
                soilToFertilizer.emplace_back(ll1[0], ll1[1], ll1[2]);
                ++i; next_row = rawFile[i];
            }
        }

        if (row.starts_with("fertilizer-to-water")) {
            ++i; auto next_row = rawFile[i];
            while (next_row != "") {
                auto ll1 = stringToVectorOfInt(next_row, ' ');
                fertilizerToWater.emplace_back(ll1[0], ll1[1], ll1[2]);
                ++i; next_row = rawFile[i];
            }
        }

        if (row.starts_with("water-to-light")) {
            ++i; auto next_row = rawFile[i];
            while (next_row != "") {
                auto ll1 = stringToVectorOfInt(next_row, ' ');
                waterToLight.emplace_back(ll1[0], ll1[1], ll1[2]);
                ++i; next_row = rawFile[i];
            }
        }

        if (row.starts_with("light-to-temperature")) {
            ++i; auto next_row = rawFile[i];
            while (next_row != "") {
                auto ll1 = stringToVectorOfInt(next_row, ' ');
                lightToTemperature.emplace_back(ll1[0], ll1[1], ll1[2]);
                ++i; next_row = rawFile[i];
            }
        }
        if (row.starts_with("temperature-to-humidity")) {
            ++i; auto next_row = rawFile[i];
            while (next_row != "") {
                auto ll1 = stringToVectorOfInt(next_row, ' ');
                temperatureToHumidity.emplace_back(ll1[0], ll1[1], ll1[2]);
                ++i; next_row = rawFile[i];
            }
        }

        if (row.starts_with("humidity-to-location ")) {
            ++i; auto next_row = rawFile[i];
            while ((next_row != "") && (i < rawFile.size())) {
                auto ll1 = stringToVectorOfInt(next_row, ' ');
                humidityToLocation.emplace_back(ll1[0], ll1[1], ll1[2]);
                ++i;
                if (i < rawFile.size()) next_row = rawFile[i];
            }
        }
    }

    std::vector<IntT> locList;
    for (auto curSeed : seedList) {
        auto const soil = plantMap(seedToSoil, curSeed);
        auto const fert = plantMap(soilToFertilizer, soil);
        auto const water = plantMap(fertilizerToWater, fert);
        auto const light = plantMap(waterToLight, water);
        auto const temp = plantMap(lightToTemperature, light);
        auto const hum = plantMap(temperatureToHumidity, temp);
        auto const loc = plantMap(humidityToLocation,hum);
        locList.push_back(loc);
    }

    const auto result1 = sr::min(locList);
    cout << "Total sum of scores for Part I : " << result1 << endl;
    assert(result1 == 323142486);


    // Brute force works well in C++
    IntT minLoc = numeric_limits<IntT>::max();
    for (int i = 0; i < seedList.size() / 2; ++i) {
        std::cout << "Seed range " << i << " of " << seedList.size() / 2 << std::endl;
        for (int j = 0; j < seedList[2 * i + 1]; ++j) {
            auto curSeed = seedList[2 * i] + j;

            auto const soil = plantMap(seedToSoil, curSeed);
            auto const fert = plantMap(soilToFertilizer, soil);
            auto const water = plantMap(fertilizerToWater, fert);
            auto const light = plantMap(waterToLight, water);
            auto const temp = plantMap(lightToTemperature, light);
            auto const hum = plantMap(temperatureToHumidity, temp);
            auto const loc = plantMap(humidityToLocation, hum);
            minLoc = min(minLoc, loc);
        }
    }


    // ---- Part 2 ---
    const auto result2 = minLoc;
    cout << "Total sum of scores for Part II: " << result2 << endl;
    assert(result2 == 79874951);

    
    return EXIT_SUCCESS; 
}
