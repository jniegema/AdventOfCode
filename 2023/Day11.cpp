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

using IntT = long long int;

bool isEmptyRow(IntT r, vector<string> const & m) {
	for (IntT i = 0; i < m[0].size(); ++i) {
		if (m[r][i] != '.') return false;
	}
	return true;
}

bool isEmptyCol(IntT c, vector<string> const& m) {
	for (IntT i = 0; i < m.size(); ++i) {
		if (m[i][c] != '.') return false;
	}
	return true;
}

void extendEmptyRowAndCols(IntT extend, vector<string> const & rawFile, vector<IntT>& rows, vector<IntT>& cols) {
	for (int j = 0; j < rawFile.size(); j++)
	{
		if (isEmptyRow(j, rawFile))
		{
			rows[j] = extend;
		}
	}

	const IntT numCols = rawFile[0].size();
	for (int j = 0; j < numCols; j++)
	{
		if (isEmptyCol(j, rawFile))
		{
			cols[j] = extend;
		}
	}
}

int main11()
{
	auto rawFile = loadFile("Data/aoc_input_11.txt");

	IntT sum1 = 0;
	IntT sum2 = 0;
	const IntT numRows = rawFile.size();
	const IntT numCols = rawFile[0].size();
	vector<IntT> rows(numRows, 1);
	vector<IntT> cols(numCols, 1);

	// Find Galaxies
	using PIntT = pair<IntT, IntT>;
	vector<PIntT> ga;
	for (int i = 0; i < rawFile.size(); ++i) {
		for (int j = 0; j < rawFile[0].size(); ++j) {
			if (rawFile[i][j] == '#') ga.emplace_back(i, j);
		}
    }

	extendEmptyRowAndCols(2, rawFile, rows, cols);

	for (int i = 0; i < ga.size(); ++i) {
		for (int j = 0; j < i; ++j) {

			auto g1x = min(ga[i].first, ga[j].first);
			auto g1y = max(ga[i].first, ga[j].first);
		
			auto g2x = min(ga[i].second, ga[j].second);
			auto g2y = max(ga[i].second, ga[j].second);
			
			auto const xDiff = reduce(next(begin(rows), g1x), next(begin(rows), g1y));
			auto const yDiff = reduce(next(begin(cols), g2x), next(begin(cols), g2y));
			const auto totl = abs(xDiff) + abs(yDiff);
			sum1 += totl;
		}
	}

	const auto result1 = sum1;
	cout << "Total sum of scores for Part I : " << result1 << endl;
	assert(result1 == 10173804);

	// Part II
	extendEmptyRowAndCols(1000000, rawFile, rows, cols);

	for (int i = 0; i < ga.size(); ++i) {
		for (int j = 0; j < i; ++j) {

			auto g1x = min(ga[i].first, ga[j].first);
			auto g1y = max(ga[i].first, ga[j].first);

			auto g2x = min(ga[i].second, ga[j].second);
			auto g2y = max(ga[i].second, ga[j].second);

			auto const xDiff = reduce(next(begin(rows), g1x), next(begin(rows), g1y));
			auto const yDiff = reduce(next(begin(cols), g2x), next(begin(cols), g2y));
			const auto totl = abs(xDiff) + abs(yDiff);
			sum2 += totl;
		}
	}

	const auto result2 = sum2;
	cout << "Total sum of scores for Part II: " << result2 << endl;
	assert(result2 == 634324905172);

	return EXIT_SUCCESS;
}

