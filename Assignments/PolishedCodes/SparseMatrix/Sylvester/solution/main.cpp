#include "Sylvester.hpp"

// Kronecker product from kron.hpp.
void kron(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B,
          Eigen::MatrixXd &C) {
  // Allocate enough space for the matrix
  C = Eigen::MatrixXd(A.rows() * B.rows(), A.cols() * B.cols());

  for (unsigned int i = 0; i < A.rows(); ++i) {
    for (unsigned int j = 0; j < A.cols(); ++j) {
      // We use eigen block operations to set the values of
      // each $n \times n$ block.
      C.block(i * B.rows(), j * B.cols(), B.rows(), B.cols()) =
          A(i, j) * B;  // $\in \mathbb{R}^{(n \times n)}$
    }
  }
}

Eigen::MatrixXd sparseSPD(int n) {
  double rate = 1.0 / n;

  // R is a random sparse matrix.
  Eigen::ArrayXXd R = Eigen::ArrayXXd::Random(n, n);
  R = R * (R.abs() < rate).cast<double>();
  
  // B is diagonally dominant.
  Eigen::MatrixXd B = R.matrix();
  B.diagonal() = R.abs().matrix().rowwise().sum();
  
  // A is symmetric and strictly diagonally dominant.
  Eigen::MatrixXd A = B + B.transpose() + Eigen::MatrixXd::Identity(n, n);
  
  return A;
}

int main() {
  int n = 8;

  // Solve Sylvester equation for a simple diagonal matrix A.
  Eigen::ArrayXd diagA = Eigen::ArrayXd::LinSpaced(n, 1, n);
  Eigen::SparseMatrix<double> Xdiag = solveDiagSylvesterEq(diagA);
  std::cout << "With A = diagonal( " << diagA.transpose() << " ),\n"
            << "the solution is X = \n"
            << std::setprecision(2) << Eigen::MatrixXd(Xdiag) << std::endl
            << std::endl;

  // Create a more complicated s.p.d. sparse matrix A,
  std::srand(31);
  Eigen::MatrixXd A = sparseSPD(n);
  std::cout << "With A = \n" << A << std::endl;
  Eigen::SparseMatrix<double> As = A.sparseView();
  As.makeCompressed();

  // Test sparseKron()
  Eigen::MatrixXd C;
  kron(A, A, C);
  std::cout << "the difference between sparseKron() and kron(): "
            << (C - sparseKron(As)).norm() << std::endl
            << std::endl;

  // Solve Sylvester equation for a s.p.d. matrix A.
  Eigen::MatrixXd X = solveSpecialSylvesterEq(As);
  std::cout << "the solution is X = \n" << X << std::endl << std::endl;

  std::cout << "the error is |X + A*X*A - A| = " << (X + A * X * A - A).norm()
            << std::endl;

  return 0;
}
