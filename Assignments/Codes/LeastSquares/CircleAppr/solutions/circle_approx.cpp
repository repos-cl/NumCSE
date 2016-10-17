#include <iostream>
#include <cmath>

#include <Eigen/Dense>

using namespace Eigen;

inline VectorXd lsqSVD(const MatrixXd & A,
                       const MatrixXd & b) {
    return A.jacobiSvd(ComputeThinU | ComputeThinV).solve(b);
}

inline VectorXd lsqHHR(const MatrixXd & A,
                       const MatrixXd & b) {
    return A.colPivHouseholderQr().solve(b);
}

inline VectorXd lsqNRM(const MatrixXd & A,
                       const MatrixXd & b) {
    return (A.transpose() * A).ldlt().solve(A.transpose() * b);
}

/* SAM_LISTING_BEGIN_1 */
/*!
 * \brief circl_alg_fit
 * \param[in] x
 * \param[in] y
 * \return
 */
Vector3d circl_alg_fit(const VectorXd &x,
                       const VectorXd & y) {
    assert(x.size() == y.size() && "Size mismatch!");

    unsigned int n = x.size();

    MatrixXd A(n,3);
    A << -2*x, -2*y, -VectorXd::One(n);
    VectorXd b = -x.array()*x.array()
            - y.array()*y.array();

//    Vector3d z = lsqSVD(A, b);
//    Vector3d z = lsqNRM(A, b);
    Vector3d z = lsqHHR(A, b);

    z(2) = std::sqrt(z(2) + z(0)*z(0) + z(1)*z(1));

    return z;
}
/* SAM_LISTING_END_1 */



/* SAM_LISTING_BEGIN_2 */
Vector3d circl_geo_fit(const VectorXd &x, const VectorXd & y) {

}
/* SAM_LISTING_END_2 */

int main(int argc, char **argv) {

}
