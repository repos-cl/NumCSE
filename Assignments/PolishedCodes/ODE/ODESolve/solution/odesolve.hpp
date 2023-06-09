#ifndef ODESOLVE_HPP
#define ODESOLVE_HPP

#include <Eigen/Dense>
#include <iostream>
#include <vector>

#include "matplotlibcpp.h"
#include "polyfit.hpp"

namespace plt = matplotlibcpp;

/**
 * \brief Applies the discrete evolution operator Psi tilde to y0.
 *
 * \tparam DiscEvlOp type for evolution operator (e.g. lambda function type)
 * \param Psi original evolution operator, must have operator(double, const
 * Eigen::VectorXd&)
 * \param p parameter p for construction of Psi tilde
 * \param h step-size
 * \param y0 previous step
 * \return Eigen::VectorXd next state
 */
/* SAM_LISTING_BEGIN_0 */
template <class DiscEvlOp>
Eigen::VectorXd psitilde(DiscEvlOp &&Psi, unsigned int p, double h,
                         const Eigen::VectorXd &y0) {
  Eigen::VectorXd y1 = y0;  // overwrite this
  // TODO: (11-3.b) apply the evolution operator \tilde{\Psi}
  // with step-size h to the value y0
  // START
  y1 = (Psi(h, y0) - (2 << (p - 1)) * Psi(h / 2., Psi(h / 2., y0))) /
       (1. - (2 << (p - 1)));
  // END
  return y1;
}
/* SAM_LISTING_END_0 */

/**
 * \brief Applies psi until final time T.
 *
 * \tparam DiscEvlOp type for evolution operator (e.g. lambda function type)
 * \param Psi original evolution operator, must have operator(double, const
 * Eigen::VectorXd&)
 * \param T final time
 * \param y0 initial data
 * \param N number of steps
 * \return std::vector<Eigen::VectorXd> of states
 */
/* SAM_LISTING_BEGIN_1 */
template <class DiscEvlOp>
std::vector<Eigen::VectorXd> odeintequi(DiscEvlOp &&Psi, double T,
                                        const Eigen::VectorXd &y0,
                                        unsigned int N) {
  std::vector<Eigen::VectorXd> Y;
  // TODO: (11-3.c) Compute y from time 0 to T using N equidistant time steps
  // return a std::vector containing all steps y_0,...,y_N
  // START
  const double h = T / N;
  double t = 0.;
  Y.reserve(N + 1);
  Y.push_back(y0);
  Eigen::VectorXd y = y0;

  while (t < T) {
    // append new value
    y = Psi(h, Y.back());
    Y.push_back(y);
    // time stepping
    t += std::min(T - t, h);
  }
  // END
  return Y;
}
/* SAM_LISTING_END_1 */

/* SAM_LISTING_BEGIN_2 */
double testcvpExtrapolatedEuler() {
  double conv_rate = 0.;
  constexpr double T = 1.;
  Eigen::VectorXd y0 = Eigen::VectorXd::Zero(1);
  auto f = [](const Eigen::VectorXd &y) -> Eigen::VectorXd {
    return Eigen::VectorXd::Ones(1) + y * y;
  };
  // TODO: (11-3.d) tabulate the values of the error corresponding to
  // \tilde{\psi}, where \psi is the explicit Euler method.
  // return the empirical convergence rate using polyfit.
  // Hint: first define a lambda for \psi. Then use psitilde to obtain a
  // suitable input for odeintequi.

  // START
  auto Psi = [&f](double h, const Eigen::VectorXd &y0) -> Eigen::VectorXd {
    return y0 + h * f(y0);
  };
  constexpr unsigned int p = 1;
  // lambda corresponding to \tilde{\psi}
  auto PsiTilde = [&Psi](double h,
                         const Eigen::VectorXd &y0) -> Eigen::VectorXd {
    return psitilde(Psi, p, h, y0);
  };

  // exact value
  Eigen::VectorXd y_ex1(1);
  y_ex1(0) = std::tan(T);
  // values for convergence study
  Eigen::ArrayXd err(11);
  Eigen::ArrayXd N(11);

  std::cout << "Error table for equidistant steps:" << std::endl;
  std::cout << "N"
            << "\t"
            << "Error" << std::endl;
  for (unsigned int i = 0; i < 11; ++i) {
    N(i) = std::pow(2, i + 2);
    Eigen::VectorXd yT = odeintequi(PsiTilde, T, y0, N(i)).back();
    err(i) = (yT - y_ex1).norm();
    std::cout << N(i) << "\t" << err(i) << std::endl;
  }
  // compute fitted rate
  Eigen::VectorXd coeffs = polyfit(N.log(), err.log(), 1);
  conv_rate = -coeffs(0);
  // END
  return conv_rate;
}
/* SAM_LISTING_END_2 */

