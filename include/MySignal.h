#ifndef SIGNAL_H
#define SIGNAL_H
#include <vector>
#include <complex>
#include "CImg.h"
using namespace cimg_library;
using std::complex;
using std::vector;
typedef vector<complex<double>> signal_1D;
typedef vector<signal_1D> signal_2D;

/**
 * @brief 将二维矩阵数据取自然对数
 * @param data 数据
 */
static void log(signal_2D &data) {
  for (auto &l : data)
    for (auto &v : l)
      v = log(v);
}

/**
 * @brief 将二维矩阵数据取指数
 * @param data 数据
 */
static void exp(signal_2D &data) {
  for (auto &l : data)
    for (auto &v : l)
      v = exp(v);
}

/**
 * @brief 将图像数据拷贝到二维矩阵
 *
 * @param img 数据来源图像
 * @return 储存数据的vector
 */
static signal_2D copyImgToVector(const CImg<unsigned char> &img) {
  signal_2D data;
  data.resize(img.height());
  for (int i = 0; i < img.width(); i++) {
    data[i].resize(img.width());
    for (int j = 0; j < img.height(); j++) {
      data[i][j].real((int)img.atXY(i, j));
      data[i][j].imag(0);
    }
  }
  return data;
}

/**
 * @brief 使用二维矩阵数据生成图像
 *
 * @param data 二维矩阵数据
 * @return 图像
 */
static CImg<unsigned char> copyVectorToImg(const signal_2D &data) {
  CImg<unsigned char> img(data.size(), data[1].size());
  for (int i = 0; i < img.width(); i++) {
    for (int j = 0; j < img.height(); j++) {
      img.atXY(i, j) = (int)data[i][j].real();
    }
  }
  return img;
}

#endif