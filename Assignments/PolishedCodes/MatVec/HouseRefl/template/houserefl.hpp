#ifndef HOUSEREFL_HPP
#define HOUSEREFL_HPP

#include <Eigen/Dense>

/**
 * \brief Compute an ONB of the space orthogonal to $v$
 * \param v vector $\in \mathbb{R}^n \setminus \{ 0 \}$
 * \param Z matrix $\in \mathbb{R}^{n-1 \times n}$
 */
/* SAM_LISTING_BEGIN_1 */
void houserefl(const Eigen::VectorXd &v, Eigen::MatrixXd &Z) {
  const unsigned int n = v.size();
  // TODO: (1-5.a) Implement the householder refelction algorithm described in
  // the pseudo code 1.5.1 to get the ONB of the space orthogonal to $v$.

  // START

  // END
}
/* SAM_LISTING_END_1 */

#endif
