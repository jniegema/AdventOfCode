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
#include <queue>
#include <numeric>
#include <iomanip>      // std::setw

using namespace std;

using IntT = long long int;
using PIntT = pair < IntT, IntT>;

struct Pos {
	IntT x;
	IntT y;
	IntT dir; // <Direction we entered from
	IntT ns; // numStraights
	IntT h; // accumulated heat so far
};

struct Pos2 {
	IntT x;
	IntT y;
	IntT dir; // <Direction we entered from
	IntT ns; // numStraights

	bool operator < (Pos2 const & o) const {
		return make_tuple(x, y, dir, ns) < make_tuple(o.x, o.y, o.dir, o.ns);
	}
};

struct Pos3 {
	IntT x;
	IntT y;
	IntT dir; // <Direction we entered from
	IntT h; // accumulated heat so far

	bool operator < (Pos3 const& o) const {
		return make_tuple(x, y, dir,h) < make_tuple(o.x, o.y, o.dir,o.h);
	}
};


void printMap2(auto const& f) {
	auto nr = f.size();
	auto nc = f[0].size();
	for (int r = 0; r < (nr); ++r) {
		for (int c = 0; c < nc; ++c) {
			cout << setw(4) << f[r][c].first << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void printMap3(auto const& f) {
	auto nr = f.size();
	auto nc = f[0].size();
	for (int r = 0; r < (nr); ++r) {
		for (int c = 0; c < nc; ++c) {
			auto dir = f[r][c].second;
			if (dir ==-1) cout << "x";
			if(dir == 0) cout << ">";
			if(dir == 1) cout << 'v';
			if(dir == 2) cout << '<';
			if(dir == 3) cout << '^';
		}
		cout << endl;
	}
	cout << endl;
}

int main17()
{
	const auto rawFile = loadFile("Data/aoc_input_17.txt");

	const IntT nr = rawFile.size();
	const IntT nc = rawFile[0].size();

	IntT sum1 = 0;
	IntT sum2 = 0;

	Pos start1 = { .x = 1,.y = 0, .dir = 0, .ns = 1, .h = 0 };
	Pos start2 = { .x = 0,.y = 1, .dir = 1, .ns = 1, .h = 0 };



	stack<Pos> sr;
	//queue<Pos> sr;
	sr.push(start1);
	sr.push(start2);

	//Pos end1 = { .x = nc - 1,.y = nr - 1, .dir = 0, .ns = 1, .h = 3 };
	//Pos end2 = { .x = nc - 1,.y = nr - 1, .dir = 1, .ns = 1, .h = 3 };
	//sr.push(end1);
	//sr.push(end2);

	// Memoization
	map<Pos2, IntT> memo;

	IntT bestPath = 9999999999;
//	while (!sr.empty()) {
//		auto p = sr.top(); //.front(); // 
//		sr.pop();
//
//		if (p.ns >= 4) continue;
//		
//		if (auto it = memo.find(Pos2(p.x, p.y, p.dir, p.ns)); it != memo.end()) {
//			if (it->second <= p.h) {
//				continue;
//			}
//		}
//		memo[Pos2(p.x, p.y, p.dir, p.ns)] = p.h;
//
//		// As soon as we are worse than the best path, we stop
//
//		IntT x, y;
//		x = p.x;
//		y = p.y;
//
//		const auto curHeat = rawFile[y][x] - 48;
//		const auto newHeat = p.h + curHeat;
//
//		if (newHeat > bestPath) continue;
//
//		if ((x == (nc - 1)) && (y == (nr - 1))) {
//			cout << "Reached goal with heat: " << newHeat << " " << bestPath << endl;
//			bestPath = min(bestPath, newHeat);
//			continue;
//		}
//
//		// Check if we have been here before and with less accumulated heat ... then we can stop exploring this path!
////		if (hl[y][x].first <= newHeat) continue;
////		hl[y][x] = make_pair(newHeat,p.dir);
//
//		//
//
//		if ((x > 0) && (p.dir != 0))        sr.push(Pos(x-1, y, 2, (p.dir == 2) ? (p.ns + 1) : 1, p.h = newHeat));
//		if ((y > 0) && (p.dir != 1))        sr.push(Pos(x, y-1, 3, (p.dir == 3) ? (p.ns + 1) : 1, p.h = newHeat));
//		if ((x < (nc - 1)) && (p.dir != 2)) sr.push(Pos(x + 1, y, 0, (p.dir == 0) ? (p.ns + 1) : 1, p.h = newHeat));
//		if ((y < (nr - 1)) && (p.dir != 3)) sr.push(Pos(x, y + 1, 1, (p.dir == 1) ? (p.ns + 1) : 1, p.h = newHeat));
//
//	}


	const auto result1 = bestPath;
	cout << "Total sum of scores for Part I : " << result1 << endl;
	//assert(result1 == 791);
	
	Pos3 end1 = { .x = nc - 1, .y = nr - 1, .dir = 1, .h = 0 };
	Pos3 end2 = { .x = nc - 1, .y = nr - 1, .dir = 0, .h = 0 };

	vector<vector<PIntT>> vmap(nr, vector<PIntT>(nc, make_pair(99999, 0)));
	vector<vector<PIntT>> hmap(nr, vector<PIntT>(nc, make_pair(99999, 0)));
	queue<Pos3> sr2;

	sr2.push(end1);
	sr2.push(end2);
	while (!sr2.empty()) {
		auto p = sr2.front();
		sr2.pop();

		const IntT x = p.x;
		const IntT y = p.y;

		// Arrived vertically
		if ((p.dir == 0) || (p.dir == 2)) {

			IntT heat0 = 0;
			IntT heat2 = 0;
			for (int i = 0; i <= 10; ++i) {
				if (x >= i) {
					if (i >= 4) {
						auto const newHeat = p.h + heat2;
						if (vmap[y][x - i].first > newHeat) {
							vmap[y][x - i] = make_pair(newHeat, i);
							sr2.push(Pos3(x - i, y, 1, newHeat));
						}
					}
					heat2 += rawFile[y][x - i] - 48;
				}

				if (x < (nc - i)) {
					if (i >= 4) {
						if (vmap[y][x + i].first > (p.h + heat0)) {
							vmap[y][x + i] = make_pair(p.h + heat0, i);
							sr2.push(Pos3(x + i, y, 1, p.h + heat0));
						}
					}
					heat0 += rawFile[y][x + i] - 48;
				}
			}
		}

		// Arrived horizontally
		if ((p.dir == 1) || (p.dir == 3)) {
			IntT heat1 = 0;
			IntT heat3 = 0;
			for (int i = 0; i <= 10; ++i) {
				
				if (y >= i) {
					if (i >= 4) {
						if (hmap[y - i][x].first > (p.h + heat3)) {
							hmap[y - i][x] = make_pair(p.h + heat3, i);
							sr2.push(Pos3(x, y - i, 0, p.h + heat3));
						}
					}
					heat3 += rawFile[y - i][x] - 48;
				}

				if (y < (nr - i)) {
					if (i >= 4) {
						if (hmap[y+i][x].first > (p.h + heat1)) {
							hmap[y+i][x] = make_pair(p.h + heat1, i);
							sr2.push(Pos3(x, y + i, 0, p.h + heat1));
						}
					}
					heat1 += rawFile[y + i][x] - 48;
				}

			}
		}
		//printMap2(hmap);
		//printMap2(vmap);
	}
	 

	const auto result2 = min(hmap[0][0].first,vmap[0][0].first);
	cout << "Total sum of scores for Part II: " << result2 << endl;
	assert(result2 == 900);

	return EXIT_SUCCESS;
}
	

