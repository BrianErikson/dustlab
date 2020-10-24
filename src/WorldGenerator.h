#pragma once
#include <memory>
#include <opencv2/opencv.hpp>
#include "DesertTilesheet.h"

class WorldGenerator
{
public:
  static cv::Mat get_perlin_noise(int seed, double step, int x, int y, int z, int width, int height);
  static cv::Mat get_map(const DesertTilesheet &tilesheet);

  static std::vector<cv::Point2i> neighboring_edges(cv::Mat src, int row, int col, unsigned char lower_thresh);

};
