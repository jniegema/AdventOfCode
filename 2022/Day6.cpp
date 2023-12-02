#include "StringToVector.h"
#include "FileIO.h"
#include "ContainerHelpers.h"

#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>

#include <ranges>
#include <string_view>
#include <array>
#include <string>
#include <numeric>
#include <cassert>

using namespace std;
namespace sr = std::ranges;


size_t findStart(string const& s, const size_t L) {
    vector<char> buffer(s.begin(), s.begin() + L);
    const auto N = s.size();
    for (size_t i = L; i < N; ++i) {

        if (!isAllUnique(buffer)) {
            buffer[i % L] = s[i];
        } else {
            return i;
        }
    }
    return 0;
}

int main()
{
    vector<string> rawFile = loadFile("Data/aoc_input_6.txt");

    auto const s = rawFile[0];

    const auto result1 = findStart(s, 4);
    cout << "Result for Part I : " << result1 << endl;
    assert(result1 == 1262);

    const auto result2 = findStart(s, 14);
    cout << "Result for Part II : " << result2 << endl;
    assert(result2 == 3444);

    return EXIT_SUCCESS;
}