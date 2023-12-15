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
using PIntT = pair<IntT, IntT>;

void printMap(auto const & f) {
	auto nr = f.size();
	auto nc = f[0].size();
	for (int r = 0; r < (nr); ++r) {
		for (int c = 0; c < nc; ++c) {
			cout << f[r][c];
		}
		cout << endl;
	}
	cout << endl;
}

void north(auto& f) {
	const auto nr = f.size();
	const auto nc = f[0].size();
	bool change = true;
	while (change) {
		change = false;
		for (int c = 0; c < nc; ++c) {
			for (int r = 0; r < (nr - 1); ++r) {
				if ((f[r][c] == '.') && (f[r + 1][c] == 'O')) {
					f[r][c] = 'O';
					f[r + 1][c] = '.';
					change = true;
				}
			}
		}
	};
}

void west(auto& f) {
	bool change = true;
	auto nr = f.size();
	auto nc = f[0].size();
	while (change) {
		change = false;
		for (int r = 0; r < nr; ++r) {
			for (int c = 0; c < (nc-1); ++c) {
				if ((f[r][c] == '.') && (f[r][c+1] == 'O'))
				{
					f[r][c] = 'O';
					f[r][c+1] = '.';
					change = true;
				}
			}
		}
	};
}

void south(auto& f) {
	bool change = true;
	int nr = f.size();
	int nc = f[0].size();
	while (change) {
		change = false;
		for (int c = 0; c < nc; ++c) {
			for (int r = (nr - 1); r > 0 ; r--) {
				if ((f[r][c] == '.') && (f[r - 1][c] == 'O'))
				{
					f[r][c] = 'O';
					f[r - 1][c] = '.';
					change = true;
				}
			}
		}
	};
}

void east(auto& f) {
	bool change = true;
	auto nr = f.size();
	auto nc = f[0].size();
	while (change) {
		change = false;
		for (int r = 0; r < nr; ++r) {
			for (int c = (nc - 1); c>0; c--) {
				if ((f[r][c] == '.') && (f[r][c - 1] == 'O'))
				{
					f[r][c] = 'O';
					f[r][c - 1] = '.';
					change = true;
				}
			}
		}
	};
}

bool equal(auto const& f1, auto const& f2) {
	auto nr = f1.size();
	auto nc = f1[0].size();
	for (int r = 0; r < nr; ++r) {
		for (int c = 0; c < nc; c++) {
			if (f1[r][c] != f2[r][c]) return false;
		}
	}
	return true;
}

int main14()
{
	const auto rawFile = loadFile("Data/aoc_input_14.txt");

	IntT sum1 = 0;
	IntT sum2 = 0;

	IntT cnt = 0;
	IntT start = 0;
	IntT nr = rawFile.size();
	IntT nc = rawFile[0].size();

	auto f = rawFile;
	north(f);
	// Score
	for (int r = 0; r < (nr); ++r) {
		for (int c = 0; c < nc; ++c) {
			if (f[r][c] == 'O') {
				//cout << "Rock in row " << r << " " << (nr - r)  << " pts" << endl;
				sum1 += (nr - r);
			}
		}
	}
	const auto result1 = sum1;
	cout << "Total sum of scores for Part I : " << result1 << endl;
	assert(result1 == 108614);

	f = rawFile;
	vector<vector<string>> ll;
	bool cycle_found = false;
	IntT cycle = 0;
	IntT cycle_length = 0;
	IntT cycle_start = 0;
	while (!cycle_found ) {
		ll.push_back(f);

		north(f);
		west(f);
		south(f);
		east(f);

		cycle++;

		IntT sumX = 0;
		for (int r = 0; r < (nr); ++r) {
			for (int c = 0; c < nc; ++c) {
				if (f[r][c] == 'O') {
					//cout << "Rock in row " << r << " " << (nr - r)  << " pts" << endl;
					sumX += (nr - r);
				}
			}
		}
		cout << cycle << " " << sumX ;

		// Compare against all previous to check if we have a cycle
		for (IntT c1 = 0; c1 < IntT(ll.size()); ++c1) {
			if (equal(f, ll[c1])) {
				cycle_found = true;
				cycle_start = c1;
				cycle_length = cycle - c1;
				cout << " -> Cycle detected (equivalent to after " << c1 <<") : " << cycle_start << " " << cycle_length << endl;
			}
		}
		cout << endl;
	}
	IntT targetCycles = 1000000000;

	IntT tt = targetCycles;
	while (tt > cycle) { tt -= cycle_length; }
	cout << "tt=" << tt << endl;

	auto finalConfig = tt;// cycle_start + (targetCycles % cycle_length);
	cout << "Should be equivalent to after cycle " << finalConfig << endl;
	f = ll[finalConfig];
	for (int r = 0; r < (nr); ++r) {
		for (int c = 0; c < nc; ++c) {
			if (f[r][c] == 'O') {
				//cout << "Rock in row " << r << " " << (nr - r)  << " pts" << endl;
				sum2 += (nr - r);
			}
		}
	}
	const auto result2 = sum2;
	cout << "Total sum of scores for Part II: " << result2 << endl;
	assert(result2 == 96447);

	return EXIT_SUCCESS;
}
