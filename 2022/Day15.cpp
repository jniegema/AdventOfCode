#include "StringToVector.h"
#include "FileIO.h"

//#include <boost/graph/graph_traits.hpp>
//#include <boost/graph/adjacency_list.hpp>
//#include <boost/graph/dijkstra_shortest_paths.hpp>
//#include <boost/multiprecision/cpp_int.hpp>
//#include <boost/spirit/include/qi_parse.hpp>
//#include <boost/spirit/include/qi.hpp>
//
//#include <Eigen/Dense>

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

using namespace std;

using IntT = ptrdiff_t;

struct Interval {
    Interval(IntT ss, IntT ee) :
        s(min(ss, ee)),
        e(max(ss, ee)) {}

    IntT s;
    IntT e;

    auto operator <=>(Interval const & a) const = default;
};

bool canFuse(Interval const& i1, Interval const& i2) {
    return !(i2.s > (i1.e + 1)) && !(i2.e < (i1.s - 1));
}

Interval fuse(Interval const& i1, Interval const& i2) {
    assert(canFuse(i1, i2));
    return Interval(min(i1.s,i2.s), max(i1.e, i2.e));
}


template<typename IterT>
IterT fuseIntervals(IterT b, IterT e) {
    std::sort(b, e);

    IterT c = b;
    IntT minI = b->s;
    IntT maxI = b->e;
    for (IterT cc = std::next(b); cc != e; ++cc) {
        if (cc->s <= maxI) { maxI = max(maxI, cc->e); continue; }

        c->s = minI; c->e = maxI; ++c;
        minI = cc->s; maxI = cc->e;
    }
    c->s = minI; c->e = maxI; ++c;

    return c;
}


int main15()
{
    const auto rawFile = loadFile("aoc_input_15.txt");

    std::vector<std::array<IntT, 5>> sensorList;
    for (auto const& curRow : rawFile) {
        //if (curRow.empty()) continue;
        auto const list = stringToVector(curRow,' ');
        auto s1 = list[2].substr(2); s1.pop_back();
        auto s2 = list[3].substr(2); s2.pop_back();
        auto b1 = list[8].substr(2); b1.pop_back();
        auto b2 = list[9].substr(2);// b2.pop_back();

        const IntT x1 = stoi(s1);
        const IntT y1 = stoi(s2);
        const IntT x2 = stoi(b1);
        const IntT y2 = stoi(b2);

        const IntT distance = abs(x1 - x2)  +  abs(y1 - y2);
        sensorList.push_back({ x1,y1,distance,x2,y2 });
    }

    IntT const yIdx = 2000000;
    std::vector<IntT> set;
    std::set<IntT> set2;

    for (const auto [x,y,d,xb,yb] : sensorList) {

        if (yb == yIdx) { set2.insert(xb); }

        auto const o = d - abs(y - yIdx);

        if (o >= 0) {
            auto const c1 =  x - o;
            auto const c2 =  x + o;

            for (IntT i = c1; i <= c2; ++i) { set.push_back(i); }
        }
    }
    std::sort(set.begin(), set.end());
    set.erase(std::unique(set.begin(), set.end()), set.end());

    std::vector<IntT> set3;
    std::set_difference(set.begin(), set.end(), set2.begin(), set2.end(), std::back_inserter(set3));

    const auto result1 = set3.size(); //  6124805
    assert(result1 == 6124805);
    std::cout << "Result for Part I : " << result1 << std::endl;
    

    IntT maxY = 4000000;
    std::vector<std::vector< Interval>> setX;
    std::vector<Interval> set4;
    setX.reserve(maxY);


    for (IntT yc = 0; yc <= maxY; ++yc) {

        for (const auto [x, y, d, xb, yb] : sensorList) {

            auto const o = d - abs(y - yc);

            if( o >= 0) {
                auto const c1 = x - o;
                auto const c2 = x + o;
                set4.emplace_back(c1,c2);
            }
        }
        set4.erase(fuseIntervals(set4.begin(), set4.end()), set4.end());
        setX.push_back(set4);
        set4.clear();
    }

    IntT ii = 0;
    IntT result2 = 0;
    for (auto const & curSet : setX) {
        if (curSet.size() != 1 || (curSet[0].s>0) || (curSet[0].e < 4000000) ) {
             result2 = IntT(ii) + (curSet[0].e + 1) * IntT(4000000);
        }
        ii++;
    }

    std::cout << "Result for Part II : " << result2 << std::endl;
    assert(result2 == 12555527364986);
    return EXIT_SUCCESS;
}
