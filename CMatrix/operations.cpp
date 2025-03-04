#include "operations.h"

void rref(Eigen::MatrixXd& m) {
	Eigen::RowVectorXd rowSwap;
	int rows = static_cast<int>(m.rows());
	int cols = static_cast<int>(m.cols());

	int pivotCount = 0;

	for (int i = 0; i < cols && pivotCount < rows; i++) {
		int maxIndex = pivotCount;
		for (int j = pivotCount + 1; j < rows; j++) {
			if (fabs(m(j, i)) > fabs(m(maxIndex, i))) {
				maxIndex = j;
			}
		}
		if (m(maxIndex, i) == 0) continue;
		if (maxIndex > pivotCount) { // bring row with largest value to pivoting position
			rowSwap = m.row(pivotCount).eval();
			m.row(pivotCount) = m.row(maxIndex);
			m.row(maxIndex) = rowSwap;
		}
		for (int j = pivotCount + 1; j < rows; j++) { // zero out rows below
			m.row(j) = (m.row(j) * m(pivotCount, i) - m.row(pivotCount) * m(j, i)) / m(pivotCount, i);
		}
		pivotCount++;
	}
	for (int i = pivotCount - 1; i >= 0; i--) { // back substitution
		int pivotCol = 0;
		for (int j = 0; j < cols; j++) { // locate pivot
			if (m(i, j) != 0) {
				pivotCol = j;
				break;
			}
		}
		for (int j = i - 1; j >= 0; j--) { // zero out rows above
			m.row(j) = m.row(j) * m(i, pivotCol) - m.row(i) * m(j, pivotCol);
		}
		m.row(i) = m.row(i) / m(i, pivotCol); 
	}
	for (int i = 0; i < rows; i++) { // i don't like stupid -0.0
		for (int j = 0; j < cols; j++) {
			if (m(i, j) == -0.0) m(i, j) = 0.0;
		}
	}
}

void pLUDecomp(Eigen::MatrixXd& m, Eigen::MatrixXd& p, Eigen::MatrixXd& l, Eigen::MatrixXd& u) {
	int rows = static_cast<int>(m.rows());
	int cols = static_cast<int>(m.cols());

	Eigen::PartialPivLU<Eigen::MatrixXd> lu(m);

	p = lu.permutationP();

	l = Eigen::MatrixXd::Identity(rows, rows);
	l.block(0, 0, rows, cols).triangularView<Eigen::StrictlyLower>() = lu.matrixLU();
	
	u = lu.matrixLU().triangularView<Eigen::Upper>();
}

void lUDecomp(Eigen::MatrixXd& m, Eigen::MatrixXd& l, Eigen::MatrixXd& u) {
	int rows = static_cast<int>(m.rows());
	int cols = static_cast<int>(m.cols());

	l = Eigen::MatrixXd::Identity(rows, rows);
	u = m;

	for (int i = 0; i < cols; i++) {
		int pivotRow = i;
		for (int j = pivotRow + 1; j < rows; j++) {
			if (u(pivotRow, i) == 0) continue;
			double rowScalar = u(j, i) / u(pivotRow, i);
			u.row(j) = (u.row(j) * u(pivotRow, i) - u.row(pivotRow) * u(j, i)) / u(pivotRow, i);
			l(j, i) = rowScalar;
		}
	}

	for (int i = 0; i < cols; i++) {
		for (int j = i + 1; j < rows; j++) {
			if (u(j, i) == -0.0) u(j, i) = 0.0;
		}
	}
}
