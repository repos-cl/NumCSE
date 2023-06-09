///////////////////////////////////////////////////////////////////////////
/// Demonstration code for lecture "Numerical Methods for CSE" @ ETH Zurich
/// (C) 2016 SAM, D-MATH
/// Author(s): Thomas Etterlin <thomaset@student.ethz.ch>
/// Repository: https://gitlab.math.ethz.ch/NumCSE/NumCSE/
/// Do not remove this header.
//////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <iostream>
#include <limits>
#include <random>

#include <Eigen/Dense>
#include <Eigen/QR>
#include <figure/figure.hpp>

using namespace std;
using namespace Eigen;

int main() {
  /* SAM_LISTING_BEGIN_0 */
  //! Curing Wilkinson's counterexample by random perturbation
  MatrixXd res(20, 3);
  mt19937 gen(42); // seed
  // normal distribution, mean = 0.0, stddev = 1.0
  std::normal_distribution<> bellcurve;
  for (int n = 10; n <= 10 * 20; n += 10) {
    // Build Wilkinson matrix
    MatrixXd A(n, n); A.setIdentity();
    A.triangularView<StrictlyLower>().setConstant(-1);
    A.rightCols<1>().setOnes();
    // imposed solution
    VectorXd x = VectorXd::Constant(n, -1).binaryExpr(
        VectorXd::LinSpaced(n, 1, n),
        [](double x, double y) { return pow(x, y); });
    double relerr = (A.lu().solve(A * x) - x).norm() / x.norm();
    // Randomly perturbed Wilkinson matrix by matrix with iid
    // $N(0,{\mathrm{eps}})$ distributed entries
    MatrixXd Ap = A.unaryExpr([&](double x) {
      return x + numeric_limits<double>::epsilon() * bellcurve(gen);
    });
    double relerrp = (Ap.lu().solve(Ap * x) - x).norm() / x.norm();
    res(n / 10 - 1, 0) = n;
    res(n / 10 - 1, 1) = relerr;
    res(n / 10 - 1, 2) = relerrp;
  }
  /* SAM_LISTING_END_0 */
  // Plotting
  mgl::Figure fig;
  fig.setlog(false, true);
  fig.plot(res.col(0), res.col(1), " *m-").label("unperturbed matrix");
  fig.plot(res.col(0), res.col(2), " +r-").label("randn perturbed");
  fig.xlabel("matrix size n");
  fig.ylabel("relative error");
  fig.legend(0.05, 0.5);
  fig.save("wilkpert");
  return 0;
}
