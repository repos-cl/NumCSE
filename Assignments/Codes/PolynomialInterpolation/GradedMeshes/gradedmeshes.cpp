#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include <Eigen/Dense>
#include <Eigen/QR>

#include <mgl2/mgl.h>

#include <figure/figure.hpp>

using namespace Eigen;

std::vector<size_t> order(const VectorXd &values) {
    std::vector<size_t> indices(values.size());
    std::iota(begin(indices), end(indices), static_cast<size_t>(0));
    std::sort(begin(indices), end(indices),
        [&](size_t a, size_t b) { return values[a] < values[b]; }
    );
    return indices;
}

VectorXd polyfit(const VectorXd &x, const VectorXd &y, size_t order)
{
    Eigen::MatrixXd A(x.size(), order+1);
    Eigen::VectorXd result;

    assert(x.size() == y.size());
    assert(x.size() >= order+1);

    // Create matrix
    for (size_t i=0; i<x.size(); ++i) {
        for (size_t j=0; j<order+1; ++j) {
            A(i, j) = pow(x(i), j);
        }
    }

    // Solve for linear least squares fit
    VectorXd coeff = A.householderQr().solve(y);
    coeff.conservativeResize(order + 1);

    return coeff;
}

/* @brief Compute values of interpolant in knots $\Vx$ from $(t_i,y_i)$
 * @param[in] x Vector of knots
 * @param[in] t Vector of nodes
 * @param[in] y Vector of values of interpolant in nodes $\Vt$
 * @param[out] s Vector of values of interpolant in knots $\Vx$
 */
/* SAM_LISTING_BEGIN_0 */
VectorXd PwLineIntp(const VectorXd &x, const VectorXd &t,
                    const VectorXd &y)

{
    assert(t.size() == y.size() &&
          "t and y must have same size!");

    // Initialization
    size_t n = t.size();
    auto t_indices = order(t);
    size_t m = x.size();
    auto x_indices = order(x);
    // You can also implement a solution which does not need
    // sorted vectors and e.g.\ for each knot $x_j$ looks
    // for the closest node $t_{i1}$ and the next closest node $t_{i2}$.
    // However, such solution will not become more efficient
    // if you give as input already sorted vectors: for each knot $x_j$
    // you will always have to iterate along the sorted vector $t$
    // to find the included node $t_i$.

    VectorXd s(m);

#if SOLUTION
    size_t i = 0;
    for(size_t j=0; j<m; ++j) {

        bool intpOK = false;
        while(i < n-1) {

            bool inInterval = (t(t_indices[i]) < x(x_indices[j])) &&
                              (x(x_indices[j]) < t(t_indices[i+1]));

            if(inInterval) {
                intpOK = true;

                double gamma = (y(t_indices[i+1]) - y(t_indices[i])) /
                               (t(t_indices[i+1]) - t(t_indices[i]));
                double beta = y(t_indices[i]) - gamma * t(t_indices[i]);

                s(x_indices[j]) = gamma * x(x_indices[j]) + beta;

                break;
            } else if (std::abs(x(x_indices[j]) - t(t_indices[i]))   < 1e-6) { // Else comparison fails...
                intpOK = true;
                s(x_indices[j]) = y(t_indices[i]);
                break;
            } else if (std::abs(x(x_indices[j]) - t(t_indices[i+1])) < 1e-6) { // Else comparison fails...
                intpOK = true;
                s(x_indices[j]) = y(t_indices[i+1]);
                break;
            } else {
                ++i;
            }
        }
        if(!intpOK) {
            std::exit(EXIT_FAILURE); // $x \not\in [t_{\min},t_{\max}]$
        }
    }
#else // TEMPLATE
    // TODO:
#endif // TEMPLATE

    return s;
}
/* SAM_LISTING_END_0 */



