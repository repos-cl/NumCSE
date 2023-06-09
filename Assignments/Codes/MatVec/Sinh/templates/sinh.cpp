#include <iostream>
#include <iomanip>
#include <cmath>


int main() {

    // Lambda function, call with sinh(x)
    /* SAM_LISTING_BEGIN_1 */
    auto sinh_unstable = [] (double x) {
        double t = std::exp(x);
        return .5 * (t - 1./t);
    };
    /* SAM_LISTING_END_1 */

    /* SAM_LISTING_BEGIN_2 */
    // TODO: compute relative error for $10^{-k}, k  = 1,\dots,10$
    /* SAM_LISTING_END_2 */
}
