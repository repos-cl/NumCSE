#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <Eigen/Dense>

#include "copy.hpp"
#include "doctest.h"

struct TestData {
  TestData() {
    f = [](double x) { return std::log(std::abs(x) + 1) * std::pow(x, 3); };

    g = [](double x, double y) { return x * (y - 1) * std::sin(6 * x * y); };

    Q.nodes_.resize(5);
    Q.nodes_ << -1., -std::sqrt(3. / 7.), 0, std::sqrt(3. / 7.), 1.;
    Q.weights_.resize(5);
    Q.weights_ << 0.1, 49. / 90., 32. / 45., 49. / 90., 0.1;

    a = -2.;
    b = 3.;
    N = 5;
  }

  std::function<double(double)> f;
  std::function<double(double, double)> g;

  QuadRule Q;

  double a;
  double b;
  unsigned int N;
};

TestData data;

TEST_SUITE("NestedQuad") {
  TEST_CASE("double evalquad" * doctest::description("evalquad()")) {
    const double sol = evalquad(data.a, data.b, data.f, data.Q);
    const double stud = evalquad_TEST(data.a, data.b, data.f, data.Q);

    CHECK(std::abs(sol - stud) == doctest::Approx(0.).epsilon(1e-8));
  }

  TEST_CASE("double gaussquadtriangle" * doctest::description("evalquad()")) {
    const double sol = gaussquadtriangle(data.g, data.N);
    const double stud = gaussquadtriangle_TEST(data.g, data.N);

    // make sure that changing the order of integration passes the test
    CHECK(std::abs(sol - stud) == doctest::Approx(0.).epsilon(1e-4));
  }

  TEST_CASE("double gaussquadtriangle_loop" *
            doctest::description("evalquad() in loop")) {
    MESSAGE("This function wasn't tested. Run the program to see its output.");
  }

  TEST_CASE("void convtest2DQuad" *
            doctest::description("convtest2DQuad() for 2 nodes")) {
    MESSAGE("This function wasn't tested. Run the program to see its output.");
  }
}