int main() {
    {
// Compute convergence rate for interpolation by piecewise linear polyn.
// Uniform mesh in $[0,1]$, singular $f(t) = t^\alpha$, h-convergence
/* SAM_LISTING_BEGIN_1 */
        // Initialization
        size_t NumAlph = 15;
        size_t NumN = 50;
        VectorXd alphas = VectorXd::LinSpaced(NumAlph,0.1,2.9);
        VectorXd nn = VectorXd::LinSpaced(NumN,1,50); // Used nodes

        // Evaluation points
        VectorXd x = VectorXd::LinSpaced(1000,0,1);

        MatrixXd Err(NumAlph,NumN); // Error with max norm
        MatrixXd LocErr(NumAlph,NumN); // Location of maximal error
        for(size_t i=0; i<NumN; ++i) {
            size_t n = nn(i);

            // Nodes
            VectorXd t = VectorXd::LinSpaced(n+1,0,1);

            for(size_t j=0; j<NumAlph; ++j) {
                VectorXd s = x.array().pow(alphas(j));
                VectorXd y = t.array().pow(alphas(j));

                VectorXd p = PwLineIntp(x, t, y); // Interpolation

                size_t PosErr;
                Err(j,i) = (s - p).cwiseAbs().maxCoeff(&PosErr);

                // PosErr is index of point in $x$ with max error
                // LocErr is index of subinterval  with max error
                std::vector<double> tmp(t.size());
                VectorXd::Map(&tmp.front(), t.size()) = t -
                        x(PosErr)*VectorXd::Ones(t.size());
                LocErr(j,i) = count_if(tmp.begin(), tmp.end(),
                                       [] (double val) {return val <= 0;}) - 1;
                // "count\_if" only works if $t$ are already sorted!
#if INTERNAL
                //< // Warning if the maximal error is not where expected
                //< if((alphas(j)<2 && LocErr(j,i)!=0) ||
                //< (alphas(j)>2 && LocErr(j,i)!=n-1)) {
                //< std::cout << "(alpha=" << alphas(j) << ", N=" << n <<
                //< "), max. err. in interval " << LocErr(j,i) << std::endl;
                //< }
#endif // INTERNAL
            }
        }

#if INTERNAL
        mgl::Figure fig1;
        fig1.title("Pw. lin. intp. on uniform meshes: error in max-norm");
        fig1.setlog(true, true); // Set loglog scale
        for(size_t i=0; i<NumAlph; ++i) {
            fig1.plot(nn, Err.row(i)).label("alpha="+std::to_string(alphas(i)));
        }
        fig1.xlabel("n = # subintervals");
        fig1.legend(0, 0);
        fig1.save("PwLineConv_1.eps");
#endif // INTERNAL

        // Estimate convergence rate
        VectorXd rates(NumAlph);
        for(size_t i=0; i<NumAlph; ++i) {
            VectorXd coeff = polyfit(nn.array().log(), Err.row(i).array().log(), 1);
            rates(i) = -coeff(0);
        }

#if INTERNAL
        mgl::Figure fig2;
        fig2.title("Pw. lin. intp. on uniform meshes: error in max-norm");
        fig2.plot(alphas, rates);
        fig2.xlabel("alpha");
        fig2.ylabel("conv. rate");
        fig2.save("PwLineConv_2.eps");
#endif // INTERNAL
/* SAM_LISTING_END_1 */
    }
    {
// Compute convergence rate for interpolation by piecewise linear polyn.
// Beta-graded mesh in $[0,1]$, singular $f(t) = t^\alpha$, h-convergence
// for different betas
/* SAM_LISTING_BEGIN_2 */
        // Initialization
        size_t NumAlph = 3;
        size_t NumBeta = 11;
        size_t NumN = 50;
        VectorXd alphas(NumAlph); alphas << 0.5, 0.75, 4/3;
        VectorXd betas = VectorXd::LinSpaced(NumBeta,1,31);
        VectorXd nn = VectorXd::LinSpaced(NumN,1,50); // Used nodes

        // Evaluation points
        VectorXd x = VectorXd::LinSpaced(1000,0,1);
        std::vector<MatrixXd> Err(NumAlph); // Error with max norm (3D data)
        std::vector<MatrixXd> LocErr(NumAlph); // Location of maximal error (3D data)
        for(size_t i=0; i<NumAlph; ++i) {
            Err[i] = MatrixXd(NumBeta,NumN);
            LocErr[i] = MatrixXd(NumBeta,NumN);
        }

        for(size_t i=0; i<NumN; ++i) {
            size_t n = nn(i);

            for(size_t j=0; j<NumBeta; ++j) {
                // Nodes
                VectorXd t = VectorXd::LinSpaced(n+1,0,1).array().pow(betas(j));

                for(size_t k=0; k<NumAlph; ++k) {
                    VectorXd s = x.array().pow(alphas(k));
                    VectorXd y = t.array().pow(alphas(k));

                    VectorXd p = PwLineIntp(x, t, y); // Interpolation

                    size_t PosErr;
                    Err[k](j,i) = (s - p).cwiseAbs().maxCoeff(&PosErr);

                    // PosErr is index of point in $x$ with max error
                    // LocErr is index of subinterval  with max error
                    std::vector<double> tmp(t.size());
                    VectorXd::Map(&tmp.front(), t.size()) = t -
                                   x(PosErr)*VectorXd::Ones(t.size());
                    LocErr[k](j,i) = count_if(tmp.begin(), tmp.end(),
                                   [] (double val) {return val <= 0;}) - 1;
                    // "count\_if" only works when $t$ are already sorted!
                }
            }
        }

        MatrixXd rates(NumAlph,NumBeta);
        for(size_t i=0; i<NumAlph; ++i) {
#if INTERNAL
            mgl::Figure fig1;
            fig1.title("Pw. lin. intp. on uniform meshes: "
                       +std::to_string(alphas(i)));
            fig1.setlog(true, true); // Set loglog scale
            for(size_t j=0; j<NumBeta; ++j) {
                fig1.plot(nn, Err[i].row(j)).label("beta="+std::to_string(betas(j)));
            }
            fig1.xlabel("n = # subintervals");
            fig1.legend(0, 0);
            fig1.save("PwLineGraded_1_"+std::to_string(alphas(i))+".eps");
#endif // INTERNAL

            // Estimate convergence rate
            for(size_t j=0; j<NumBeta; ++j) {
                size_t skip = 4;
                VectorXd row = Err[i].row(j);
                VectorXd coeff = polyfit(nn.tail(nn.size()-skip).array().log(),
                                 row.tail(row.size()-skip).array().log(),1);
                rates(i) = -coeff(0);
            }

#if INTERNAL
            mgl::Figure fig2;
            fig2.title("Pw. lin. intp. on uniform meshes: "
                       +std::to_string(alphas(i)));
            size_t m2;
            size_t m1 = rates.row(i).maxCoeff(&m2);
            VectorXd betas_(1); betas_ << betas(m2);
            VectorXd rates_(1); rates_ << m1;
            fig2.plot(betas,  rates.row(i));
            fig2.plot(betas_, rates_, "+");
            fig2.xlabel("beta");
            fig2.ylabel("conv. rate");
            fig2.save("PwLineGraded_2_"+std::to_string(alphas(i))+".eps");
#endif // INTERNAL
        }
/* SAM_LISTING_END_2 */
    }
    {
// Plot of algebraically graded mesh
/* SAM_LISTING_BEGIN_3 */
        // Initialization
        size_t n = 10;
        int beta = 2;

        VectorXd t  = VectorXd::LinSpaced(n+1,0,n)/n;
        VectorXd y  = t.array().pow(beta);
        VectorXd t_ = VectorXd::LinSpaced(101,0,1);
        VectorXd y_ = t_.array().pow(beta);

#if INTERNAL
        mgl::Figure fig;
        fig.plot(t_, y_, "r");
        for(size_t i=0; i<n+1; ++i) {
            VectorXd t_tmp(3); t_tmp << t(i), t(i), 0;
            VectorXd y_tmp(3); y_tmp << 0, y(i), y(i);
            fig.plot(t_tmp, y_tmp, "b");
            fig.plot(t_tmp.tail(2), y_tmp.head(2), " r+");
        }
        fig.xlabel("uniform mesh");
        fig.ylabel("algeb. graded mesh, beta=2");
        fig.save("GradedMesh_cpp.eps");
#endif // INTERNAL
/* SAM_LISTING_END_3 */
    }
}
