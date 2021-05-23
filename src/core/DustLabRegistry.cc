#include "DustLabRegistry.h"
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
    t.t.set(glm::ortho(0.f, 32.f, 0.f, 32.f, -1.0f, 1.f));
  }

  {
    auto &t = this->ecs.emplace<ETransform>(this->world_);
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

Timer DustLabRegistry::timer_oneshot(long ms, const std::function<void()>& callback) {
  Timer timer{
    std::make_shared<TimerEvent<std::function<void()>>>(callback)};
  this->timers_.emplace_back(timer);
  this->timer_loop_.schedule(timer.get(), ms);

  return timer;
}

Timer DustLabRegistry::timer_recurring(long ms, const std::function<void()> &callback) {
  Timer timer{
      std::make_shared<TimerEvent<std::function<void()>>>([=, this](){
        callback();
        this->timer_recurring(ms, callback);
      })};
  this->timers_.emplace_back(timer);
  this->timer_loop_.schedule(timer.get(), ms);

  return timer;
}

void DustLabRegistry::subscribe(const std::shared_ptr<Listener> &listener) {
  this->listeners_.emplace_back(listener);
}

void DustLabRegistry::unsubscribe(const std::shared_ptr<Listener> &listener) {
  auto iter = std::find(this->listeners_.begin(), this->listeners_.end(), listener);
  while (iter != this->listeners_.end()) {
    this->listeners_.erase(iter);
    iter = std::find(this->listeners_.begin(), this->listeners_.end(), listener);
  }
}

double DustLabRegistry::rand() {
  return this->rand_(this->rand_eng_);
}
