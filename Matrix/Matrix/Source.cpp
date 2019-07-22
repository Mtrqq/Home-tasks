#include <iostream>
#include <vector>

#include "Matrix.h"

int main()
{
	Matrix<int, 3, 3> detMatr{};
	std::cout << detMatr.Determinant() << std::endl;
	system("pause");
	
}
