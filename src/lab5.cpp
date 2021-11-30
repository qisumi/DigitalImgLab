#include "../include/CImg.h"
#include <iostream>
using namespace cimg_library;
using namespace std;

using uint8 = unsigned char;
using Point = pair<int, int>;

constexpr double sigma_g = 900;

/**
 * @brief 给定图像子区域，求区域内灰度的均值和方差
 *
 * @param p1 左上角坐标
 * @param p2 右下角坐标
 * @return 返回均值和方差
 */
pair<double, double> getS(const CImg<uint8> img, Point &&p1, Point &&p2) {
  // 区域变量初始化
  int l = p1.first;
  int t = p1.second;
  int r = p2.first;
  int b = p2.second;
  
  // 统计变量初始化                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
  int pixelNum = (r - l + 1) * (b - t + 1);
  double avg = 0;
  double sigma = 0;
  
  // 计算均值
  for (int x = l; x <= r; x++)
    for (int y = t; y <= b; y++)
      avg += img.atXY(x, y);
  avg /= pixelNum;

  // 计算方差
  for (int x = l; x <= r; x++)
    for (int y = t; y <= b; y++)
      sigma += (img.atXY(x, y) - avg) * (img.atXY(x, y) - avg);
  sigma /= pixelNum;

  return {avg, sigma};
}

int main() {

  // 读取源图，初始化变量
  CImg<uint8> source("badlena.bmp");
  int width = source.width();
  int height = source.height();
  CImg<uint8> target(width, height,1,1,0);

  // source.display("badlina.bmp", false);

  for (int x = 3; x < width - 3; x++) {
    for (int y = 3; y < height - 3; y++) {
      // 计算区域内像素灰度值的均值和方差
      auto s = getS(source, {x - 3, y - 3}, {x + 3, y + 3});
      double avg = s.first;
      double sigma = s.second;

      // 计算滤波结果
      uint8 gray =
          source._atXY(x, y) - (sigma_g / sigma) * (source.atXY(x, y) - avg);

      // 生成目标图像
      target.atXY(x, y) = gray;
    }
  }
  target.display("target",false);
  target.save("goodlena.bmp");
  return 0;
}