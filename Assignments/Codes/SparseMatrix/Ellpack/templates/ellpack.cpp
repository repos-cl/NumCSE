#include <iostream>
#include <vector>

#include <Eigen/Dense>
#include <Eigen/Sparse>

using namespace Eigen;

using Triplet_new  = Triplet<double>;
using Triplets = std::vector<Triplet_new>;
using Vector = VectorXd;
using index_t = std::ptrdiff_t;

/* @brief Class representing a sparse matrix in Ellpack format
 * \param[in] triplets Vector of Eigen triplets
 * \param[in] m Number of rows of matrix $A$
 * \param[in] n Number of columns of matrix $A$
 */
/* SAM_LISTING_BEGIN_0 */
class EllpackMat {
public:
    EllpackMat(const Triplets & triplets, index_t m, index_t n);

    double operator() (index_t i, index_t j) const;

    void mvmult(const VectorXd &x, VectorXd &y) const;

    void mtvmult(const VectorXd &x, VectorXd &y) const;

private:
    std::vector<double>  val; //< Vector containing values
    // corresponding to entries in 'col'
    std::vector<index_t> col; //< Vector containing column
    // indices of the entries in 'val'.
    // The position of a cell in 'val' and 'col'
    // is determined by its row number and original position in 'triplets'

    index_t maxcols; //< Number of non-empty columns
    index_t m,n; //< Number of rows, number of columns
};
/* SAM_LISTING_END_0 */

/* @brief Retrieve value of cell $(i,j)$
 * \param[in] i Row index
 * \param[in] j Column index
 * \param[out] val Value of cell $(i,j)$
 */
double EllpackMat::operator() (index_t i, index_t j) const {
    assert(0 <= i && i < m && 0 <= j && j < n && "Index out of bounds!");

    for(index_t l = i*maxcols; l < (i+1)*maxcols; ++l) {
        if( col[l] == j ) return val[l];
    }
    return 0;
}

/* @brief Constructor of class EllpackMat from vector of triplets
 * \param[in] triplets Vector of Eigen triplets
 * \param[in] m Number of rows of matrix $A$
 * \param[in] n Number of columns of matrix $A$
 */
/* SAM_LISTING_BEGIN_1 */
EllpackMat::EllpackMat(const Triplets & triplets,
                       index_t m, index_t n)
    : m(m), n(n) {
    // TODO: implement the constructor of class
    // EllpackMat from vector of triplets
}
/* SAM_LISTING_END_1 */

/* @brief Compute $y = Ax$ exploiting the Ellpack format
 * \param[in] x Vector of matrix-vector multiplication
 * \param[out] y Vector from $y = Ax$
 */
/* SAM_LISTING_BEGIN_2 */
void EllpackMat::mvmult(const VectorXd &x, VectorXd &y) const {
    assert(x.size() == n && "Incompatible vector x size!");
    assert(y.size() == m && "Incompatible vector y size!");

    // TODO: implement operation $y = Ax$
}
/* SAM_LISTING_END_2 */

/* @brief Compute $y = A_t x$ exploiting the Ellpack format, where $A$ is transposed
 * \param[in] x Vector of matrix-vector multiplication
 * \param[out] y Vector from $y = Ax$
 */
/* SAM_LISTING_BEGIN_3 */
void EllpackMat::mtvmult(const VectorXd &x, VectorXd &y) const {
    assert(x.size() == m && "Incompatible vector x size!");
    assert(y.size() == n && "Incompatible vector y size!");

    // TODO: implement operation $y = A^t x$
}
/* SAM_LISTING_END_3 */

/* SAM_LISTING_BEGIN_4 */
int main(int, char**) {
    // Vector of triplets
    Triplets triplets;

    // Data
    unsigned int m = 3, n = 6;
    unsigned int ntriplets = 9;

    // Reserve space for triplets
    triplets.reserve(ntriplets);

    // Fill in some triplets
    triplets.push_back(Triplet_new(1,2,4));
    triplets.push_back(Triplet_new(0,0,5));
    triplets.push_back(Triplet_new(1,2,6));
    triplets.push_back(Triplet_new(2,5,7));
    triplets.push_back(Triplet_new(0,4,8));
    triplets.push_back(Triplet_new(1,3,9));
    triplets.push_back(Triplet_new(2,2,10));
    triplets.push_back(Triplet_new(2,1,11));
    triplets.push_back(Triplet_new(1,0,12));

    // Build Eigen::SparseMatrix
    SparseMatrix<double> S(m,n);
    S.setFromTriplets(triplets.begin(), triplets.end());

    // Build Ellpack matrix
    EllpackMat E(triplets, m, n);

    //// TEST

    std::cout << " ------------- Test of y = A*x --------------- " << std::endl;
    Vector x(6);
    x << 4,5,6,7,8,9;
    Vector Sx = S*x;
    std::cout << "Sparse S*x ="  << std::endl << Sx << std::endl;
    Vector Ex = Vector::Zero(m);
    E.mvmult(x, Ex);
    std::cout << "Ellpack E*x =" << std::endl << Ex << std::endl;
    std::cout << "l2-norm of the difference = " << (Sx - Ex).norm() << std::endl;

    std::cout << " ------------- Test of y = A^t*x ------------- " << std::endl;
    x.resize(3);
    x << 1,2,3;
    Vector Stx = S.transpose()*x;
    std::cout << "Sparse S^t*x ="  << std::endl << Stx  << std::endl;
    Vector Etx = Vector::Zero(n);
    E.mtvmult(x, Etx);
    std::cout << "Ellpack E^t*x =" << std::endl << Etx << std::endl;
    std::cout << "l2-norm of the difference = " << (Stx - Etx).norm() << std::endl;
}
/* SAM_LISTING_END_4 */
