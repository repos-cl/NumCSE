///////////////////////////////////////////////////////////////////////////
/// Demonstration code for lecture "Numerical Methods for CSE" @ ETH Zurich
/// (C) 2016 SAM, D-MATH
/// Author(s): Thomas Etterlin <thomaset@student.ethz.ch>
/// Repository: https://gitlab.math.ethz.ch/NumCSE/NumCSE/
/// Do not remove this header.
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <cmath>
#include <iomanip>
#include <iostream>

#include <Eigen/Dense>
#include <figure/figure.hpp>

#include "timer.h"

using namespace Eigen;

//! script for timing different implementations of matrix multiplications
//! no BLAS is used in Eigen!
/* SAM_LISTING_BEGIN_0 */
void mmtiming() {
  int nruns = 3, minExp = 2, maxExp = 10;
  MatrixXd timings(maxExp - minExp + 1, 5);
  for (int p = 0; p <= maxExp - minExp; ++p) {
    Timer t1, t2, t3, t4; // timer class
    int n = std::pow(2, minExp + p);
    MatrixXd A = MatrixXd::Random(n, n);
    MatrixXd B = MatrixXd::Random(n, n);
    MatrixXd C = MatrixXd::Zero(n, n);
    for (int q = 0; q < nruns; ++q) {
      // Loop based implementation no template magic
      t1.start();
      for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
          for (int k = 0; k < n; ++k)
            C(i, j) += A(i, k) * B(k, j);
      t1.stop();
      // dot product based implementation little template magic
      t2.start();
      for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
          C(i, j) = A.row(i).dot(B.col(j));
      t2.stop();
      // matrix-vector based implementation middle template magic
      t3.start();
      for (int j = 0; j < n; ++j)
        C.col(j) = A * B.col(j);
      t3.stop();
      // Eigen matrix multiplication template magic optimized
      t4.start();
      C = A * B;
      t4.stop();
    }
    timings(p, 0) = n;
    timings(p, 1) = t1.min();
    timings(p, 2) = t2.min();
    timings(p, 3) = t3.min();
    timings(p, 4) = t4.min();
  }
  std::cout << std::scientific << std::setprecision(3) << timings << std::endl;
  /* SAM_LISTING_END_0 */
  // Plotting
  mgl::Figure fig;
  fig.setFontSize(4);
  fig.setlog(true, true);
  fig.plot(timings.col(0), timings.col(1), " +r-").label("loop implementation");
  fig.plot(timings.col(0), timings.col(2), " *m-")
      .label("dot-product implementation");
  fig.plot(timings.col(0), timings.col(3), " ^b-")
      .label("matrix-vector implementation");
  fig.plot(timings.col(0), timings.col(4), " ok-")
      .label("Eigen matrix product");
  fig.xlabel("matrix size n");
  fig.ylabel("time [s]");
  fig.legend(0.05, 0.95);
  fig.save("mmtiming");
}
