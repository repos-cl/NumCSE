///////////////////////////////////////////////////////////////////////////
/// Demonstration code for lecture "Numerical Methods for CSE" @ ETH Zurich
/// (C) 2016 SAM, D-MATH
/// Author(s): Thomas Etterlin <thomaset@student.ethz.ch>
/// Repository: https://gitlab.math.ethz.ch/NumCSE/NumCSE/
/// Do not remove this header.
//////////////////////////////////////////////////////////////////////////

#include <Eigen/Dense>
#include <iostream>

#include "zerosquadpolstab.hpp"

int main () {
	double alpha = 1;
	double beta = -1;
	std::cout << zerosquadpolstab::zerosquadpolstab(alpha, beta) << std::endl;
	return 0;
}
