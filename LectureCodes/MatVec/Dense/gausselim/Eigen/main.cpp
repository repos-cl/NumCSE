#include <iostream>
#include <Eigen/Dense>
#include <cmath>
using namespace std;
using namespace Eigen;
#include "gausselim.hpp"

int main () {
	int n = 3;
	int m = 4;
	MatrixXd A(n,n);
	MatrixXd B(n,m);
	A << 	1, 0, 2,
			-1, 4, 1,
			-2, 1, 2;
			
	B << 	1, 4, 5, 6,
			7, 8, 2, 4,
			9, 0, 1, 9;
	std::cout << "A=\n" << A << std::endl << "B=\n"<< B << std::endl;
	MatrixXd X(n,m);
	gausselimsolvemult(A, B, X);
	std::cout << "X=\n" << X << std::endl;
	return 0;
}