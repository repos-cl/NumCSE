#pragma once

#include <cmath>
#include <iostream>
#include <limits>

/* SAM_LISTING_BEGIN_1 */
double myfunction(double x) {
  // We require that $x > 0$.
  // First we factorize $x$ as $x=2^{n+r}=2^n * x_{new}$
  // with integer $n$ and $r\in[ 0.5, 1.5 ]$.
  // Set $y = n\log(2)$ and $z=\log(x_{new})$,
  // then $\log(x) = y + z$.
  double dy = 0.693147180559945;  // dy = std::log(2)
  double y = 0.;
  // If x $x=2^{n+r}$ with positive $n$:
  while (x > 2. * std::sqrt(2.)) {
    x /= 2.;
    y += dy;
  }  // \Label[line]{cq:1}
  // If x $x=2^{n+r}$ with negative $n$:
  while (x < std::sqrt(2.)) {
    x *= 2.;
    y -= dy;
  }  // \Label[line]{cq:2}
  // Now, we have found $y$, and we use Newton iteration for the
  // function $f(z) = \exp(z) - x_{new}$ to find $z = \log(x_{new})$.
  // Initial guess:
  double z = x - 1.;  // \Label[line]{cq:3}
  // The update is $dz = -f(z)/f'(z)$
  double dz = x * std::exp(-z) - 1.;
  while (std::abs(dz / z) > std::numeric_limits<double>::epsilon()) {
    z += dz;
    dz = x * std::exp(-z) - 1.;
  }
  return y + z + dz;  // \Label[line]{cq:4}
}
/* SAM_LISTING_END_1 */

/* SAM_LISTING_BEGIN_2 */
double myfunction_modified(double x) {
  double dy = 0.693147180559945;  // dy = std::log(2)
  double y = 0.;
  while (x > 2. * std::sqrt(2.)) {
    x /= 2.;
    y += dy;
  }
  while (x < std::sqrt(2.)) {
    x *= 2.;
    y -= dy;
  }
  double z = x - 1.;
  double dz = x * std::exp(-z) - 1.;
  // TO DO: (9-2.e) Write a for-loop that achieves the same accuracy
  // as the third while-loop of myfunction(). I.e. fix the number of
  // iterations before looping.
  // START
  
  // END
  return y + z + dz;
}
/* SAM_LISTING_END_2 */
