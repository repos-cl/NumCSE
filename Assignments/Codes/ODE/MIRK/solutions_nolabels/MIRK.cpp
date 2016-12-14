//// 
//// Copyright (C) 2016 SAM (D-MATH) @ ETH Zurich
//// Author(s): lfilippo <filippo.leonardi@sam.math.ethz.ch> 
//// Contributors: tille, jgacon, dcasati
//// This file is part of the NumCSE repository.
////
#include <iostream>
#include <vector>

#include <Eigen/Dense>

using namespace Eigen;

//! \brief Perform 2 steps of newton method applied to F and its jacobian DF
//! \tparam Func type for function F
//! \tparam Jac type for jacobian DF of F
//! \param[in] F function F, for which F(z) = 0 is needed
//! \param[in] DF Jacobian DF of the function F
//! \param[in,out] z initial guess and final approximation for F(z) = 0
template <class Func, class Jac>
void newton2steps(const Func & F, const Jac & DF, VectorXd & z) {
    // First Newton step
    VectorXd znew = z - DF(z).lu().solve(F(z));
    // Second Newton step
    z = znew - DF(znew).lu().solve(F(znew));
}

//! \brief Perform a single step of the MIRK scheme applied to the scalar ODE y' = f(y)
//! \tparam Func type for function f
//! \tparam Jac type for jacobian df of f
//! \param[in] f function f, as in y' = f(y)
//! \param[in] df Jacobian df of the function f
//! \param[in] y0 previous value
//! \param[in] h step-size
//! \return value y1 at next step
template <class Func, class Jac>
double MIRKstep(const Func & f, const Jac & df, double y0, double h) {
    // Coefficients of MIRK
    const double v1  = 1;
    const double v2  = 344./2025.;
    const double d21 = -164./2025.;
    const double b1  = 37./82.;
    const double b2  = 45./82.;
    
    // F derived from MIRK scheme (vector valued)
    auto F = [&f, &y0, &v1, &v2, &d21, &b1, &b2, &h] (const VectorXd & z) -> VectorXd {
        VectorXd ret(3);
        ret << z(0) - (1-v1)*y0 - v1*z(2),
               z(1) - (1-v2)*y0 - v2*z(2) - h*d21*f(z(0)),
               z(2) - y0 - h*(b1*f(z(0))+b2*f(z(1)));
        return ret;
    };
    // Jacobian of F (matrix-valued)
    auto DF = [&df, &v1, &v2, &d21, &b1, &b2, &h] (const VectorXd & z) -> MatrixXd {
        MatrixXd M(3,3);
        M << 0,0, v1,
             h*d21*df(z(0)), 0,  v2,
             h*b1*df(z(0)),h*b2*df(z(1)),0;
        return MatrixXd::Identity(3,3) - M;
    };
    // Initial Guess for Newton steps
    VectorXd z(3);
    z << 0,0,y0;
    // Approximate z, s.t. F(z) = 0
    newton2steps(F, DF, z);
    
    return z(2);
}

//! \brief Solve an ODE y' = f(y) using MIRK scheme on equidistant steps
//! \tparam Func type for function f
//! \tparam Jac type for jacobian df of f
//! \param[in] f function f, as in y' = f(y)
//! \param[in] df Jacobian df of the function f
//! \param[in] y0 initial value
//! \param[in] T final time
//! \param[in] N number of steps
//! \return value approximating y(T)
template <class Func, class Jac>
double MIRKsolve(const Func & f, const Jac & df, double y0, double T, unsigned int N) {
    
    // Step size
    const double h = T / N;
    // Will contain next step
    double ynext = y0;
    for(unsigned int i = 0; i < N; ++i) {
        // Do one step
        ynext = MIRKstep(f, df, ynext, h);
    }
    // Return final value at t = T
    return ynext;
}

int main(int, char**) {
    // r.h.s
    auto f = [] (double y) -> double { return 1 + y*y; };
    // Jacobian of $f$
    auto df = [] (double y) -> double { return 2*y; };
    // Initial data
    const double y0 = 0.;
    // Final time
    const double T = 1.;
    // Exact solution at t = T = 1
    const double yex = tan(1);
    
    //// PROBLEM h: TEST
    std::cout << "*** PROBLEM h:" << std::endl;
    // Table header
    std::cout << "N" << "\t" << "yend" << "\t" << "err" << std::endl;
    for(unsigned int N = 4; N < 512; N=N<<1) {
        // Solve up to time T = 1, using N equidistant steps
        double yend = MIRKsolve(f, df, y0, T, N);
        // Compute error
        double err = std::abs(yex - yend);
        // Print table
        std::cout << N << "\t" << yend << "\t" << err << std::endl;
    }
}
