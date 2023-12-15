#include "FileIO.h"
#include "ContainerHelpers.h"
#include "StringToVector.h"

#include <iostream>
#include <set>
#include <algorithm>
#include <ranges>
#include <numeric>
#include <cassert>
#include <cctype>
#include <regex>
#include <map>
#include <unordered_map>
#include <stack>
#include <numeric>

using namespace std;

using IntT = long long int;


IntT hashf(string const& st) {
	return sr::fold_left(st, 0, [](auto a, auto b) {return ((a + b) * 17) % 256; });
}

int main()
{
	const auto rawFile = loadFile("Data/aoc_input_15.txt");

	IntT sum1 = 0;
	IntT sum2 = 0;

	IntT cnt = 0;
	IntT start = 0;
	IntT nr = rawFile.size();
	IntT nc = rawFile[0].size();

	auto f = rawFile;

	// Testing the hash function
	auto const st = string("HASH");
	assert(hashf(st) == 52);
		
	auto const lst = stringToVector(rawFile[0], ',');
	sum1 = sr::fold_left(lst | sv::transform(hashf), 0, plus<IntT>());

	const auto result1 = sum1;
	cout << "Total sum of scores for Part I : " << result1 << endl;
	assert(result1 == 505379);
	
	vector<vector<pair<string, IntT>>> boxes(256);

	for (auto const op : lst) {
		if (op.contains('-')) {
			auto const label = trimString(op, '-');
			auto const box = hashf(label);
			if (auto it = sr::find_if(boxes[box], [label](auto const& x) {return x.first == label; }); it!=boxes[box].end()) {
				boxes[box].erase(it);
			}
		}

		if (op.contains('=')) {
			auto const str2 = stringToVector(op, '=');
			auto const label = str2[0];
			IntT const num = toInt(str2[1]);
			auto const box = hashf(label);
			if (auto it = sr::find_if(boxes[box], [label](auto const& x) {return x.first == label; }); it != boxes[box].end()) {
				it->second = num;
			} else {
				boxes[box].emplace_back(label, num);
			}
		}
	}

	// Score
	sum2 = 0;
	for (IntT boxId = 0; boxId < boxes.size(); boxId++) {
		for (IntT slotId = 0; slotId < boxes[boxId].size(); ++slotId) {
			sum2 += (boxId + 1) * (slotId+1) * boxes[boxId][slotId].second;
		}
	}

	const auto result2 = sum2;
	cout << "Total sum of scores for Part II: " << result2 << endl;
	assert(result2 == 263211);

	return EXIT_SUCCESS;
}
