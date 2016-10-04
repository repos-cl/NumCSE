#include <iostream>
#include <vector>

#include <Eigen/Dense>
#include <Eigen/Sparse>

using namespace Eigen;

/* @brief Compute the matrix $C$ from $A$
 * @param[in] A An $n \times n$ matrix
 * @param[out] C The $(n^2) \times (n^2)$ matrix
 * from $A\otimes I+I\otimes A$
 */
/* SAM_LISTING_BEGIN_0 */
SparseMatrix<double> buildC(const MatrixXd &A)
{
    // Initialization
    int n = A.rows();
    SparseMatrix<double> C(n*n,n*n);
    std::vector<Triplet<double> > triplets;
    MatrixXd I = MatrixXd::Identity(n,n);

    for(int i=0; i<n; i++) {
      for(int j=0; j<n; j++) {
	// Set diagonal
        if(i == j) {
          for(int k1=0; k1<n; k1++) {
            for(int k2=0; k2<n; k2++) {
              Triplet<double>
              triplet(i*n+k1, j*n+k2, A(i,j)*I(k1,k2)+A(k1,k2));
              triplets.push_back(triplet);
             }
          }
	// Set other cells
        } else {
          for(int k=0; k<n ; k++) {
            Triplet<double> triplet(i*n+k,j*n+k,A(i,j));
            triplets.push_back(triplet);
          }
        }
      }
    }
    C.setFromTriplets(triplets.begin(), triplets.end());
    C.makeCompressed();

    return C;
}
/* SAM_LISTING_END_0 */

/* @brief Solve the Lyapunov system
 * @param[in] A An $n \times n$ matrix
 * @param[out] X The $n \times n$ solution matrix
 */
/* SAM_LISTING_BEGIN_1 */
void solveLyapunov(const MatrixXd &A, MatrixXd &X)
{
    // Initialization
    int n = A.rows();
    SparseMatrix <double> C = buildC(A);
    MatrixXd I = MatrixXd::Identity(n,n);
    VectorXd b(n*n);
    b = Map<MatrixXd>(I.data(),n*n,1);
    VectorXd vecX(n*n);

    SparseLU<SparseMatrix <double> > solver;
    solver.compute(C);
    vecX = solver.solve(b);
    X = Map<MatrixXd>(vecX.data(),n,n);
}
/* SAM_LISTING_END_1 */

/* SAM_LISTING_BEGIN_2 */
int main() {
    // Initialization
    unsigned int n = 5;
    MatrixXd A(n,n);
    A << 10, 2, 3, 4, 5,
         6, 20, 8, 9, 1,
         1, 2, 30, 4, 5,
         6, 7, 8, 20, 0,
         1, 2, 3, 4, 10;

    // Test 'buildC'
    SparseMatrix <double> C = buildC(A);
    std::cout << "C = " << C << std::endl;

    // Test 'solveLyapunov'
    MatrixXd X(n,n);
    solveLyapunov(A,X);
    std::cout << "X = " << X << std::endl;

    // Verify the solution if you obtain zero
    MatrixXd I = MatrixXd::Identity(n,n);
    std::cout << "Correct if close to 0: "
              << (A*X + X*A.transpose() - I).norm()
              << std::endl;
}
/* SAM_LISTING_END_2 */
