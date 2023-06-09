# include <iostream>
# include <complex>
# include <Eigen/Dense>
# include <unsupported/Eigen/FFT>
using Eigen::ArrayXcd;
using Eigen::VectorXcd;

/* SAM_LISTING_BEGIN_0 */
void trigipequidcomp(const VectorXcd& a, const VectorXcd& b, const unsigned N, VectorXcd& y) {
  const unsigned n = a.size() - 1;
  if (N < (2*n - 1)) {
    std::cerr << "N is too small! Must be larger than 2*n";
    return;
  }
  const std::complex<double> iu(0,1); // imaginary unit
  // build vector \Blue{$\gamma$}
  VectorXcd gamma(2*n + 1);
  gamma(n) = a(0);
  for (unsigned k = 0; k < n; ++k) {
    gamma(k) = 0.5*( a(n - k) + iu*b(n - k - 1) );
    gamma(n + k + 1) = 0.5*( a(k + 1) - iu*b(k) );
  }
  // zero padding to obtain \Blue{$\wt{\Vc}$}
  VectorXcd ch(N); ch << gamma, VectorXcd::Zero(N - (2*n + 1));

  // realize multiplication with conjugate fourier matrix
  Eigen::FFT<double> fft;
  VectorXcd chCon = ch.conjugate(); 
  VectorXcd v = fft.fwd(chCon).conjugate();

  // final scaling, implemented without efficiency considerations
  y = VectorXcd(N);
  for (unsigned k = 0; k < N; ++k) 
    y(k) = v(k) * std::exp( -2.*k*n*M_PI/N*iu );
}
/* SAM_LISTING_END_0 */
