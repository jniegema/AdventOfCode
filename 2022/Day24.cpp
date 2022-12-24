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

using MapT = vector<pair<IntT, IntT>>;
using SetT = set<pair<IntT, IntT>>;
using MMapT = multimap<pair<IntT, IntT>, IntT>;


struct A {
    IntT r;
    IntT c;
    IntT minute;
    auto operator <=>(A const& a) const = default;

};

struct B {
    IntT r;
    IntT c;
    IntT dr;
    IntT dc;

    //auto operator <=>(B const& b) const = default;
    auto operator <(B const& b) const {
        if (r != b.r) {
            return (r < b.r);
        } else {
            return (c < b.c);
        }
    }

    auto operator ==(B const& b) const {
        //if (r == b.r) { return (r < b.r); }
        return (r == b.r) && (c == b.c);
    }
};


std::multiset<B> updateBlizzards(std::multiset<B> const & bb, IntT const M, IntT const N) {
    std::multiset<B> newB; 
    for (auto& b : bb) {
        IntT r = b.r + b.dr;
        IntT c = b.c + b.dc;
        if (r == 0) { r = M - 2; }
        if (r == (M - 1)) { r = 1; }
        if (c == 0) { c = N - 2; }
        if (c == (N - 1)) { c = 1; }
        newB.emplace(r, c, b.dr, b.dc);
    }
    return newB;
}

bool isFree(IntT r, IntT c, std::multiset<B> const& b, IntT const M, IntT const N) {
    if ((r == 0) && (c == 1)) return true;
    if ((r == (M-1)) && (c == (N-2))) return true;
    if( (r < 1) || (r>(M-2)) || (c < 1) || (c > (N - 2)) ) return false;

    return !b.contains(B(r, c, 0, 0));
}

int main()
{
    const auto rawFile = loadFile("Data/aoc_input_24.txt");

    const IntT M = ssize(rawFile);
    const IntT N = ranges::max(rawFile | views::transform(ranges::ssize));

    std::multiset<B> bStart;
    for(IntT r=0; r<M; ++r){

        for (IntT c = 0; c < N; ++c){
            auto const curChar = rawFile[r][c];
            if (curChar == '>') bStart.emplace(r, c, 0, 1);
            if (curChar == '<') bStart.emplace(r, c, 0,-1);
            if (curChar == '^') bStart.emplace(r, c,-1, 0);
            if (curChar == 'v') bStart.emplace(r, c, 1, 0);
        }

    }

    std::vector<std::multiset<B>> bs;
    bs.push_back(bStart);

    const IntT startR = 0;
    const IntT startC = 1;

    /// Do a bfs to find the path:
    set<A> visted;
    queue<A> q;
    q.push({ startR,startC, 0 }); 

    IntT curStage = 0;
    IntT result1 = 0;
    IntT result2 = 0;
    while (!q.empty())
    {
        const auto [r, c, m] = q.front();
        q.pop();
        if (visted.contains({ r,c,m })) continue;
        visted.emplace(r, c, m);

        if (ssize(bs) < (m + 2)) { bs.push_back(updateBlizzards(bs[m], M, N)); }
        auto const& b = bs[m + 1];

        if (r == (M - 1) && c == (N - 2)) {
            
            if (curStage == 0) {
                curStage = 1;
                result1 = m;
                while (!q.empty()) q.pop();
            }
            
            if(curStage == 2) {
                result2 = m;
                break;
            }
        }

        if( (curStage == 1) && (r == 0) && (c == 1)) {
            curStage = 2;
            while (!q.empty()) q.pop();
        }

       if (isFree(r+1, c, b, M, N)) q.push({ r + 1, c, m + 1 });
       if (isFree(r-1, c, b, M, N)) q.push({ r - 1, c, m + 1 });
       if (isFree(r, c+1, b, M, N)) q.push({ r, c + 1, m + 1 });
       if (isFree(r, c-1, b, M, N)) q.push({ r, c - 1, m + 1 });
       if (isFree(r, c, b, M, N)) q.push({ r, c, m + 1 });
    }


    assert(result1 == 279);
    std::cout << "Result for Part I : " << result1 << std::endl;
    
    assert(result2 == 762);
    std::cout << "Result for Part II : " << result2 << std::endl;

    return EXIT_SUCCESS;
}