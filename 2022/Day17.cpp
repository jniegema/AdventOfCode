#include "StringToVector.h"
#include "FileIO.h"

//#include <boost/graph/graph_traits.hpp>
//#include <boost/graph/adjacency_list.hpp>
//#include <boost/graph/dijkstra_shortest_paths.hpp>
//#include <boost/multiprecision/cpp_int.hpp>
//#include <boost/spirit/include/qi_parse.hpp>
//#include <boost/spirit/include/qi.hpp>
//
#include <Eigen/Dense>

#include <iostream>
#include <iterator>
#include <algorithm>
#include <ranges>
#include <string_view>
#include <array>
#include <numeric>
#include <cassert>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include<iostream>
#include<vector>
#include<queue>
#include<stack>


#include <chrono>
using namespace std::chrono;


using namespace std;

using IntT = ptrdiff_t;
using M = Eigen::Matrix<int, Eigen::Dynamic, 9>;
using V = Eigen::Matrix<int, 1, 7>;

int xPos;
int yPos;

using M2 = Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic>;

M2 ax = M2::Ones(1, 4);
M2 bx = M2({ {0, 1, 0},
            {1, 1, 1},
            {0, 1, 0} });

M2 cx =M2( { {1, 1, 1},
            {0, 0, 1},
            {0, 0, 1} });

M2 dx = M2::Ones(4, 1);
M2 ex = M2::Ones(2, 2);

std::vector<M2> v = { ax,bx,cx,dx,ex };



bool collides(M const& m, M2 const& m2, int x, int y) {
    const int r = m2.rows();
    const int c = m2.cols();
    
    auto const t = m.block(y, x, r, c).cwiseProduct(m2).eval();
    int result = t.sum();

    return result >0;
}

void print(M const& m, M2 const& cb, int x, int y) {
    M m2 = m;
    m2.block(y, x, cb.rows(), cb.cols()) = cb*2;
    for (int i = (y + cb.rows() + 1); i >= 0; i--) {
        std::cout << m2.row(i) << std::endl;
    }
    std::cout << std::endl;
}


int main17()
{
    const auto rawFile = loadFile("aoc_input_17.txt");

    auto const instructions = rawFile[0];

    IntT maxBlocks = 20000; //2022;

    M m = M::Zero(4* maxBlocks, 9);
    m.col(0).setConstant(8);
    m.col(8).setConstant(8);
    m.row(0).setConstant(8);


    IntT step = 0;
    IntT curBlock = 0;

    IntT top = 0;

    IntT x = 3;
    IntT y = top + 4;
   
    IntT numInstructions = instructions.size();
    std::vector<IntT> vh;
    while (curBlock < maxBlocks) {

        auto const& cb = v[curBlock%5];
        const char curInst = instructions[step%numInstructions];
     
 //       print(m, cb, x, y);
 //       std::cout << "Instruction: " << curInst << std::endl;
        if (curInst == '<') { x--; }
        if (curInst == '>') { x++; }

        if (collides(m, cb, x, y)) {
            // Undo move
            if (curInst == '<') { x++; }
            if (curInst == '>') { x--; }
        }

//        print(m, cb, x, y);
 //       std::cout << "Moving Down:" <<std::endl;

        // Move one down
        y--;

        if (collides(m, cb, x, y)) {
 //           std::cout << "Block collided!" << std::endl;
            y++;
            m.block(y, x, cb.rows(), cb.cols()) = m.block(y, x, cb.rows(), cb.cols())+cb;
            
            const auto old_top = top;
            top = max(top, y + cb.rows()-1);

            //std::cout << curBlock << " " << top - old_top << std::endl;
            vh.push_back(top - old_top);
            std::cout << top - old_top;
            curBlock++;
            x = 3;
            y = top + 4;

        }  
        step++;
    }

    int periodicity = 34;

    //std::vector<IntT> v2 = {1,2,1,2,0,1,2,1,2,0,1,3,2,0,0,1,3,3,4,0,1,2,3,0,1,1,3,2,2,0,0,2,3,4,0};
    //std::vector<IntT> v1 = {1,3,2,1,2,1,3,2,2,0,1,3,2,0,2,1,3,3,4,0,1,2,3,0,1,1,3,2,2,0,0,2,3,4,0};
    //IntT e1 = std::accumulate(v1.begin(), v1.end(), 0);
    //IntT e2 = std::accumulate(v2.begin(), v2.end(), 0);
    //IntT e31 = std::accumulate(vh.begin(), vh.end(), 0);

    //IntT res1 = maxBlocks / ssize(v2);
    //IntT res2 = maxBlocks % ssize(v2);
    //IntT e3 = std::accumulate(v2.begin(), v2.begin()+res2, 0);

    //
    //IntT check = res1 * ssize(v2) + res2;
    //IntT height = e1 + (res1-1) * e2 + e3;

    IntT maxBlocksB = 1000000000000; maxBlocks; // 1000000000000; //2022;
    IntT period = 1710;
    IntT res1B = maxBlocksB / period;
    IntT res2B = maxBlocksB % period;
    IntT check = res1B * period + res2B;

    IntT e1B = std::accumulate(vh.begin(), vh.begin() + period, 0);
    IntT e2B = std::accumulate(vh.begin() + period, vh.begin() + 2* period, 0);
    IntT e3B = std::accumulate(vh.begin() + period, vh.begin() + + period + res2B, 0);
    IntT heightB = e1B + (res1B - 1) * e2B + e3B;



    IntT result1 = top;
    std::cout << "Result for Part I : " << result1 << std::endl;

    IntT result2 = 2;
    std::cout << "Result for Part II : " << result2 << std::endl;

    return EXIT_SUCCESS;
}





