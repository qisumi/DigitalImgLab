#ifndef MYFFT_H
#define MYFFT_H

#include <cmath>
#include <utility>
#include "MySignal.h"

namespace myfft {

using std::complex;

const double PI = 4 * std::atan(1);

static signal_1D FFT(const signal_1D &signal) {
  int len = signal.size();
  if (len == 1)
    return signal;
  signal_1D arrEven(len / 2);
  signal_1D arrOdd(len / 2);
  for (int i = 0; i < len / 2; i++) {
    arrEven[i] = signal[2 * i];
    arrOdd[i] = signal[2 * i + 1];
  }
  arrEven = FFT(arrEven);
  arrOdd = FFT(arrOdd);
  signal_1D ans(len);
  for (int i = 0; i < len / 2; i++) {
    complex<double> W = exp(complex<double>(0, -2 * PI * i / len));
    ans[i] = arrEven[i] + arrOdd[i] * W;
    ans[i + len / 2] = arrEven[i] - arrOdd[i] * W;
  }
  return ans;
}

static signal_1D IFFT(const signal_1D &signal) {
  int len = signal.size();
  if (len == 1)
    return signal;
  signal_1D arrEven(len / 2);
  signal_1D arrOdd(len / 2);
  for (int i = 0; i < len / 2; i++) {
    arrEven[i] = signal[2 * i];
    arrOdd[i] = signal[2 * i + 1];
  }
  arrEven = FFT(arrEven);
  arrOdd = FFT(arrOdd);
  signal_1D ans(len);
  for (int i = 0; i < len / 2; i++) {
    complex<double> W = exp(complex<double>(0, 2 * PI * i / len));
    ans[i] = arrEven[i] + arrOdd[i] * W;
    ans[i + len / 2] = arrEven[i] - arrOdd[i] * W;
  }
  for (auto &v : ans)
    v /= len;
  return ans;
}

static signal_2D FFT(const signal_2D &signal) {
  signal_2D ans(signal);
  for (int i = 0; i < signal.size(); i++) {
    ans[i] = FFT(ans[i]);
  }
  for (int i = 0; i < ans.size(); i++) {
    for (int j = 0; j <= i; j++) {
      swap(ans[i][j], ans[j][i]);
    }
  }
  for (auto &v : ans)
    v = FFT(v);
  for (int i = 0; i < ans.size(); i++) {
    for (int j = 0; j <= i; j++) {
      swap(ans[i][j], ans[j][i]);
    }
  }
  return ans;
}

static signal_2D IFFT(const signal_2D &signal) {
  signal_2D ans(signal);
  for (int i = 0; i < signal.size(); i++) {
    ans[i] = IFFT(ans[i]);
  }
  for (int i = 0; i < ans.size(); i++) {
    for (int j = 0; j <= i; j++) {
      swap(ans[i][j], ans[j][i]);
    }
  }
  for (auto &v : ans)
    v = IFFT(v);
  for (int i = 0; i < ans.size(); i++) {
    for (int j = 0; j <= i; j++) {
      swap(ans[i][j], ans[j][i]);
    }
  }
  return ans;
}

static void fftshift(signal_2D& data){
    for(int x=0;x<data.size()/2;x++)
    {
      for(int y=0;y<data[0].size()/2;y++)
      {
        swap(data[x][y],data[x+data.size()/2][y+data[0].size()/2]);
      }
    }
    for(int x=data.size()/2;x<data.size();x++)
    {
      for(int y=0;y<data[0].size()/2;y++)
      {
        swap(data[x][y],data[x-data.size()/2][y+data[0].size()/2]);
      }
    }
}
} // namespace myfft
#endif