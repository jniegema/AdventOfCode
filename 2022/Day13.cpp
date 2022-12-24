#include "StringToVector.h"
#include "FileIO.h"

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi.hpp>

#include <iostream>
#include <iterator>
#include <algorithm>
#include <ranges>
#include <string_view>
#include <array>
#include <numeric>
#include <cassert>
#include <map>

using IntT = std::ptrdiff_t;

using namespace boost::spirit;
using namespace std;


struct Expr : std::vector<boost::variant<std::string, Expr>>
{
    using std::vector<boost::variant<std::string, Expr>>::vector;

    friend std::ostream& operator << (std::ostream& os, const Expr& v) {
        os << "{";
        std::string delim = "";
        for (auto const& el : v) {
            os << delim << el;
            delim = ";";
        }
        os << "}";
        return os;
    }

};
using Var = boost::variant<std::string, Expr>;


Expr parseLine(std::string input) {

    using It = std::string::const_iterator;
    using Sk = qi::space_type;
    qi::rule<It, std::string(), Sk> num;
    qi::rule<It, boost::variant<std::string, Expr>(), Sk> term;
    qi::rule<It, Expr(), Sk> expr;

    num  = +(qi::char_ - ',' - ']' - '[');
    term = num | expr;
    expr = '[' >> -(term % ',') >> ']';
    Expr res;
    qi::phrase_parse(input.cbegin(), input.cend(), expr, qi::space, res);

    return res;
}

IntT compare(const Expr& a, const Expr& b);

IntT compare(const Var& x, const Var& y) {
    if ((x.which() == 0) && (y.which() == 0)) {
        const IntT i1 = std::stoi(boost::get<std::string>(x));
        const IntT i2 = std::stoi(boost::get<std::string>(y));
        return i2-i1;
    }

    if ((x.which() == 1) && (y.which() == 1)) {
        const auto i1 = boost::get<Expr>(x);
        const auto i2 = boost::get<Expr>(y);
        return compare(i1, i2);
    }

    if ((x.which() == 0) && (y.which() == 1)) {
        return compare(Expr{x}, y);
    }

    if ((x.which() == 1) && (y.which() == 0)) {
        return compare(x, Expr{y});
    }

    abort();
}


IntT compare(const Expr& a, const Expr& b) {

    const auto Nx = ssize(a);
    const auto Ny = ssize(b);
    const auto N = min(Nx, Ny);
    for (IntT i = 0; i < N; ++i) {
        const auto result = compare(a[i], b[i]);
        if (result != 0) return result;
    }
    return Ny-Nx;
}

bool compare2(const Expr& a, const Expr& b) {
    return compare(a, b) > 0;
}

bool operator== (const Expr& a, const Expr& b) {
    return compare(a, b) == 0;
}


int main13()
{
    std::vector<Expr> list;
    const auto rawFile = loadFile("aoc_input_13.txt");
    for (auto const& curRow : rawFile) {
        if (curRow.empty()) continue;
        list.push_back(parseLine(curRow));
    }

    //// Print the make sure it worked!
    //for (auto const& el : list) {
    //    std::cout << el << std::endl;
    //}

    std::vector<int> inRightOrder;
    for (int i = 0; i < ssize(list) / 2; ++i) {
//        std::cout  << i + 1 << ": ";
        if ( compare2(list[2 * i], list[2 * i + 1]) ) {
//            std::cout << " CORRECT ORDER!" << std::endl;
            inRightOrder.push_back(i + 1);
        }
//        else {
//            std::cout << " WRONG ORDER!" << std::endl;
//        }
    }

    const auto result1 = std::accumulate(begin(inRightOrder), end(inRightOrder), 0);
    std::cout << "Result for Part I : " << result1 << std::endl;


    // Adding markers for Part II
    const auto marker1 = parseLine("[[2]]");
    const auto marker2 = parseLine("[[6]]");

    list.push_back(marker1);
    list.push_back(marker2);

    std::sort(list.begin(), list.end(), compare2);

    const auto p1 = std::distance(list.begin(), std::find(list.begin(), list.end(), marker1))+1;
    const auto p2 = std::distance(list.begin(), std::find(list.begin(), list.end(), marker2))+1;

    const auto result2 = p1*p2;
    std::cout << "Result for Part II : " << result2 << std::endl;

    return EXIT_SUCCESS;
}