// CMatrix.cpp : the beginning of an era

#include "CMatrix.h"
#define printMatrix(m)  std::cout << m << "\n" << std::endl // used for debugging
#define flushCin() std::cin.clear(); std::cin.ignore(INT_MAX, '\n')
#define NUM_COMMANDS sizeof(commands) / sizeof(char*)
#define NUM_INPUT_MATRICES 8
#define NUM_OUTPUT_MATRICES 3
#define MAX_PRINT_LENGTH 11
#define mul 6765884
#define mod 23

typedef struct cmd {
	const char *key;
	void (*exe)();
} cmd;

static cmd cmdArray[mod];

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

static bool done;
static bool editing;
static bool rowWise;
static int editingMatrix;
static int editingRow;
static int editingCol;

static std::vector<Eigen::MatrixXd> in(NUM_INPUT_MATRICES);
static std::vector<Eigen::MatrixXd> out(NUM_OUTPUT_MATRICES);
static Eigen::MatrixXd swap;

void printEditInterface() {
	int rows = static_cast<int>(in[editingMatrix].rows());
	int cols = static_cast<int>(in[editingMatrix].cols());
	std::cout << "\nMatrix[" << static_cast<char>(editingMatrix + 'A') << "] ";
	std::cout << rows << "x" << cols << std::endl;
	std::cout << in[editingMatrix] << std::endl; // replace with custom function that highlights current entry
	std::cout << "[" << static_cast<char>(editingMatrix + 'A') << "](" << editingRow + 1 << ", " << editingCol + 1 << ") ";
}

int alphaToIndex(const char c) {
	if (c - 'A' >= 0 && c - 'A' < NUM_INPUT_MATRICES) {
		return c - 'A';
	} else if (c - 'a' >= 0 && c - 'a' < NUM_INPUT_MATRICES) {
		return c - 'a';
	} else {
		return -1;
	}
}

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
			flushCin();
			if (editing) printEditInterface();
			goto input; 
		}
	}
	return hash;
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
		flushCin();
		return;
	}
	if (std::cin.peek() == '\n') {
		std::cout << "Enter matrix letter A through H: " << std::endl;
	}
	char matrixI;
	std::cin >> matrixI;
	editingMatrix = alphaToIndex(matrixI);
	if (editingMatrix == -1) {
		std::cout << "Please enter a valid matrix letter (A to H or a to h)" << std::endl;
		flushCin();
		return;
	}
	editingRow = 0;
	editingCol = 0;
	rowWise = true;
	editing = true;

	while (editing && !done) {
		printEditInterface();

		double inputNum;
		int rows = static_cast<int>(in[editingMatrix].rows());
		int cols = static_cast<int>(in[editingMatrix].cols());
		if((std::cin >> inputNum).good()) {
			if (editingRow < rows && editingCol < cols) {
				in[editingMatrix](editingRow, editingCol) = inputNum;
			}
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
	std::cout << "Edit mode exited\n" << std::endl;
}

void cmdDimension() {
	if (!editing) {
		std::cout << "Edit a matrix to change dimensions" << std::endl;
		flushCin();
		return;
	}
	int oldRows = static_cast<int>(in[editingMatrix].rows());
	int oldCols = static_cast<int>(in[editingMatrix].cols());

	int newRows;
	int newCols;

	if (std::cin.peek() == '\n') {
		std::cout << "Enter number of rows: " << std::endl;
	}
	if ((std::cin >> newRows).bad()) {
		std::cout << "Invalid number of rows" << std::endl;
		flushCin();
		return;
	}
	if (std::cin.peek() == '\n') {
		std::cout << "Enter number of columns: " << std::endl;
	}
	if ((std::cin >> newCols).bad()) {
		std::cout << "Invalid number of columns" << std::endl;
		flushCin();
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

void cmdColumn() {
	if (!editing) {
		std::cout << "Edit a matrix to select column" << std::endl;
		flushCin();
		return;
	}
	int temp;
	int cols = static_cast<int>(in[editingMatrix].cols());
		
	if (std::cin.peek() == '\n') {
		rowWise = false;
		editingRow = 0;
		editingCol = 0;
		return;
	}
	if ((std::cin >> temp).bad()) {
		std::cout << "Invalid column number" << std::endl;
		flushCin();
		return;
	}
	if (temp > cols || temp < 1) {
		std::cout << "Matrix does not contain column " << temp << std::endl;
		flushCin();
		return;
	}
	
	rowWise = false;
	editingRow = 0;
	editingCol = temp - 1;
}

void cmdRow() {
	if (!editing) {
		std::cout << "Edit a matrix to select row" << std::endl;
		flushCin();
		return;
	}
	int temp;
	int rows = static_cast<int>(in[editingMatrix].rows());

	if (std::cin.peek() == '\n') {
		rowWise = true;
		editingRow = 0;
		editingCol = 0;
		return;
	}
	if ((std::cin >> temp).bad()) {
		std::cout << "Invalid row number" << std::endl;
		flushCin();
		return;
	}
	if (temp > rows || temp < 1) {
		std::cout << "Matrix does not contain contain row " << temp << std::endl;
		flushCin();
		return;
	}
	rowWise = true;
	editingCol = 0;
	editingRow = temp - 1;
}

void cmdEntry() {
	if (!editing) {
		std::cout << "Edit a matrix to select entry" << std::endl;
		flushCin();
		return;
	}
	int row;
	int col;

	if (std::cin.peek() == '\n') {
		std::cout << "Enter row of the desired entry: " << std::endl;
	}
	if ((std::cin >> row).bad()) {
		std::cout << "Invalid row number" << std::endl;
		flushCin();
		return;
	}
	if (std::cin.peek() == '\n') {
		std::cout << "Enter column of the desired entry " << std::endl;
	}
	if ((std::cin >> col).bad()) {
		std::cout << "Invalid number of columns" << std::endl;
		flushCin();
		return;
	}

	int rows = static_cast<int>(in[editingMatrix].rows());
	int cols = static_cast<int>(in[editingMatrix].cols());

	if (row > rows || row < 1 || col > cols || col < 1) {
		std::cout << "No entry at (" << row << ", " << col << ")" << std::endl;
		return;
	}
	editingRow = row - 1;
	editingCol = col - 1;
}

void cmdIdentity() {
	if (!editing) {
		std::cout << "Edit a matrix to fill in with the identity" << std::endl;
		return;
	}
	int rows = static_cast<int>(in[editingMatrix].rows());
	int cols = static_cast<int>(in[editingMatrix].cols());

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (i == j) {
				in[editingMatrix](i, j) = 1.0;
			} else {
				in[editingMatrix](i, j) = 0.0;
			}
		}
	}
	editingRow = 0;
	editingCol = 0;
}

void cmdClear() {
	if (!editing) {
		std::cout << "Edit a matrix to clear it" << std::endl;
		return;
	}
	int rows = static_cast<int>(in[editingMatrix].rows());
	int cols = static_cast<int>(in[editingMatrix].cols());

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			in[editingMatrix](i, j) = 0.0;
		}
	}
	editingRow = 0;
	editingCol = 0;
}

