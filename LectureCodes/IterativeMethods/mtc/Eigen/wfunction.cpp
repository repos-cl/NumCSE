///////////////////////////////////////////////////////////////////////////
/// Demonstration code for lecture "Numerical Methods for CSE" @ ETH Zurich
/// (C) 2022 SAM, D-MATH
/// Author(s): R. Hiptmair
/// Repository: https://gitlab.math.ethz.ch/NumCSE/NumCSE/
/// Do not remove this header.
//////////////////////////////////////////////////////////////////////////

#define _USE_MATH_DEFINES
#include <cmath>
#include <exception>
#include <iostream>
#include <vector>

double Lambert_W(double xi, int maxit = 20) {
  const double xe = xi * M_E;
  if (xe < -1.0) {
    throw std::domain_error("W-function argument out of range");
  }
  // Iteration count
  int cnt = 0;
  // Initial guess from approximation with parabola
  double w = std::sqrt(1.0 + xe) - 1.0;
  // Previous iterate
  double w_old = -1.0;
  while ((w_old != w) && (cnt < maxit)) {
    w_old = w;
    const double ew = std::exp(w);
    const double F = w * ew - xi;
    w -= F / (ew * (w + 1.0));
    cnt++;
  }
  if (cnt == maxit) {
    throw cnt;
  }
  return w;
}

void check_W(double x) {
  double W;
  try {
    W = Lambert_W(x);
  } catch (std::domain_error& err) {
    std::cerr << "W-function: argument out of range" << std::endl;
  } catch (int cnt) {
    std::cerr << "W-function: no convergence after " << cnt << " iterations"
              << std::endl;
  }
  std::cout << "W-function check for x = " << x
            << ": res = " << (W * std::exp(W) - x) << std::endl;
}

int main(int /*argc*/, char** /*argv*/) {
  const std::vector<double> xv{-0.2, 0.1, 0.5, 1.0, 2.0, 10.0};
  for (double x : xv) {
    check_W(x);
  }
  return 0;
}
