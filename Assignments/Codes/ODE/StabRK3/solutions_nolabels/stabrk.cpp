//// 
//// Copyright (C) 2016 SAM (D-MATH) @ ETH Zurich
//// Author(s): lfilippo <filippo.leonardi@sam.math.ethz.ch> 
//// Contributors: tille, jgacon, dcasati
//// This file is part of the NumCSE repository.
////
#include "rkintegrator.hpp"

//! \file stabrk.cpp Solve prey/predator model with RK-SSM method

Eigen::Vector2d predprey(Eigen::Vector2d y0, double T, unsigned N)
{
    double h = T / N;
    Eigen::Vector2d y_ = y0;

    auto pp = [&] (Eigen::Vector2d y) {
        Eigen::Vector2d y_dot;
        y_dot(0) = (1 - y(1))*y(0);
        y_dot(1) = (y(0) - 1)*y(1);
        return y_dot;
    };

    for(unsigned i=0; i<N; ++i) {
        Eigen::Vector2d k1 = pp(y_);
        Eigen::Vector2d k2 = pp(y_ + h*k1);
        Eigen::Vector2d k3 = pp(y_ + (h/4.)*k1 + (h/4.)*k2);
        y_ = y_ + (h/6.)*k1 + (h/6.)*k2 + (2.*h/3.)*k3;
    }

    return y_;
}

int main() {
    // Implementation of Butcher scheme
    unsigned int s = 3;
    Eigen::MatrixXd A(s,s);
    Eigen::VectorXd b(s);
    A << 0,      0,      0,
         1.,     0,      0,
         1./4.,  1./4.,  0;
    b << 1./6.,  1./6.,  2./3.;

    // Coefficients and handle for prey/predator model
    double alpha1 = 1.;
    double alpha2 = 1.;
    double beta1 = 1.;
    double beta2 = 1.;
    auto f = [&alpha1, &alpha2, &beta1, &beta2] (const Eigen::VectorXd & y) {
        Eigen::VectorXd temp = y;
        temp(0) *= alpha1 - beta1*y(1);
        temp(1) *= -alpha2 + beta2*y(0);
        return temp;
    };

    // Dimension of state space
    unsigned int d = 2;

    // Final time for model
    double T = 1.;

    // Initial value for model
    Eigen::VectorXd y0(d);
    y0 << 100, 1;

    // Array of number of steps (for convergence study)
    std::vector<unsigned int> N = {4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192};
    unsigned int k = 16384;

    // Initialize RK with Butcher table
    RKIntegrator<Eigen::VectorXd> RK(A,b);
    auto sol = RK.solve(f, T, y0, k);
    for(auto v: sol) {
        std::cout << v << std::endl;
    }

    // Exact value y(10) at final time T (approximated)
    Eigen::VectorXd yex = RK.solve(f, T, y0, k).back();

    // Start convergence study
    std::cout  << std::setw(15) << "N"  << std::setw(15) << "error" << std::setw(15) << "rate" << std::endl;
    double errold;
    for(unsigned int i = 0; i < N.size(); ++i) {
        auto res = RK.solve(f, T, y0, N[i]);
        double err = (res.back() - yex).norm();
        std::cout  << std::setw(15) << N[i] << std::setw(15) << err;
        if(i > 0) {
            std::cout << std::setw(15) << log2(errold / err);
        }
        errold = err;
        std::cout  << std::endl;
    }
}
