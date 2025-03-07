// CMatrix.cpp : the beginning of an era

#include "CMatrix.h"
#define printMatrix(m)  std::cout << m << "\n" << std::endl // used for debugging
#define NUM_COMMANDS sizeof(commands) / sizeof(char*)
#define NUM_INPUT_MATRICES 8
#define NUM_OUTPUT_MATRICES 3
#define mul 6765884
#define mod 23

typedef struct cmd {
	const char *key;
	void (*exe)();
} cmd;

static const char* commands[] = {
	"help",
	"pmat",
	"pout",
	"done",
	"edit",
	"d",
	"c",
	"r",
	"e",
	"I",
	"clr",
	"sv",
	"add",
	"mul",
	"inv",
	"det",
	"rref",
	"plu",
	"lu",
	"eig",
	"sto"
};

static cmd cmdArray[mod];

static std::vector<Eigen::MatrixXd> in(NUM_INPUT_MATRICES);
static std::vector<Eigen::MatrixXd> out(NUM_OUTPUT_MATRICES);
static Eigen::MatrixXd swap;

static bool done;
static bool editing;
static bool rowWise;
static int editingMatrix;
static int editingRow;
static int editingCol;

unsigned int hashString(const char c[]) {
	unsigned int hash = 0;
	while(*c) {
		hash = hash * mul + *c;
		c++;
	}
	return hash % mod;
}

void printHashCollisions() {
	int hashes[NUM_COMMANDS];
	
	for(int i = 0; i < NUM_COMMANDS; i++) {
		hashes[i] = hashString(commands[i]);
		std::cout << commands[i] << " -> " << hashes[i] << std::endl;
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

unsigned int getCommandHash() {
	char word[5];
input:
	std::cin >> word;
	unsigned int hash = hashString(word);

	// validate match
	for (int i = 0; word[i] != '\0' || cmdArray[hash].key[i] != '\0'; i++) {
		if (word[i] != cmdArray[hash].key[i]) {
			std::cout << "Invalid command" << std::endl; 
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			goto input; 
		}
	}
	return hash;
}

// get rid of user input matrix after edit mode is implemented
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

void cmdHelp() {
	std::cout << help << std::endl;
}

void cmdPmat() {
	for (int i = 0; i < NUM_INPUT_MATRICES; i++) {
		char letter = 'A';
		std::cout << "[" << static_cast<char>(letter + i) << "] ";
		std::cout << static_cast<int>(in[i].rows()) << "x" << static_cast<int>(in[i].cols()) << std::endl;
		std::cout << in[i] << std::endl;
	}
}

void cmdPout() {
	for (int i = 0; i < NUM_OUTPUT_MATRICES; i++) {
		std::cout << "[" << i + 1 << "] ";
		std::cout << static_cast<int>(out[i].rows()) << "x" << static_cast<int>(out[i].cols()) << std::endl;
		std::cout << out[i] << std::endl;
	}
}

void cmdDone() {
	done = true;
}

void cmdEdit() {
	if (editing) {
		std::cout << "Exit current matrix before editing another" << std::endl;
		return;
	}
	char matrixI;
	std::cin >> matrixI;
	matrixI = matrixI - 'A';
	if (matrixI >= 0 && matrixI < 8) {
		editingMatrix = matrixI;
	} else {
		std::cout << "Please enter a valid matrix letter (A through H, capitalized)" << std::endl;
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
		return;
	}
	editingRow = 0;
	editingCol = 0;
	rowWise = true;
	editing = true;

	while (editing && !done) {
		double inputNum;
		int rows = static_cast<int>(in[editingMatrix].rows());
		int cols = static_cast<int>(in[editingMatrix].cols());
		std::cout << "[" << static_cast<char>(editingMatrix + 'A') << "] ";
		std::cout << rows << "x" << cols << std::endl;
		std::cout << in[editingMatrix] << std::endl; // replace with custom function that highlights current entry
		std::cout << "[" << static_cast<char>(editingMatrix + 'A') << "](" << editingRow << ", " << editingCol << ") ";
		if((std::cin >> inputNum).good()) {
			in[editingMatrix](editingRow, editingCol) = inputNum;
			if (rowWise) {
				editingCol++;
				if (editingCol >= cols) {
					editingCol = 0;
					editingRow++;
				}
			} else {
				editingRow++;
				if (editingRow >= rows) {
					editingRow = 0;
					editingCol++;
				}
			}
			if (editingRow >= rows || editingCol >= cols) {
				editingRow = 0;
				editingCol = 0;
			}
		} else {
			std::cin.clear();
			int hash = getCommandHash();
			cmdArray[hash].exe();
		}
	}
}

void cmdDimension() {
	int oldRows = static_cast<int>(in[editingMatrix].rows());
	int oldCols = static_cast<int>(in[editingMatrix].cols());

	int newRows;
	int newCols;

	if ((std::cin >> newRows).bad()) {
		std::cout << "Enter an integer number for rows" << std::endl;
		return;
	}
	if ((std::cin >> newCols).bad()) {
		std::cout << "Enter an integer number for columns" << std::endl;
		return;
	}
	swap = in[editingMatrix];
	in[editingMatrix].resize(newRows, newCols);

	for (int i = 0; i < newRows; i++) {
		for (int j = 0; j < newCols; j++) {
			if (i < oldRows && j < oldCols) {
				in[editingMatrix](i, j) = swap(i, j);
			} else {
				in[editingMatrix](i, j) = 0;
			}
		}
	}
}

void (*cmdPointers[NUM_COMMANDS])() = {
	cmdHelp,
	cmdPmat,
	cmdPout,
	cmdDone,
	cmdEdit,
	cmdDimension
};

void cmdTableInit() {
	for (int i = 0; i < NUM_COMMANDS; i++) {
		int hash = hashString(commands[i]);
		cmdArray[hash].key = commands[i];
		cmdArray[hash].exe = cmdPointers[i];
	}

}

int main()
{
	cmdTableInit();

	done = false;
	editing = false;

	while (!done) {
		unsigned int hash = getCommandHash();
		cmdArray[hash].exe();
	}
	return 0;
}
