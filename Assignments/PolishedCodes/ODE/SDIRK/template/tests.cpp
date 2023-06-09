#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <Eigen/Dense>
#include <vector>

#include "copy.hpp"
#include "doctest.h"

struct TestData {
  TestData() {
    z0 << 1.0, 0.0;
    gamma = {1.0, (3 + std::sqrt(3)) / 6.0};
    T = {5.0, 6.0, 7.0, 9.0, 10.0};
    N = {20, 40, 80, 160, 320};
  }

  Eigen::Vector2d z0;
  double h;
  std::vector<double> gamma;
  std::vector<double> T;
  std::vector<unsigned int> N;
};

TestData data;

TEST_SUITE("SDIRK") {
  TEST_CASE("Eigen::Vector2d sdirkStep" * doctest::description("sdirkStep")) {
    for (unsigned int i = 0; i < data.gamma.size(); i++) {
      for (unsigned int j = 0; j < data.T.size(); j++) {
        for (unsigned int k = 0; k < data.N.size(); k++) {
          // Use equidistant timesteps
          const double h = data.T[j] / data.N[k];

          Eigen::Vector2d sol = sdirkStep(data.z0, h, data.gamma[i]);
          Eigen::Vector2d stud = sdirkStep_TEST(data.z0, h, data.gamma[i]);

          CHECK((sol - stud).norm() == doctest::Approx(0.).epsilon(1e-6));
        }
      }
    }
  }

  TEST_CASE("double cvgSDIRK" * doctest::description("cvgSDIRK")) {
    MESSAGE("This function wasn't tested. Run the program to see its output.");
  }
}
