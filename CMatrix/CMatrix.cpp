// CMatrix.cpp : the beginning of an era

#include "CMatrix.h"
#define printMatrix(m)  std::cout << m << "\n" << std::endl // used for debugging
#define BUFSIZE 128

char buf[BUFSIZE];

#define mul 6765884
#define mod 23

unsigned int hashString(const char c[]) {
	unsigned int hash = 0;
	while(*c) {
		hash = hash * mul + *c;
		c++;
	}
	return hash % mod;
}

void validateHashing() {
	int hashes[NUM_COMMANDS];
	
	for(int i = 0; i < NUM_COMMANDS; i++) {
		hashes[i] = hashString(commands[i]);
		std::cout << hashes[i] << std::endl;
	}

	int collisions = 0;
	for(int i = 0; i < NUM_COMMANDS; i++) {
		int check = hashes[i];
		for (int j = i + 1; j < NUM_COMMANDS; j++) {
			if (hashes[i] == hashes[j]) {
				collisions++;
				std::cout << "HIT " << commands[i] << " and " << commands[j] << " - " << hashes[i] << std::endl;
			}
		}
	}
	std::cout <<"Collisions: " << collisions << std::endl;
}


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
	


	return 0;
}
