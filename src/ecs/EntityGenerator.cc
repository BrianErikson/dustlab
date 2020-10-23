#include "EntityGenerator.h"
#include "core.h"

std::optional<entt::entity> EntityGenerator::BasicBot() {
  if (!this->witchcraft_spritesheet_->init()) {
    return {};
  }

  auto sprite_node = this->registry_.ecs.create();
  this->registry_.ecs.emplace<ETransform>(sprite_node);
  this->registry_.ecs.emplace<EName>(sprite_node).value = "Character";

  auto &ess = this->registry_.ecs.emplace<ESpriteModel>(sprite_node);
  ess.value = this->witchcraft_spritesheet_;
  ess.row = 0;
  ess.col = 0;
  ess.transform.set_translation(-this->witchcraft_spritesheet_->model_offset(ess.row, ess.col));
  const auto cell_size = this->witchcraft_spritesheet_->cell_size();
  ess.transform.set_scale({1/cell_size.width, 1/cell_size.height});

  auto &anim_timer = this->registry_.ecs.emplace<Timer>(sprite_node);

  anim_timer = this->registry_.timer_recurring(150, [sprite_node]() {
      DustLabRegistry::instance().ecs.patch<ESpriteModel>(sprite_node, [&](ESpriteModel &ess) {
        if (ess.col < ess.value->cols()) {
          ess.col++;
        } else {
          ess.col = 0;
        }

        ess.transform.set_translation(-ess.value->model_offset(ess.row, ess.col));
      });
  });

  return sprite_node;
}
