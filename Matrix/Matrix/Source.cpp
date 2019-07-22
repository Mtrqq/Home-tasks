#include <iostream>
#include <vector>

#include "Matrix.h"

int main()
{
	Matrix<int, 4, 1> matrix{ {1},{2},{3},{4} };
	Matrix<double, 1, 5> another{ {9.9,9,8.2,8.1,7} };
	std::cout << 2 * matrix * another * 2;
	Matrix<int, 3, 3> detMatr(3);
	std::cout << "\n\n\n" << detMatr.Determinant();
	
}
