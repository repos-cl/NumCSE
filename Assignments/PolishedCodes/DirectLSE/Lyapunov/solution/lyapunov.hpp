#ifndef LYAPUNOV_HPP
#define LYAPUNOV_HPP

#include <iostream>
#include <vector>

#include <Eigen/Dense>
#include <Eigen/Sparse>

/**
 * @brief Compute the matrix $C$ from $A$
 * 
 * @param A An $n \times n$ matrix 
 * @return Eigen::SparseMatrix<double> The $(n^2) \times (n^2)$ matrix from $A\otimes I+I\otimes A$
 */
/* SAM_LISTING_BEGIN_0 */
Eigen::SparseMatrix<double> buildC(const Eigen::MatrixXd &A) {
  // Initialization
  const unsigned int n = A.rows();
  Eigen::SparseMatrix<double> C(n * n, n * n);
  std::vector<Eigen::Triplet<double>> triplets;
  Eigen::MatrixXd I = Eigen::MatrixXd::Identity(n, n);

  // Iterate over n*n blocks.
  for (unsigned int i = 0; i < n; ++i) {
    for (unsigned int j = 0; j < n; ++j) {
      // TODO: (2-8.e) Fill in the (i,j)-th block of C.
      // Hint: The vector triplets should contain the position
      // and value the non-zero entries of C.
      // Use triplets.push_back() to add a new
      // Triplet<double> to the list.
      // START
      // Set diagonal
      if (i == j) {
        for (unsigned int k1 = 0; k1 < n; ++k1) {
          for (unsigned int k2 = 0; k2 < n; ++k2) {
            Eigen::Triplet<double> triplet(i * n + k1, j * n + k2,
                                    A(i, j) * I(k1, k2) + A(k1, k2));
            triplets.push_back(triplet);
          }
        }
        // Set other cells
      } else {
        for (unsigned int k = 0; k < n; ++k) {
          Eigen::Triplet<double> triplet(i * n + k, j * n + k, A(i, j));
          triplets.push_back(triplet);
        }
      }
      // END
    }
  }
  C.setFromTriplets(triplets.begin(), triplets.end());
  C.makeCompressed();
  return C;
}
/* SAM_LISTING_END_0 */

/**
 * @brief Solve the Lyapunov system
 * 
 * @param A An $n \times n$ matrix
 * @param X The $n \times n$ solution matrix
 */
/* SAM_LISTING_BEGIN_1 */
void solveLyapunov(const Eigen::MatrixXd &A, Eigen::MatrixXd &X) {
  // To solve A*X + X*A^T = I, we must first write
  // the LSE in "canonical form", C*vecX = b.
  const unsigned int n = A.rows();
  Eigen::SparseMatrix<double> C;
  Eigen::MatrixXd I = Eigen::MatrixXd::Identity(n, n);
  Eigen::VectorXd b(n * n);
  Eigen::VectorXd vecX(n * n);

  // TODO: (2-8.f) Fill in the entries of C, b, and vecX.
  // Hint: Use SparseLU to solve the LSE.
  // START
  C = buildC(A);
  b = Eigen::Map<Eigen::MatrixXd>(I.data(), n * n, 1);
  Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
  solver.compute(C);
  vecX = solver.solve(b);
  // END

  X = Eigen::Map<Eigen::MatrixXd>(vecX.data(), n, n);
}
/* SAM_LISTING_END_1 */

/* SAM_LISTING_BEGIN_2 */
bool testLyapunov() {
  // Initialization
  constexpr unsigned int n = 5;
  Eigen::MatrixXd A(n, n);
  A << 10, 2, 3, 4, 5, 6, 20, 8, 9, 1, 1, 2, 30, 4, 5, 6, 7, 8, 20, 0, 1, 2, 3,
      4, 10;

  constexpr double tol = 1e-8;
  bool works = false;

  // TODO: (2-8.g)
  // i) Print the full system matrix C (with zeroes),
  // ii) print the solution X, and
  // iii) return true if and only if the error defined
  // by || A*X + X*A^T - I || is less than tol.
  // START
  // Test 'buildC'
  Eigen::SparseMatrix<double> C = buildC(A);
  std::cout << "C = " << C << std::endl;

  // Test 'solveLyapunov'
  Eigen::MatrixXd X(n, n);
  solveLyapunov(A, X);

  std::cout << "X = " << X << std::endl;

  // Verify the solution if you obtain zero
  Eigen::MatrixXd I = Eigen::MatrixXd::Identity(n, n);
  double error = (A * X + X * A.transpose() - I).norm();
  std::cout << "Correct if close to 0: " << error << std::endl;

  works = (error < tol);
  // END

  return works;
}
/* SAM_LISTING_END_2 */

#endif
