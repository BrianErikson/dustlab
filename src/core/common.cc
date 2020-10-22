#include "common.h"
#include <Transform.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

DustLabRegistry &DustLabRegistry::instance() {
  static DustLabRegistry instance;
  return instance;
}

DustLabRegistry::DustLabRegistry() {
  {
    auto &t = this->ecs.emplace<ETransform>(this->camera_);
    t.type = TransformType::PROJECTION;
    t.t.set(glm::ortho(0.f, 1.f, 0.f, 1.f, 0.0f, 10.f));
  }

  {
    auto &t = this->ecs.emplace<ETransform>(this->world_);
    t.type = TransformType::VIEW;
    t.t.set(glm::mat4{1.f});
  }
}

entt::entity DustLabRegistry::camera() {
  return this->camera_;
}

entt::entity DustLabRegistry::world() {
  return this->world_;
}

Transform& DustLabRegistry::view() {
  return this->ecs.get<ETransform>(this->world_).t;
}

Transform& DustLabRegistry::projection() {
  return this->ecs.get<ETransform>(this->camera_).t;
}

void DustLabRegistry::update(long dt) {
  this->timer_loop_.advance(dt);

  this->timers_.erase(std::remove_if(this->timers_.begin(), this->timers_.end(),
      [](const std::shared_ptr<TimerEventInterface>& a){
        return !a->active();
      }), this->timers_.end());

  for (auto &listener : this->listeners_) {
    for (auto &observed : listener->observer) {
      listener->callback(observed);
    }
    listener->observer.clear();
  }
}

void DustLabRegistry::timer_oneshot(long ms, const std::function<void()>& callback) {
  std::shared_ptr<TimerEvent<std::function<void()>>> timer{
    std::make_shared<TimerEvent<std::function<void()>>>(callback)};
  this->timers_.emplace_back(timer);
  this->timer_loop_.schedule(timer.get(), ms);
}

void DustLabRegistry::timer_recurring(long ms, const std::function<void()> &callback) {
  std::shared_ptr<TimerEvent<std::function<void()>>> timer{
      std::make_shared<TimerEvent<std::function<void()>>>([=, this](){
        callback();
        this->timer_recurring(ms, callback);
      })};
  this->timers_.emplace_back(timer);
  this->timer_loop_.schedule(timer.get(), ms);
}

void DustLabRegistry::subscribe(const std::shared_ptr<Listener> &listener) {
  this->listeners_.emplace_back(listener);
}

void DustLabRegistry::unsubscribe(Listener *listener) {
  std::erase_if(this->listeners_, [listener](const std::shared_ptr<Listener> &a) {
    return a.get() == listener;
  });
}
