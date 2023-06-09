#ifndef ADAPTINTERP_HPP
#define ADAPTINTERP_HPP

#include <Eigen/Dense>
#include <cmath>
#include <iostream>
#include <vector>

#include "intpolyval.hpp"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

/**
 * @brief Implements the greedy algorithm as described in the problem statement.
 *
 * @tparam Function Scalar functor object providing double operator()(double)
 * const
 * @param f function
 * @param a lower interval bound
 * @param b upper interval bound
 * @param tol termination tolerance
 * @param N number of equidistant sampling points
 * @param errortab (possible) error vector
 * @return Eigen::VectorXd vector interpolation nodes
 */
/* SAM_LISTING_BEGIN_1 */
template <class Function>
Eigen::VectorXd adaptivepolyintp(Function&& f, double a, double b, double tol,
                                 unsigned int N,
                                 std::vector<double>* errortab = nullptr) {
  // Generate sampling points and evaluate $f$ there
  Eigen::VectorXd sampling_points = Eigen::VectorXd::LinSpaced(N, a, b),
                  fvals_at_sampling_points = sampling_points.unaryExpr(f);

  // TODO: (6-3.a) Implement the greedy algorithm for adaptive interpolation.
  // Ignore the errortab part of this function for now.
  // TODO: (6-3.b) save the error in errortab
  // START
  // Approximate $\max |f(x)|$
  const double maxf = fvals_at_sampling_points.cwiseAbs().maxCoeff();

  // Adaptive mesh (initial node)
  std::vector<double> t{(a + b) / 2.},          // Set of interpolation nodes
      y{static_cast<double>(f((a + b) / 2.))},  // Values at nodes
      errors;                                   // Error at nodes

  for (std::size_t i = 0; i < N; ++i) {
    // *** Step 1: interpolate with current nodes
    //   need to convert std::vector to
    //   Eigen::VectorXd to use the function intpolyval
    Eigen::Map<Eigen::VectorXd> te(t.data(), t.size());
    Eigen::Map<Eigen::VectorXd> ye(y.data(), y.size());
    Eigen::VectorXd intpolyvals_at_sampling_points =
        intpolyval(te, ye, sampling_points);

    // *** Step 2: find node where error is the largest
    Eigen::VectorXd err =
        (fvals_at_sampling_points - intpolyvals_at_sampling_points).cwiseAbs();

    // We use an Eigen "Visitor"
    // https://eigen.tuxfamily.org/dox/group__TutorialReductionsVisitorsBroadcasting.html
    int idx = 0;
    const double max = err.maxCoeff(&idx);

    // Step 3: check termination criteria
    if (max < tol * maxf) {
      return te;
    }
    // Step 4: add this node to our set of nodes
    t.push_back(sampling_points(idx));
    y.push_back(fvals_at_sampling_points(idx));
    // (6-3.b): save error
    if (errortab != nullptr) {
      errortab->push_back(max);
    }
  }
  std::cerr << "Desired accuracy could not be reached." << std::endl;
  // END
  return sampling_points;  // return all sampling points
}
/* SAM_LISTING_END_1 */

/* SAM_LISTING_BEGIN_2 */
void plotInterpolationError() {
  // Declare test functions
  auto f1 = [](double t) { return std::sin(std::exp(2 * t)); };
  auto f2 = [](double t) { return std::sqrt(t) / (1 + 16 * t * t); };

  // Test interval
  constexpr double a = 0, b = 1;
  // Get interpolation nodes
  constexpr unsigned int N = 1000;  // no. of sampling points
  constexpr double tol = 1e-6;      // tolerance

  plt::figure();

  // TODO: (6-3.c) generate the plots of error vs number of nodes for f1, f2
  // START
  Eigen::VectorXd tf1, tf2;      // nodes for f1 resp. f2
  std::vector<double> ef1, ef2;  // errors for f1 resp. f2

  tf1 = adaptivepolyintp(f1, a, b, tol, N, &ef1);
  tf2 = adaptivepolyintp(f2, a, b, tol, N, &ef2);
  Eigen::VectorXd n1 = Eigen::VectorXd::LinSpaced(ef1.size(), 1, ef1.size());
  Eigen::VectorXd n2 = Eigen::VectorXd::LinSpaced(ef2.size(), 1, ef2.size());
  // Plot
  plt::title("Error VS step");
  plt::xlabel("No. of interpolation nodes");
  plt::ylabel("$max |f(t) - I_Tf(t)|$");
  plt::semilogy(n1, ef1, "ro", {{"label", "$f_1(t) = sin(e^{2t})$"}});
  plt::semilogy(n2, ef2, "bo", {{"label", "$f_2(t) = \\sqrt{t}/(1 + 16t^2)$"}});
  plt::legend();
  // END

  plt::savefig("./cx_out/intperrplot.png");
}
/* SAM_LISTING_END_2 */

#endif
