#pragma once

#include <Eigen/Dense>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include "hermloceval.hpp"

using namespace Eigen;


std::vector<Vector2d> closedPolygonalInterpolant(std::vector<Vector2d> Sigma,
                                                 const VectorXd &x) {
  // Note: we assume that x is sorted
  assert(x.minCoeff() >= 0 && x.maxCoeff() <= 1 && "x out of range");

  std::vector<Vector2d> res(x.size());
  unsigned int n = Sigma.size();
  // extend Sigma with periodic condition
  Sigma.push_back(Sigma[0]);

  // define variables delta_1, ..., delta_n and lambda_0,...,lambda_n
  std::vector<Vector2d> d(n);
  std::vector<double> delta(n);
  std::vector<double> lambda(n + 1);

  lambda[0] = 0.0;
  for (unsigned int i = 0; i < n; ++i) {
    d[i] = Sigma[i + 1] - Sigma[i];
    delta[i] = (d[i]).norm();
    lambda[i + 1] = lambda[i] + delta[i];
  }
  // TO DO (0-3.a)
  // START
  
  // END
  return res;
}



std::vector<Vector2d> closedHermiteInterpolant(std::vector<Vector2d> Sigma,
                                               const VectorXd &x) {
  // Note: we assume that x is sorted
  assert(x.minCoeff() >= 0 && x.maxCoeff() <= 1 && "x out of range");
  std::vector<Vector2d> res(x.size());

  unsigned int n = Sigma.size();
  // extend Sigma with periodic condition
  Sigma.push_back(Sigma[0]);

  // define variables delta_1,...,delta_n and lambda_0,...lambda_n
  std::vector<Vector2d> d(n);
  std::vector<double> delta(n);
  std::vector<double> lambda(n + 1);

  lambda[0] = 0.0;
  for (unsigned int i = 0; i < n; ++i) {
    d[i] = Sigma[i + 1] - Sigma[i];
    delta[i] = d[i].norm();
    lambda[i + 1] = lambda[i] + delta[i];
  }

  // define the slopes as in the definition of the interpolating
  // closed cubic Hermite curve
  std::vector<Vector2d> slopes(n + 1);
  // TO DO (0-3.b)
  // START
  
  // END
  return res;
}



template <typename CurveFunctor>
std::pair<std::vector<Vector2d>, std::vector<Vector2d>>
adaptedHermiteInterpolant(CurveFunctor &&c, unsigned int nmin,
                          const VectorXd &x, double tol = 1.0e-3) {
  // Note: we assume that x is sorted
  assert(x.minCoeff() >= 0 && x.maxCoeff() <= 1 && "x out of range");
  // will contain the result
  std::vector<Vector2d> eval; 
  std::vector<Vector2d> Sigma;
  
  VectorXd t = VectorXd::LinSpaced(nmin + 1, 0, 1);
  // TO DO (0-3.c)
  // START
  
  // END
  return std::make_pair(eval,Sigma);
}

