#include "WorldGenerator.h"
#include <noise/noise.h>
#include <experimental/ranges/iterator>

using namespace noise;

cv::Mat WorldGenerator::get_perlin_noise(int seed, double step, int x, int y, int z, int width, int height) {
  module::Perlin perlin{};
  perlin.SetNoiseQuality(NoiseQuality::QUALITY_BEST);
  perlin.SetSeed(seed);

  cv::Mat image(height, width, CV_8UC1);

  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      image.at<unsigned char>(row, col) = std::clamp(
          static_cast<unsigned int>(255u * ((perlin.GetValue(
              x + (step * col), y + (step * row), z) + 1.0) / 2.0)),
              0u, 255u);
    }
  }

  return image;
}

Map WorldGenerator::get_map() {
  const int void_upper_thresh = 128;
  Map map(32, 32, CV_32SC1);

  auto noise1 = WorldGenerator::get_perlin_noise(0, 0.1, 0, 0, 0, map.cols, map.rows);
  for (int row = 0; row < map.rows; row++) {
    for (int col = 0; col < map.cols; col++) {
      const auto &src_val = noise1.at<unsigned char>(row, col);
      auto &dest_val = map.at<int32_t>(row, col);
      if (src_val < void_upper_thresh) {
        dest_val = TileFlags::MEDIUM_VOID;
        continue;
      }

      dest_val = TileFlags::MEDIUM_SOLID | TileFlags::TYPE_CENTER;
      /*
      auto edge_dirs = WorldGenerator::neighboring_edges(noise1, row, col, 128);
      if (edge_dirs.empty()) {
        dest_val = TileFlags::MEDIUM_SOLID | TileFlags::TYPE_CENTER;
        continue;
      }

      const auto &first_edge = edge_dirs[0];
      const auto is_void = [&](int row, int col) -> bool {
        if (std::find(edge_dirs.begin(), edge_dirs.end(), cv::Point2i{col, row}) != edge_dirs.end()) {
          return true;
        }

        return false;
      };

      // TODO if surrounded by edges
      if (edge_dirs.size() > 1) {
        // if exterior corner
        dest_val = TileFlags::MEDIUM_SOLID | TileFlags::TYPE_CORNER;
        if (is_void(1, 0) && is_void(0, 1)) {
          dest_val |= TileFlags::DIR_TOP_RIGHT;
        }
        else if (is_void(-1, 0) && is_void(0, -1)) {
          dest_val |= TileFlags::DIR_BOTTOM_LEFT;
        }
        else if (is_void(-1, 0) && is_void(0, 1)) {
          dest_val |= TileFlags::DIR_TOP_LEFT;
        }
        else if (is_void(1, 0) && is_void(0, -1)) {
          dest_val |= TileFlags::DIR_BOTTOM_RIGHT;
        }
        continue;

        // TODO if two-sided edge
      }
      else if (first_edge.x != 0 && first_edge.y != 0) {
        dest_val = TileFlags::MEDIUM_SOLID | TileFlags::TYPE_CENTER;
      }
      else {
        dest_val = TileFlags::MEDIUM_SOLID | TileFlags::TYPE_EDGE;
        if (first_edge.x == 1) dest_val |= TileFlags::DIR_RIGHT;
        else if (first_edge.x == -1) dest_val |= TileFlags::DIR_LEFT;
        else if (first_edge.y == 1) dest_val |= TileFlags::DIR_TOP;
        else if (first_edge.y == -1) dest_val |= TileFlags::DIR_BOTTOM;
        else dest_val = TileFlags::MEDIUM_ERR;
      }
       */
    }
  }

  return map;
}

std::vector<cv::Point2i> WorldGenerator::neighboring_edges(cv::Mat src, int row, int col,
                                                           unsigned char lower_thresh) {
  std::vector<cv::Point2i> neighbor_voids;
  assert(src.type() == CV_8UC1);
  const auto &val = src.at<unsigned char>(row, col);
  if (val < lower_thresh) {
    return neighbor_voids;
  }

  cv::Point2i target_pos;
  const auto check_target = [&]() {
    if (src.at<unsigned char>(target_pos.y, target_pos.x) < lower_thresh) {
      neighbor_voids.emplace_back(target_pos);
    }
  };

  int row_above = row - 1;
  int row_below = row + 1;
  int col_left = col - 1;
  int col_right = col + 1;

  if (row_above >= 0) {
    target_pos = {col, row_above};
    check_target();

    if (col_left >= 0) {
      target_pos = {col_left, row_above};
      check_target();
    }

    if (col_right < src.cols) {
      target_pos = {col_right, row_above};
      check_target();
    }
  }

  if (row_below < src.rows) {
    target_pos = {col, row_below};
    check_target();

    if (col_left >= 0) {
      target_pos = {col_left, row_below};
      check_target();
    }

    if (col_right < src.cols) {
      target_pos = {col_right, row_below};
      check_target();
    }
  }

  return neighbor_voids;
}
