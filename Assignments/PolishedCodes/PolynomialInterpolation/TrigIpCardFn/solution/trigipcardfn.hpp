#ifndef TRIGIPCARDFN_HPP
#define TRIGIPCARDFN_HPP

#include <Eigen/Dense>
#include <cmath>
#include <complex>
#include <iostream>
#include <unsupported/Eigen/FFT>

#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

/**
 * @brief Efficient FFT-based computation of coefficients in expansion
 *  \eqref{eq:trigpreal} for a trigonometric interpolation polynomial in
 * equidistant points \Blue{$(\frac{j}{2n+1},y_j)$}, \Blue{$j=0,\ldots,2n$}. IN
 *  : \texttt{y} has to be a row vector of odd length, return values are column
 *  vectors
 *      \texttt{a}, \texttt{b} will be used to save the expansion coefficients
 */
void trigipequid(const Eigen::VectorXd& y, Eigen::VectorXcd& a,
                 Eigen::VectorXcd& b) {
  const unsigned int N = y.size();
  if (N % 2 != 1) {
    std::cerr << "Number of points must be odd!\n";
    return;
  }
  const unsigned int n = (N - 1) / 2;
  // prepare data for fft
  std::complex<double> i(0, 1);  // imaginary unit
  std::vector<std::complex<double> > f(N);
  std::vector<std::complex<double> > c;

  for (unsigned int k = 0; k < N; ++k) {
    // see \eqref{tip:FM}
    f[k] = y(k) * std::exp(2 * M_PI * i * (double(n) / N * k));
  }
  Eigen::FFT<double> fft;
  fft.fwd(c, f);  // -> c = fft(f);

  // From \eqref{eq:trigpcomp}: \Blue{$\alpha_j =
  // \frac{1}{2}(\gamma_{n-j}+\gamma_{n+j})$} and \Blue{$\beta_j =
  // \frac{1}{2i}(\gamma_{n-j}-\gamma_{n+j})$}, \Blue{$j=1,\ldots,n$},
  // \Blue{$\alpha_0 = \gamma_n$}
  a = Eigen::VectorXcd(n + 1);
  b = Eigen::VectorXcd(n);

  a(0) = c[n];
  for (unsigned int l = 1; l <= n; ++l) {
    a(l) = c[n - l] + c[n + l];
    b(l - 1) = -i * (c[n - l] - c[n + l]);
  }
  // dont forget scaling factor of forward FFT!
  a /= N;
  b /= N;
}

/**
 * @brief Evaluation of trigonometric interpolation polynomial through
 * \Blue{$(\frac{j}{2n+1},y_j)$}, \Blue{$j=0,\ldots,2n$} in equidistant points
 * \Blue{$\frac{k}{N}$}, \Blue{$k=0,N-1$} IN : \texttt{y} = vector of values to
 * be interpolated
 *      \texttt{q} (COMPLEX!) will be used to save the return values
 */
void trigpolyvalequid(const Eigen::VectorXd y, const int M,
                      Eigen::VectorXd& q) {
  const unsigned int N = y.size();
  if (N % 2 == 0) {
    std::cerr << "Number of points must be odd!\n";
    return;
  }
  const unsigned int n = (N - 1) / 2;
  // computing coefficient \Blue{$\gamma_j$}, see \eqref{tip:FM}
  Eigen::VectorXcd a, b;
  trigipequid(y, a, b);

  std::complex<double> i(0, 1);
  Eigen::VectorXcd gamma(2 * n + 1);
  gamma(n) = a(0);
  for (unsigned int k = 0; k < n; ++k) {
    gamma(k) = 0.5 * (a(n - k) + i * b(n - k - 1));
    gamma(n + k + 1) = 0.5 * (a(k + 1) - i * b(k));
  }

  // zero padding
  Eigen::VectorXcd ch(M);
  ch << gamma, Eigen::VectorXcd::Zero(M - (2 * n + 1));

  // build conjugate fourier matrix
  Eigen::FFT<double> fft;
  const Eigen::VectorXcd chCon = ch.conjugate();
  const Eigen::VectorXcd v = fft.fwd(chCon).conjugate();

  // multiplicate with conjugate fourier matrix
  Eigen::VectorXcd q_complex = Eigen::VectorXcd(M);
  for (unsigned int k = 0; k < M; ++k) {
    q_complex(k) = v(k) * std::exp(-2. * k * n * M_PI / M * i);
  }
  // complex part is zero up to machine precision, cut off!
  q = q_complex.real();
}

