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

enum class MT {
	FF,
	Con
};

struct Module {
	MT type;
	vector<string> output;
	int ffState = 0;

	map<string, int> conStat;


	int process(string const & sender, int signal) {
		if (type == MT::FF) {
			if (signal == 1) return -1;  //< Stop
			ffState = 1 - ffState;
			return ffState;
		}

		if (type == MT::Con) {
			conStat[sender] = signal;

			bool allHigh = true;
			for (const auto stat : conStat) {
				if (stat.second == 0) {
					allHigh = false; break;
				}
			}
			
			return allHigh ? 0 : 1;
		}
	}
};


int main20()
{
	const auto rawFile = loadFile("Data/aoc_input_20.txt");

	IntT sum1 = 0;
	IntT sum2 = 0;
	vector<string> broadcaster;
	map<string, Module> modules;
	vector<string> conmod;

	for (const auto row : rawFile) {
		auto const split1 = stringToVector(row, '>');

		auto type = trimString(split1[0], " -");
		auto const targets = stringToVector(removeAll(split1[1]," "), ',');

		if (type == "broadcaster") {
			broadcaster = targets;
		}
		else if (type.starts_with("%")) {
			const auto name = trimString(type, "%& ");
			modules[name] = Module{ .type = MT::FF, .output = targets };
		}
		else if (type.starts_with("&")) {
			const auto name = trimString(type, "%& ");
			conmod.push_back(name);
			modules[name] = Module{ .type = MT::Con, .output = targets };
		}
	}

	for (auto mod : modules) {
		for (auto const conname : conmod) {
			if (sr::contains(mod.second.output, conname)) {
				modules[conname].conStat[mod.first] = 0;
			}
		}
	}

	queue<tuple<string,string, int>> st;
	
	IntT low = 0;  //< Button pressed
	IntT high = 0;

	for (int bp = 0; bp < 100000; bp++) {
		if (bp % 1000000 == 0) cout << bp << endl;
		low++;
		//cout << "botton" << " " << 0 << " -> " << "broadcaster" << " " << endl;

		for (auto const &b : broadcaster) {
			st.push({ b, "broadcaster", 0 });  // To, From , Signal
			//cout << "broadcaster" << " " << 0 << " -> " << b << " " << endl;

			low++;
		}

		while (!(st.empty())) {
			auto const ss = st.front();
			st.pop();

			auto const & name = get<0>(ss);
			const auto& sender = get<1>(ss);
			auto& mod = modules[name];
			auto const newSignal = mod.process(sender, get<2>(ss));

			if (name == "th") {
				bool print = false;
				for (auto const& x : mod.conStat) {
					if (x.second != 0) { print = true; break; }
				}
				if (print) {
					cout << bp << " ";
					for (auto const& x : mod.conStat) {
						cout << x.first << "=" << x.second << ", ";
					}
					cout << endl;
				}
			}

			if (newSignal >= 0) {
				for (auto const & out : mod.output) {
					if (out == "rx" && newSignal==0) {
						cout << "========== Result: " << bp << endl;
						abort();
					}
					if (newSignal == 0) low++;
					if (newSignal == 1) high++;
					st.push({ out,name,newSignal });
				//	cout << name << " " << newSignal << " -> " << out << " " << endl;
				}

			}
		}

		bool print = false;
		for (auto const& x : modules["th"].conStat) {
			if (x.second != 0) { print = true; break; }
		}
		if( print ) {
			cout << bp << " ";
			for (auto const x : modules["th"].conStat) {
				cout << x.first << "=" << x.second << ", ";
			}
			cout << endl;
		}


	}

	const auto result1 = low*high;
	cout << "Total sum of scores for Part I : " << result1 << endl;
	//assert(result1 == 409898);
	

	// Simplification: If we have a con module with a single input, it is an inverter ... 
	IntT a = 3739;
	IntT b = 4027;
	IntT c = 3923;
	IntT d = 3793;

	 sum2 = lcm(lcm(lcm(a, b), c), d);
	const auto result2 = sum2;
	cout << "Total sum of scores for Part II: " << result2 << endl;
	//assert(result2 == 113057405770956);

	return EXIT_SUCCESS;
}

