#include <Eigen/Dense>
#include <iostream>

#define NDEBUG
#include "timer.h"
#include "toeplitz.hpp"

using namespace Eigen;

int main() {
  int n;

  // Initialization
  n = 3;
  VectorXd c(n), r(n), x(n);
  c << 1, 2, 3;
  r << 1, 4, 5;
  // Test toeplitz
  std::cout << "T = " << std::endl;
  std::cout << toeplitz(c, r) << std::endl;
  x << 6, 7, 8;

  // Compute with both functions toepmatmult and toepmult
  std::cout << "Check that toepmatmult and toepmult are correct" << std::endl;
  VectorXd y_1 = toepmatmult(c, r, x);
  VectorXd y_2 = toepmult(c, r, x);
  std::cout << "Error = " << (y_1 - y_2).norm() << std::endl;

  // Initialization
  n = 4;
  VectorXd h(n), y(n);
  h << 1, 2, 3, 4;
  y << 5, 6, 7, 8;

  // Compute with both functions ttmatsolve and ttrecsolve
  std::cout << "Check that ttmatsolve and ttrecsolve are correct" << std::endl;
  VectorXd x_1 = ttmatsolve(h, y);
  VectorXd x_2 = ttrecsolve(h, y, 2);
  std::cout << "Error = " << (x_1 - x_2).norm() << std::endl;

  // compute and print the runtime comparsion for toepmatmult vs toepmult
  // and for ttmatsolve vs ttrecsole
  std::cout << "Runtime comparison of "
            << "toepmatmult vs toepmult and ttmatsolve vs ttrecsolve"
            << std::endl;

  // call runtime computation
  runtime_toeplitz();

  // Test ttsolve
  std::cout << "Testing ttsolve:" << std::endl;
  VectorXd k(5), l(5), sol(5);
  k << 1, 2, 3, 4, 5;
  l << 6, 7, 8, 9, 10;
  sol << 6, -5, 0, 0, 0;

  x = ttsolve(k,l);
  std::cout << "x =" << std::endl;
  std::cout << x << std::endl;
  if(x.size() == sol.size() && (x-sol).norm() < 1e-9 )
    std::cout << "Test for ttsolve passed!" << std::endl;
  else
    std::cout << "Test for ttsolve failed!" << std::endl;
}
