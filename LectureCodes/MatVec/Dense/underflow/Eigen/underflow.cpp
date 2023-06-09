///////////////////////////////////////////////////////////////////////////
/// Demonstration code for lecture "Numerical Methods for CSE" @ ETH Zurich
/// (C) 2016 SAM, D-MATH
/// Author(s): Thomas Etterlin <thomaset@student.ethz.ch>
/// Repository: https://gitlab.math.ethz.ch/NumCSE/NumCSE/
/// Do not remove this header.
//////////////////////////////////////////////////////////////////////////
/* SAM_LISTING_BEGIN_0 */
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <limits> 
using namespace std;
int main(){	
	cout.precision(15);
	double min = numeric_limits<double>::min();
	double res1 = M_PI*min/123456789101112;
	double res2 = res1*123456789101112/min;
	cout << res1 << endl << res2 << endl;
}
/* SAM_LISTING_END_0 */
