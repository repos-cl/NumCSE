#include<iostream>
#include<Eigen/Dense>
#include "gradientflow.hpp"

using namespace Eigen;

int main() {

  double lambda = 10., T=0.1;
  int N = 10000;  
  Vector2d d, y0;
  d << 1, 0;
  y0 << 1, 0;
  std::cout << "T = " << T << ", lambda = " << lambda << std::endl;
  
  auto Y = solveGradientFlow(d, lambda, y0, T, N);
  std::cout << "Final value (exact): " << Y.back().transpose() << std::endl;
  
  double exact = Y.back()(0);
  VectorXd Ns(6);
  Ns << 10, 20, 40, 80, 160, 320;
  std::cout << "Error table: \n";
  std::cout << "N  & error norm\\n" << std::endl;
  for(int i=0; i<Ns.size(); i++) {
      N = Ns(i);
      auto Y = solveGradientFlow(d, lambda, y0, T, N);
      std::cout << N << " &\t" << std::abs(Y.back()(0) - exact) << "\t\\n" << std::endl;
  }
  
  return 0;
}
