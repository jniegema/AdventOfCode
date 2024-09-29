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
 
// GetNum
int getNumTiles(vector<vector<IntT>> const &  neighbors, IntT sIdx, IntT steps) {

	const IntT N = neighbors.size();
	vector<IntT> numVis1(N, 0);
	numVis1[sIdx] = 1;

	vector<IntT> numVis2(N, 0);

	for (IntT step = 0; step < steps; ++step) {

		for (IntT i = 0; i < N; ++i) {
			numVis2[i] = 0;
			if (sr::any_of(neighbors[i], [numVis1](auto i) { return numVis1[i] > 0; })) { numVis2[i] = 1; }
		}
		swap(numVis1, numVis2);
	}

	return reduce(begin(numVis1), end(numVis1));
}

int getNumTiles2(vector<string> const& rawFile, pair<IntT, IntT> s, IntT steps) {
	const IntT nr = rawFile.size();
	const IntT nc = rawFile[0].size();
	set<PIntT> result;
	stack<tuple<IntT, IntT, IntT>> st;
	st.push({ s.first, s.second, 0 });

	set<tuple<IntT, IntT, IntT>> seen;
	set<tuple<IntT, IntT>> seen2;
	while (!st.empty()) {
		auto const cur = st.top();
		st.pop();

		auto const r = get<0>(cur);
		auto const c = get<1>(cur);
		auto const step = get<2>(cur);

		if (step == steps) {
			result.insert({ r,c });
			continue;
		}

		if (seen.contains({ r,c,step })) { continue; } else { seen.insert({ r,c,step }); }
		//if (step % 2 == 0) {
		//	if (seen2.contains({ r,c })) { continue; }
		//	else { seen2.insert({ r,c }); }
		//}

		IntT rr = ((r)+(abs((r) / nr) + 2) * nr) % nr;
		IntT cc = ((c)+(abs((c) / nc) + 2) * nc) % nc;
		IntT rp1 = ((r + 1) + (abs((r + 1) / nr) + 2) * nr) % nr;
		IntT rm1 = ((r - 1) + (abs((r - 1) / nr) + 2) * nr) % nr;
		IntT cp1 = ((c + 1) + (abs((c + 1) / nc) + 2) * nc) % nc;
		IntT cm1 = ((c - 1) + (abs((c - 1) / nc) + 2) * nc) % nc;

		if (rawFile[rp1][cc] == '.') st.push({ r + 1,c,step + 1 });
		if (rawFile[rm1][cc] == '.') st.push({ r - 1,c,step + 1 });
		if (rawFile[rr][cp1] == '.') st.push({ r,c + 1,step + 1 });
		if (rawFile[rr][cm1] == '.') st.push({ r,c - 1,step + 1 });
	}
	return result.size();
}



