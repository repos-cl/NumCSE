#ifndef MATRIXFIT_HPP
#define MATRIXFIT_HPP

//
// Copyright (C) 2016 SAM (D-MATH) @ ETH Zurich
// Author(s): lfilippo <filippo.leonardi@sam.math.ethz.ch>
// Contributors: tille, jgacon, dcasati
// This file is part of the NumCSE repository.
//

#include <Eigen/Dense>
#include <iostream>
#include <unsupported/Eigen/KroneckerProduct>

/**
 * @brief Solve the minimization problem $\argmin |M|_{F}$
 *
 * @param z An $n$-dimensional vector constraining the solution $M^*$ via
 * $M^*z = g$
 * @param g An $n$-dimensional vector constraining the solution $M^*$ via
 * $M^*z = g$
 * @return Eigen::MatrixXd M^* The solution to the minimization problem
 */
/* SAM_LISTING_BEGIN_1 */
Eigen::MatrixXd min_frob(const Eigen::VectorXd &z, const Eigen::VectorXd &g) {
  assert(z.size() == g.size() && "Size mismatch!");
  const unsigned int n = g.size();
  Eigen::MatrixXd M_mat = Eigen::MatrixXd::Zero(n, n);
  // TODO: (3-9.d) solve the augmented normal equations
  // Hint: Use Map to reshape a vector into a matrix
  // START
  // Build temporary matrix $\mathbf{C}$.
  Eigen::MatrixXd C =
      kroneckerProduct(Eigen::MatrixXd::Identity(n, n), z.transpose());
  Eigen::VectorXd minus_p = (C * C.transpose()).ldlt().solve(g);
  Eigen::VectorXd M = C.transpose() * minus_p;
  M_mat = Eigen::Map<Eigen::MatrixXd>(M.data(), n, n).transpose();
  // END
  return M_mat;
}
/* SAM_LISTING_END_1 */

/**
 * @brief Tests min_frob
 *
 * @param n scalar containing the length of z and g
 * @return true if the formula M = gz^T/ ||z||^2 is correct
 * @return false otherwise
 */
/* SAM_LISTING_BEGIN_2 */
bool testMformula(unsigned int n) {
  constexpr double tol = 1e-10;
  // TODO: (3-9.e) compare the formula: return true if the relative
  // error in Frobenius norm is less than tol.
  // Hint: Define z using LinSpaced and g using replicate()
  // START
  Eigen::VectorXd z = Eigen::VectorXd::LinSpaced(n, 1, n);
  Eigen::Vector2d v = {1, -1};
  Eigen::VectorXd g = v.replicate(static_cast<int>(n) / 2, 1);

  Eigen::MatrixXd Mstar = min_frob(z, g);

  Eigen::MatrixXd M = g * z.transpose() / z.squaredNorm();  // $M$
  // Safe test for realtive smallness of error
  return ((Mstar - M).norm() < tol * M.norm());
  // END
  return false;
}
/* SAM_LISTING_END_2 */
#endif
