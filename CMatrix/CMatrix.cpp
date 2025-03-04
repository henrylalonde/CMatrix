// CMatrix.cpp : the beginning of an era

#include "CMatrix.h"
#define printMatrix(m)  std::cout << m << "\n" << std::endl // used for debugging
#define BUFSIZE 128

char buf[BUFSIZE];

void userInputMatrix(Eigen::MatrixXd& m) {
	int rows;
	int columns;

	std::cout << "Rows: ";
	std::cin >> rows;

	std::cout << "Columns: ";
	std::cin >> columns;

	m.resize(rows, columns);
	m.setZero();
	std::cout << m << std::endl;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			std::cout << "m(" << i + 1 << ", " << j + 1 << ") = ";
			std::cin >> m(i, j);
			std::cout << m << std::endl;
		}
	}

}

int main()
{
	std::vector<Eigen::MatrixXd> in(8);
	std::vector<Eigen::MatrixXd> out(3);

	userInputMatrix(in[0]);
	rref(in[0], out[0]);
	std::cout << out[0];
	
	return 0;
}
