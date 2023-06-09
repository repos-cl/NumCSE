#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <Eigen/Dense>
#include <cmath>

#include "discreteconvolution.hpp"
#include "doctest.h"

TEST_SUITE("Implementing discrete convolution") {
  TEST_CASE("Eigen::VectorXd pconv_fast(()" *
            doctest::description("Check that output matches naive periodic "
                                 "convolution. Does not test efficiency.")) {
    Eigen::VectorXi N(3);
    N << 6, 16, 199;

    for (unsigned int i = 0; i < N.size(); ++i) {
      Eigen::VectorXd p =
          Eigen::ArrayXd::LinSpaced(N[i], 0.0, 1.0).cos().matrix();
      Eigen::VectorXd x =
          Eigen::ArrayXd::LinSpaced(N[i], -1.0, 1.0).exp().matrix();
      Eigen::VectorXd z = pconv_fast(p, x);

      // Correctness check
      CHECK((pconv(p, x) - z).norm() == doctest::Approx(0.).epsilon(1e-10));

      // Symmetry check
      CHECK((z - pconv_fast(x, p)).norm() ==
            doctest::Approx(0.).epsilon(1e-10));
    }
  }
}
