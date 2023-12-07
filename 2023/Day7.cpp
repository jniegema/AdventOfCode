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

const std::vector<char> types = { 'A', 'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2' };
const std::map<char, int> typeValue = { {'A',13}, {'K',12}, {'Q',11}, {'J', 10}, {'T', 9}, {'9', 8}, {'8', 7}, {'7', 6}, {'6', 5}, {'5',4}, {'4', 3}, {'3', 2}, {'2', 1} };

// Part II
const std::vector<char> types2 = { 'A', 'K', 'Q', 'T', '9', '8', '7', '6', '5', '4', '3', '2' };
const std::map<char, int> typeValue2 = { {'A',13}, {'K',12}, {'Q',11}, {'J', 0}, {'T', 9}, {'9', 8}, {'8', 7}, {'7', 6}, {'6', 5}, {'5',4}, {'4', 3}, {'3', 2}, {'2', 1} };

int getType(std::string cards) {
    std::vector<int> howMany(types.size(), 0);
    for (int i = 0; i < types.size();++i) {
        const auto curType = types[i];
        howMany[i] = sr::count_if(cards, [curType](char x) {return x == curType; });
    }

    // 5 of a kind
    if( sr::any_of(howMany, [](auto x) {return x == 5; })) return 7;
    
    if (sr::any_of(howMany, [](auto x) {return x == 4; })) return 6;
    
    if ((sr::any_of(howMany, [](auto x) {return x == 3; }))&&
        (sr::any_of(howMany, [](auto x) {return x == 2; }))) return 5;
    
    if (sr::any_of(howMany, [](auto x) {return x == 3; })) return 4;

    if (sr::count_if(howMany, [](auto x) {return x == 2; })==2) return 3;
    
    if (sr::any_of(howMany, [](auto x) {return x == 2; })) return 2;

    return 1;
}


int getType2(std::string cards) {
    int maxType = 0;
    if (cards.contains('J')) {
        for (int i = 0; i < 5; ++i) {
            if (cards[i] == 'J') {
                for (char r : types2) {
                    cards[i] = r;
                    maxType = max(maxType, getType2(cards));
                    if (maxType == 7) return maxType; //< Best possible hand, no need to keep trying!
                }
                break;
            }
        }
    } else {
        maxType = getType(cards);
    }
    return maxType;
}


bool isLess(std::string a, std::string b) {
    for (int i = 0; i < 5; ++i) {
        const auto ta = typeValue.at(a[i]);
        const auto tb = typeValue.at(b[i]);
        if (ta != tb) return ta < tb;
    }
    return false;
}

bool isLess2(std::string a, std::string b) {
    for (int i = 0; i < 5; ++i) {
        const auto ta = typeValue2.at(a[i]);
        const auto tb = typeValue2.at(b[i]);
        if (ta != tb) return ta < tb;
    }
    return false;
}

bool cardIsLess(std::pair<std::string, IntT> a, std::pair<std::string, IntT> b) {
    
    const auto tA = getType(a.first);
    const auto tB = getType(b.first);

    if (tA < tB) return true;
    if (tB < tA) return false;

    return isLess(a.first,b.first);
}

bool cardIsLess2(std::pair<std::string, IntT> a, std::pair<std::string, IntT> b) {

    const auto tA = getType2(a.first);
    const auto tB = getType2(b.first);

    if (tA < tB) return true;
    if (tB < tA) return false;

    return isLess2(a.first, b.first);
}

IntT sumCards(std::vector < std::pair<std::string, IntT> > const& cardList) {

    //IntT sum = 0;
    //for (auto const [i, card] : sv::enumerate(cardList)) {
    //    sum += card.second * (i + 1);
    //}
    //return sum;

    // Is this really better?     
    return sr::fold_left(sv::enumerate(cardList) | sv::transform([](auto x) {auto const [i, card] = x; return (i + 1) * card.second; }), IntT(0), std::plus<IntT>());
}

int main()
{
    const auto rawFile = loadFile("Data/aoc_input_7.txt");

    std::vector < std::pair<std::string,IntT> > cardList;
    for (auto const row : rawFile) {
        const auto split = stringToVector(row, ' ');
        cardList.emplace_back(split[0], toInt(split[1]));
    }

    sr::sort(cardList, cardIsLess);
    const auto result1 = sumCards(cardList);
    cout << "Total sum of scores for Part I : " << result1 << endl;
    assert(result1 == 250058342);

    // ---- Part 2 ---
    sr::sort(cardList, cardIsLess2);
    const auto result2 = sumCards(cardList);
    cout << "Total sum of scores for Part II: " << result2 << endl;
    assert(result2 == 250506580);
    
    return EXIT_SUCCESS; 
}
