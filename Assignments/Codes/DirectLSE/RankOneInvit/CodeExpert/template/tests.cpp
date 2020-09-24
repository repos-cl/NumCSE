#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "copy.hpp"

// includes for test data
#include <Eigen/Dense>

struct TestData {
	TestData() {
		tol = 1e-3;
		n = 50;
		d = Eigen::VectorXd::Random(n);
	}
	double tol;
	int n;
	Eigen::VectorXd d;
};

TestData data;

TEST_SUITE("RankOneInvit") {
	
	TEST_CASE("double rankoneinvit" * doctest::description("lmin")) {
		const double stud = rankoneinvit_TEST(data.d, data.tol);
		const double sol = rankoneinvit(data.d, data.tol);
		CHECK(sol == doctest::Approx(stud).epsilon(1e-6));
	}
	
	TEST_CASE("double rankoneinvit_fast" * doctest::description("lmin_fast")) {
		const double stud = rankoneinvit_fast_TEST(data.d, data.tol);
		const double sol = rankoneinvit_fast(data.d, data.tol);
		CHECK(sol == doctest::Approx(stud).epsilon(1e-6));
	}
	
	TEST_CASE("void rankoneinvit_runtime" * doctest::description("runtime")) {
		MESSAGE("This function wasn't tested. Run the program to see its output.");
	}
	
}