/**
 * \brief Adaptive timestepper based on psi tilde.
 *
 * \tparam DiscEvlOp type for evolution operator (e.g. lambda function type)
 * \param Psi low-order evolution operator, must have operator(double,
 * const Eigen::VectorXd&)
 * \param T final time
 * \param y0 initial data
 * \param h0 initial step size
 * \param p parameter p for construction of Psi tilde
 * \param reltol relative tolerance for error control
 * \param abstol absolute tolerance for error control
 * \param hmin minimal step size
 * \return std::pair<std::vector<double>, std::vector<Eigen::VectorXd>> of time
 * and corresponding state
 */
/* SAM_LISTING_BEGIN_3 */
template <class DiscEvlOp>
std::pair<std::vector<double>, std::vector<Eigen::VectorXd>> odeintssctrl(
    DiscEvlOp &&Psi, double T, const Eigen::VectorXd &y0, double h0,
    unsigned int p, double reltol, double abstol, double hmin) {
  std::vector<double> t;
  std::vector<Eigen::VectorXd> Y;
  // TODO: (11-3.e)  Compute y from time 0 to T with adaptive time step.
  // Display a warning if the tolerance cannot be met with minimum
  // step size hmin. return a pair of vectors containing the times and
  // the computed values.
  // START
  t.push_back(0.);
  Y.push_back(y0);
  Eigen::VectorXd y = y0;

  double h = h0;
  while (t.back() < T && h > hmin) {
    // psitilde as higher order step
    Eigen::VectorXd y_high = psitilde(Psi, p, std::min(T - t.back(), h), y);
    Eigen::VectorXd y_low = Psi(std::min(T - t.back(), h), y);
    // estimated error of current step
    const double est = (y_high - y_low).norm();
    // effective tolerance
    const double tol = std::max(reltol * y.norm(), abstol);

    h = h * std::max(0.5, std::min(2., std::pow(tol / est, 1. / (p + 1))));

    if (est < tol) {
      // accept time step
      y = y_high;
      Y.push_back(y_high);
      t.push_back(t.back() + std::min(T - t.back(), h));
    }
  }
  if (h < hmin) {
    std::cerr << "Warning: Failure at t = " << t.back()
              << ". Unable to meet integration tolerances without reducing the "
                 "step size below the smallest value allowed ("
              << hmin << ") at time t." << std::endl;
  }
  // END
  return std::make_pair(t, Y);
}
/* SAM_LISTING_END_3 */

/* SAM_LISTING_BEGIN_4 */
void solveTangentIVP() {
  auto f = [](const Eigen::VectorXd &y) -> Eigen::VectorXd {
    return Eigen::VectorXd::Ones(1) + y * y;
  };
  Eigen::VectorXd y0 = Eigen::VectorXd::Zero(1);
  plt::figure();
  // TODO: (11-3.f) run the adaptive integration algorithm and plot the
  // resulting values of y(t).
  // Hint: you might use a loop to convert a std::vector<Eigen::VectorXd> into a
  // std::vector<double>, since each Eigen::VectorXd has size 1

  // START
  constexpr double T = 1.5;
  constexpr unsigned int p = 1;
  constexpr double h0 = 1. / 100.;

  auto Psi = [&f](double h, const Eigen::VectorXd &y0) -> Eigen::VectorXd {
    return y0 + h * f(y0);
  };
  // run adaptive algoritm
  std::pair<std::vector<double>, std::vector<Eigen::VectorXd>> vec_pair =
      odeintssctrl(Psi, T, y0, h0, p, 10e-4, 10e-6, 10e-5);
  std::vector<double> t = vec_pair.first;
  std::vector<Eigen::VectorXd> Y = vec_pair.second;
  // convert type for plot
  std::vector<double> y(Y.size());
  for (unsigned i = 0; i < Y.size(); ++i) {
    y[i] = Y[i](0);
  }

  // plotting results
  plt::plot(t, y, "+", {{"label", "approx IVP"}});
  // exact solution
  Eigen::VectorXd x = Eigen::VectorXd::LinSpaced(100, 0.0, T);
  Eigen::VectorXd exact = x.array().tan();
  plt::plot(x, exact, {{"label", "tangent"}});
  plt::legend();

  plt::title("Approximate vs exact solution");
  plt::xlabel("t");
  plt::ylabel("y");
  plt::grid("True");

  // END
  plt::savefig("./cx_out/tangent.png");
}
/* SAM_LISTING_END_4 */
#endif
