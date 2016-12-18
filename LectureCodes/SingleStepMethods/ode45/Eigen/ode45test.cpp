///////////////////////////////////////////////////////////////////////////
/// Demonstration code for lecture "Numerical Methods for CSE" @ ETH Zurich
/// (C) 2016 SAM, D-MATH
/// Author(s): R. Hiptmair
/// Repository: https://gitlab.math.ethz.ch/NumCSE/NumCSE/
/// Do not remove this header.
//////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <math.h>

// Selection of embedded RK-SSM
// #define MATLABCOEFF true

#include "ode45_boost.hpp"

using namespace std;

/* SAM_LISTING_BEGIN_0 */
int main(void)
{
  // Types to be used for a scalar ODE with state space \Blue{$\bbR$}
  using StateType = double;
  using RhsType = std::function<StateType(StateType)>;
  // Logistic differential equation \eqref{eq:logode}
  RhsType f = [](StateType y) { return 5*y*(1-y); };
  StateType y0 = 0.2;   // Initial value
  // Exact solution of IVP, see \eqref{eq:logodesol}
  auto y = [y0](double t) { return y0/(y0+(1-y0)*exp(-5*t)); };
  // State space \Blue{$\bbR$}, simple modulus supplies norm
  auto normFunc = [](StateType x){ return fabs(x); };
  
  // Invoke explicit Runge-Kutta method with stepsize control
  ode45<StateType,RhsType> integrator(f);
  std::vector<std::pair<StateType, double>> states = integrator.solve(y0,1.0,normFunc);
  // Output information accumulation during numerical integration
  integrator.options.do_statistics = true; integrator.print();
  
  for (auto state : states)
    std::cout << "t = " << state.second << ", y = " << state.first
	      << ", |err| = " << fabs(state.first - y(state.second)) << std::endl;
}
/* SAM_LISTING_END_0 */
