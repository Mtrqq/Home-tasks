#include "KDTree.h"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <random>

using namespace std::chrono;

void FindNearestTree(const nostd::KDTree<2> &tree, const nostd::KDPoint<2> &to_find)
{
	auto start = steady_clock::now();
	auto p = tree.GetClosestTo(to_find);
	std::cout << "Tree: " << std::fixed << std::setprecision(10) << duration<double>((steady_clock::now() - start)).count();
	std::cout << "\nOutput : ";
	p.Print(std::cout);
	std::cout << "\n\n";
}

void FindNearestBruteforce(const std::vector<nostd::KDPoint<2>> &pts,const nostd::KDPoint<2> &to_find)
{
	auto start = steady_clock::now();
	double min{ std::numeric_limits<double>::max() };
	nostd::KDPoint<2> output{ pts[0] };
	for (const auto &point : pts)
	{
		auto distance = point.DistanceTo(to_find);
		if (distance < min)
		{
			output = point;
			min = distance;
		}
	}
	std::cout << std::fixed << std::setprecision(10) << "Brute force nearest : " << duration<double>((steady_clock::now() - start)).count();
	std::cout << "\nOutput : ";
	output.Print(std::cout);
	std::cout << "\n\n";
}

int main()
{
	std::uniform_real_distribution<> u_id(0.0, 100000.0);
	std::mt19937 generator{ std::random_device{}() };
	std::vector<nostd::KDPoint<2>> points;
	points.reserve(1000000);
	for (int i = 0; i < 1000000; ++i)
	{
		points.push_back({ u_id(generator),u_id(generator) });
	}
	auto PointToFind = nostd::KDPoint<2>{ u_id(generator),u_id(generator) };
	nostd::KDTree<2> unbalancedTree;
	for (const auto &point : points)
	{
		unbalancedTree.Insert(point);
	}
	std::cout << "Unbalanced ";
	FindNearestTree(unbalancedTree, PointToFind); // How this could be faster ?
	FindNearestTree({ points }, PointToFind);
	FindNearestBruteforce(points, PointToFind);
	std::cin.get();
}