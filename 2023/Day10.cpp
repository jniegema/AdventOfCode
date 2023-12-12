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
#include <numeric>
using namespace std;

using IntT  = long long int;
using PIntT = pair<IntT, IntT>;

PIntT nextStep(vector<string> const& m, vector<PIntT> const& path) {
	auto cu = path.back();
	auto pr = *prev(path.end(), 2);
	auto ir = cu.first;
	auto ic = cu.second;
	auto cur = m[ir][ic];

	if ((pr.first == cu.first) && ((pr.second + 1) == cu.second)) {
		if (cur == '-') return make_pair(ir, ic + 1);
		if (cur == '7') return make_pair(ir + 1, ic);
		if (cur == 'J') return make_pair(ir - 1, ic);
	}

	if ((pr.first == cu.first) && ((pr.second - 1) == cu.second)) {
		if (cur == '-') return make_pair(ir, ic - 1);
		if (cur == 'F') return make_pair(ir + 1, ic);
		if (cur == 'L') return make_pair(ir - 1, ic);
	}

	// From to to bottom
	if (((pr.first + 1) == cu.first) && ((pr.second) == cu.second)) {
		if (cur == '|') return make_pair(ir + 1, ic);
		if (cur == 'L') return make_pair(ir, ic + 1);
		if (cur == 'J') return make_pair(ir, ic - 1);
	}

	if (((pr.first - 1) == cu.first) && ((pr.second) == cu.second)) {
		if (cur == '|') return make_pair(ir - 1, ic);
		if (cur == '7') return make_pair(ir, ic - 1);
		if (cur == 'F') return make_pair(ir, ic + 1);
	}
	assert(false);
	return make_pair(0,0);
}

int main10()
{
	const auto rawFile = loadFile("Data/aoc_input_10.txt");
	const auto m = rawFile;

	IntT sum1 = 0;
	IntT sum2 = 0;

	// Find the starting point
	PIntT start;
	for (int j = 0; j < rawFile.size(); j++)
	{
		auto row = rawFile[j];
		for (int i = 0; i < row.size(); ++i) {
			if (row[i] == 'S') {
				start = make_pair(j, i);
			}
		}
	}

	vector<PIntT> path1, path2;
	path1.emplace_back(start);
	path2.emplace_back(start);

	// To right:   -,7,J
	// To left:    -,L,F
	// To top:     |,7,F
	// To bottom:  |,L,J

	auto const ir = start.first; // Row
	auto const ic = start.second; // Col
	vector<pair<IntT, IntT>> paths;

	bool toR = false;
	bool toL = false;
	bool toT = false;
	bool toB = false;

	if (ic < m[0].size() - 1) {
		auto const chR = m[ir][ic + 1];
		if ((chR == '-') || (chR == '7') || (chR == 'J')) {
			paths.emplace_back(ir, ic + 1);
			toR = true;
		}
	}

	if (ic > 0) {
		auto const chL = m[ir][ic - 1];
		if ((chL == '-') || (chL == 'L') || (chL == 'F')) {
			paths.emplace_back(ir, ic - 1);
			toL = true;
		}
	}

	if (ir > 0) {
		auto const chT = m[ir - 1][ic];
		if ((chT == '|') || (chT == '7') || (chT == 'F')) {
			paths.emplace_back(ir - 1, ic);
			toT = true;
		}
	}
	if (ir < (m.size() - 1)) {
		auto const chB = m[ir + 1][ic];
		if ((chB == '|') || (chB == 'L') || (chB == 'J')) {
			paths.emplace_back(ir + 1, ic);
			toB = true;
		}
	}

	path1.emplace_back(paths[0]);
	path2.emplace_back(paths[1]);


	// What is the start symbol?
	char startSym = 'S';
	if (toL && toR) startSym = '-';
	if (toT && toB) startSym = '|';
	if (toT && toR) startSym = 'L';
	if (toT && toL) startSym = 'J';
	if (toB && toR) startSym = 'F';
	if (toB && toL) startSym = '7';


	auto nextStep1 = nextStep(m, path1);
	path1.emplace_back(nextStep1);

	auto nextStep2 = nextStep(m, path2);
	path2.emplace_back(nextStep2);

	while (nextStep1 != nextStep2) {
		nextStep1 = nextStep(m, path1);
		path1.emplace_back(nextStep1);
		nextStep2 = nextStep(m, path2);
		path2.emplace_back(nextStep2);
	}

	const auto result1 = path1.size() - 1;
	cout << "Total sum of scores for Part I : " << result1 << endl;
	assert(result1 == 6812);


	// Part II
	vector<PIntT> fullPoly = path1;
	auto nextStep3 = nextStep(m, fullPoly);
	while (nextStep3 != fullPoly.front()) {
		fullPoly.emplace_back(nextStep3);
		nextStep3 = nextStep(m, fullPoly);
	}

	sum2 = 0;
	set<PIntT> mm(fullPoly.begin(), fullPoly.end());

	auto m2 = m;
	for (int i = 0; i < m.size(); ++i) {
		for (int j = 0; j < m[0].size(); ++j) {
			bool isOnPoly = mm.contains(make_pair(i, j));
			if (!isOnPoly) m2[i][j] = '.';
		}
	}

	// Need to back-substitute the start symbol 'S'
	m2[start.first][start.second] = startSym;

	for (int i = 0; i < m.size(); ++i) {
		bool inside = false;
		bool hadL = false;
		bool hadF = false;
		for (int j = 0; j < m[0].size(); ++j) {

			const bool isOnPoly = mm.contains(make_pair(i, j));
			const char curChar = m2[i][j];
			if (!isOnPoly) {
				if (inside) {
					m2[i][j] = 'I';
					sum2++;
				}
				continue;
			}

			if (curChar == '|') { inside = !inside; continue; }

			if ((curChar == 'F')) { hadF = true; continue; }
			if (hadF && (curChar == 'J')) { hadF = false; inside = !inside; continue; }
			if (hadF && (curChar == '7')) { hadF = false; continue; }


			if ((curChar == 'L')) { hadL = true; continue; }
			if (hadL && (curChar == '7')) { hadL = false; inside = !inside; continue; }
			if (hadL && (curChar == 'J')) { hadL = false; continue; }
		}
	}

	// Print the cleaned map?
	//for (int i = 0; i < m.size(); ++i) {
	//    for (int j = 0; j < m[0].size(); ++j) {
	//        cout << m2[i][j];
	//    }
	//    cout << endl;
	//}

	const auto result2 = sum2;
	cout << "Total sum of scores for Part II: " << result2 << endl;
	assert(result2 == 527);

	return EXIT_SUCCESS;
}

