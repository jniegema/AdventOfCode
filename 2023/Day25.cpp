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

IntT isParitioned(map<string, set<string>>const & con) {

	set<string> seen;

	stack<string> st;
	st.push(con.begin()->first);

	while (!st.empty()) {
		auto const cur = st.top();
		st.pop();
		
		seen.insert(cur);

		for (auto const & n : con.at(cur) ) {
			if (!seen.contains(n)) st.push(n);
		}
	}

	if (seen.size() == con.size()) {
		cout << "Not partitioned" << endl;
	}
	else {
		cout << "Partitioned" << endl;
	}
	 
	return seen.size();
}

pair<int, vector<int>> globalMinCut(vector<vector<int>> mat) {
	pair<int, vector<int>> best = { INT_MAX, {} };
	int n = mat.size();
	vector<vector<int>> co(n);

	for (int i = 0; i < n; i++)
		co[i] = { i };

	for (int ph = 1; ph < n; ph++) {
		vector<int> w = mat[0];
		size_t s = 0, t = 0;
		for (int it = 0; it < n - ph; it++) { // O(V^2) -> O(E log V) with prio. queue
			w[t] = INT_MIN;
			s = t, t = max_element(w.begin(), w.end()) - w.begin();
			for (int i = 0; i < n; i++) w[i] += mat[t][i];
		}
		best = min(best, { w[t] - mat[t][t], co[t] });
		co[s].insert(co[s].end(), co[t].begin(), co[t].end());
		for (int i = 0; i < n; i++) mat[s][i] += mat[t][i];
		for (int i = 0; i < n; i++) mat[i][s] = mat[s][i];
		mat[0][t] = INT_MIN;
	}

	return best;
}

int main25()
{
	auto rawFile = loadFile("Data/aoc_input_25t.txt");

	IntT sum1 = 0;
	IntT sum2 = 0;

	IntT nr = rawFile.size();
	IntT nc = rawFile[0].size();

	map<string, set<string>> con;
	for (auto const& row : rawFile) {
		auto const split = stringToVector(row, ':');
		auto const vv = stringToVector(trimString(split[1], " "), ' ');

		for (const auto v : vv) {
			con[split[0]].insert(v);
			con[v].insert(split[0]);

		}

	}
	map<string, IntT> lookup;
	IntT cnt = 0;
	for (auto const& s : con) {
		lookup[s.first] = cnt;
		cnt++;
	}

	auto const N = lookup.size();
	vector<vector<int>> mat(N, vector<int>(N, 0));
	for (auto const& s : con) {
		IntT i = lookup[s.first];

		for (auto const& s2 : s.second) {
			IntT j = lookup[s2];
			mat[i][j] = 1;
		}

	}

	auto const result = globalMinCut(mat);

	//auto const res1 = isParitioned(con);

	//// Cut the example connections
	//con["hfx"].erase("pzl");
	//con["pzl"].erase("hfx");

	//con["bvb"].erase("cmg");
	//con["cmg"].erase("bvb");
	//
	//con["nvd"].erase("jqt");
	//con["jqt"].erase("nvd");
	//
	//auto const res2 = isParitioned(con);

	const auto result1 = result.second.size() * ( N- result.second.size());
	cout << "Total sum of scores for Part I : " << result1 << endl;
	//assert(result1 == 409898);
	


	const auto result2 = sum2;

	cout << "Total sum of scores for Part II: " << result2 << endl;
	//assert(result2 == 113057405770956);
	 
	return EXIT_SUCCESS;
}
