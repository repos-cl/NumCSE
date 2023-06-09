#include <Eigen/Dense>
#include <cassert>
#include <iostream>
#include <stdint.h>
#include <string.h>

// reads a bmp file and returns a Eigen::MatrixXi formated like:
// 00000000bbbbbbbbggggggggrrrrrrrr modfied version of
// http://stackoverflow.com/questions/9296059/read-pixel-value-in-bmp-file
// See also https://solarianprogrammer.com/2018/11/19/cpp-reading-writing-bmp-images/
Eigen::MatrixXi readBMP(const std::string filename) {
  FILE *f = fopen(filename.c_str(), "rb");

  if (f == nullptr) {
    std::cerr << "Error reading file " << filename << std::endl;
    exit(1);
  }

  unsigned char info[54];
  fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

  // extract image height and width from header
  const int width = *reinterpret_cast<int *>(&info[18]);
  const int height = *reinterpret_cast<int *>(&info[22]);

  Eigen::MatrixXi mat(height, width);

  int row_padded = (width * 3 + 3) & (~3);
  unsigned char *data = new unsigned char[row_padded];

  for (int i = 0; i < height; i++) {
    fread(data, sizeof(unsigned char), row_padded, f);
    for (int j = 0; j < width * 3; j += 3) {
      int val = (data[j] << 16) | (data[j + 1] << 8) | (data[j + 2]);
      mat(height - i - 1, j / 3) = val;
    }
  }

  fclose(f);
  delete[] data;

  return mat;
}

// extracts a color channel from an image
Eigen::MatrixXd getcolor(Eigen::MatrixXi img, int channel) {
  assert(channel >= 0 && channel <= 2);
  Eigen::MatrixXd ret(img.rows(), img.cols());
  for (int i = 0; i < img.rows(); ++i) {
    for (int j = 0; j < img.cols(); ++j) {
      int p = (img(i, j) >> (channel * 8)) & 0xff;
      ret(i, j) = (double)p;
    }
  }

  return ret;
}

// converts rgb to greyscale
Eigen::MatrixXd greyscale(Eigen::MatrixXi img) {
  Eigen::MatrixXd grey(img.rows(), img.cols());
  for (int i = 0; i < img.rows(); ++i) {
    for (int j = 0; j < img.cols(); ++j) {
      int p = img(i, j);
      grey(i, j) =
          (double)(((p >> 16) & 0xff) + ((p >> 8) & 0xff) + (p & 0xff)) / 3.;
    }
  }

  return grey;
}
