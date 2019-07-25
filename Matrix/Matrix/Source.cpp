#include <iostream>
#include <vector>

#include "Matrix.h"

//do not declare determinant
//remove NeededType from walker
// complex template

int main()
{
	Matrix<int, 3, 3> matrix(2);
	std::cout << matrix.Power(5) << "\n\n\n";
	std::cout << matrix.Power(5) - Matrix<float, 3, 3>(5.5) * 5 << "\n\n\n";
	std::cout << Matrix<long, 3, 3>{ {1, 2, 3}, { 4,5,6 }, { 7,8,9 }}.Power(4) << "\n\n\n";
	std::cout << (Matrix<int, 5, 4>(15) * Matrix<double, 4, 15>(2)).Transposed() << "\n\n" << std::endl;
	Matrix<std::string, 5, 5> m;
	Matrix<int, 2, 2> matr{ {4,2},{500,2} };
	std::cout << matr.Determinant();
}
