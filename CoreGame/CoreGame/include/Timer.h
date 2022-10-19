//
// Created by Jacopo Gasparetto on 17/10/22.
//

#ifndef COREGAME_TIMER_H
#define COREGAME_TIMER_H

#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>

namespace KCE {
class thread_guard {
  std::thread m_thread;

public:
  explicit thread_guard(std::thread &t) : m_thread{std::move(t)} {}
  ~thread_guard() {
    if (m_thread.joinable()) m_thread.join();
    std::cout << "Thread destroyed" << std::endl;
  }
};

class Timer {
  std::atomic<bool> clear = false;
  int _delay = 0;
  int _interval = 0;
  std::thread m_thread;

public:
  Timer() = default;
  ~Timer() { stop(); }
  template <typename Function> void setTimeout(Function function, int delay);
  template <typename Function> void setInterval(Function function, int interval);
  bool is_running() const { return !clear; }
  [[nodiscard]] int get_delay() const { return _delay; }
  [[nodiscard]] int get_interval() const { return _interval; }
  void stop() { this->clear = true; if (m_thread.joinable()) m_thread.join();}
};

template <typename Function> void Timer::setTimeout(Function function, int delay) {
  _delay = delay;
  stop();
  this->clear = false;
  std::thread t{([=]() {
    if (this->clear) return;
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    if (this->clear) return;
    function();
    this->clear = true;
  })};
  m_thread = std::move(t);
}

template <typename Function> void Timer::setInterval(Function function, int interval) {
  _interval = interval;
  stop();
  this->clear = false;
  std::thread t{([=]() {
    while (is_running()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(interval));
      if (this->clear) return;
      function();
    }
    this->clear = true;
  })};
  m_thread = std::move(t);
}
} // namespace KCE

#endif // COREGAME_TIMER_H
