#pragma once
#include <SDL2/SDL.h>
#include <entt/entt.hpp>
#include <Transform.h>
#include <timer-wheel.h>
#include <random>

struct Listener {
  entt::observer observer;
  std::function<void(entt::entity)> callback;
};

using Timer = std::shared_ptr<TimerEvent<std::function<void()>>>;

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
  Timer timer_oneshot(long ms, const std::function<void()>& callback);
  Timer timer_recurring(long ms, const std::function<void()>& callback);

  void subscribe(const std::shared_ptr<Listener> &listener);
  void unsubscribe(const std::shared_ptr<Listener> &listener);

  double rand();

  entt::registry ecs{};
  float zoom{0.03125f};

private:
  DustLabRegistry();
  std::list<std::shared_ptr<Listener>> listeners_{};
  std::list<std::shared_ptr<TimerEventInterface>> timers_{};
  TimerWheel timer_loop_{};

  entt::entity camera_{this->ecs.create()};
  entt::entity world_{this->ecs.create()};

  std::random_device rand_dev_{};
  std::default_random_engine rand_eng_{this->rand_dev_()};
  std::uniform_real_distribution<double> rand_{0.0, 1.0};
};
