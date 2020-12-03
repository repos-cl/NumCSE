#ifndef LASERQUAD_HPP
#define LASERQUAD_HPP
////
//// Copyright (C) 2016 SAM (D-MATH) @ ETH Zurich
//// Author(s): lfilippo <filippo.leonardi@sam.math.ethz.ch>
//// Contributors: tille, jgacon, dcasati
//// This file is part of the NumCSE repository.
////
#include <cmath>
#include <iomanip>
#include <iostream>

#include "gaussquad.hpp"
#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

//! @brief Compute $\int_a^b f(x) dx \approx \sum w_i f(x_i)$ (with scaling of w
//! and x)
//! @tparam func template type for function handle f (e.g.\ lambda function)
//! @param[in] a left boundary in [a,b]
//! @param[in] b right boundary in [a,b]
//! @param[in] f integrand
//! @param[in] Q quadrature rule
//! @return Approximation of integral $\int_a^b f(x) dx$
/* SAM_LISTING_BEGIN_1 */
template <class Function>
double evalquad(const double a, const double b, Function &&f,
                const QuadRule &Q) {
  double I = 0.;
  // TO DO: (8-4.b) Use Q to approximate the integral of f over [a,b]
  // START

  // END
  return I;
}
/* SAM_LISTING_END_1 */

//! @brief Compute double integral $\int_\Delta f(x,y) dx dy$.
//! Use nested Gauss quadrature.
//! @tparam func Template type for function handle f (e.g.\ lambda function),
//! having operator (double x, double y) -> double
//! @param[in] f integrand, f(x,y) must be defined
//! @param[in] N number of quadrature points (in each direction)
//! @return Approximation of integral $\int_\Delta f(x,b) dx dy$
/* SAM_LISTING_BEGIN_2 */
template <class Function>
double gaussquadtriangle(const Function &f, const unsigned N) {
  double I = 0.;
  // TO DO: (8-4.c) Use N-node gaussquad() to integrate f over
  // the triangle 0<=x,y, x+y<=1.
  // START

  // END
  return I;
}
/* SAM_LISTING_END_2 */

// EQUIVALENT: Loop based, copy-and-paste implementation
/* SAM_LISTING_BEGIN_9 */
template <class Function>
double gaussquadtriangle_loop(const Function &f, const unsigned N) {
  // Get nodes/weights for integral over dx and dy
  QuadRule Q;
  gaussquad(N, Q);
  // Integration over y from 0 to 1 of $g(y) := \int_0^{1-y} I(x,y) dx$
  double I = 0;
  double a = 0., b = 1.;
  for (unsigned int i = 0; i < Q.weights.size(); ++i) {
    // Find out the y at which we are
    double y = (Q.nodes(i) + 1.) * (b - a) / 2. + a;
    // Define $f_y(x)$ (y is fixed and f\_y is a function of x)
    auto f_y = [&f, &y](double x) { return f(x, y); };
    // Compute g(y) as $\int_0^{1-y} I(x,y) dx$
    I += evalquad(0, 1 - y, f_y, Q) * Q.weights(i);
  }
  // Rescale interval
  return I * (b - a) / 2.;
}
/* SAM_LISTING_END_9 */

/* SAM_LISTING_BEGIN_3 */
void convtest2DQuad(unsigned int nmax = 20) {
  // "Exact" integral
  constexpr double I_ex = 0.366046550000405;
  plt::figure();
  // TO DO: (8-4.d) Tabulate the error of gaussquadtriangle() for
  // a laser beam intensity, using n=1,2,3,...,nmax nodes.
  // START

  // END
  plt::savefig("./cx_out/convergence.png");
}
/* SAM_LISTING_END_3 */

#endif
