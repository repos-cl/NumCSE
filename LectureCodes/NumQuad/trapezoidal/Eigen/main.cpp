#include "trapezoidal.hpp"
#include <iostream>

int main() {
  auto f = [](double x) { return x * x; };
  auto g = [](double x) { return 10 * x; };
  const double a = 0, b = 1;
  const unsigned N = 1000;

  for (unsigned n = 2; n < N; n *= 4) {
    std::cout << "=========== " << n << " steps ==========\n";
    std::cout << "x^2 on [0,1]: " << trapezoidal(f, a, b, n) << " <-> "
              << equidTrapezoidalRule(f, a, b, n+1) << "\n";
    std::cout << "10x on [0,1]: " << trapezoidal(g, a, b, n) << " <-> "
              << equidTrapezoidalRule(g, a, b, n+1) << "\n";
  }

  return 0;
}
