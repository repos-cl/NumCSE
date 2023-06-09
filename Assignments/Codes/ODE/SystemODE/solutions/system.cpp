#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/SparseLU>
#include <vector>
#include <iomanip>

#include "errors.hpp"

using namespace Eigen;

/* SAM_LISTING_BEGIN_1 */
template <class Function, class State>
void rk4step(const Function &odefun, double h,
             const State & y0, State & y1)
{
    auto k1 = odefun(y0);
    auto k2 = odefun(y0 + h/2*k1);
    auto k3 = odefun(y0 + h/2*k2);
    auto k4 = odefun(y0 + h*k3);

    y1 = y0 + h/6*k1 + h/3*k2 + h/3*k3 + h/6*k4;
}
/* SAM_LISTING_END_1 */

int main() {
/* SAM_LISTING_BEGIN_0 */
    // Construct data for RK order 4
    MatrixXd A = MatrixXd::Zero(4,4);
    A(1,0) = .5;
    A(2,1) = .5;
    A(3,2) = 1;
    VectorXd b(4);
    b << 1./6, 1./3, 1./3, 1./6;
    
    // Construct data for the IVP
    double T = 1;
    int n = 5;
    VectorXd y0(2*n);
    for(int i = 0; i < n; ++i) {
        y0(i)=(i+1.)/n;
        y0(i+n)=-1;
    }

    auto f = [n] (VectorXd y) {
        VectorXd fy(2*n);
        
        VectorXd g(n);
        g(0) = y(0)*(y(1)+y(0));
        g(n-1) = y(n-1)*(y(n-1)+y(n-2));
        for(int i = 1; i < n-1; ++i) {
            g(i) = y(i)*(y(i-1)+y(i+1));
        }
        
        Eigen::SparseMatrix<double> C(n,n);
        C.reserve(3);
        for(int i = 0; i < n; ++i) {
            C.insert(i,i) = 2;
            if(i < n-1) C.insert(i,i+1) = -1;
            if(i >= 1)  C.insert(i,i-1) = -1;
        }
        C.makeCompressed();
//      fy.head(n) = y.head(n);
        fy.head(n) = y.tail(n);
        
        Eigen::SparseLU< Eigen::SparseMatrix<double> > solver;
        solver.analyzePattern(C);
        solver.compute(C);
        fy.tail(n) = solver.solve(g);
        return fy;
    };

    errors(f, T, y0, A, b);
/* SAM_LISTING_END_0 */
}
