///////////////////////////////////////////////////////////////////////////
/// Demonstration code for lecture "Numerical Methods for CSE" @ ETH Zurich
/// (C) 2016 SAM, D-MATH
/// Author(s): Thomas Etterlin <thomaset@student.ethz.ch>
/// Repository: https://gitlab.math.ethz.ch/NumCSE/NumCSE/
/// Do not remove this header.
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <Eigen/Dense>
#include <cassert>

namespace lufak {


using Eigen::MatrixXd;
using Eigen::Index;

inline
/* SAM_LISTING_BEGIN_0 */
//! Algorithm of Crout: LU-factorization of \Blue{$\VA\in\bbK^{n,n}$}
std::pair<MatrixXd, MatrixXd> lufak(const MatrixXd &A) {
  Index n = A.rows(); 
  assert(n == A.cols()); // Ensure matrix is square
  MatrixXd L{MatrixXd::Identity(n, n)};
  MatrixXd U{MatrixXd::Zero(n, n)};
  for (Index k = 0; k < n; ++k) {
    // Compute row of \Blue{$\VU$}
    for (Index j = k; j < n; ++j) {
      U(k, j) = A(k, j) - (L.block(k, 0, 1, k) * U.block(0, j, k, 1))(0, 0);
    }
    // Compute column of \Blue{$\VL$}
    for (Index i = k + 1; i < n; ++i) {
      L(i, k) = (A(i, k) - (L.block(i, 0, 1, k) * U.block(0, k, k, 1))(0, 0)) /
                U(k, k);
    }
  }
  return { L, U };
}
/* SAM_LISTING_END_0 */


} //namespace lufak

