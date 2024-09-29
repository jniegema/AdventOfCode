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

using PartT = map<char, IntT>;

struct MM {
	IntT mi;
	IntT ma;
};

 enum class Op {
	l,
	g
};

struct Rule {
	char var;
	Op   op;
	IntT val;
	string target;

	bool check(PartT p) const {
		if (op == Op::l) { return p[var] < val; }
		return p[var] > val;
	}

	std::array<MM, 4> apply(std::array<MM,4>& m) {
		std::array<MM, 4> complement = m;

		if (op == Op::l) {
			if (var == 'x') { m[0].ma = min(m[0].ma, val); complement[0].mi = max(complement[0].mi, val - 1); }
			if (var == 'm') { m[1].ma = min(m[1].ma, val); complement[1].mi = max(complement[1].mi, val - 1); }
			if (var == 'a') { m[2].ma = min(m[2].ma, val); complement[2].mi = max(complement[2].mi, val - 1); }
			if (var == 's') { m[3].ma = min(m[3].ma, val); complement[3].mi = max(complement[3].mi, val - 1); }
		}
		else {
			if (var == 'x') {m[0].mi = max(m[0].mi, val); complement[0].ma = min(complement[0].ma, val + 1);}
			if (var == 'm') {m[1].mi = max(m[1].mi, val); complement[1].ma = min(complement[1].ma, val + 1);}
			if (var == 'a') {m[2].mi = max(m[2].mi, val); complement[2].ma = min(complement[2].ma, val + 1);}
			if (var == 's') {m[3].mi = max(m[3].mi, val); complement[3].ma = min(complement[3].ma, val + 1);}
		}
		return complement;
	}


};

struct Rules {
	vector<Rule> rules;
	string final;

	string apply(PartT p) const {
		for (auto r : rules) {
			if (r.check(p)) return r.target;
		}
		return final;
	}
};


int main19()
{
	const auto rawFile = loadFile("Data/aoc_input_19.txt");

	IntT sum1 = 0;
	IntT sum2 = 0;


	IntT i = 0;
	map<string, Rules> rules;

	for (; i < rawFile.size();++i ) {
		auto row = rawFile[i];
		if (row.empty()) break;


		auto r1 = stringToVector(row, '{');
		auto name = r1[0];
		auto rls = stringToVector(trimString(r1[1],"}"), ',');
		
		Rules r;
		r.final = rls.back();
		rls.pop_back();
		for (auto rr : rls) {
			auto rr2 = stringToVector(rr, ':');
			auto target = rr2[1];

			auto rr3 = stringToVector(rr2[0], '<');
			if (rr3.size() == 2) {
				r.rules.emplace_back(rr3[0][0], Op::l, toInt(rr3[1]), target);
			}else {
				auto rr3 = stringToVector(rr2[0], '>');
				r.rules.emplace_back(rr3[0][0], Op::g, toInt(rr3[1]), target);
			}

		}
		rules[name] = r;
	}

	i++;
	vector< map<char, IntT> > parts;
	for (; i < rawFile.size(); ++i) {
		auto row = rawFile[i];
		auto pp = stringToVector(trimString(row, "{}"),',');
		
		map<char, IntT> part;
		for (auto const p : pp) {
			auto cp = stringToVector(p, '=');
			part[cp[0][0]] = toInt(cp[1]);
		}
		parts.push_back(part);
	}

	for (auto p : parts) {
		string status = "in";

		while (!((status == "R") || (status == "A"))) {
			status = rules[status].apply(p);
		}

		if (status == "A") {
			sum1 += p['x'] + p['m'] + p['a'] + p['s'];
		}
	}


	const auto result1 = sum1;
	cout << "Total sum of scores for Part I : " << result1 << endl;
	assert(result1 == 409898);
	

	// Find all paths that lead to A:
	vector<array<MM, 4>> paths;

	stack<pair<string,array<MM,4>> > s;
	array<MM, 4> start = { MM{0,4001}, MM{0,4001},MM{0,4001},MM{0,4001} };

	s.push({ "in",start });
	while (!s.empty()) {
		auto c = s.top();
		s.pop();

		// Look up all possible outcomes;
		auto r = rules[c.first];
		auto pro = c.second;
		for (auto rr : r.rules) {
			auto complement = rr.apply(pro);

			if (rr.target == "A") {
				paths.push_back(pro);
			} else if (rr.target != "R") {
				s.push({ rr.target ,pro });
			}
			pro = complement;
		}
		if (r.final == "A") {
			paths.push_back(pro);
		}else if (r.final != "R") {
			s.push({ r.final ,pro });
		}
	}

	IntT com = 0;
	for (auto p : paths) {
		IntT a = p[0].ma - p[0].mi - 1;
		IntT b = p[1].ma - p[1].mi - 1;
		IntT c = p[2].ma - p[2].mi - 1;
		IntT d = p[3].ma - p[3].mi - 1;

		com += a * b * c * d;
	}


	const auto result2 = com;
	cout << "Total sum of scores for Part II: " << result2 << endl;
	assert(result2 == 113057405770956);

	return EXIT_SUCCESS;
}