void cmdSave() {
	if (!editing) {
		std::cout << "Edit a matrix to save it" << std::endl;
		return;
	}
	std::cout << "Matrix saved. Exiting edit mode..." << std::endl;
	editing = false;
}

void cmdAdd() {
	if (editing) {
		std::cout << "Save matrix before doing operations" << std::endl;
		return;
	}
	char m1;
	char m2;

	if (std::cin.peek() == '\n') {
		std::cout << "Specify first operand (a matrix A to H or a to h)" << std::endl;
	}
	std::cin >> m1;
	m1 = alphaToIndex(m1);
	if (m1 == -1) {
		std::cout << "Invalid first operand" << std::endl;
		flushCin();
		return;
	}
	if (std::cin.peek() == '\n') {
		std::cout << "Specify second operand (a matrix A to H or a to h)" << std::endl;
	}
	std::cin >> m2;
	m2 = alphaToIndex(m2);
	if (m2 == -1) {
		std::cout << "Invalid second operand" << std::endl;
		flushCin();
		return;
	}

	if (in[m1].rows() != in[m2].rows() || in[m1].cols() != in[m2].cols()) {
		std::cout << "Matrices must have same dimensions" << std::endl;
		flushCin();
		return;
	}
	
	out[0] = in[m1] + in[m2];
	std::cout << out[0] << std::endl;
}

void (*cmdPointers[NUM_COMMANDS])() = {
	cmdHelp,
	cmdPmat,
	cmdPout,
	cmdDone,
	cmdEdit,
	cmdDimension,
	cmdColumn,
	cmdRow,
	cmdEntry,
	cmdIdentity,
	cmdClear,
	cmdSave,
	cmdAdd
};
const char* noCmd = "\n";

void cmdTableInit() {
	for (int i = 0; i < mod; i++) {
		cmdArray[i].key = noCmd;
		cmdArray[i].exe = 0;
	}
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
