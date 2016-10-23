#include <complex>
#include <iostream>
#include <Eigen/Dense>
#include <unsupported/Eigen/FFT>

/* SAM_LISTING_BEGIN_0 */
template <typename Scalar>
void fft2(Eigen::MatrixXcd &C, const Eigen::MatrixBase<Scalar> &Y) {
  using idx_t = Eigen::MatrixXcd::Index;
  const idx_t m = Y.rows(),n=Y.cols();
  C.resize(m,n);
  Eigen::MatrixXcd tmp(m,n);

  Eigen::FFT<double> fft; // Helper class for DFT
  // Transform rows of matrix \Blue{$\VY$}
  for (idx_t k=0;k<m;k++) {
    Eigen::VectorXcd tv(Y.row(k));
    tmp.row(k) = fft.fwd(tv).transpose();
  }

  // Transform columns of temporary matrix
  for (idx_t k=0;k<n;k++) {
    Eigen::VectorXcd tv(tmp.col(k));
    C.col(k) = fft.fwd(tv);
  }
}
/* SAM_LISTING_END_0 */  

/* SAM_LISTING_BEGIN_1 */
template <typename Scalar>
void ifft2(Eigen::MatrixXcd &C, const Eigen::MatrixBase<Scalar> &Y) {
  using idx_t = Eigen::MatrixXcd::Index;
  const idx_t m = Y.rows(),n=Y.cols();
  fft2(C,Y.conjugate()); C = C.conjugate()/(m*n);
}
/* SAM_LISTING_END_1 */  

/* SAM_LISTING_BEGIN_2 */
// Straightforward implementation of 2D periodic convolution
template <typename Scalar1,typename Scalar2,class EigenMatrix>
void pmconv_basic(const Eigen::MatrixBase<Scalar1> &X,const Eigen::MatrixBase<Scalar2> &Y,
		  EigenMatrix &Z) {
  using idx_t = typename EigenMatrix::Index;
  using val_t = typename EigenMatrix::Scalar;
  const idx_t n=X.cols(),m=X.rows();
  if ((m!=Y.rows()) || (n!=Y.cols())) throw std::runtime_error("pmconv: size mismatch");
  Z.resize(m,n);
  // Implementation of \eqref{eq:mpconv}
  auto idxwrap = [] (const idx_t L,int i)
    { if (i>=L) i -= L; else if (i<0) i += L; return i; };
  for (int i=0;i<m;i++) for (int j=0;j<n;j++) {
      val_t s=0;
      for (int k=0;k<m;k++) for (int l=0;l<n;l++)
	  s += X(k,l)*Y(idxwrap(m,i-k),idxwrap(n,j-l));
      Z(i,j) = s; 
    }
}
/* SAM_LISTING_END_2 */

/* SAM_LISTING_BEGIN_3 */
// DFT based implementation of 2D periodic convolution
template <typename Scalar1,typename Scalar2,class EigenMatrix>
void pmconv(const Eigen::MatrixBase<Scalar1> &X,const Eigen::MatrixBase<Scalar2> &Y,
	    EigenMatrix &Z) {
  using idx_t = typename EigenMatrix::Index;
  using val_t = typename EigenMatrix::Scalar;
  const idx_t n=X.cols(),m=X.rows();
  if ((m!=Y.rows()) || (n!=Y.cols())) throw std::runtime_error("pmconv: size mismatch");
  Z.resize(m,n); Eigen::MatrixXcd T(m,n),Xh(m,n),Yh(m,n);
  T = Y.template cast<val_t>(); ifft2(Yh,T);
  T = (X.template cast<val_t>()).conjugate(); fft2(Xh,T);
  T = Xh.cwiseProduct(Yh.conjugate());
  ifft2(Z,T);
}
/* SAM_LISTING_END_3 */