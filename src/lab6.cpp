#include "../include/CImg.h"
#include "../include/MyFft.h"
#include <iostream>

using namespace std;
using namespace myfft;
using namespace cimg_library;

int main() {
  //读取图片
  CImg<unsigned char> img("bone.bmp");
  CImg<unsigned char> img_expand(512, 512, 1, 1, 1);
  for (int x = 0; x < img.width(); x++)
    for (int y = 0; y < img.height(); y++)
      img_expand.atXY(x + 76, y + 23) = img.atXY(x, y);

  //对图片依次进行取对数、傅里叶变换、低频移至中心操作
  auto data = copyImgToVector(img_expand);
  log(data);
  FFT(data);
  fftshift(data);

  /* 高斯同态滤波的参数 */
  double rh = 1.7; //增强高频系数
  double rl = 0.8; //削减低频系数
  double c = 1; //函数坡度的锐利度
  double D0 = 650; //截止频率

  /* 同态滤波 */
  for (int i = 0; i < data.size(); i++) {
    for (int j = 0; j < data[0].size(); j++) {
      double temp = (data.size() / 2.0 - i) * (data.size() / 2.0 - i) +
                    (data[0].size() / 2.0 - j) * (data[0].size() / 2.0 - j);
      data[i][j] *= (rh - rl) * (1 - exp(-c * temp / D0 / D0)) + rl;
    }
  }
  
  //对图像进行还原、傅里叶逆变换、取指数
  fftshift(data);
  IFFT(data);
  exp(data);

  //保存图像
  auto imga = copyVectorToImg(data);
  imga.display("bone", false);
  imga.save("bone_h.bmp");
  return 0;
}