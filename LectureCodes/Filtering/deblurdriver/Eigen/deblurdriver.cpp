# include <iostream>
# include <Eigen/Dense>
# include <mgl2/mgl.h>
# include "kronecker.hpp"
# include "image.hpp" // provides plot command
# include "psf.hpp"
# include "blur.hpp"
# include "deblur.hpp"
using Eigen::MatrixXd; using Eigen::VectorXd;

void deblurdriver() {
  // Generate artificial ``image''
  MatrixXd M(3,3); M << 8,1,6,3,5,7,4,9,2;
  MatrixXd P = 31*kron(M, MatrixXd::Ones(30,40));
  image(P, "Original", "dborigimage.eps");
  // Generate point spread function
  MatrixXd S; psf(5, S); 
  // Blur image
  MatrixXd C = blur(P, S);
  image(C, "Blurred image", "dbblurredimage.eps");
  // Deblur image
  MatrixXd D = deblur(C, S);
  image(D, "Deblurred image", "dbdeblurredimage.eps");

  std::printf("Difference of images (Frobenius norm): %f\n", (P - D).norm()/P.norm());
}

int main() {
  deblurdriver();
  return 0;
}
