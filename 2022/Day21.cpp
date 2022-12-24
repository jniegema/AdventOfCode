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


using namespace std;

enum OpTypes {
    ADD,
    MUL,
    SUB,
    DIV,
    NUM
};

using IntT = int64_t;

struct Operation {

    Operation() = default;

    Operation(IntT in_num) : type(NUM), num(in_num) {}
    
    Operation(char op, std::string in_val1, std::string in_val2) : val1(in_val1), val2(in_val2), num(0) {
        if (val2.empty()) { type = NUM; num = stoi(in_val1); }
        
        if (op == '+') type = ADD;
        if (op == '*') type = MUL;
        if (op == '-') type = SUB;
        if (op == '/') type = DIV;
    }

    OpTypes type;
    std::string val1;
    std::string val2;
    IntT num;
};

map<std::string, Operation> m;
map<std::string, string> m2;

IntT eval(Operation const& o) {

    if (o.type == NUM) return o.num;

    const auto a = eval(m[o.val1]);
    const auto b = eval(m[o.val2]);

    if (o.type == ADD) return a + b;
    if (o.type == SUB) return a - b;
    if (o.type == MUL) return a * b;
    if (o.type == DIV) return a / b;
    abort();
}


IntT rootVal1;
IntT rootVal2;
bool evalRoot2(Operation const& o) {

    auto num1 = eval(m[o.val1]);
    auto num2 = eval(m[o.val2]);
    std::cout << num1 << "  " << num2 << std::endl;
    return num1 == num2;
}


IntT evalInv(std::string c) {

    auto const &res = m2[c];
    auto const &op1 = m[res];

    if (res == "root") {
        return (op1.val1 == c) ? rootVal2 : rootVal1;
    }

    IntT a = evalInv(res);

    if (op1.val1 != c) {
        auto const &op2 = op1.val1;
        const auto b = eval(m[op2]);

        if (op1.type == ADD) return a - b;
        if (op1.type == SUB) return b - a;
        if (op1.type == MUL) return a / b;
        if (op1.type == DIV) return b / a;
    } else {
        auto const &op2 = op1.val2;
        const auto b = eval(m[op2]);
        if (op1.type == ADD) return a - b;
        if (op1.type == SUB) return a + b;
        if (op1.type == MUL) return a / b;
        if (op1.type == DIV) return a * b;
    }
    abort();
}


int main21()
{
    const auto rawFile = loadFile("aoc_input_21.txt");

    for (auto curStr : rawFile) {
        auto v = stringToVectorNoEmpty(curStr, ' ');
        v[0].pop_back();
        std::string name = v[0];
        
        if (v.size() == 2) {
            m[name] = Operation(stoi(v[1]));

        } else {
            m[name] = Operation(v[2][0], v[1], v[3]);
            
            m2[v[1]]=name;
            m2[v[3]]=name;
        }
    }

    const auto result1 = eval(m["root"]);
    assert(result1==70674280581468);
    std::cout << "Result for Part I : " << result1 << std::endl;


    rootVal1 = eval(m[m["root"].val1]);
    rootVal2 = eval(m[m["root"].val2]);
    const auto result2 = evalInv("humn");
    assert(result2==3243420789721);

    std::cout << "Result for Part II : " << result2 << std::endl;


    return EXIT_SUCCESS;
}