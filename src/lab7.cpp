#include "../include/CImg.h"
#include <algorithm>
#include <iostream>
#include <queue>
#include <stdlib.h>
#include <vector>

using namespace cimg_library;

#define SET_RGB(img, x, y, rgb)                                                \
  img.atXY((x), (y), 0, 0) = rgb[0], img.atXY((x), (y), 0, 1) = rgb[1],        \
                        img.atXY((x), (y), 0, 2) = rgb[2]
int BLACK[] = {0, 0, 0};
int GREEN[] = {0, 255, 0};
int WHITE[] = {255, 255, 255};
int RED[] = {255, 0, 0};
using uint8 = unsigned short;

CImg<uint8> binary_filter(CImg<uint8> &source) {
  int width = source.width();
  int height = source.height();
  CImg<uint8> target(source);
  for (int x = 1; x < width - 1; x++) {
    for (int y = 1; y < height - 1; y++) {
      if (target.atXY(x, y)) {
        int sum = 0;
        int count = 0;
        for (int xx = x - 1; xx <= x + 1; xx++) {
          for (int yy = y - 1; yy <= y + 1; yy++) {
            if (xx < 0 || xx >= width || yy < 0 || yy >= height)
              continue;
            if (!source.atXY(xx, yy)) {
              target.atXY(x, y) = 0;
              goto out;
            }
          }
        }
      }
    out:;
    }
  }
  return target;
}

CImg<uint8> img_partition(CImg<uint8> &source) {
  int width = source.width();
  int height = source.height();
  CImg<uint8> target(width, height);
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < width * 0.5; y++) {
      target.atXY(x, y) = source.atXY(x, y) > 150 ? 255 : 0;
    }
  }
  for (int x = 0; x < width; x++) {
    for (int y = width * 0.5; y < width * 0.7; y++) {
      target.atXY(x, y) = source.atXY(x, y) > 140 ? 255 : 0;
    }
  }
  for (int x = 0; x < width; x++) {
    for (int y = height * 0.7; y < height; y++) {
      target.atXY(x, y) = source.atXY(x, y) > 100 ? 255 : 0;
    }
  }
  return binary_filter(target);
}

CImg<uint8> edge_detect(CImg<uint8> &source) {
  int width = source.width();
  int height = source.height();
  CImg<uint8> target(width, height, 1, 3);
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      if (source.atXY(x, y)) {
        for (int xx = x - 1; xx <= x + 1; xx++) {
          for (int yy = y - 1; yy <= y + 1; yy++) {
            if (xx < 0 || xx >= width || yy < 0 || yy >= height)
              continue;
            if (!source.atXY(xx, yy)) {
              SET_RGB(target, x, y, GREEN);
              goto out2;
            }
          }
        }
      }
    out2:;
    }
  }
  return target;
}

int count_object(CImg<uint8> &source) {
  int ans = 0;
  struct Point {
    int x;
    int y;
  };
  CImg<uint8> target(source);
  char s[4];
  std::queue<Point> quque;
  std::vector<Point> pos;
  int width = source.width();
  int height = source.height();
  for (int x = 0; x < width; x++)
    for (int y = 0; y < height; y++) {
      if (!source.atXY(x, y, 0, 1))
        continue;
      for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++)
          if (source.atXY(x, y, 0, 0))
            source.atXY(x, y, 0, 0) = 0;
      ans++;
      pos.push_back({x, y});
      _itoa_s(ans, s, 10);
      quque.push({x, y});
      while (!quque.empty()) {
        int _x, _y;
        _x = quque.front().x;
        _y = quque.front().y;
        quque.pop();
        SET_RGB(source, _x, _y, RED);
        for (int xx = _x - 2; xx <= _x + 2; xx++) {
          for (int yy = _y - 2; yy <= _y + 2; yy++) {
            if (xx < 0 || xx >= width || yy < 0 || yy >= height)
              continue;
            if (source.atXY(xx, yy, 0, 1)) {
              quque.push({xx, yy});
            }
          }
        }
      }
    }
  for (int i = 0; i < pos.size(); i++) {
    _itoa_s(i + 1, s, 10);
    if (i < pos.size() - 1) {
      if (i == 84 || i==48) {
        std::cout << pos[i].x << " " << pos[i].y << std::endl;
        target.draw_text(pos[i].x + 10, pos[i].y - 10, s, RED, BLACK);
      } else
        target.draw_text(pos[i].x, pos[i].y, s, RED, BLACK);
    } else {
      target.draw_text(pos[i].x - 5, pos[i].y, s, RED, BLACK);
    }
  }
  target.display("", false);
  return ans;
}

int main() {
  CImg<uint8> source("rice.bmp");
  int width = source.width();
  int height = source.height();
  CImg<uint8> target = img_partition(source);
  target.display("rice", false);

  CImg<uint8> filtered = binary_filter(target);
  filtered.display("filtered", false);

  CImg<uint8> edge = edge_detect(filtered);
  edge.display("edge detect", false);
  std::cout << count_object(edge) << std::endl;

  return 0;
}