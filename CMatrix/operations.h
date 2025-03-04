#pragma once

#include <Eigen/Dense>

// const signifies input and all other parameters are output

void rref(const Eigen::MatrixXd& m, Eigen::MatrixXd& rref);

void pLUDecomp(const Eigen::MatrixXd& m, Eigen::MatrixXd& p, Eigen::MatrixXd& l, Eigen::MatrixXd& u);

void lUDecomp(const Eigen::MatrixXd& m, Eigen::MatrixXd& l, Eigen::MatrixXd& u);
