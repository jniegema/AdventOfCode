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
	IntT x, y, z;
};

struct Brick {
	Brick(IntT x1, IntT y1, IntT z1, IntT x2, IntT y2, IntT z2) :
		start{ x1, y1, z1 },
		end{x2, y2, z2} {
		assert(z1 <= z2);
	}


	bool operator < (Brick const& other) const {
		return start.z < other.start.z;
	}
	Pos start, end;
};

bool lowerBrick (Brick const& b1, Brick const& b2) {
	return b1.start.z < b2.start.z;
}


vector<Brick> bricks;
map<tuple<IntT, IntT, IntT>, IntT> world;

int main22()
{
	auto rawFile = loadFile("Data/aoc_input_22.txt");

	IntT sum1 = 0;
	IntT sum2 = 0;
	
	IntT nr = rawFile.size();
	IntT nc = rawFile[0].size();

	for (auto const& row : rawFile) {
		const auto split = stringToVector(row, '~');
		const auto pos1 = stringToVectorOfInt(split[0], ',');
		const auto pos2 = stringToVectorOfInt(split[1], ',');
		bricks.emplace_back(pos1[0],pos1[1],pos1[2],pos2[0],pos2[1],pos2[2]);
	}

	sr::sort(bricks, lowerBrick);

	for (auto const [i,b] : sv::enumerate(bricks )) {
	
		bool canFall = true;
		auto z = b.start.z - 1;
		while (canFall && z>0) {

			for (IntT x = b.start.x; x <= b.end.x; ++x) {
				for (IntT y = b.start.y; y <= b.end.y; ++y) {			
					if (world.contains({ z,y,x})) {
						canFall = false;
					}
				}
			}
			
			if( canFall) z--;
		}

		const auto dz = (b.start.z) - (z+1);
		b.start.z -= dz;
		b.end.z -= dz;
		for (IntT x = b.start.x; x <= b.end.x; ++x) {
			for (IntT y = b.start.y; y <= b.end.y; ++y) {
				for (IntT z = b.start.z; z <= b.end.z; ++z) {
					world[{ z,y,x}]=i;
				}
			}
		}
	}

	// Figure which we could remove
	vector<std::set<IntT>> supportedBy(bricks.size(), std::set<IntT>());
	vector<std::set<IntT>> supports(bricks.size(), std::set<IntT>());
	set<IntT> cantBeRemoved;
	for (IntT i = (bricks.size() - 1); i >= 0; i--) {
		const auto& b = bricks[i];
		
		// Check one below
		const auto z = b.start.z - 1;
		for (IntT x = b.start.x; x <= b.end.x; ++x) {
			for (IntT y = b.start.y; y <= b.end.y; ++y) {
				if (auto const it = world.find({ z,y,x }); it!=world.end() ) {
					supportedBy[i].insert(it->second);
				}
			}
		}

		if (supportedBy[i].size() == 1) { cantBeRemoved.insert(*supportedBy[i].begin()); }

		for (auto sp : supportedBy[i]) {
			supports[sp].insert(i);
		}
	}



	const auto result1 = bricks.size()- cantBeRemoved.size();
	cout << "Total sum of scores for Part I : " << result1 << endl;
	//assert(result1 == 409898);
	
	// Strange solution that relies on having straight lines to all neighbors:

	vector<IntT> chainRection(bricks.size(), 0);
	for (auto const [i, b] : sv::enumerate(bricks)) {

		stack<IntT> toCheck;
		for (auto const sprt : supports[i]) { toCheck.push(sprt); }
		
		set<IntT> willFall;
		willFall.insert(i);
		
		while (!toCheck.empty()) {
			auto const tc = toCheck.top();
			toCheck.pop();

			// Anything left supporting this one:
			std::vector<int> diff{};

			sr::set_difference(supportedBy[tc], willFall, std::back_inserter(diff));

			if (diff.size() == 0) {
				willFall.insert(tc);
				for (auto const sprt : supports[tc]) { toCheck.push(sprt); }
			}
		}
		chainRection[i] = willFall.size()-1;
	}



	const auto result2 = reduce(begin(chainRection), end(chainRection));

	cout << "Total sum of scores for Part II: " << result2 << endl;
	//assert(result2 == 113057405770956);

	return EXIT_SUCCESS;
}
