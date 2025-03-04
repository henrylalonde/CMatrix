#pragma once

#include <Eigen/Dense>

void rref(Eigen::MatrixXd& m);

void pLUDecomp(Eigen::MatrixXd& m, Eigen::MatrixXd& p, Eigen::MatrixXd& l, Eigen::MatrixXd& u);

void lUDecomp(Eigen::MatrixXd& m, Eigen::MatrixXd& l, Eigen::MatrixXd& u);
