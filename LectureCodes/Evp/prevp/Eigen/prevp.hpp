// #define EIGEN_USE_MKL_ALL

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/Eigenvalues>
#include <figure.hpp>
#include "graphMarketMatrixLoader.hpp"
#include "prbuildA.hpp"

void prevp(std::string path)
{
	// load connectivity matrix and build transition matrix
	Eigen::MatrixXi G;
	loadGraphMarketMatrix(G, path);
	Eigen::MatrixXd A = prbuildA(G, 0.15);

	Eigen::EigenSolver<Eigen::MatrixXd> ev(A);
	auto eigenVectors = ev.eigenvectors();

	std::cout << ev.eigenvalues() << std::endl;
}
