///////////////////////////////////////////////////////////////////////////
/// Demonstration code for lecture "Numerical Methods for CSE" @ ETH Zurich
/// (C) 2016 SAM, D-MATH
/// Author(s): Thomas Etterlin <thomaset@student.ethz.ch>
/// Repository: https://gitlab.math.ethz.ch/NumCSE/NumCSE/
/// Do not remove this header.
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <cmath>

#include <Eigen/Dense>

using namespace Eigen;

/* SAM_LISTING_BEGIN_0 */
//! C++ function computing the zeros of a quadratic polynomial
//! $\xi\to \xi^2+\alpha\xi+\beta$ by means of the familiar discriminant
//! formula $\xi_{1,2} = \frac{1}{2}(-\alpha\pm\sqrt{\alpha^2-4\beta})$. However
//! this implementation is \emph{vulnerable to round-off}! The zeros are
//! returned in a column vector
Vector2d zerosquadpol(double alpha, double beta) {
  Vector2d z;
  double D = std::pow(alpha, 2) - 4 * beta; // discriminant
  if (D < 0)
    throw "no real zeros";
  else {
    // The famous discriminant formula
    double wD = std::sqrt(D);
    z << (-alpha - wD) / 2, (-alpha + wD) / 2; // \Label[line]{zsq:1}
  }
  return z;
}
/* SAM_LISTING_END_0 */
