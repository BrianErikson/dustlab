#pragma once
#include <unordered_map>
#include <vector>
#include "Transform.h"
#include "Spritesheet.h"

using TileFlag = int32_t;

namespace TileFlags {
// Solid, platform, etc. Primary descriptor, or Tileset major subcategory
constexpr TileFlag MEDIUM_MASK = 0x000F;
// Edge, Corner, Center
constexpr TileFlag TYPE_MASK = 0x00F0;
// Cardinal directions, including diagonals
constexpr TileFlag DIR_MASK = 0xFF00;

constexpr TileFlag MEDIUM_ERR = 1 << 0;
constexpr TileFlag MEDIUM_VOID = 1 << 1;
constexpr TileFlag MEDIUM_SOLID = 1 << 2;

constexpr TileFlag TYPE_EDGE = 1 << 4;
constexpr TileFlag TYPE_CORNER = 1 << 5;
constexpr TileFlag TYPE_CENTER = 1 << 6;

constexpr TileFlag DIR_SHIFT = 8;
}

struct RenderTile {
  Transform transform{};
  int row{0};
  int col{0};
};

class Tilesheet : public Spritesheet
{
public:
  Tilesheet(const std::string &spritesheet_path,
            const std::string &spritesheet_json_path);
  bool init() override;

  /**
   * Retrieve a random tile from the specified flags
   * \param unit_interval a number between 0 and 1 to be used as a randomizer
   * \param flags Specifications for specific tile to retrieve
   * \return A random tile from the specified set
   */
  RenderTile& tile(double unit_interval, TileFlag flags);

private:
  void generate_tiles(auto&& obj, TileFlag flags);
  void generate_direction_tiles(int row, int col, TileFlag flags, int base_rotation);

  std::string json_path_{};
  std::unordered_map<TileFlag, std::vector<RenderTile>> render_tiles_{};
};
