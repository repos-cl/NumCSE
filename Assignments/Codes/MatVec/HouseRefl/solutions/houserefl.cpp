#include <Eigen/Dense>
#include <iostream>
#include <ctime>

/* \brief Compute an ONB of the space orthogonal to $v$
 * \param[in] v vector $\in \mathbb{R}^n \setminus \{ 0 \}$
 * \param[out] Z matrix $\in \mathbb{R}^{n-1 \times n}$
 */
/* SAM_LISTING_BEGIN_1 */
void houserefl(const Eigen::VectorXd & v, Eigen::MatrixXd & Z)
{
    unsigned int n = v.size();
    Eigen::VectorXd w = v.normalized();
    Eigen::VectorXd u=w;
    u(0) += 1;
    Eigen::VectorXd q = u.normalized();
    Eigen::MatrixXd X = Eigen::MatrixXd::Identity(n, n) - 2*q*q.transpose();
    Z = X.rightCols(n-1);
}
/* SAM_LISTING_END_1 */

int main(int argc, char ** argv) {
    // Check what houserefl does to random vector

    // Initialize random number generator
    srand((unsigned int) time(0));

    // Size of test vector
    unsigned int n = 6;
    // Optionally read from input arguments
    if(argc >= 2) n = std::atoi(argv[1]);

    Eigen::VectorXd v = Eigen::VectorXd::Random(n); // Not truly random if missing srand
    Eigen::MatrixXd Z;

    houserefl(v, Z);

    std::cout << "v = " << v << std::endl;
    std::cout << "Z = " << Z << std::endl;
}
