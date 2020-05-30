#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "copy.hpp"

#include <Eigen/Dense>
#include <vector>

struct TestData {
	// TODO: test with the commented-out values (current ones are
	// the ones mentioned in the assignment so should work...)
	TestData() {
		z0 << 0.0, 0.0;
		gamma = {/*1.0,*/ (3 + std::sqrt(3))/6.0};
		T = {/*5.0, 6.0, 7.0, 9.0,*/ 10.0};
		N = {20, 40, 80, 160, 320, 640};
	}
	
	Eigen::Vector2d z0;
	double h;
	std::vector<double> gamma;
	std::vector<double> T;
	std::vector<unsigned int> N;
};

TestData data;

// TODO: redefinition compilation issues with sdirkSolve. 
// Maybe because the function signature is multiline?
TEST_SUITE("SDIRK") {
	// TODO: values returned are 0 for both sol and stud.
	TEST_CASE("Eigen::Vector2d sdirkStep" * doctest::description("sdirkStep")) {
		for (int i = 0; i < data.gamma.size(); i++) {
			for (int j = 0; j < data.T.size(); j++) {
				for (int k = 0; k < data.N.size(); k++) {
					// Use equidistant timesteps 
					double h = data.T[j]/data.N[k];
					
					Eigen::Vector2d sol = sdirkStep(data.z0, h, data.gamma[i]);
					Eigen::Vector2d stud = sdirkStep_TEST(data.z0, h, data.gamma[i]);
					
					/*
					// Debugging
					printf("Sol: %f, %f \n", sol(0), sol(1));
					printf("Stud: %f, %f \n\n", stud(0), stud(1));
					*/
					
					CHECK((sol - stud).norm() == doctest::Approx(0.).epsilon(1e-6));
				}
			}
		}
	}
	
	// TODO: this might not work since the definition has a newline
	/*
	TEST_CASE("std::vector<Eigen::Vector2d> sdirkSolve" * doctest::description("sdirkSolve")) {
		CHECK(1 == 1);
	}
	*/

	TEST_CASE("std::vector<Eigen::Vector2d> sdirkSolve" * doctest::description("sdirkSolve") * doctest::skip()) {}
	
	TEST_CASE("double cvgSDIRK" * doctest::description("cvgSDIRK") * doctest::skip()) {}
}










