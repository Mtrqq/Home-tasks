#include "KDTree.h"
#include "KDRectangle.h"
#include "TestFunctions.h"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <random>


int main()
{
	std::uniform_real_distribution<> u_id(0.0, 100000.0);
	std::mt19937 generator{ std::random_device{}() };
	std::vector<nostd::KDPoint<2>> points;
	points.reserve(10'000'000);
	std::vector<double> coordinates(2);
	for (int i = 0; i < 100; ++i)
	{
		std::generate(coordinates.begin(), coordinates.end(), [&] {return u_id(generator); });
		points.emplace_back(coordinates);
	}
	TestNPSearch(points, 100);
	TestRangeSearch(points, 100);
	std::cin.get();
}