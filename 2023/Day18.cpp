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
};

void printMapX(auto const& f) {
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

//map<IntT,vector<IntT>> wall;

int main18()
{
	const auto rawFile = loadFile("Data/aoc_input_18.txt");

	IntT x = 0;
	IntT y = 0;
	IntT prevX = 0;
	IntT prevY = 0;
	
	const auto v1 = stringToVector(rawFile.back(), ' ');

	map<IntT, vector<pair<IntT,char>>> wall2;
	vector<Pos> v;
	v.emplace_back(0, 0);
	char sym = 'x';
	for (auto row : rawFile) {
		auto vx = stringToVector(row, ' ');
		char dir = vx[0][0];
		IntT length = toInt(vx[1]);
		
		string color = trimString(vx[2], "()");
		IntT dir2 = color[6]-48;
		if (dir2 == 0) dir = 'R';
		if (dir2 == 1) dir = 'D';
		if (dir2 == 2) dir = 'L';
		if (dir2 == 3) dir = 'U';

		string hexStr = string("0x") + color.substr(1, 5);
		length = stoll(hexStr, nullptr, 16);

		//cout << color  << " " << dir << " " << length << endl;

		switch (dir) {
		case 'R': 
			x+=length;
			break;

		case 'L': 
			x-=length;
			break;
		case 'U':
				y+=length;
			break;

		case 'D':
				y-=length;
			break;
		}
		v.emplace_back(x, y);

		prevX = x;
		prevY = y;
	}




	v.pop_back();
	IntT N = v.size();
	
	// Counting right-turns and left-turns
	IntT rt = 0;
	IntT lt = 0;
	for (IntT i = 0; i < N; ++i) {
		const double x = v[i].x;
		const double y = v[i].y;

		const double x0 = (i == 0) ? v[N - 1].x : v[i - 1].x;
		const double x1 = (i == (N - 1)) ? v[0].x : v[i + 1].x;
		const double y0 = (i == 0) ? v[N - 1].y : v[i - 1].y;
		const double y1 = (i == (N - 1)) ? v[0].y : v[i + 1].y;
		const auto dx0 = x - x0;
		const auto dx1 = x1 - x;
		
		const auto dy0 = y - y0;
		const auto dy1 = y1 - y;

		if( ((dx0 * dy1) > 0) || (dy0*dx1) <0 ) {
			lt++;
		} else {
			rt++;
		}
	}

	// Area of polygon via shoestring
 	double area = 0;
	double len = 0;
	for (IntT i = 0; i < (N-1); ++i) {
		const double x0 = v[i].x;
		const double x1 = v[i + 1].x;
		const double y0 = v[i].y;
		const double y1 = v[i + 1].y;
		area += -y0 * x1 + x0 * y1;
		len+= abs(x1 - x0) + abs(y1 - y0) - 1;
	}
	area += -double(v[N - 1].y) * v[0].x + double(v[N - 1].x) * v[0].y;
	len += abs(double(v[0].x) - v[N - 1].x) + abs(double(v[0].y) - v[N - 1].y) - 1.;

	area = 0.5 * abs(area);

	IntT corners = rt*3+lt;
	area += 0.5 * len;
	area += 0.25 * corners;


	//vector<vector<char>> mp(nr, vector<char>(nc, '.'));

	// Flood-fill?

	IntT sum1 = 0;
	IntT sum2 = area;
	//for (auto& item : wall2) {
	//	auto cy = item.first;
	//	auto& xl = item.second;
	//	sr::sort(xl);


	//	bool inside = false;
	//	bool hadL = false;
	//	bool hadF = false;
	//	IntT x       = xl[0].first;
	//	char curChar = xl[0].second;
	//	if (curChar == '|') { inside = !inside; }

	//	if ((curChar == 'F')) { hadF = true;  }
	//	if (hadF && (curChar == 'J')) { hadF = false; inside = !inside;  }
	//	if (hadF && (curChar == '7')) { hadF = false;  }


	//	if ((curChar == 'L')) { hadL = true; }
	//	if (hadL && (curChar == '7')) { hadL = false; inside = !inside;  }
	//	if (hadL && (curChar == 'J')) { hadL = false;  }

	//	mp[cy - minY][x - minX] = curChar;
	//	for (int j = 1; j < xl.size();++j) {
	//		curChar = xl[j].second;

	//		if (x != xl[j].first) {
	//			if (inside) {
	//				sum1 += xl[j].first - x - 1;
	//				for (IntT xx = (x + 1); xx < xl[j].first; ++xx) {
	//					mp[cy - minY][xx - minX] = 'I';
	//					sum2++;
	//				}

	//			}
	//		}
	//		x = xl[j].first;
	//		mp[cy - minY][x - minX] = curChar;

	//		if (curChar == '|') { inside = !inside; continue; }

	//		if ((curChar == 'F')) { hadF = true; continue; }
	//		if (hadF && (curChar == 'J')) { hadF = false; inside = !inside; continue; }
	//		if (hadF && (curChar == '7')) { hadF = false; continue; }


	//		if ((curChar == 'L')) { hadL = true; continue; }
	//		if (hadL && (curChar == '7')) { hadL = false; inside = !inside; continue; }
	//		if (hadL && (curChar == 'J')) { hadL = false; continue; }
	//	}
	//	sum1 += xl.size();
	//	sum2 += xl.size();
	//}

	//printMapX(mp);



	//const IntT nr = rawFile.size();
	//const IntT nc = rawFile[0].size();

	//IntT sum2 = 0;

	const auto result1 = sum1;
	cout << "Total sum of scores for Part I : " << result1 << endl;
	assert(result1 == 0);
	



	const auto result2 = sum2;
	cout << "Total sum of scores for Part II: " << result2 << endl;
	assert(result2 == 0 );

	return EXIT_SUCCESS;
}