int main()
{
	auto rawFile = loadFile("Data/aoc_input_21.txt");

	IntT sum1 = 0;
	IntT sum2 = 0;
	
	IntT nr = rawFile.size();
	IntT nc = rawFile[0].size();

	PIntT s;
	IntT sIdx;
	vector<PIntT> garden;
	map<PIntT,IntT> gardenLookup;

	for (IntT r = 0; r < nr; ++r) {
		for (IntT c = 0; c < nc; ++c) {
			auto ch = rawFile[r][c];

			if (ch == 'S') {
				s = { r,c }; rawFile[r][c] = '.';
				sIdx = garden.size();
			}
			
			if ((ch == 'S') || (ch == '.')) {
				garden.emplace_back(r,c);
				gardenLookup[{r, c}] = garden.size() - 1;
			}
		}

	}

	vector<IntT> numNei(garden.size(), 0);
	vector< vector<IntT>> neighbors(garden.size(),vector<IntT>());
	vector< vector<IntT>> neighbors2(garden.size(), vector<IntT>());
	
	//for (auto const [r, c] : garden) {
	for (IntT i = 0; i < garden.size(); ++i){
		IntT r = garden[i].first;
		IntT c = garden[i].second;

		IntT rr = ((r)+(abs((r) / nr) + 2) * nr) % nr;
		IntT cc = ((c)+(abs((c) / nc) + 2) * nc) % nc;
		IntT rp1 = ((r + 1) + (abs((r + 1) / nr) + 2) * nr) % nr;
		IntT rm1 = ((r - 1) + (abs((r - 1) / nr) + 2) * nr) % nr;
		IntT cp1 = ((c + 1) + (abs((c + 1) / nc) + 2) * nc) % nc;
		IntT cm1 = ((c - 1) + (abs((c - 1) / nc) + 2) * nc) % nc;

		IntT num = 0;
	/*	if (rawFile[rp1][cc] == '.') {num++;  neighbors[i].push_back(gardenLookup[{rp1, cc}]); }
		if (rawFile[rm1][cc] == '.') { num++;  neighbors[i].push_back(gardenLookup[{rm1, cc}]); }
		if (rawFile[rr][cp1] == '.') { num++;  neighbors[i].push_back(gardenLookup[{rr, cp1}]); }
		if (rawFile[rr][cm1] == '.') { num++;  neighbors[i].push_back(gardenLookup[{rr, cm1}]); }*/
		
		
		if ((r<(nr-1)) && (rawFile[r + 1][c] == '.')) { num++;  neighbors[i].push_back(gardenLookup[{r+1, c}]); }
		if ((r >0) && rawFile[r - 1][c] == '.') { num++;  neighbors[i].push_back(gardenLookup[{r-1, c}]); }
		if ((c < (nc - 1)) && rawFile[r][c+1] == '.') { num++;  neighbors[i].push_back(gardenLookup[{r, c+1}]); }
		if ((c > 0) && rawFile[r][c-1] == '.') { num++;  neighbors[i].push_back(gardenLookup[{r, c-1}]); }

		if ((r == (nr - 1)) && (rawFile[0][c] == '.')) { num++;  neighbors2[i].push_back(gardenLookup[{0, c}]); }
		if ((r == 0) && rawFile[nr - 1][c] == '.') { num++;  neighbors2[i].push_back(gardenLookup[{nr - 1, c}]); }
		if ((c == (nc - 1)) && rawFile[r][0] == '.') { num++;  neighbors2[i].push_back(gardenLookup[{r, 0}]); }
		if ((c == 0) && rawFile[r][nc - 1] == '.') { num++;  neighbors2[i].push_back(gardenLookup[{r, nc - 1}]); }


		numNei[i]=num;
	}

	//	vector<IntT> numVis1a(garden.size(), 0);
//	vector<IntT> numVis1b(garden.size(), 0); //< The odd panels
//	numVis1a[sIdx] = 1;
//
//	vector<IntT> numVis2a(garden.size(), 0);
//	vector<IntT> numVis2b(garden.size(), 0);
//
//	
//	IntT maxMax = 20;// 26501365;
//	for (IntT maxSteps = 1; maxSteps < maxMax; ++maxSteps) {
//		const IntT tiles = getNumTiles(neighbors, sIdx, maxSteps);
//		print("{}\n", tiles);
//	}
//
//	for (IntT maxSteps = 1; maxSteps < maxMax; ++maxSteps) {
//
//		for (IntT i = 0; i < garden.size(); ++i) {
//				numVis2a[i] = 0;
//				numVis2b[i] = 0;
//
//				IntT maxVa = 0;
//				IntT maxVb = 0;
//				for (auto const nei : neighbors[i]) {
//					maxVa = max(maxVa, numVis1a[nei]);
//					maxVb = max(maxVb, numVis1b[nei]);
//				}
//				numVis2a[i] += maxVa;
//				numVis2b[i] += maxVb;
//
//				// Coupling via the neighbors
//				maxVa = 0;
//				maxVb = 0;
//				for (auto const nei : neighbors2[i]) {
//					maxVa = max(maxVa, numVis1a[nei]);
//					maxVb = max(maxVb, numVis1b[nei]);
//
//			    }
//				if (maxVa > 0) numVis2b[i]+=1;
//				if (maxVb > 0) numVis2a[i] += 1;
//		}
//	
//
//		//print("Step {}, Sum {}\n", maxSteps, reduce(numVis2.begin(), numVis2.end()));
//		swap(numVis1a, numVis2a);
//		swap(numVis1b, numVis2b);
//		print("{}: ",maxSteps);
//		//for (auto const ss : numVis2a) { print("{}, ", ss); }
//		//for (auto const ss : numVis2b) { print("{}, ", ss); }
//		for (IntT i = 0; i < garden.size(); ++i) { print("{}, ", numVis2a[i]+ numVis2b[i]); }
//
//
//		print("\n");
//	}
////	print("Step {}, Sum {}\n", maxMax, reduce(numVis2.begin(), numVis2.end()));
//
//	vector<IntT> maxRes(maxMax,0);
//	for (IntT maxSteps = 1; maxSteps < maxMax; ++maxSteps) {
//
//		set<PIntT> result;
//
//		stack<tuple<IntT, IntT, IntT>> st;
//		st.push({ s.first, s.second, 0 });
//
//		set<tuple<IntT, IntT, IntT>> seen;
//		set<tuple<IntT, IntT>> seen2;
//		while (!st.empty()) {
//			auto const cur = st.top();
//			st.pop();
//
//			auto const r = get<0>(cur);
//			auto const c = get<1>(cur);
//			auto const step = get<2>(cur);
//
//			if (step == maxSteps) {
//				result.insert({ r,c });
//				continue;
//			}
//
//			if (seen.contains({ r,c,step })) {
//				continue;
//			}
//			else { seen.insert({ r,c,step }); }
//
//
//			IntT rr = ((r)+(abs((r) / nr) + 2) * nr) % nr;
//			IntT cc = ((c)+(abs((c) / nc) + 2) * nc) % nc;
//			IntT rp1 = ((r + 1) + (abs((r + 1) / nr) + 2) * nr) % nr;
//			IntT rm1 = ((r - 1) + (abs((r - 1) / nr) + 2) * nr) % nr;
//			IntT cp1 = ((c + 1) + (abs((c + 1) / nc) + 2) * nc) % nc;
//			IntT cm1 = ((c - 1) + (abs((c - 1) / nc) + 2) * nc) % nc;
//
//			if (rawFile[rp1][cc] == '.') st.push({ r + 1,c,step + 1 });
//			if (rawFile[rm1][cc] == '.') st.push({ r - 1,c,step + 1 });
//			if (rawFile[rr][cp1] == '.') st.push({ r,c + 1,step + 1 });
//			if (rawFile[rr][cm1] == '.') st.push({ r,c - 1,step + 1 });
//		}
//
//		maxRes[maxSteps] = result.size();
//
//		IntT cntc = 0;
//		IntT cntl = 0;
//		IntT cntr = 0;
//		IntT cntt = 0;
//		IntT cntb = 0;
//		IntT cnttl = 0;
//		IntT cnttr = 0;
//
//		for (auto const [rx, cx] : result) {
//			if ((rx >= 0) && (rx < nr) && (cx >= 0) && (cx < nc)) cntc++;
//			if ((rx >= nr) && (rx < (2*nr)) && (cx >= 0) && (cx < nc)) cntr++;
//			if ((rx >=-nr) && (rx < 0) && (cx >= 0) && (cx < nc)) cntl++;
//			if ((rx >= 0) && (rx < nr) && (cx >= nc) && (cx < (2 * nc))) cntt++;
//			if ((rx >= 0) && (rx < nr) && (cx >=-nc) && (cx < 0)) cntb++;
//
//			if ((rx >= nr) && (rx < (2 * nr)) && (cx >= nc) && (cx < (2 * nc))) cnttr++;
//			if ((rx >= -nr) && (rx < 0) && (cx >= nc) && (cx < (2 * nc))) cnttl++;
//
//		}
//		//print("Steps {}, Solution {}, Diff {}, Panels {} {} {} {} {} {} {}\n", maxSteps, maxRes[maxSteps], maxRes[maxSteps] - maxRes[maxSteps - 1], cntr, cntc,cntl, cntt, cntb, cnttl,cnttr);
//
//
//		// Collapse and sum all the results
//		vector<IntT> sums(garden.size(), 0);
//		for (auto const [r, c] : result) {
//			IntT rr = ((r)+(abs((r) / nr) + 2) * nr) % nr;
//			IntT cc = ((c)+(abs((c) / nc) + 2) * nc) % nc;
//			sums[gardenLookup[{rr, cc}]]++;
//		}
//
//		print("{}: ", maxSteps);
//		for (auto const ss : sums) { print("{}, ", ss); }
//		print("\n");
//	}
//
//
//
//	const auto result1 = sum1;
//	cout << "Total sum of scores for Part I : " << result1 << endl;
	//assert(result1 == 409898);
	
	// The "extrapolation" solution



	// Strange solution that relies on having straight lines to all neighbors:
	const IntT numSteps = 26501365;

	const IntT numTiles = numSteps / nr;
	const IntT restStep = numSteps % nr;


	// Go to end of center tile:
	//const IntT numTiles1 = getNumTiles2(rawFile, s, restStep);
	//const IntT numTiles2 = getNumTiles2(rawFile, s, restStep + nr);
	//const IntT numTiles3 = getNumTiles2(rawFile, s, restStep + 2*nr);


	//// Fit polynomial
	//print("{},{}, {},{}, {},{}\n", restStep, numTiles1, (restStep + nr), numTiles2, (restStep + 2 * nr), numTiles3);

	auto const xxx = numSteps / IntT(17161);
	auto const xxx2 = double(numSteps)/ double(17161);
	print("{} {} \n", xxx,xxx2);

	const auto result2 = ((IntT(14483) * double(numSteps) * double(numSteps)) / double(17161) + (double(30203) * double(numSteps)) / double(17161) + double(32932) / double(17161));
	cout << "Total sum of scores for Part II: " << setprecision(18) << result2 << endl;
	//assert(result2 == 113057405770956);

	return EXIT_SUCCESS;
}

//if ((r<(nr-1)) && (rawFile[r + 1][c] == '.')) st.push({ r + 1,c,step+1});
//if ((r >0) && rawFile[r - 1][c] == '.') st.push({ r -1,c,step + 1 });
//if ((c < (nc - 1)) && rawFile[r][c+1] == '.') st.push({ r,c+1,step + 1 });
//if ((c > 0) && rawFile[r][c-1] == '.') st.push({ r,c-1,step + 1 });
