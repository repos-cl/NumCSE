#ifndef SHAPE_IDENT_HPP
#define SHAPE_IDENT_HPP

#include <Eigen/Dense>

/*!
 * @brief shape_ident_matrix Build matrix $\mathbf{B}$.
 * Build the overdetermined system matrix arising from the
 * point $\mathbf{x}^i$.
 * @param[in] X A $2 \times n$ matrix with model points.
 * @return The system matrix $\mathbf{B}$.
 */
/* SAM_LISTING_BEGIN_0 */
Eigen::MatrixXd shape_ident_matrix(const Eigen::MatrixXd &X) {
  assert(X.rows() == 2 && "X must have 2 rows!");

  unsigned int n = X.cols();
  Eigen::MatrixXd B = Eigen::MatrixXd::Zero(2 * n, 4);

  // TODO: (3-7.c) Build system matrix $\mathbf{B}$.

  // START

  // END

  return B;
}
/* SAM_LISTING_END_0 */

/*!
 * @brief solve_lsq Solve least square problem.
 * Build the overdetermined system, find best solution in least square sense.
 * Then return norm of residual and store in $\mathbf{A}$ the $2 \times 2$
 * linear transformation that is the LSQ solution of the system.
 * @param[in] X Model points, a $2 \times n$ Matrix.
 * @param[in] P Real points, a $2 \times n$ Matrix.
 * @param[out] A Best $2 \times 2$ linear trtansfomration (in LSQ sense).
 * @return Norm of residual.
 */
/* SAM_LISTING_BEGIN_1 */
double solve_lsq(const Eigen::MatrixXd &X, const Eigen::MatrixXd &P,
                 Eigen::MatrixXd &A) {
  assert(X.rows() == 2 && "X must have 2 rows!");
  assert(P.rows() == 2 && "P must have 2 rows!");
  assert(X.cols() == P.cols() && "P and X must have same size!");

  unsigned n = X.cols();
  double norm_of_residual = 0.;

  // TODO: (3-7.d) solve LSQ problem, return best linear approximation and
  // residual

  // START

  // END

  return norm_of_residual;
}
/* SAM_LISTING_END_1 */

//! Enum used for classification.
enum Shape { Stop, Priority };

/*!
 * @brief identify Choose if points represent a stop or priority sign.
 * Use LSQ to find best linear transformation for both models, then
 * decide which one fits the best.
 * @param[in] Xstop "Model" points for stop sign.
 * @param[in] Xpriority "Model" points for priority sign.
 * @param[in] P Real points (e.g. photographed).
 * @param[in] A Return the $2 \times 2$ linear transformation matrix.
 * @return The decided shape.
 */
/* SAM_LISTING_BEGIN_2 */
Shape identify(const Eigen::MatrixXd Xstop, const Eigen::MatrixXd Xpriority,
               const Eigen::MatrixXd &P, Eigen::MatrixXd &A) {
  // TODO: (3-7.e) Use residual do decide wether shape defines stop or priority
  // road sign.

  // START

  // END
}
/* SAM_LISTING_END_2 */

#endif
