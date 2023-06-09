///////////////////////////////////////////////////////////////////////////
/// Demonstration code for lecture "Numerical Methods for CSE" @ ETH Zurich
/// (C) 2016 SAM, D-MATH
/// Author(s): Ralf Hiptmair
/// Repository: https://gitlab.math.ethz.ch/NumCSE/NumCSE/
/// Do not remove this header.
//////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "dft2d.hpp"
#include "freqbasmatgen.hpp"

using namespace std;
using namespace Eigen;

/* SAM_LISTING_BEGIN_0 */
int main() {
  using Comp = complex<double>;
  const VectorXcd::Index m=7,n = 5;

  cout << "(I) Testing 2D DFT and convolution" << endl;
  {
    // Testing Fourier basis martrices
    cout << "DFT of Fourier basis matrices" << endl;
    cout << "(Mapped to unit vectors by inverse DFT)" << endl;
    int k=5,l=3;
    MatrixXcd B = freqbasmatgen(m,n,k,l);
    MatrixXcd C(m,n); ifft2(C,B);
    cout << "B = " << endl << B << endl;
    cout << "C = " << endl << C << endl;
  }

  cout << "Testing 2D convolution" << endl;
  {
  // Test 2D convolution
    cout << "Comparison of loop-based and DFT-based implementation" << endl;
    cout << "(both should return the same result except for roundoff)" << endl;
    MatrixXd Y(m,n);
    MatrixXcd X(m,n);
    MatrixXcd Z1(m,n),Z2(m,n);
    for (int k=0;k<m;k++)
      for (int j=0;j<n;j++) {
	Y(k,j) = (double)std::min(k,j);
	X(k,j) = std::complex<double>(1.0,1.0)*(double)(k+j);
      }

    /*
    MatrixXcd Y(m,n), X(m,n);
    MatrixXcd Z1(m,n),Z2(m,n);
    X = freqbasmatgen(m,n,2,1);
    Y = freqbasmatgen(m,n,2,1); */
    
    pmconv_basic(X,Y,Z1);
    pmconv(X,Y,Z2);
    cout << "X = " << endl << X << endl;
    cout << "Y = " << endl << Y << endl;
    cout << "Z1 = " << endl << Z1 << endl;
    cout << "Z2 = " << endl << Z2 << endl;
  }

  cout << "(III) Testing 2D DFT" << endl;
  {
  // Test: 2D DFT
  // Initialize matrix for testing 2D DFT
  // MATLAB: Y = triu((1:m)'*(1:n))+i*(ones(m,n));
  MatrixXcd Y = MatrixXcd::Constant(m,n,Comp(0.0,1.0));
  for (int k=0;k<m;k++)
    for (int j=0;j<n;j++)
      { if (k<=j) Y(k,j) += Comp(k*j,0.0); }

  MatrixXcd C(m,n);
  fft2(C,Y);
  cout << "Y = " << endl << Y << endl;
  cout << "C = " << endl << C << endl;
  ifft2(Y,C);
  cout << "Y = " << endl << Y << endl;
  }
}
/* SAM_LISTING_END_0 */

/* Test code in MATLAB for 2D DFT
% Testing script for 2D dft
n = 5; m = 7; 
Y = triu((0:m-1)'*(0:n-1))+i*ones(m,n);
Fn = exp((2*pi*i/n)*((0:n-1)'*(0:n-1)));
Fm = exp((2*pi*i/m)*((0:m-1)'*(0:m-1)));
C = Fm*Y*Fn,
fft2(conj(Y)),
 */
