#include "KDTree.h"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <random>

using namespace std::chrono;

std::pair<double, nostd::KDPoint<3>> FindNearestTree(const nostd::KDTree<3> &tree, const nostd::KDPoint<3> &to_find)
{
	auto start = steady_clock::now();
	auto output = tree.GetClosestTo(to_find);
	return std::make_pair(duration<double>((steady_clock::now() - start)).count(), output);
}

std::pair<double, nostd::KDPoint<3>> FindNearestBruteforce(const std::vector<nostd::KDPoint<3>> &pts,const nostd::KDPoint<3> &to_find)
{
	auto start = steady_clock::now();
	double min{ std::numeric_limits<double>::max() };
	nostd::KDPoint<3> output{ pts[0] };
	for (const auto &point : pts)
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

int main()
{
	std::uniform_real_distribution<> u_id(0.0, 100000.0);
	std::mt19937 generator{ std::random_device{}() };
	std::vector<nostd::KDPoint<3>> points;
	points.reserve(1000000);
	for (int i = 0; i < 1000000; ++i)
	{
		points.push_back({ u_id(generator),u_id(generator), u_id(generator) });
	}
	nostd::KDTree<3> balancedTree{ points },unbalancedTree;
	for (const auto &point : points)
	{
		unbalancedTree.Insert(point);
	}

	double first{}, second{}, third{};
	unsigned testsCount{ 1000 };
	for (unsigned i = 0; i < testsCount; ++i)
	{
		auto PointToFind = nostd::KDPoint<3>{ u_id(generator),u_id(generator),  u_id(generator) };
		auto p1 = FindNearestTree(unbalancedTree, PointToFind); // How this could be faster ?
		auto p2 = FindNearestTree(balancedTree, PointToFind);
		auto p3 = FindNearestBruteforce(points, PointToFind);
		first += p1.first;
		second += p2.first;
		third += p3.first;
	}
	std::cout.setf(std::ios::fixed);
	std::cout << "Average calculations speed (s)\n\n"
		<< "Unbalanced tree : " << std::setprecision(10) <<  first << std::endl
		<< "Balanced tree : " << std::setprecision(10) << second << std::endl
		<< "Brute force : " << std::setprecision(10) << third;
	std::cin.get();
}