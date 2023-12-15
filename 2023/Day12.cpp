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
#include <string_view> // C++17
using namespace std;

using IntT = long long int;

map<pair<string, vector<IntT>>, IntT> memo;

//
IntT getNumSolutions(string_view s, vector<IntT> const& group, IntT grpPos) { //}, string curStr) {
	const IntT N = s.size();

	// Minimum characters needed for all groups:
	const IntT sg = reduce(group.begin(), group.end()) + (group.size()-1); // 2nd term for the separator
	if (N < sg) {
		return 0; // If we have more groups than characters left
	}

	// No more groups
	if (group.empty()) {
		// Rest must be '.' or '?'
		if (s.contains('#')) {			return 0;}
		return 1;
	}

	// Use memoization
	auto const key = make_pair(string(s), group);
	auto const it = memo.find(key);
	if (it != memo.end()) return it->second;

	IntT sum = 0;
	// We can just trim a '.' (or a '?' which we interpret as a '.'
	if (s.starts_with(".") || s.starts_with("?")) {
		sum = getNumSolutions(s.substr(1, N - 1), group, grpPos);
	}

	if (s.starts_with("#") || s.starts_with("?")) {
		const IntT grpLen = group[0];
		const bool isAllowed = !(s.substr(0, grpLen).contains('.'));
		if (isAllowed) {
			// Two case: Group is exactly the rest of the string
			if (grpLen == N) {
				auto newGrp = group;
				newGrp.erase(newGrp.begin());
				sum += getNumSolutions("", newGrp, grpPos); // , curStr);

				//// If this was the last group then we have a solution
				//if (group.size() == 1) {
				//	std::cout << curStr << s << " WORKS!" << std::endl;
				//	return sum+1;
				//}
				//std::cout << curStr << s << " Fails (at least one group left)!" << std::endl;
				//return sum; // Otherwise not!
			}

			if( (grpLen < N) && (s[grpLen] != '#')) {		 // Group is shorter, then the next char can't be #
				auto newGrp = group;
				newGrp.erase(newGrp.begin());
				sum += getNumSolutions(s.substr(grpLen + 1, N - grpLen - 1), newGrp, grpPos);
			}
				
		}
	}

	memo[key] = sum; // Save for memoization
	return sum;
}



int main12()
{
	auto rawFile = loadFile("Data/aoc_input_12.txt");

	IntT sum1 = 0;
	IntT sum2 = 0;

	IntT cnt = 0;
	for (auto const row : rawFile) {
		cnt++;
		auto const split = stringToVector(row, ' ');
		auto s = split[0];
		auto const group = stringToVectorOfInt(split[1], ',');

		// Some pre-processing to potentially remove a few '?'
		if (s[0] == '#') {
			for (int i = 0; i < group[0]; ++i) {
				s[i] = '#';
			}
			s[group[0]] = '.';
		}

		if (s.back() == '#') {
			for (int i = 0; i < group.back(); ++i) {
				s[s.size()-i-1] = '#';
			}
			s[s.size()-group.back()-1] = '.';
		}

		const auto pt1 = getNumSolutions(s, group, 0);// , "");
		sum1 += pt1;
		//std::cout << "Cnt: " << cnt << " " << pt1 << " " << sum1 << endl;
	}


	const auto result1 = sum1;
	cout << "Total sum of scores for Part I : " << result1 << endl;
	assert(result1 == 7118);

	// Part II 
	cnt = 0;
	memo.clear();
	for (auto const row : rawFile) {
		cnt++;
		auto const split = stringToVector(row, ' ');
		auto const s0 = split[0];
		auto const g = stringToVectorOfInt(split[1], ',');

		// Extend for part II
		auto s = s0 + '?' + s0 + '?' + s0 + '?' + s0 + '?' + s0;
		auto group = g;
		group.insert(group.end(), g.begin(), g.end());
		group.insert(group.end(), g.begin(), g.end());
		group.insert(group.end(), g.begin(), g.end());
		group.insert(group.end(), g.begin(), g.end());

		// Some pre-processing to potentially remove a few '?'
		if (s[0] == '#') {
			for (int i = 0; i < group[0]; ++i) {
				s[i] = '#';
			}
			s[group[0]] = '.';
		}

		if (s.back() == '#') {
			for (int i = 0; i < group.back(); ++i) {
				s[s.size() - i - 1] = '#';
			}
			s[s.size() - group.back() - 1] = '.';
		}

		const auto pt2 = getNumSolutions(s, group, 0);// , "");
		sum2 += pt2;
		std::cout << "Cnt: " << cnt << " " << pt2 << " " << sum2 << endl;
	}

	const auto result2 = sum2;
	cout << "Total sum of scores for Part II: " << result2 << endl;
	assert(result2 == 7030194981795);

	return EXIT_SUCCESS;
}
