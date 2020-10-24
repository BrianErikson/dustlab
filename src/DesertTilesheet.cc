#include "DesertTilesheet.h"
#include <fstream>
#include <json.hpp>
#include <glm/gtx/transform.hpp>
#include <SDL2/SDL_log.h>

using json = nlohmann::json;

DesertTilesheet::DesertTilesheet() : Spritesheet("./res/textures/desert_tileset.png"),
json_path_{"./res/textures/desert_tileset.json"} {
}

bool DesertTilesheet::init() {
  std::ifstream t(this->json_path_);
  std::string str((std::istreambuf_iterator<char>(t)),
                   std::istreambuf_iterator<char>());
  const auto jdata = json::parse(str);
  assert(!jdata.empty());

  if (!Spritesheet::init({jdata["tile_width"], jdata["tile_height"]})) {
    return false;
  }

  const auto &tiles = jdata["tiles"];
  {
    const auto &solid_tiles = tiles["solid"];
    this->generate_tiles(solid_tiles["edge"], TileFlags::MEDIUM_SOLID | TileFlags::TYPE_EDGE);
    this->generate_tiles(solid_tiles["corner"], TileFlags::MEDIUM_SOLID | TileFlags::TYPE_CORNER);
    this->generate_tiles(solid_tiles["center"], TileFlags::MEDIUM_SOLID | TileFlags::TYPE_CENTER);
  }

  return true;
}

void DesertTilesheet::generate_tiles(auto &&obj, TileFlag flags) {
  assert(obj.is_array());
  TileFlag type = flags & TileFlags::TYPE_MASK;

  if (type == TileFlags::TYPE_CENTER) {
    for (const auto &jtile : obj) {
      this->generate_direction_tiles(jtile["row"], jtile["col"], flags, 0);
    }
  }
  else {
    for (const auto &jtile : obj) {
      std::string str_orient = jtile["orientation"];
      int base_rot = 0;
      if (str_orient == "top-right" || str_orient == "right") {
        base_rot = -90;
      }
      else if (str_orient == "bottom-right" || str_orient == "bottom") {
        base_rot = 180;
      }
      else if (str_orient == "bottom-left" || str_orient == "left") {
        base_rot = 90;
      }
      else if (str_orient != "top") {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Tilegen: Found tile with no orientation; skipping");
        return;
      }

      this->generate_direction_tiles(jtile["row"], jtile["col"], flags, base_rot);
    }
  }
}

void DesertTilesheet::generate_direction_tiles(int row, int col, TileFlag flags, int base_rotation) {
  TileFlag medium = flags & TileFlags::MEDIUM_MASK;
  TileFlag type = flags & TileFlags::TYPE_MASK;
  assert(!(flags & TileFlags::DIR_MASK));

  auto offset = this->model_offset(row, col);
  for (int r = 0; r < 360; r += 90) {
    TileFlag dir = r << TileFlags::DIR_SHIFT;
    Transform t;
    t.set_rotation((float)(r - base_rotation));
    t.set_translation(offset);
    this->render_tiles_[medium | type | dir].emplace_back(RenderTile{t, row, col});
  }
}

/*
void DesertTilesheet::render(int x, int y, TileFlag flags) {

  glm::mat4 world_offset{glm::translate(glm::mat4{1.f}, glm::vec3{x, y, 0.f})};

  this->registry_.sprite_renderer().render(*this, tile.transform.get_matrix() * world_offset, {1.f, 1.f, 1.f},
                                           tile.row, tile.col);
}
 */

const RenderTile& DesertTilesheet::tile(double unit_interval, TileFlag flags) const {
  assert(this->render_tiles_.contains(flags));
  const auto &tiles = this->render_tiles_.at(flags);
  assert(!tiles.empty());

  int idx = std::floor(unit_interval * tiles.size());
  return tiles[idx];
}
