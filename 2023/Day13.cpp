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

bool isMirrorCol(IntT start, IntT numRows, IntT colPivot, vector<string> const& data) {
	IntT numCols = data[start].size();
	IntT numMirrorCols = min(colPivot, numCols - colPivot);
	bool isMirror = true;
	bool foundDifference = false;
	PIntT diffPos;
	for (IntT j = 0; j < numMirrorCols; ++j) {
	//	std::cout << "Comparing cols " << (colPivot - j - 1) << " and " << (colPivot + j) << endl;
		for (IntT i = 0; i < numRows; ++i) {
			if (data[start + i][(colPivot - j - 1)] != data[start + i][(colPivot + j)]) {
				if (foundDifference) return false; //< More than one difference
				diffPos = make_pair(i,j);
				foundDifference = true;
				//return false; //isMirror = false;
			}// 
		}
	}
	return isMirror;
}

PIntT isMirrorCol2(IntT start, IntT numRows, IntT colPivot, vector<string> const& data) {
	IntT numCols = data[start].size();
	IntT numMirrorCols = min(colPivot, numCols - colPivot);
	bool isMirror = true;
	bool foundDifference = false;
	PIntT diffPos(-1,-1);
	for (IntT j = 0; j < numMirrorCols; ++j) {
		//	std::cout << "Comparing cols " << (colPivot - j - 1) << " and " << (colPivot + j) << endl;
		for (IntT i = 0; i < numRows; ++i) {
			if (data[start + i][(colPivot - j - 1)] != data[start + i][(colPivot + j)]) {
				if (foundDifference) return make_pair(-1,-1); //< More than one difference
				diffPos = make_pair(i, j);
				foundDifference = true;
				//return false; //isMirror = false;
			}// 
		}
	}
	return diffPos;
}


bool isMirrorRow(IntT start, IntT numRows, IntT pivot, vector<string> const & data) {
	IntT numCols = data[start].size();
	IntT numMirrorRows = min(pivot, numRows - pivot);
	bool isMirror = true;
	for (IntT i = 0; i < numMirrorRows; ++i) {
		//std::cout << "Comparing rows " << start + (pivot - i - 1) << " and " << start + (pivot + i) << endl;
		for (IntT j = 0; j < numCols; ++j) {	
			if (data[start + (pivot - i - 1)][j] != data[start + (pivot + i)][j]) {
				return false; //isMirror = false;
			}// return false;
		}
	}
	return isMirror;
}

PIntT isMirrorRow2(IntT start, IntT numRows, IntT pivot, vector<string> const& data) {
	IntT numCols = data[start].size();
	IntT numMirrorRows = min(pivot, numRows - pivot);
	bool isMirror = true;
	PIntT diffPos(-1, -1);
	for (IntT i = 0; i < numMirrorRows; ++i) {
		//std::cout << "Comparing rows " << start + (pivot - i - 1) << " and " << start + (pivot + i) << endl;
		for (IntT j = 0; j < numCols; ++j) {
			if (data[start + (pivot - i - 1)][j] != data[start + (pivot + i)][j]) {
				if(!isMirror) return make_pair(-1,-1); //isMirror = false;
				diffPos = make_pair(i, j);
				isMirror = false;
			}// return false;
		}
	}
	return diffPos;
}

int main13()
{
	auto rawFile = loadFile("Data/aoc_input_13.txt");

	IntT sum1 = 0;
	IntT sum2 = 0;

	IntT cnt = 0;
	vector<PIntT> patterns;
	IntT start = 0;
	for (int i = 0; i < rawFile.size();++i) {
		auto const row = rawFile[i];
		if (row.empty()) {
			patterns.emplace_back(start, i);
			start = i + 1;
		}
	}
	patterns.emplace_back(start, rawFile.size());

	IntT colSum = 0;
	IntT rowSum = 0;
	for (auto const [start, end] : patterns) {
		//cout << "Testing pattern from " << start << " to " << end << endl;
		// Try along cols:
		IntT numRows = end - start;
		IntT numCols = rawFile[start].size();
		for (IntT colPivot = 1; colPivot < numCols; ++colPivot) {
			//cout << "Testing Column Pivot " << colPivot << endl;
			if (isMirrorCol(start, numRows, colPivot, rawFile)) {
				//cout << "  Col mirror at " << colPivot << endl;
				colSum += colPivot;
			}
			else {
				//cout << "  Not a col mirror !" << endl;

			}
		}

		// Try along rows:
		for (IntT rowPivot = 1; rowPivot < numRows; ++rowPivot) {
			//cout << "Testing Pivot " << rowPivot << endl;
			if( isMirrorRow(start, numRows, rowPivot, rawFile) ){
				cout << "  Row mirror at " << rowPivot << endl;
				rowSum += rowPivot*100;
			}
			else {
				//cout << "  Not a row mirror !" << endl;

			}

		}

	}


	const auto result1 = colSum+rowSum;
	cout << "Total sum of scores for Part I : " << result1 << endl;
	assert(result1 == 34821);


	colSum = 0;
	rowSum = 0;
	for (auto const [start, end] : patterns) {
		cout << "Testing pattern from " << start << " to " << end << endl;
		// Try along cols:
		IntT numRows = end - start;
		IntT numCols = rawFile[start].size();
		for (IntT colPivot = 1; colPivot < numCols; ++colPivot) {
			//cout << "Testing Column Pivot " << colPivot << endl;
			auto diffPos = isMirrorCol2(start, numRows, colPivot, rawFile);
			if ((diffPos.first)!=-1) {
				cout << "  Almost Col mirror at " << colPivot << " with difference at " << diffPos.first << " " << diffPos.second << endl;
				colSum += colPivot;
			}
			else {
				//cout << "  Not a col mirror !" << endl;

			}
		}

		// Try along rows:
		for (IntT rowPivot = 1; rowPivot < numRows; ++rowPivot) {
			cout << "Testing Pivot " << rowPivot << endl;
			auto diffPos = isMirrorRow2(start, numRows, rowPivot, rawFile);
			if ((diffPos.first) != -1) {
				cout << "  Almost row mirror at " << rowPivot << " with difference at " << diffPos.first << " " << diffPos.second << endl;
				rowSum += rowPivot * 100;
			}
			else {
				//cout << "  Not a row mirror !" << endl;

			}

		}

	}


	const auto result2 = colSum + rowSum;
	cout << "Total sum of scores for Part II: " << result2 << endl;
	assert(result2 == 36919);

	return EXIT_SUCCESS;
}