//int main()
//{
//    const auto rawFile = loadFile("aoc_input_17t.txt");
//
//    auto const instructions = rawFile[0];
//
//    M m = M::Zero(100000, 9);
//
//
//
//
//    m.col(0).setConstant(8);
//    m.col(8).setConstant(8);
//    m.row(0).setConstant(8);
//
//
//    IntT step = 0;
//    IntT curBlock = 0;
//
//    IntT top = 0;
//    IntT maxTop = 90000;
//    IntT yToKeep = 50000;
//
//    IntT yOffset = 0;
//
//
//    IntT x = 3;
//    IntT y = top + 4;
//
//    IntT numInstructions = instructions.size();
//    IntT maxBlocks = 2022;
//    M mToKeep = M::Zero(yToKeep, 9);
//    while (curBlock < maxBlocks) {
//
//        auto const& cb = v[curBlock % 5];
//        const char curInst = instructions[step % numInstructions];
//
//    //    print(m, cb, x, y);
//
//        if (curInst == '<') { x--; }
//        if (curInst == '>') { x++; }
//
//        if (collides(m, cb, x, y)) {
//            // Undo move
//            if (curInst == '<') { x++; }
//            if (curInst == '>') { x--; }
//        }
//
//     //   std::cout << "Instruction: " << curInst << std::endl;
//    //    print(m, cb, x, y);
//
//   //     std::cout << "Moving Down:" <<std::endl;
//
//                // Move one down
//        y--;
//
//        if (collides(m, cb, x, y)) {
//            y++;
//            m.block(y, x, cb.rows(), cb.cols()) = m.block(y, x, cb.rows(), cb.cols()) + cb;
//            top = max(top, y + cb.rows() - 1+ yOffset);
//         //   std::cout << "Block collided!" << std::endl;
//         //   print(m, cb, x, y);
//
//            auto reTop = top - yOffset;
//            if (reTop > maxTop) {
//                mToKeep = m.block(reTop - yToKeep+1, 0, yToKeep, m.cols());
//                m.block(0, 0, yToKeep, m.cols()) = mToKeep;
//                m.block(yToKeep, 1, reTop- yToKeep, m.cols() - 2).setConstant(0);
//                m.block(0, 0, 1, 9).setConstant(8);
//                yOffset += (reTop - yToKeep + 1);
//               // std::cout << "New situation: " << curInst << std::endl;
//                //print(m, cb, x, y);
//            }
//
//            curBlock++;
//            x = 3;
//            y = top + 4- yOffset;
//
//        }
//        step++;
//
//
//
//        std::cout << step << " " <<  top << std::endl;
//    }
//    //print(m, ax, x, y);
//
//    IntT result1 = top;
//    std::cout << "Result for Part I : " << result1 << std::endl;
//
//    IntT result2 = 2;
//    std::cout << "Result for Part II : " << result2 << std::endl;
//
//    return EXIT_SUCCESS;
//}
