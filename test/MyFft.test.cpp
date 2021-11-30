/**
 * @file MyFft.test.cpp
 * @brief This file is to test the correction of
 * functions defined in include/MyFft.h
 * the output should be a matrix of complex like
 * {1,2,3
 *  4,5,6
 *  7,8,9}
 * @date 2021-11-29
 */

#include "../include/MyFft.h"
#include <iostream>
using namespace std;
using namespace myfft;

int main() {
  signal_2D t = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  signal_2D a = IFFT(FFT(t));
  for (auto &i : t) {
    for (auto &v : i) {
      cout << v << " ";
    }
    cout << endl;
  }
  return 0;
}