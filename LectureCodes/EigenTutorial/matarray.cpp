// **********************************************************************
// Eigen tutorial codes: http://eigen.tuxfamily.org/dox/GettingStarted.html
// **********************************************************************

#include <Eigen/Dense>
#include <iostream>

using namespace Eigen;
using namespace std;

void matArray(int nrows, int ncols) {
  MatrixXd m1(nrows, ncols), m2(nrows, ncols);
  for (int i = 0; i < m1.rows(); i++)
    for (int j = 0; j < m1.cols(); j++) {
      m1(i, j) = (double)(i + 1) / (j + 1);
      m2(i, j) = (double)(j + 1) / (i + 1);
    }

  // Entry-wise product, not a matrix product
  MatrixXd m3 = (m1.array() * m2.array()).matrix();
  // Explicit entry-wise operations on matrices are possible
  MatrixXd m4(m1.cwiseProduct(m2));

  cout << "m1 = " << endl << m1 << endl << endl;
  // Entry-wise logarithm
  cout << "Log(m1) = " << endl << log(m1.array()) << endl << endl;
  // Entry-wise boolean expression, true cases counted
  cout << (m1.array() > 3).count() << " entries of m1 > 3" << endl;
}

int main(int argc, char **argv) {
  cout << "matArray(6, 7)" << endl << endl;
  matArray(6, 7);
}