#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "copy.hpp"
#include <vector>
#include <Eigen/Dense>

using namespace Eigen;

struct TestData {
	double T = 1.;
	int N = 1;
  
	Vector3d y0;
	y0 << 0.1, 0.2, 0.4;
	  
	auto f = [] (const Vector3d &y) -> Vector3d {
	  Vector3d fy;
	  fy << y(0)*y(1), y(1)*y(2), y(2)-y(0);
	  return fy;
	};
	  
	auto Jf = [] (const Vector3d &y) {
	  Matrix3d J;
	  J << y(1),y(0),0,0,y(2),y(1),-1,0,1;
	  return J;
	};
};

TestData data;

TEST_SUITE("CrossProduct") {
	TEST_CASE("std::vector<VectorXd> solve_imp_mid" * doctest::description("Implicit midpoint")) {
		std::vector<VectorXd> impl_mid_sol = solve_imp_mid(data.f, data.Jf, data.T, data.y0, data.N);
		std::vector<VectorXd> impl_mid_stud = solve_imp_mid_TEST(data.f, data.Jf, data.T, data.y0, data.N);

		auto sol = impl_sol.back();
		auto stud = impl_stud.back();

		CHECK((sol - stud).norm() == doctest::Approx(0.).epsilon(1e-6));
	}

	TEST_CASE("std::vector<VectorXd> solve_lin_mid" * doctest::description("Implicit linear midpoint")) {
		std::vector<VectorXd> impl_lin_mid_sol = solve_lin_mid(data.f, data.Jf, data.T, data.y0, data.N);
		std::vector<VectorXd> impl_lin_mid_stud = solve_lin_mid_TEST(data.f, data.Jf, data.T, data.y0, data.N);

		auto sol = impl_lin_mid_sol.back();
		auto stud = impl_lin_mid_stud.back();

		CHECK((sol - stud).norm() == doctest::Approx(0.).epsilon(1e-6));
	}
	
	TEST_CASE("void tab_crossprod" * docttest::description("Tabulate results" ) * doctest::skip()) {}
}

