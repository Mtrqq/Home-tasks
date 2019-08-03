#include "TestFunctions.h"

#include <chrono>
#include <random>
#include <iostream>

using namespace std::chrono;

std::pair<double, nostd::KDPoint<tested_dimmension>> FindNearestTree(const nostd::KDTree<tested_dimmension> & tree, const nostd::KDPoint<tested_dimmension> & to_find)
{
	auto start = steady_clock::now();
	auto output = tree.GetClosestTo(to_find);
	return std::make_pair(duration<double>((steady_clock::now() - start)).count(), output);
}

std::pair<double, nostd::KDPoint<tested_dimmension>> FindNearestBruteforce(const std::vector<nostd::KDPoint<tested_dimmension>> & pts,
																		   const nostd::KDPoint<tested_dimmension> & to_find)
{
	auto start = steady_clock::now();
	double min{ std::numeric_limits<double>::max() };
	nostd::KDPoint<tested_dimmension> output{ pts[0] };
	for (const auto& point : pts)
	{
		auto distance = point.DistanceTo(to_find);
		if (distance < min)
		{
			output = point;
			min = distance;
		}
	}
	return std::make_pair(duration<double>((steady_clock::now() - start)).count(), output);
}

std::pair<double, std::vector<nostd::KDPoint<tested_dimmension>>> PointsInSectorTree(const nostd::KDTree<tested_dimmension> & tree,
																					 const nostd::KDRectangle<tested_dimmension> & selection)
{
	auto start = steady_clock::now();
	auto output = tree.GetPointsInSection(selection);
	return std::make_pair(duration<double>((steady_clock::now() - start)).count(), output);
}

std::pair<double, std::vector<nostd::KDPoint<tested_dimmension>>> BruteForcePtsInSector(const std::vector<nostd::KDPoint<tested_dimmension>> & pts
																						, const nostd::KDRectangle<tested_dimmension> & selection)
{
	auto start = steady_clock::now();
	std::vector<nostd::KDPoint<tested_dimmension>> output;
	for (const auto& point : pts)
	{
		if (selection.Contains(point))
			output.push_back(point);
	}
	return std::make_pair(duration<double>((steady_clock::now() - start)).count(), output);
}

void TestNPSearch(const std::vector<nostd::KDPoint<tested_dimmension>> & points, unsigned tests_count)
{
	std::uniform_real_distribution<> u_id(0.0, 100000.0);
	std::mt19937 generator{ std::random_device{}() };
	nostd::KDTree<tested_dimmension> balancedTree{ points }, unbalancedTree;
	for (const auto& point : points)
	{
		unbalancedTree.Insert(point);
	}

	double first{}, second{}, third{};
	std::vector<double> coordinates(tested_dimmension);
	for (unsigned i = 0; i < tests_count; ++i)
	{
		std::generate(coordinates.begin(), coordinates.end(), [&] {return u_id(generator); });
		auto PointToFind = nostd::KDPoint<tested_dimmension>{ coordinates };
		auto p1 = FindNearestTree(unbalancedTree, PointToFind); // How this could be faster ?
		auto p2 = FindNearestTree(balancedTree, PointToFind);
		auto p3 = FindNearestBruteforce(points, PointToFind);
		first += p1.first;
		second += p2.first;
		third += p3.first;
	}
	std::cout.setf(std::ios::fixed);
	std::cout << "Average calculations speed (s)\n\n"
		<< "Unbalanced tree : " << std::setprecision(10) << first << std::endl
		<< "Balanced tree : " << std::setprecision(10) << second << std::endl
		<< "Brute force : " << std::setprecision(10) << third;
}

void TestRangeSearch(const std::vector<nostd::KDPoint<tested_dimmension>> & points, unsigned tests_count)
{
	std::uniform_real_distribution<> u_id(0.0, 100000.0);
	std::mt19937 generator{ std::random_device{}() };
	nostd::KDTree<tested_dimmension> balancedTree{ points }, unbalancedTree;
	for (const auto& point : points)
	{
		unbalancedTree.Insert(point);
	}

	double first{}, second{}, third{};
	std::vector<double> point1(tested_dimmension), point2(tested_dimmension);
	for (unsigned i = 0; i < tests_count; ++i)
	{
		std::generate(point1.begin(), point1.end(), [&] {return u_id(generator); });
		std::generate(point2.begin(), point2.end(), [&] {return u_id(generator); });
		nostd::KDRectangle<tested_dimmension> Selection{ nostd::KDVector<tested_dimmension>{point1},
														 nostd::KDVector<tested_dimmension>{point2} };
		auto p1 = PointsInSectorTree(unbalancedTree, Selection);
		auto p2 = PointsInSectorTree(balancedTree, Selection);
		auto p3 = BruteForcePtsInSector(points, Selection);
		first += p1.first;
		second += p2.first;
		third += p3.first;
	}
	std::cout.setf(std::ios::fixed);
	std::cout << "\n\nAverage calculations speed (s)\n\n"
		<< "Unbalanced tree : " << std::setprecision(10) << first << std::endl
		<< "Balanced tree : " << std::setprecision(10) << second << std::endl
		<< "Brute force : " << std::setprecision(10) << third;
}