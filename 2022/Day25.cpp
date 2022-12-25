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
#include <set>
#include <cstdio>
#include <string>
#include <queue>


using namespace std;
using IntT = ptrdiff_t;

IntT toDecimal(std::string s) {

    IntT N = s.size();
    IntT n;
    IntT exp = 1;
    IntT sum = 0;
    for (IntT i = 0; i < N; ++i) {
        char c = s[N - i - 1];
        if (c == '2') n = 2;
        if (c == '1') n = 1;
        if (c == '0') n = 0;
        if (c == '-') n = -1;
        if (c == '=') n = -2;
        sum += n * exp;
        exp *= 5;
    }
    return sum;

}

std::string toSnafu(IntT n) {

    IntT exp = 1;
    IntT d = 1;
    while ( (2*exp) <= n) { exp *= 5; d++; }


    std::string s(d, '0');
    IntT remainder = n;
    for (int j = 0; j <d; j++) {

        auto c = (double)remainder / (double)exp;
        auto d = int(round(c));
        remainder -= d*exp;

        if (d == 2) s[j] = '2';
        if (d == 1) s[j] = '1';
        if (d == 0) s[j] = '0';
        if (d == -1) s[j] = '-';
        if (d == -2) s[j] = '=';
        exp /= 5;
    }

    return s;
}

int main()
{
    const auto rawFile = loadFile("Data/aoc_input_25.txt");

    const IntT M = ssize(rawFile);
    const IntT N = ranges::max(rawFile | views::transform(ranges::ssize));

    IntT sum = 0;
    for (auto n : rawFile) {

        const auto a = toDecimal(n);
//        const auto b = toSnafu(a);
        //std::cout << n << " " << a << " " << b << endl;
        sum += a;
    }

    const auto result1 = toSnafu(sum);
    assert(result1 == "2=12-100--1012-0=012");
    std::cout << "Result for Part I : " << result1 << std::endl;

 
    return EXIT_SUCCESS;
}