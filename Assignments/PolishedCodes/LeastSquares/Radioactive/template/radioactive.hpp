#ifndef RADIOACTIVE_HPP
#define RADIOACTIVE_HPP

#include <Eigen/Dense>
#include <vector>

#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

/**
 * @brief Implements the Gauss-Newton method with absolute stopping criterion
 *
 * @tparam F
 * @tparam DF
 * @param[inout] x Eigen::Vector4d = (A_0, B_0, lambda_1, lambda_2), on call:
 * initial guess, on return: x s.t. f(x) = 0
 * @param f the function s.t. f(x) = 0
 * @param df the Jacobian of f
 * @param tol absolute tolerance for stopping, defaults to 1e-14
 * @return std::vector<double> the L_inf norms of the update steps
 */
/* SAM_LISTING_BEGIN_2 */
template <typename F, typename DF>
std::vector<double> GaussNewton(Eigen::Vector4d& x, F&& f, DF&& df,
                                const double tol = 1e-14) {
  std::vector<double> gn_update;

  // TODO: (9-11.d) Implement the Newton iteration for the given Least squares
  // problem. Put in gn_update the norms of the iteration steps for further
  // investigation. START

  // END
  return gn_update;
}
/* SAM_LISTING_END_2 */

/**
 * @brief Plots the fitted $\Phi_B$ along with the measured data. Also plots
 * the norms of the iteration updates.
 *
 * @param t Eigen::VectorXd time values
 * @param m Eigen::VectorXd corresponding measured values of $\Phi_B$
 * @param x Eigen::VectorXd s.t. F(x) = 0, i.e. the solution given by the
 * Newton iteration
 * @param gn_update std::vector<double> norms of the update steps
 */
/* SAM_LISTING_BEGIN_3 */
void plot(const Eigen::ArrayXd& t, const Eigen::VectorXd& m,
          const Eigen::VectorXd& x, const std::vector<double>& gn_update) {
  plt::figure();
  // TODO: (9-11.d) Plot the fitted PhiB along with the measurements using
  // matplotlibcpp
  // START

  // END
  plt::savefig("./cx_out/solution.png");  //! TODO
  plt::figure();
  // TODO: (9-11.d) Plot the iteration error using matplotlibcpp.
  // START

  // END
  plt::savefig("./cx_out/convergence.png");  //! TODO
}
/* SAM_LISTING_END_3 */

#endif
