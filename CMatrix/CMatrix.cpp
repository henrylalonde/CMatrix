// CMatrix.cpp : the beginning of an era

#include "CMatrix.h"
#define printMatrix(m)  std::cout << m << "\n" << std::endl // used for debugging
#define BUFSIZE 64
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

static char buf[BUFSIZE];

static int done;

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
	done = 1;
}

void (*cmdPointers[NUM_COMMANDS])() = {
	cmdHelp,
	cmdPmat,
	cmdPout,
	cmdDone
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
	char word[5];

	cmdTableInit();

	done = 0;

	while (!done) {
	input:
		std::cin >> buf;
		int i;
		for (i = 0; i < BUFSIZE; i++) {
			if (buf[i] == '\n' || buf[i] == '\0' || buf[i] == ' ') {
				word[i] = '\0';
				break;
			}
			word[i] = buf[i];
		}
		int hash = hashString(word);

		// validate match
		for (i = 0; word[i] != '\0' || cmdArray[hash].key[i] != '\0'; i++) {
			if (word[i] != cmdArray[hash].key[i]) {
				std::cout << "Invalid command" << std::endl; 
				// may also need to zero out buf, but we'll see
				goto input; 
			}
		}
		cmdArray[hash].exe();
	}
	return 0;
}
