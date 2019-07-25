#include "KDTree.h"

int main()
{
	std::vector<nostd::KDPoint<2>> points{ {1,1},{2,2},{3,3} };
	nostd::KDTree<2> KD{ points };
	for (const auto &point : KD)
	{
		std::cout << point;
	}
}