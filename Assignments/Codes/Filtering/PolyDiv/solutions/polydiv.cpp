#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <limits>
#include <vector>

#include <Eigen/Dense>
#include <unsupported/Eigen/FFT>

#include "timer.h"

using namespace Eigen;

/* @brief Polynomial multiplication -- naive implementation
 * @param[in] u Vector of coefficients of polynomial $u$
 * @param[in] v Vector of coefficients of polynomial $v$
 * @param[out] uv Vector of coefficients of polynomial $uv = u*v$
 */
/* SAM_LISTING_BEGIN_0 */
VectorXd polyMult_naive(const VectorXd & u, const VectorXd & v)
{
    // Initialization
    int m = u.size();
    int n = v.size();
    int dim = std::max(m, n);
    
    VectorXd u_tmp = u;
    u_tmp.conservativeResizeLike(VectorXd::Zero(dim));
    VectorXd v_tmp = v;
    v_tmp.conservativeResizeLike(VectorXd::Zero(dim));
    
    VectorXd uv(m+n-1); // Degree is (m-1) + (n-1)

    for(int i=0; i<uv.size(); ++i) {
		int fst = std::max(0, i - (dim - 1));
		int lst = std::min(i, dim - 1);
		for(int j=fst; j<=lst; ++j) {
			uv(i) += u_tmp(j) * v_tmp(i-j);
		}
	}

	return uv;
}
/* SAM_LISTING_END_0 */

/* @brief Polynomial multiplication -- efficient implementation
 * @param[in] u Vector of coefficients of polynomial $u$
 * @param[in] v Vector of coefficients of polynomial $v$
 * @param[out] uv Vector of coefficients of polynomial $uv = u*v$
*/
/* SAM_LISTING_BEGIN_1 */
VectorXd polyMult_fast(const VectorXd & u, const VectorXd & v)
{
	// Initialization
	int m = u.size();
	int n = v.size();

	VectorXd u_tmp = u;
	u_tmp.conservativeResizeLike(VectorXd::Zero(u.size() + n - 1));
	VectorXd v_tmp = v;
	v_tmp.conservativeResizeLike(VectorXd::Zero(v.size() + m - 1));

	VectorXd uv;

	Eigen::FFT<double> fft;
	VectorXcd u_tmp_ = fft.fwd(u_tmp);
	VectorXcd v_tmp_ = fft.fwd(v_tmp);
	VectorXcd tmp = u_tmp_.cwiseProduct(v_tmp_);
	uv = fft.inv(tmp).real();

	return uv;
}
/* SAM_LISTING_END_1 */

/* @brief Polynomial division -- efficient implementation
 * @param[in] uv Vector of coefficients of polynomial $uv$
 * @param[in] u Vector of coefficients of polynomial $u$
 * @param[out] uv Vector of coefficients of polynomial $v$
 * from $uv = u*v$ (division without remainder)
*/
/* SAM_LISTING_BEGIN_2 */
VectorXd polyDiv(const VectorXd & uv, const VectorXd & u)
{
	// Initialization
	int mn = uv.size();
	int m = u.size();
	int dim = std::max(mn, m);
	
	VectorXd uv_tmp = uv;
	uv_tmp.conservativeResizeLike(VectorXd::Zero(dim));
	VectorXd u_tmp  = u;
	u_tmp.conservativeResizeLike(VectorXd::Zero(dim));

/* SAM_LISTING_BEGIN_3 */
	// Machine precision
	double epsilon = std::numeric_limits<double>::epsilon();
	for(int i=dim-1; i>=0; --i) { // Right-most value of longest vector
		// is not necessarily numerically different than 0...
		if(std::abs(uv_tmp(i)) > epsilon*uv.norm()) {
			// No problem
			break;
		} else if(std::abs(u_tmp(i)) > epsilon*u.norm()) {
            std::exit(EXIT_FAILURE);
		}
	}
/* SAM_LISTING_END_3 */

	VectorXd v;

	Eigen::FFT<double> fft;
	VectorXcd uv_tmp_ = fft.fwd(uv_tmp);
	VectorXcd u_tmp_ = fft.fwd(u_tmp);
	VectorXcd tmp = uv_tmp_.cwiseQuotient(u_tmp_);
	v = fft.inv(tmp).real();

	v.conservativeResizeLike(VectorXd::Zero(mn - m + 1));
										// (mn-1) - (m-1) + 1
	return v;
}
/* SAM_LISTING_END_2 */

int main() {
	// Initialization
	int m = 4;
	int n = 3;
	VectorXd u(m);
	VectorXd v(n);
	u << 1, 2, 3, 4;
	v << 10, 20, 30;

	// Compute with both functions
	std::cout << "Check that all functions are correct" << std::endl;

	VectorXd uv_1 = polyMult_naive(u, v);
	std::cout << "Naive multiplicator: "
			  << std::endl << uv_1 << std::endl;

	VectorXd uv_2 = polyMult_fast(u, v);
	std::cout << "Efficient multiplicator: "
			  << std::endl << uv_2 << std::endl;

	std::cout << "Error = " << (uv_1 - uv_2).norm() << std::endl;

	VectorXd v_new = polyDiv(uv_2, u);
	std::cout << "Error of efficient division = " << (v - v_new).norm()
			  << std::endl;

	// Initialization
	int repeats = 3;
	VectorXd uv;

	// Compute runtimes of different multiplicators
	std::cout << "Runtime comparison of naive v efficient multiplicator"
			  << std::endl;

	// Header
	std::cout << std::setw(20) << "n"
			  << std::setw(20) << "time naive [s]"
			  << std::setw(20) << "time fast [s]"
			  << std::endl;

	// Loop over vector size
	for(int p = 2; p <= 15; ++p) {
		// Timers
		Timer tm_naive, tm_effic;
		int n = pow(2,p);

		// Repeat test many times
		for(int r = 0; r < repeats; ++r) {
			u = VectorXd::Random(n);
			v = VectorXd::Random(n);

			// Compute runtime of naive multiplicator
			tm_naive.start();
			uv = polyMult_naive(u, v);
			tm_naive.stop();
			// Compute runtime of efficient multiplicator
			tm_effic.start();
			uv = polyMult_fast(u, v);
			tm_effic.stop();
		}
		
		
		// Print runtimes
		std::cout << std::setw(20) << n
				  << std::scientific << std::setprecision(3)
				  << std::setw(20) << tm_naive.min()
				  << std::setw(20) << tm_effic.min()
				  << std::endl;
	}
	
}
