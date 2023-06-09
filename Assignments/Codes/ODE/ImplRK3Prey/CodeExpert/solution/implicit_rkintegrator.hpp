#pragma once

#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>

#include <Eigen/Dense>

#include "dampnewton.hpp"

using namespace Eigen;

/*!
 *! \file implicit_rkintegrator.hpp A header only file implementing a
 *! implicit runge kutta integrator.
 */

/*!
 *! \brief Compute the Kronecker product $C = A \otimes B$.
 *! \param[in] A Matrix $m \times n$.
 *! \param[in] B Matrix $l \times k$.
 *! \return Kronecker product of A and B of dim $ml \times nk$.
 */
MatrixXd kron(const MatrixXd &A, const MatrixXd &B) {
  MatrixXd C(A.rows() * B.rows(), A.cols() * B.cols());
  for (unsigned int i = 0; i < A.rows(); ++i) {
    for (unsigned int j = 0; j < A.cols(); ++j) {
      C.block(i * B.rows(), j * B.cols(), B.rows(), B.cols()) = A(i, j) * B;
    }
  }
  return C;
}

/* SAM_LISTING_BEGIN_1 */
/*!
 *! \brief Implements a Runge-Kutta implicit solver for a
 *! given Butcher tableau for autonomous ODEs.
 */
class implicit_RKIntegrator {
public:
  /*!
   *! \brief Constructor for the implicit RK method.
   *! Performs size checks and copies A and b into internal storage.
   *! \param[in] A Matrix containing coefficents of Butcher tableau.
   *! \param[in] b Vector containing coefficients of
   *! lower part of Butcher tableau.
   */
  implicit_RKIntegrator(const MatrixXd &A, const VectorXd &b)
      : A(A), b(b), s(b.size()) {
    assert(A.cols() == A.rows() && "Matrix must be square.");
    assert(A.cols() == b.size() && "Incompatible matrix/vector size.");
  }

  /*!
   *! \brief Perform the solution of the ODE.
   *! Solve an autonomous ODE y' = f(y), y(0) = y0, using an
   *! implicit RK scheme given in the Butcher tableau provided in the
   *! constructor. Performs N equidistant steps upto time T
   *! with initial data y0.
   *! \tparam Function type for function implementing the rhs function.
   *! Must have VectorXd operator()(VectorXd x)
   *! \tparam Function2 type for function implementing the Jacobian of f.
   *! Must have MatrixXd operator()(VectorXd x)
   *! \param[in] f function handle for rhs in y' = f(y), e.g.
   *! implemented using lambda funciton
   *! \param[in] Jf function handle for Jf, e.g.
   *! implemented using lambda funciton
   *! \param[in] T final time T
   *! \param[in] y0 initial data y(0) = y0 for y' = f(y)
   *! \param[in] N number of steps to perform.
   *! Step size is h = T / N. Steps are equidistant.
   *! \return vector containing all steps y^n (for each n)
   *! including initial and final value
   */
  template <class Function, class Jacobian>
  std::vector<VectorXd> solve(Function &&f, Jacobian &&Jf, double T, const VectorXd &y0, unsigned int N) const {
    // Iniz step size
    double h = T / N;

    // Will contain all steps, reserve memory for efficiency
    std::vector<VectorXd> res;
    res.reserve(N + 1);

    // Store initial data
    res.push_back(y0);
    // START
    // Initialize some memory to store temporary values
    VectorXd ytemp1 = y0;
    VectorXd ytemp2 = y0;
    // Pointers to swap previous value
    VectorXd *yold = &ytemp1;
    VectorXd *ynew = &ytemp2;

    // Loop over all fixed steps
    for (unsigned int k = 0; k < N; ++k) {
      // Compute, save and swap next step
      step(f, Jf, h, *yold, *ynew);
      res.push_back(*ynew);
      std::swap(yold, ynew);
    }
    // END
    return res;
  }

private:
  // START
  /*!
   *! \brief Perform a single step of the RK method for the
   *! solution of the autonomous ODE
   *! Compute a single explicit RK step y^{n+1} = y_n + \sum ...
   *! starting from value y0 and storing next value in y1
   *! \tparam Function type for function implementing the rhs.
   *! Must have VectorXd operator()(VectorXd x)
   *! \tparam Jacobian type for function implementing the Jacobian of f.
   *! Must have MatrixXd operator()(VectorXd x)
   *! \param[in] f function handle for ths f, s.t. y' = f(y)
   *! \param[in] Jf function handle for Jf, e.g. implemented using lambda
   *function 
   *! \param[in] h step size ! \param[in] y0 initial VectorXd 
   *! \param[out] y1 next step y^{n+1} = y^n + ...
   */
  template <class Function, class Jacobian>
  void step(Function &&f, Jacobian &&Jf, double h, const VectorXd &y0, VectorXd &y1) const {

    int d = y0.size();
    MatrixXd eye = MatrixXd::Identity(d, d);

    // Handle for the function F describing the
    // equation satisfied by the stages g
    auto F = [&y0, h, d, this, &f, &eye](VectorXd gv) {
      VectorXd Fv = gv;
      for (int j = 0; j < s; j++) {
        Fv = Fv - h * kron(A.col(j), eye) * f(y0 + gv.segment(j * d, d));
      }
      return Fv;
    };

    // Handle for the Jacobian of F.
    auto JF = [&y0, h, d, &Jf, this, &eye](VectorXd gv) {
      MatrixXd DF(s * d, s * d);
      for (int j = 0; j < s; j++) {
        DF.block(0, j * d, s * d, d) =
            kron(A.col(j), eye) * Jf(y0 + gv.segment(j * d, d));
      }
      DF = MatrixXd::Identity(s * d, s * d) - h * DF;
      return DF;
    };

    // Obtain stages with damped Newton method
    VectorXd gv = VectorXd::Zero(s * d);
    dampnewton(F, JF, gv);

    // Calculate y1
    MatrixXd K(d, s);
    for (int j = 0; j < s; j++)
      K.col(j) = f(y0 + gv.segment(j * d, d));
    y1 = y0 + h * K * b;
  }
  // END
  //<! Matrix A in Butcher scheme
  const MatrixXd A;
  //<! Vector b in Butcher scheme
  const VectorXd b;
  //<! Size of Butcher matrix and vector A and b
  unsigned int s;
};
/* SAM_LISTING_END_1 */
