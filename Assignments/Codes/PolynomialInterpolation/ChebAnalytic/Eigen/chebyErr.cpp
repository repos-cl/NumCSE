#include "../../../../../Utils/polyfit.hpp"
#include "../../../../../Utils/polyval.hpp"
#include "./intpolyval.hpp"
#include "chebyApprox.hpp"
#include <Eigen/Dense>
#include <figure/figure.hpp>
#include <vector>

using Eigen::ArrayXd;
using Eigen::VectorXd;

int main() {
  const unsigned N = 1000;
  const double maxRho = std::exp(asinh(M_PI / 3));

  std::pair<double, double> tmp = chebyApprox(maxRho, N);
  const double m = tmp.first, rho = tmp.second;

  // Test for degrees 1 to 20
  VectorXd degrees = VectorXd::LinSpaced(20, 1, 20), upperBound(20);

  for (int j = 0; j < degrees.size(); ++j) {
    upperBound(j) = m *
                    std::sqrt(2 * (1. / (maxRho * maxRho) + maxRho * maxRho)) /
                    (std::pow(rho, degrees(j) + 1) - 1);
  }

  // Compute actual error for any degree
  auto f = [](double x) { return 1. / (1 + std::exp(-3 * x)); };
  std::vector<double> errInf;

  for (int l = 0; l < degrees.size(); ++l) {
    const int n = degrees(l);
    // Cheby nodes and value of f at the nodes
    ArrayXd k = ArrayXd::LinSpaced(n + 1, 0, n);
    VectorXd t = ((2 * k + 1) / (2 * (n + 1)) * M_PI).cos().matrix(),
             y = t.unaryExpr(f);
    // Evaluate error at a couple of points
    VectorXd x = VectorXd::LinSpaced(500, -1, 1), F = x.unaryExpr(f);

    // Use barycentric formula (in intpolyval.hpp)
    VectorXd LnF;
    intpolyval(t, y, x, LnF);
    errInf.push_back((F - LnF).cwiseAbs().maxCoeff());
  }

  mgl::Figure fig;
  fig.setlog(false, true);
  fig.plot(degrees, errInf, " +r").label("Computed error");
  fig.plot(degrees, upperBound, " ^b").label("Upper bound");
  fig.xlabel("degree n");
  fig.ylabel("L^\\infty error");
  fig.legend(0, 0);
  fig.save("error1.eps");
  return 0;
}
