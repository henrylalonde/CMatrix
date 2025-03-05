const char help[] =
"CMatrix: a terminal matrix calculator for quick maths\n"
"--------\n"
"Format: <command> <option> <option> <option>... etc\n"
"\n"
"Options Types:\n"
"<matrix> - a capital letter specifying one of the matrices A-H\n"
"<float> - a floating point number\n"
"<row> <column> - integers\n"
"<output> - an output matrix of the previous operation, specified 1-3\n"
"\n"
"help - displays this help message\n"
"pmat - prints all matrices, filled and unfilled\n"
"pout - print all matrices currently in output buffer 1-3\n"
"done - leave the program\n"
"\n"
"edit <matrix> - enter edit mode on the specified matrix\n"
"	Edit Mode:\n"
"	<float> - specify the value for the current entry\n"
"	d <rows> <columns> - resize matrix to specified dimensions (default 1 1)\n"
"	c <column> - edit matrix column-wise starting at specified column (default 1)\n"
"	r <row> - edit matrix row-wise starting at specified row (default 1)\n"
"	e <row> <column> - jump to edit a specific entry (default 1 1)\n"

"	I - turn matrix into identity matrix with ones along diagonal\n"
"	clr - set all entries to 0\n"
"	sv - save current matrix and exit edit mode\n"
"\n"
"add <matrix> <matrix> - matrix addition (dimensions must be equal)\n"
"mul <float> <matrix> - scalar multiplication\n"
"mul <matrix> <matrix> - matrix multiplication (columns of first = rows of second)\n"
"inv <matrix> - the inverse of the specified matrix\n"
"det <matrix> - the determinany of the matrix\n"
"rref <matrix> - reduced row echelon form of the matrix\n"
"plu <matrix> - a PLU decomposition of the matrix\n"
"lu <matrix> - the LU decomposition of the matrix\n"
"eig <matrix> - the eignvalues and eigenvectors of the matrix\n"
"\n"
"sto <output> <matrix> - store the previous output in one of the input matrices.\n"
;

const char* commands[] = {
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

#define NUM_COMMANDS sizeof(commands) / sizeof(char*)