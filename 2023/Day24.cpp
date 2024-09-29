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

template<typename T>
struct Pos3 {
	Pos3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
	T x, y, z;
};

template<typename T>
Pos3<T> operator+ (Pos3<T> a, Pos3<T> b) {
	return Pos3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template<typename T>
Pos3<T> operator- (Pos3<T> a, Pos3<T> b) {
	return Pos3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template<typename T>
T vlen(Pos3<T> a) {
	return sqrt(a.x* a.x+ a.y * a.y + a.z * a.z );
}



using Pos3d = Pos3<double>;
using Pos3i = Pos3<IntT>;
/*
   Calculate the line segment PaPb that is the shortest route between
   two lines P1P2 and P3P4. Calculate also the values of mua and mub where
	  Pa = P1 + mua (P2 - P1)
	  Pb = P3 + mub (P4 - P3)
   Return FALSE if no solution exists.
*/
bool LineLineIntersect( Pos3d p1, Pos3d p2, Pos3d p3, Pos3d p4, Pos3d* pa, Pos3d* pb, double* mua, double* mub)
{
	const double EPS = 1e-14;

	const Pos3d p43 = p4 - p3;
	if (abs(p43.x) < EPS && abs(p43.y) < EPS && abs(p43.z) < EPS) return false;

	const Pos3d p21 = p2 - p1;
	if (abs(p21.x) < EPS && abs(p21.y) < EPS && abs(p21.z) < EPS) return false;

	const Pos3d p13 = p1 - p3;
	const double d1343 = p13.x * p43.x + p13.y * p43.y + p13.z * p43.z;
	const double d4321 = p43.x * p21.x + p43.y * p21.y + p43.z * p21.z;
	const double d1321 = p13.x * p21.x + p13.y * p21.y + p13.z * p21.z;
	const double d4343 = p43.x * p43.x + p43.y * p43.y + p43.z * p43.z;
	const double d2121 = p21.x * p21.x + p21.y * p21.y + p21.z * p21.z;

	const double denom = d2121 * d4343 - d4321 * d4321;
	if (abs(denom) < EPS) return false;
	const double numer = d1343 * d4321 - d1321 * d4343;

	*mua = numer / denom;
	*mub = (d1343 + d4321 * (*mua)) / d4343;

	pa->x = p1.x + *mua * p21.x;
	pa->y = p1.y + *mua * p21.y;
	pa->z = p1.z + *mua * p21.z;
	pb->x = p3.x + *mub * p43.x;
	pb->y = p3.y + *mub * p43.y;
	pb->z = p3.z + *mub * p43.z;

	return true;
}




int main24()
{
	auto rawFile = loadFile("Data/aoc_input_24.txt");

	IntT sum1 = 0;
	IntT sum2 = 0;

	IntT nr = rawFile.size();
	IntT nc = rawFile[0].size();

	vector<Pos3i> pList;
	vector<Pos3i> vList;

	for (auto const& row : rawFile) {
		auto const split = stringToVector(row, '@');
		auto const pp = stringToVectorOfInt(trimString(split[0], " "), ',');
		auto const vv = stringToVectorOfInt(trimString(split[1], " "), ',');

		Pos3 ppp{ pp[0], pp[1], pp[2] };
		pList.push_back(ppp);
		Pos3 vvv{ vv[0], vv[1], vv[2] };
		vList.push_back(vvv);
	}

	const IntT N = pList.size();
	//const double xMin = 7; // 200000000000000; //
	//const double  xMax = 27; // 400000000000000; //27; // 
	//const double  yMin = 7;// 200000000000000; //7;// 
	//const double  yMax = 27;// 400000000000000; //27;// 

	const double  xMin = 200000000000000; //
	const double  xMax = 400000000000000; //27; // 
	const double  yMin = 200000000000000; //7;// 
	const double  yMax = 400000000000000; //27;// 
	                     
//	vector<IntT> hasCrossed(N, 0);

	for (int i = 0; i < N; ++i) {
		//if (hasCrossed[i] == 1) continue;

		for (int j = (i+1); j < N; ++j) {


		//	if ((hasCrossed[i] == 1) || (hasCrossed[j] == 1) )continue;

			using T = IntT;
			const T ax = pList[i].x;
			const T ay = pList[i].y;

			const T bx = vList[i].x;
			const T by = vList[i].y;

			const T cx = pList[j].x;
			const T cy = pList[j].y;

			const T dx = vList[j].x;
			const T dy = vList[j].y;

			const T den = (bx * dy - by * dx);

			if( den == 0) {
				print("{} {} {} @ {} {} |\n", i, ax, ay, bx, by);
				print("{} {} {} @ {} {} |\n", j, cx, cy, dx, dy);
				cout << "Don't intersect!\n" << endl;

			} else {
				const double t = (dx * double(ay - cy) + dy * double(cx - ax)) / double(den);
				const double u = (bx * double(cy - ay) + by * double(ax - cx)) / double(dx * by - dy * bx);

				const double xx = ax + t * bx;
				const double yy = ay + t * by;

				//print("{} {} {} @ {} {} | {}\n",i, ax, ay, bx, by, t);
				//print("{} {} {} @ {} {} | {}\n",j, cx, cy, dx, dy, u);

				//if (t < 0) {
				//	cout << " Crossed in the past for A\n";
				//}
				//if (u < 0) {
				//	cout <<" Crossed in the past for B\n";
				//}
				if ((t < 0) || (u < 0)) {
					cout << endl;  continue;
				}


				if ((xx >= xMin) && (xx <= xMax) && (yy >= yMin) && (yy <= yMax)) { 
					sum1++;
					//print(" Crosses at {} {} inside!\n\n", xx, yy);
					//hasCrossed[i] = 1;
					//hasCrossed[j] = 1;
				}
				else {
					//print(" Crosses at {} {} outside!\n\n", xx, yy);
				}
			}



		}

	}
	//const auto x = reduce(hasCrossed.begin(), hasCrossed.end());

	const auto result1 = sum1;
	cout << "Total sum of scores for Part I : " << result1 << endl;
	//assert(result1 == 409898);
	

	// Do we have co-linear lines in 3d?
	vector<Pos3d> normals;
	for (int i = 0; i < N; ++i) {
		for (int j = (i + 1); j < N; ++j) {

			using T = double;
			const T ax = (pList[i].x - xMin) / double(xMin);;
			const T ay = (pList[i].y - xMin) / double(xMin);;
			const T az = (pList[i].z - xMin) / double(xMin);;

			const T blen = sqrt(double(vList[i].x) * vList[i].x + vList[i].y * vList[i].y + vList[i].z * vList[i].z);
			const T bx = vList[i].x / blen;
			const T by = vList[i].y / blen;
			const T bz = vList[i].z / blen;

			const T cx = (pList[j].x - xMin) / double(xMin);
			const T cy = (pList[j].y - xMin) / double(xMin);;
			const T cz = (pList[j].z - xMin) / double(xMin);;

			const T dlen = sqrt(double(vList[j].x) * vList[j].x + vList[j].y * vList[j].y + vList[j].z * vList[j].z);
			const T dx = vList[j].x/ dlen;
			const T dy = vList[j].y / dlen;
			const T dz = vList[j].z / dlen;

			// Do we intersect?
			Pos3d p1(ax, ay, az);
			Pos3d p2(bx,by,bz);
			Pos3d p3(cx, cy, cz);
			Pos3d p4(dx, dy, dz);
			Pos3d pa(0,0,0), pb(0,0,0);
			double  mua, mub;
			bool doesInt = LineLineIntersect(p1, p2, p3, p4, &pa, &pb, &mua, &mub);
			if (doesInt) {

				auto const dst = vlen(pa - pb);
				if (dst < 1e-6) {
					print("Vectors {} ({},{},{})  and {} ({},{},{}) do intersect {} ?!\n", i, bx, by, bz, j, dx, dy, dz, dst);
				}
			}

			////COmpute cross product of velocities
			//auto const crx = by * dz - bz * dy;
			//auto const cry = bz * dx - bx * dz;
			//auto const crz = bx * dy - by * dx;

			//if ((crx == 0) || (cry == 0) || (crz == 0)) {
			//	print("Vectors {} ({},{},{})  and {} ({},{},{}) are parallel {} {} {}!\n", i, bx,by,bz, j,dx,dy,dz, crx, cry, crz);
			//	
			//	// They form a plane with the following normal vector

			//	const T ex = ax - cx;
			//	const T ey = ay - cy;
			//	const T ez = az - cz;

			//	auto const nx = by * ez - bz * ey;
			//	auto const ny = bz * ex - bx * ez;
			//	auto const nz = bx * ey - by * ex;
			//	normals.emplace_back(nx, ny, nz);
			
			//}
		}
	}

	//// Pairwise, we should be able to get the 
	//auto const NN = normals.size();
	//for (int i = 0; i < NN; ++i) {
	//	auto const ni = normals[i];
	//	for (int j = (i + 1); j < NN; ++j) {
	//		auto const nj = normals[j];

	//		double const nx = ni.y * nj.z - ni.z * nj.y;
	//		double const ny = ni.z * nj.x - ni.x * nj.z;
	//		double const nz = ni.x * nj.y - ni.y * nj.x;
	//		double const len = sqrt(nx * nx + ny * ny + nz * nz);
	//		print("Normal v: {} {} {}\n", nx / len, ny / len, nz / len);

	//	}
	//}

	// Try fix-point iteration to solve the system of nonlinear equations
	// We have 6 unknowns, so lets try 3 hail stones for a total of 6 equations
	Pos3d pr(0.5, 0.5, 0.5);
	Pos3d vr(0, 0, 0);

	vector<Pos3d> p;
	vector<Pos3d> v;

	// Lets just print some equations for Wolfram alpha to solve:
	for (int i = 0; i < 4; ++i) {
		//p.emplace_back((pList[i].x-xMin)/double(xMin), (pList[i].y - xMin) / double(xMin), (pList[i].z - xMin) / double(xMin));
		//v.emplace_back(vList[i].x/vl, vList[i].y / vl, vList[i].z / vl);

//		print("(x-a)*(v-e)==(y-b)*(u-d),", pList[i].x, vList[i].y, pList[i].y, vList[i].x);
//		print("(y-b)*(w-f)==(z-c)*(v-e),", pList[i].y, vList[i].z, pList[i].z, vList[i].y);
		print("(x-{})*(v-{})==(y-{})*(u-{}),", pList[i].x, vList[i].y, pList[i].y, vList[i].x);
		print("(y-{})*(w-{})==(z-{})*(v-{}),", pList[i].y, vList[i].z, pList[i].z, vList[i].y);
//		print("(x-a)*(v-({}))==(y-b)*(u-({})),", vList[i].y, vList[i].x);
//		print("(y-b)*(w-({}))==(z-c)*(v-({})),", vList[i].z, vList[i].y);
	}


	for (int i = 0; i < 20; ++i) {
		Pos3d pr2(0, 0, 0);
		Pos3d vr2(-1, -1, 1);

		pr2.x = (p[0].x - pr.x) * (vr.y - v[0].y) - (p[0].y - pr.y) * (vr.x - v[0].x) + pr.x;
		pr2.y = (p[0].y - pr.y) * (vr.z - v[0].z) - (p[0].z - pr.z) * (vr.y - v[0].y) + pr.y;
		pr2.z = (p[1].x - pr.x) * (vr.y - v[1].y) - (p[1].y - pr.y) * (vr.x - v[1].x) + pr.z;
		vr2.x = (p[1].y - pr.y) * (vr.z - v[1].z) - (p[1].z - pr.z) * (vr.y - v[1].y) + vr.x;
		vr2.y = (p[2].x - pr.x) * (vr.y - v[2].y) - (p[2].y - pr.y) * (vr.x - v[2].x) + vr.y;
		vr2.z = (p[2].y - pr.y) * (vr.z - v[2].z) - (p[2].z - pr.z) * (vr.y - v[2].y) + vr.z;

		print(" {} {} {} {} {} {}\n", pr2.x, pr2.y, pr2.z, vr2.x, vr2.y, vr2.z);
		pr = pr2;
		vr = vr2;
	}



	const auto result2 = sum2;

	cout << "Total sum of scores for Part II: " << result2 << endl;
	//assert(result2 == 113057405770956);
	 
	return EXIT_SUCCESS;
}
