#ifndef MATRIXFIT_HPP
#define MATRIXFIT_HPP

////
//// Copyright (C) 2016 SAM (D-MATH) @ ETH Zurich
//// Author(s): lfilippo <filippo.leonardi@sam.math.ethz.ch>
//// Contributors: tille, jgacon, dcasati
//// This file is part of the NumCSE repository.
////
#include <Eigen/Dense>
#include <iostream>
#include <unsupported/Eigen/KroneckerProduct>

using namespace Eigen;

/* @brief Solve the minimization problem $\argmin |M|_{F}$
 * @param[in] z An $n$-dimensional vector constraining the solution $M^*$ via
 * $M^*z = g$
 * @param[in] g An $n$-dimensional vector constraining the solution $M^*$ via
 * $M^*z = g$
 * @param[out] M^* The solution to the minimization problem
 */
/* SAM_LISTING_BEGIN_1 */
MatrixXd min_frob(const VectorXd& z, const VectorXd& g) {
  assert(z.size() == g.size() && "Size mismatch!");
  unsigned int n = g.size();
  // TO DO (4-9.d): solve the augmented normal equations
  // Hint: Use Map to reshape a vector into a matrix
  // START

  // END
}
/* SAM_LISTING_END_1 */

/* @param[in] n scalar containing the length of z and g
 * @param[out] boolean = true if the formula M = gz^T/ ||z||^2 is correct
 */
/* SAM_LISTING_BEGIN_2 */
bool testMformula(unsigned int n) {
  double tol = 1e-10;
  // TO DO (4-9.e): compare the formula: return true if the relative
  // error in Frobenius norm is less than tol.
  // Hint: Define z using LinSpaced and g using replicate()
  // START

  // END
  return 0;
}
/* SAM_LISTING_END_2 */
#endif
