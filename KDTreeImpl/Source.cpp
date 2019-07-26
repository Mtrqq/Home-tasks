#include "KDTree.h"

#include <iostream>

int main()
{
	std::vector<nostd::KDPoint<2>> points{ {4,3},{1,5},{7,4},{4,7}, {2,0},{3,7},{6,2},{5,6},{0,1} };
	nostd::KDTree<2> KD{ points };
	KD.Insert({ 5,5 });
	auto vec = KD.ToVector();
	for (auto p : vec)
	{
		p.Print(std::cout);
	}
	std::cout << "\n\nFind {5,5} after insertion : " << KD.HasPoint({ 5,5 }) << std::endl;
	KD.Delete({ 5,5 });
	std::cout << "\nFind {5,5} after deletion : " << KD.HasPoint({ 5,5 });
}