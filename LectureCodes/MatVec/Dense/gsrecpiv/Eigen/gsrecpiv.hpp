///////////////////////////////////////////////////////////////////////////
/// Demonstration code for lecture "Numerical Methods for CSE" @ ETH Zurich
/// (C) 2016 SAM, D-MATH
/// Author(s): Thomas Etterlin <thomaset@student.ethz.ch>
/// Repository: https://gitlab.math.ethz.ch/NumCSE/NumCSE/
/// Do not remove this header.
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include <limits>

#include <Eigen/Dense>

#include "gsrecpiv.hpp"

using namespace Eigen;

/* SAM_LISTING_BEGIN_0 */
MatrixXd gsrecpiv(const MatrixXd &A){
	int n = A.rows();
	MatrixXd result = A;
	if(n > 1){
		int j; double p; // p = relativly largest pivot, j = pivot row index 
		p = ( result.col(0).cwiseAbs().cwiseQuotient(
			A.cwiseAbs().rowwise().maxCoeff() ) ).maxCoeff(&j);// \label{gsrp:1} 
		if( p <  std::numeric_limits<double>::epsilon() * result.norm() )
			throw std::logic_error("nearly singular");// \label{gsrp:2} 			
		result.row(0).swap(result.row(j));// \label{gsrp:3} 
		VectorXd fac = result.col(0).tail(n-1) / result(0,0);// \label{gsrp:f}
		result.bottomRightCorner(n-1,n-1) = 
					gsrecpiv( result.bottomRightCorner(n-1,n-1)
							- fac * result.row(0).tail(n-1) );// \label{gsrp:4}  
		result.col(0).tail(n-1) = -fac;// \label{gsrp:5} 
	}
	return result;
}
/* SAM_LISTING_END_0 */
