#pragma once
#include <SDL2/SDL.h>
#include <entt/entt.hpp>
#include <Transform.h>
#include <timer-wheel.h>

struct Listener {
  entt::observer observer;
  std::function<void(entt::entity)> callback;
};

class DustLabRegistry {
public:
  static DustLabRegistry& instance();
  DustLabRegistry(DustLabRegistry const&) = delete;
  void operator=(DustLabRegistry const&) = delete;

  entt::entity camera();
  entt::entity world();
  Transform& view();
  Transform& projection();

  void update(long dt);
  void timer_oneshot(long ms, const std::function<void()>& callback);
  void timer_recurring(long ms, const std::function<void()>& callback);

  void subscribe(const std::shared_ptr<Listener> &listener);
  void unsubscribe(Listener *listener);

  entt::registry ecs{};

private:
  DustLabRegistry();
  std::list<std::shared_ptr<Listener>> listeners_;
  std::list<std::shared_ptr<TimerEventInterface>> timers_;
  TimerWheel timer_loop_;
  entt::entity camera_{this->ecs.create()};
  entt::entity world_{this->ecs.create()};
};
