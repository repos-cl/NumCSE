#ifndef GAUSSQUAD_HPP
#define GAUSSQUAD_HPP

#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

/* SAM_LISTING_BEGIN_0 */
struct QuadRule {
  Eigen::VectorXd nodes_, weights_;
};

QuadRule gaussquad(unsigned n) {
  // Initialize bidiagonal matrix \Blue{$\VJ_n$}
  Eigen::MatrixXd M = Eigen::MatrixXd::Zero(n, n);
  for (unsigned i = 1; i < n; ++i) {
    const double b = i / std::sqrt(4. * i * i - 1.);  // \Label[line]{gw:3}
    M(i, i - 1) = b;
    M(i - 1, i) = b;  // \Label[line]{gw:3x}
  }
  // \eigen's built-in helper class for eigenvalue problems
  // (use method for symmetric matrices, exploiting the structure)
  Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eig(M);

  QuadRule qr;
  qr.nodes_ = eig.eigenvalues();
  qr.weights_ =
      2 * eig.eigenvectors().topRows<1>().array().pow(2);  // \Label[line]{gw:4}
  
  return qr;
}
/* SAM_LISTING_END_0 */

#endif

