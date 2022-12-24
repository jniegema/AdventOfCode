#include "StringToVector.h"
#include "FileIO.h"

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/multiprecision/cpp_int.hpp>


#include <iostream>
#include <iterator>
#include <algorithm>
#include <ranges>
#include <string_view>
#include <array>
#include <numeric>
#include <cassert>
#include <map>

using namespace boost::multiprecision;

enum OpTypes {
    ADD,
    MUL,
    SQUARE
};


using IntT = int64_t; // int128_t; // cpp_int;

struct Operation {

    Operation(OpTypes in_type, IntT in_val) : type(in_type), val(in_val) {}

    OpTypes type;
    IntT val;

    IntT eval(IntT oldVal) const {
        switch (type) {
        case ADD: return val + oldVal;
        case MUL: return val * oldVal;
        case SQUARE: {return oldVal * oldVal; }
        }
    }
};


struct Item {

    Item(IntT in_startValue) : startValue(in_startValue) {}

    IntT startValue;
    std::vector<Operation> opList;

    IntT eval() const;
    IntT eval(IntT mod) const;
};

using ItemPtr = std::shared_ptr<Item>;


IntT Item::eval() const {
    IntT result = startValue;
    for (auto const& curOp : opList) {
        result = curOp.eval(result);
    }
    return result;
}

IntT Item::eval(IntT mod) const {
    IntT result = startValue;
    for (auto const& curOp : opList) {
        result = (curOp.eval(result)) % mod;
    }
    return result;
}


std::vector<ItemPtr> parseItems(std::vector<std::string> const& tokens) {
    std::vector<ItemPtr> items;
    for (int64_t i = 2; i < std::ssize(tokens); ++i) {
        auto str = tokens[i];
        str.erase(std::remove(str.begin(), str.end(), ','), str.end());
        items.emplace_back(std::make_shared<Item>(std::stoi(str)));
    }
    return items;
}


Operation parseOp(std::string const& s1) {
    const auto ss1 = stringToVectorNoEmpty(s1, ' ');
    const auto op2 = ss1[4];
    const auto op3 = ss1[5];

    if (op2 == "+") {
        return Operation(ADD, std::stoi(op3));
    }
    if (op2 == "*") {
        if (op3 == "old") {
            return Operation(SQUARE, 00);
        } else {
            return Operation(MUL, std::stoi(op3));
        };
    }
    abort();
}

struct Test {

    Test(std::string s1, std::string s2, std::string s3) {

        auto ss1 = stringToVectorNoEmpty(s1, ' ');
        div = std::stoi(ss1[3]);

        auto ss2 = stringToVectorNoEmpty(s2, ' ');
        target1 = std::stoi(ss2[5]);

        auto ss3 = stringToVectorNoEmpty(s3, ' ');
        target2 = std::stoi(ss3[5]);
    }

    int64_t div;
    int64_t target1;
    int64_t target2;

    int64_t eval(IntT val) {
        if ((val % div) == 0) { return target1; }
        else {
            return target2;
        }
    }

    int64_t eval(Item const & item) {
        return (item.eval(div) == 0) ? target1 : target2;
    }

};


struct Monkey {

    Monkey(std::vector<std::string> s1, std::string s2, std::string s3, std::string s4, std::string s5) :
        items(parseItems(s1)),
        op(parseOp(s2)),
        test(s3,s4,s5),
        numInspected(0)
    {
    }

    std::vector<ItemPtr> items;
    Operation         op;
    Test              test;
    int64_t           numInspected;
};



IntT getResult(std::vector<Monkey> listOfMonkeys, IntT numRounds) {

    // For each round
    for (int64_t rnd = 0; rnd < numRounds; rnd++) {

        for (auto& curMonkey : listOfMonkeys) {

            for (auto curItem : curMonkey.items) {
                // Add the operation to the current item
                curItem->opList.push_back(curMonkey.op);

                // Evaluate current item
                const auto target = curMonkey.test.eval(*curItem);

                // Copy item to other monkey
                listOfMonkeys[target].items.push_back(curItem);

                curMonkey.numInspected++;
            }
            curMonkey.items.clear(); /// Monkey has no items left!
        }

    }

    std::vector<IntT> inspectedList;
    for (auto& curMonkey : listOfMonkeys) {
        inspectedList.push_back(curMonkey.numInspected);
    }

    std::sort(inspectedList.begin(), inspectedList.end(), std::greater<IntT>());

    return inspectedList[0] * inspectedList[1];
}


int main11()
{
    const auto rawFile = loadFile("aoc_input_11.txt");
    std::vector<Monkey> listOfMonkeys;

    int64_t i = 0;
    while (i < std::ssize(rawFile)) {
        listOfMonkeys.push_back(Monkey(stringToVectorNoEmpty(rawFile[i + 1], ' '), rawFile[i + 2], rawFile[i + 3], rawFile[i + 4], rawFile[i + 5]));
        i += 7;
    }

    const auto result1 = 1;
    std::cout << "Result for Part I : " << result1 << std::endl;

    const auto result2 = getResult(listOfMonkeys, 10000);
    std::cout << "Result for Part II : " << result2 << std::endl;

    return EXIT_SUCCESS;
}