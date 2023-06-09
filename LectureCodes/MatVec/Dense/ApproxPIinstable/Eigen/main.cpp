///////////////////////////////////////////////////////////////////////////
/// Demonstration code for lecture "Numerical Methods for CSE" @ ETH Zurich
/// (C) 2016 SAM, D-MATH
/// Author(s): Thomas Etterlin <thomaset@student.ethz.ch>
/// Repository: https://gitlab.math.ethz.ch/NumCSE/NumCSE/
/// Do not remove this header.
//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>

#include <Eigen/Dense>

#include "ApproxPIinstable.hpp"

int main () {
	std::cout << setprecision(15) << std::fixed;
	Eigen::MatrixXd res = ApproxPIinstable();
	std::cout << res << std::endl;
	return 0;
}