/**
 * @brief trigIpL Compute $\lambda(n)$.
 *
 * @param n $2*n+1$ will be the number of basis polynomials.
 * @param Value $\lambda(n)$.
 */
/* SAM_LISTING_BEGIN_1 */
double trigIpL(std::size_t n) {
  double lambda = 0.;
  // TODO: (5-5.h) write a function that approximately computes the
  // Lebesgue constant $\lambda(n)$
  // START
  // Nodes where to evaluate
  Eigen::ArrayXd t = Eigen::ArrayXd::LinSpaced(1e4, 0, 1);
  // Will contain value of funtion at all nodes
  Eigen::ArrayXd s = Eigen::ArrayXd::Zero(t.size());
  // Sum over all basis functions
  for (unsigned int k = 0; k <= 2 * n; ++k) {
    // Compute the $b_k$ using formula (6.5.8)
    Eigen::ArrayXd b = Eigen::ArrayXd::Zero(t.size());
    const double tk = (k + 0.5) / (2 * n + 1.);
    b = sin(M_PI * (2 * n + 1) * (t - tk)) / sin(M_PI * (t - tk));

    s += b.cwiseAbs();
  }
  // Find max and rescale
  lambda = s.maxCoeff() / (2 * n + 1);
  // END
  return lambda;
}
/* SAM_LISTING_END_1 */

/**
 * @brief Plot the shifted basis polynomials.
 * @param n $2*n+1$ will be the number of basis polynomials.
 */
/* SAM_LISTING_BEGIN_0 */
void plot_basis(unsigned int n) {
  // mesh size
  constexpr unsigned int M = 1e3;
  plt::figure();

  // TODO: (5-5.c) use the function trigpolyvalequid from
  // trigipcardfn.hpp to plot the cardinal basis function $b_0(t)$ in function
  // of $t$ for n = 5
  // START
  // basis vector $e_1$
  Eigen::ArrayXd e = Eigen::ArrayXd::Zero(2 * n + 1);
  e(0) = 1;
  Eigen::VectorXd y;
  trigpolyvalequid(e, M, y);

  Eigen::ArrayXd t = Eigen::ArrayXd::LinSpaced(M, 0, 1);
  Eigen::ArrayXd zer = Eigen::ArrayXd::Zero(t.size());

  // Shift function right a bit
  Eigen::ArrayXd y_shift(M);
  const unsigned int h = M / (2 * n + 1);
  y_shift << y.tail(h), y.head(M - h);

  // plotting functions using matplotlib
  plt::title("b_0(t)");
  plt::xlabel("t");
  plt::ylabel("y");
  plt::plot(t, y_shift, "r", {{"label", "b_0(t)"}});
  plt::plot(t, zer, "--");
  plt::legend();
  // END
  plt::savefig("./cx_out/basis_function.png");
}
/* SAM_LISTING_END_0 */

/**
 * @brief plot_lam Plot the Lebesgue constant $\lambda(n)$ in function of n.
 * @param points are $n = 2^k$, for k = 2,3,...,8
 * @param lambda are the Lebesgue constants.
 */
void plot_lam(std::vector<int>& points, std::vector<float>& lambda) {
  // plot using matplotlibcpp
  plt::figure();
  plt::title(" Lebesgue constant for trigonometric interpolation ");
  plt::xlabel("n");
  plt::ylabel("λ(n)");
  plt::plot(points, lambda);
  plt::savefig("./cx_out/lebesgue.png");
}

#endif
