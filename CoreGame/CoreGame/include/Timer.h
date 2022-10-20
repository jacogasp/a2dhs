//
// Created by Jacopo Gasparetto on 17/10/22.
//

#ifndef COREGAME_TIMER_H
#define COREGAME_TIMER_H

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

namespace KCE {

class InterruptableSleeper {
  std::timed_mutex mut_;
  std::atomic_bool lockedByCreator_ = false; // track whether the mutex is locked
  void lock() {                      // lock mutex
    mut_.lock();
    lockedByCreator_ = true;
  }
  void unlock() { // unlock mutex
    lockedByCreator_ = false;
    mut_.unlock();
  }

public:
  // lock on creation
  InterruptableSleeper() { lock(); }
  // unlock on destruction, if wake was never called
  ~InterruptableSleeper() {
    if (lockedByCreator_) {
      unlock();
    }
  }
  // called by any thread except the creator
  // waits until wake is called or the specified time passes
  template <class Rep, class Period> void sleepFor(const std::chrono::duration<Rep, Period> &timeout_duration) {
    if (mut_.try_lock_for(timeout_duration)) {
      // if successfully locked,
      // remove the lock
      mut_.unlock();
    }
  }
  // unblock any waiting threads, handling a situation
  // where wake has already been called.
  // should only be called by the creating thread
  void wake() {
    if (lockedByCreator_) {
      unlock();
    }
  }

  void reset() { lock(); }
};

class Timer {
  std::atomic<bool> clear = false;
  int _delay = 0;
  int _interval = 0;
  std::thread m_thread;
  InterruptableSleeper m_sleeper;

  void reset() {
    this->clear = true;
    if (m_thread.joinable()) m_thread.join();
  }

public:
  Timer() = default;
  ~Timer() { stop(); }
  template <typename Function> void setTimeout(Function function, int delay);
  template <typename Function> void setInterval(Function function, int interval);
  bool is_running() const { return !clear; }
  [[nodiscard]] int get_delay() const { return _delay; }
  [[nodiscard]] int get_interval() const { return _interval; }
  void stop() {
    m_sleeper.wake();
    reset();
    m_sleeper.reset();
  }
};

template <typename Function> void Timer::setTimeout(Function function, int delay) {
  _delay = delay;
  reset();
  this->clear = false;
  std::thread t{([=]() {
    if (this->clear) return;
    m_sleeper.sleepFor(std::chrono::milliseconds(delay));
    if (this->clear) return;
    function();
    this->clear = true;
  })};
  m_thread = std::move(t);
}

template <typename Function> void Timer::setInterval(Function function, int interval) {
  _interval = interval;
  reset();
  this->clear = false;
  std::thread t{([=]() {
    while (is_running()) {
      m_sleeper.sleepFor(std::chrono::milliseconds(interval));
      if (this->clear) return;
      function();
    }
    this->clear = true;
  })};
  m_thread = std::move(t);
}
} // namespace KCE

#endif // COREGAME_TIMER_H
