#include <iostream>
#include <vector>

#include "Matrix.h"

int main()
{
	Matrix<int, 15, 15> matrix(2);
	std::cout << (matrix * 2 + Matrix<double, 15, 15>(2.2));
}
