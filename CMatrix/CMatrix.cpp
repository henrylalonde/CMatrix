// CMatrix.cpp : Defines the entry point for the application.
//

#include "CMatrix.h"

int main()
{
	Eigen::Matrix4d m;

	m << 1, 2, 3, 4,
		2, 3, 4, 5,
		3, 4, 5, 6,
		4, 5, 6, 7;

	std::cout << m << std::endl;
	return 0;
}
