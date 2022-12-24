#include "StringToVector.h"

#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <string_view>
#include <array>
#include <string>
#include <numeric>
#include <cassert>


template<typename C>
bool isAllUnique(C b) {
    auto const N = b.size();
    std::sort(b.begin(), b.end());
    auto last = std::unique(b.begin(), b.end());
    b.erase(last, b.end());
    return N == b.size();
}

std::size_t findStart(std::string const& s, const std::size_t L) {
    std::vector<char> buffer(s.begin(), s.begin() + L);
    const auto N = s.size();
    for (std::size_t i = L; i < N; ++i) {

        if (!isAllUnique(buffer)) {
            buffer[i % L] = s[i];
        }
        else {
            return i;
        }
    }
    return 0;
}

void main6()
{
    std::ifstream file1("aoc_input_6.txt");
    std::vector<std::string> rawFile(std::istream_iterator<std::string>(file1), {});
    auto const s = rawFile[0];

    std::cout << "Result for Part I : " << findStart(s, 4) << std::endl;

    std::cout << "Result for Part II : " << findStart(s, 14) << std::endl;
}