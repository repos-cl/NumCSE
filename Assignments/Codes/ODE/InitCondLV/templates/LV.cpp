#include <iostream>

#include <Eigen/Dense>
#include <Eigen/Sparse>

#include "ode45.hpp"

using namespace Eigen;

/*!
 * \brief Compute the maps Phi and W at time T.
 * Use initial data given by u0 and v0.
 * \param[in] u0 First component.
 * \param[in] v0 Second component.
 * \param[in] T Final time.
 */
/* SAM_LISTING_BEGIN_1 */
std::pair<Vector2d, Matrix2d> PhiAndW(double u0,
                                      double v0,
                                      double T) {
    std::pair<Vector2d, Matrix2d> PaW;
    // TODO:
    return PaW;
}
/* SAM_LISTING_END_1 */

int main(){
    /* SAM_LISTING_BEGIN_2 */
    // Initial guess
    Vector2d y;
    y << 3, 2;
    // Period we require
    double T = 5;

    // TODO: Apply the Newton method to find initial data
    // giving solutions with period equal to 5.
    /* SAM_LISTING_END_2 */
}
