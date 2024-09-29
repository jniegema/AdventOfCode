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
#include <iomanip>     
#include <print>// std::setw

using namespace std;

using IntT = long long int;
using PIntT = pair < IntT, IntT>;
using PartT = map<char, IntT>;

struct Pos {
	IntT x, y;
};
bool operator < (Pos a, Pos b) {
	return make_pair(a.x, a.y) < make_pair(b.x, b.y);
}
bool operator == (Pos a, Pos b) {
	return (a.x==b.x) && (a.y==b.y);
}

int main23()
{
	auto rawFile = loadFile("Data/aoc_input_23.txt");

	IntT sum1 = 0;
	IntT sum2 = 0;

	IntT nr = rawFile.size();
	IntT nc = rawFile[0].size();

	Pos s(1, 0);
	Pos e(nc-2, nr-1);

	stack<tuple<IntT,IntT,vector<Pos>>> st;
	map<Pos,IntT> seen;
	vector<vector<Pos>> solutions;

	vector<Pos> nodes;
	map<Pos,IntT> nodesMap;
	for (IntT r = 0; r < nr; ++r) {
		for (IntT c = 0; c < nc; ++c) {
			if (rawFile[r][c] != '#') {
				rawFile[r][c] = '.';
				nodes.emplace_back(c, r);
				nodesMap[{c, r}] = nodes.size() - 1;
			}
		}
	}


	vector<vector<pair<IntT, IntT>>> edges(nodes.size(), vector<pair<IntT, IntT>>());
	for (auto const [i,pos] : sv::enumerate(nodes)) {
		if ((pos.x < (nc - 1)) && (rawFile[pos.y][pos.x + 1] == '.')) edges[i].emplace_back(make_pair(nodesMap[{pos.x + 1, pos.y}],1 ));
		if ((pos.x > (0)) && (rawFile[pos.y][pos.x - 1] == '.')) edges[i].emplace_back(make_pair(nodesMap[{pos.x - 1, pos.y}],1));
		if ((pos.y < (nr - 1)) && (rawFile[pos.y + 1][pos.x] == '.')) edges[i].emplace_back(make_pair(nodesMap[{pos.x, pos.y+1}],1));
		if ((pos.y > 0) && (rawFile[pos.y - 1][pos.x] == '.')) edges[i].emplace_back(make_pair(nodesMap[{pos.x, pos.y-1}],1));
	}

	bool changed = true;
	while (changed) {
		changed = false;
		for (int i = 0; i < edges.size(); i++) {
			auto& curEdge = edges[i];

			if (curEdge.size() == 2) {
				auto const n1 = curEdge[0].first;
				auto const l1 = curEdge[0].second;
				auto const n2 = curEdge[1].first;
				auto const l2 = curEdge[1].second;

				for (auto& c : edges[n1]) {
					if (c.first == i) { c.first = n2; c.second = l1 + l2; }
				}
				for (auto& c : edges[n2]) {
					if (c.first == i) { c.first = n1; c.second = l1 + l2; }
				}
				curEdge.clear();
				changed = true;
			}
		}
	}

	map<IntT, vector<pair<IntT, IntT>>> edges2;// (nodes.size(), vector<pair<IntT, IntT>>());

	for (auto const [i, e] : sv::enumerate(edges)) {

		if (e.size() > 0) {
			edges2[i] = e;
		}
	}

	stack<tuple<IntT, vector<IntT>, IntT>> stx;
	vector<pair<vector<IntT>,IntT>> solutionsX;
	auto const ex = nodesMap[e];

	stx.push({ 0, {}, 0});
	IntT curBest = 0;
	while (!stx.empty()) {

		auto const c = stx.top();
		IntT const idx = get<0>(c);
		auto h = get<1>(c);
		IntT const len = get<2>(c);
		stx.pop();

		if (idx == ex) {
			if (len > curBest) {
				curBest = len;
				solutionsX.emplace_back(h, len);

				print("New best: {}\n", len);
				continue;
			}
		}

		if (sr::contains(h, idx)) continue;

		h.emplace_back(idx);

		for (const auto e : edges2[idx]) { stx.push({ e.first,h, len+e.second }); }
	}


	IntT maxLen = 0;
	for (auto const& sol : solutionsX)
	{
		maxLen = max(maxLen, sol.second);
	}
	const auto result1 = maxLen;
	cout << "Total sum of scores for Part I : " << result1 << endl;
	//assert(result1 == 409898);
	


	const auto result2 = sum2;

	cout << "Total sum of scores for Part II: " << result2 << endl;
 
	return EXIT_SUCCESS;
}
