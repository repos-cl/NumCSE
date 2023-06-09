#include <iostream>
#include <iomanip>

#include <Eigen/Dense>

using namespace Eigen;

/* \brief Use symmetric Gauss-Seidel iterations to solve the system Ax = b
 * \param[in] A system matrix to be decompsed (L + D + U)
 * \param[in] b r.h.s. vector
 * \param[in,out] x initial guess and last iterate (approximated solution)
 * \param[in] rtol relative tolerance for termination criteria
 */
void GSIt(const MatrixXd & A, const VectorXd & b,
          VectorXd & x, double rtol) {
    /* SAM_LISTING_BEGIN_1 */
    const auto U = TriangularView<const MatrixXd, StrictlyUpper>(A);
    const auto L = TriangularView<const MatrixXd, StrictlyLower>(A);

    const auto UpD = TriangularView<const MatrixXd, Upper>(A);
    const auto LpD = TriangularView<const MatrixXd, Lower>(A);

    // A temporary vector to store result of iteration
    VectorXd temp(x.size());

    // We'll use pointer magic to
    VectorXd* xold = &x;
    VectorXd* xnew = &temp;

    // Iteration counter
    unsigned int k = 0;
    double err;

#if VERBOSE
        std::cout << std::setw(10) << "it."
                  << std::setw(15) << "err" << std::endl;
#endif // VERBOSE
    do {
        // Compute next iteration step
        *xnew = UpD.solve(b) - UpD.solve(L*LpD.solve(b - U * (*xold) ));

        // Absolute error
        err = (*xold - *xnew).norm();
#if VERBOSE
        std::cout << std::setw(10) << k++
                  << std::setw(15) << std::setprecision(3) << std::scientific
                  << err << std::endl;
#endif // VERBOSE

        // Swap role of previous/next iteration
        std::swap(xold, xnew);
    } while( err > rtol * (*xnew).norm() );

    x = *xnew;
    /* SAM_LISTING_END_1 */

    return;
}

int main(int, char**) {
    unsigned int n = 9;

    MatrixXd A = MatrixXd::Zero(n,n);
    for(unsigned int i = 0; i < n; ++i) {
        if(i > 0) A(i,i-1) = 2;
        A(i,i) = 3;
        if(i < n-1) A(i,i+1) = 1;
    }
    VectorXd b = VectorXd::Constant(n, 1);

    //// Subproblem b: test GSIt
    std::cout << "--> Test GSIt" << std::endl;

    /* SAM_LISTING_BEGIN_2 */
    VectorXd x = b;
    GSIt(A, b, x, 10e-8);

    double residual = (A*x - b).norm();

    std::cout << "Residual = " << residual << std::endl;
    /* SAM_LISTING_END_2 */
}
