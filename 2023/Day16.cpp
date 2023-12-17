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
using PIntT = pair < IntT, IntT>;

struct Pos {
	IntT x;
	IntT y;
};


struct Ray {

	Pos pos;
	int dir;	// 0: right, 1:down, 2:left, 3:up
};

void propRay(Ray& r) {
	if (r.dir == 0) { r.pos.x++; }
	if (r.dir == 1) { r.pos.y++; }
	if (r.dir == 2) { r.pos.x--; }
	if (r.dir == 3) { r.pos.y--; }
	
	return;
}

bool isValid(Ray const & r, IntT nc, IntT nr) {
	if ((r.pos.x >=0) && (r.pos.x < nc) && (r.pos.y >= 0) && (r.pos.y < nr)) return true;
	return false;
}

void printMap(auto const& f) {
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

IntT propBeam(vector<string> const & rawFile, IntT xs, IntT ys, IntT dirs) {
	IntT nr = rawFile.size();
	IntT nc = rawFile[0].size();

	vector<vector<IntT>> visits(nr, vector<IntT>(nc, 0));

	Ray start;
	start.pos = { .x = xs, .y = ys };
	start.dir = dirs;

	stack<Ray> sr;
	sr.push(start);

	set<tuple<IntT, IntT, IntT>> beenHere;

	while (!sr.empty()) {
		auto curRay = sr.top();
		sr.pop();

		if (beenHere.contains(make_tuple(curRay.pos.x, curRay.pos.y, curRay.dir))) continue;


		// Mark as visited
		beenHere.insert(make_tuple(curRay.pos.x, curRay.pos.y, curRay.dir));
		visits[curRay.pos.y][curRay.pos.x]++;

		const auto curTile = rawFile[curRay.pos.y][curRay.pos.x];

		switch (curTile) {

		case '|':
			if ((curRay.dir == 0) || (curRay.dir == 2)) {
				auto newRay = curRay;
				curRay.dir = 1;
				propRay(curRay);
				if (isValid(curRay, nc, nr)) sr.push(curRay);

				newRay.dir = 3;
				propRay(newRay);
				if (isValid(newRay, nc, nr)) sr.push(newRay);
			}
			else {
				propRay(curRay);
				if (isValid(curRay, nc, nr)) sr.push(curRay);
			}

			break;

		case '-':
			if ((curRay.dir == 1) || (curRay.dir == 3)) {
				auto newRay = curRay;
				curRay.dir = 0;
				propRay(curRay);
				if (isValid(curRay, nc, nr)) sr.push(curRay);

				newRay.dir = 2;
				propRay(newRay);
				if (isValid(newRay, nc, nr)) sr.push(newRay);
			}
			else {
				propRay(curRay);
				if (isValid(curRay, nc, nr)) sr.push(curRay);
			}
			break;

		case '/':
			if (curRay.dir == 0) { curRay.dir = 3; }
			else if (curRay.dir == 1) { curRay.dir = 2; }
			else if (curRay.dir == 2) { curRay.dir = 1; }
			else if (curRay.dir == 3) { curRay.dir = 0; }
			propRay(curRay);
			if (isValid(curRay, nc, nr)) sr.push(curRay);
			break;

		case '\\':
			if (curRay.dir == 0) { curRay.dir = 1; }
			else if (curRay.dir == 1) { curRay.dir = 0; }
			else if (curRay.dir == 2) { curRay.dir = 3; }
			else if (curRay.dir == 3) { curRay.dir = 2; }
			propRay(curRay);
			if (isValid(curRay, nc, nr)) sr.push(curRay);
			break;

		default:
			propRay(curRay);
			if (isValid(curRay, nc, nr)) sr.push(curRay);
		}
		//printMap(visits);
	//	cout << endl;
	}


	IntT sum = 0;
	for (auto curRow : visits) {
		sum += sr::count_if(curRow, [](auto x) {return x > 0; });
	}
	return sum;
}

int main()
{
	const auto rawFile = loadFile("Data/aoc_input_16.txt");

	IntT sum1 = propBeam(rawFile,0,0,0);
	IntT sum2 = 0;

	const auto result1 = sum1;
	cout << "Total sum of scores for Part I : " << result1 << endl;
	assert(result1 == 7632);
	

	IntT nr = rawFile.size();
	IntT nc = rawFile[0].size();
	IntT maxEn = 0;
	for (IntT x = 0; x < nc; ++x) {
		maxEn = max(maxEn, propBeam(rawFile, x, 0, 1));
		maxEn = max(maxEn, propBeam(rawFile, x, nr-1, 3));
	}

	for (IntT y = 0; y < nr; ++y) {
		maxEn = max(maxEn, propBeam(rawFile, 0, y, 0));
		maxEn = max(maxEn, propBeam(rawFile, nc-1, y, 2));
	}

	const auto result2 = maxEn;
	cout << "Total sum of scores for Part II: " << result2 << endl;
	assert(result2 == 8023);

	return EXIT_SUCCESS;
}
