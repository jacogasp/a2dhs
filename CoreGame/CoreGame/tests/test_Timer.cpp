//
// Created by Jacopo Gasparetto on 17/10/22.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "Timer.h"
#include "doctest.h"

#include <chrono>
#include <cmath>
#include <thread>

using namespace KCE;

TEST_CASE("Timeout") {
  Timer timer;
  int result = 0;
  int delay = 5000;      // ms
  int checkpoint = 1000; // ms
  timer.setTimeout([&result]() { result = 42; }, delay);
  CHECK_EQ(timer.get_delay(), delay);
  std::this_thread::sleep_for(std::chrono::milliseconds(checkpoint));
  CHECK_EQ(result, 0);
  CHECK(timer.is_running());
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  CHECK_EQ(result, 42);
  CHECK(!timer.is_running());
}

TEST_CASE("Interval") {
  Timer timer;
  int interval = 1000;
  int result = 0;
  int max_repetition = 5;
  int current_repetition = 0;
  double expected_duration = interval * max_repetition;

  std::chrono::time_point<std::chrono::system_clock> lastTime;
  auto startTime = std::chrono::system_clock::now();

  timer.setInterval(
      [&]() {
        lastTime = std::chrono::system_clock::now();
        result += 42;
        ++current_repetition;
        std::cout << "Repetition: " << current_repetition << std::endl;
      },
      interval);

  CHECK_EQ(timer.get_interval(), interval);
  std::this_thread::sleep_for(std::chrono::milliseconds(interval * max_repetition + 500));
  timer.stop();

  auto duration = std::chrono::duration<double>{lastTime - startTime}.count() * 1000;
  CHECK_EQ(current_repetition, max_repetition);
  CHECK_EQ(result, 42 * max_repetition);
  CHECK_LE(std::abs(duration - static_cast<double>(interval * max_repetition)), expected_duration * 0.05);
}

TEST_CASE("Multiple timeout") {
  Timer timer;
  int delay1 = 100;
  int delay2 = 200;
  int total = 0;
  int expected_result = 2;

  timer.setTimeout([&]() { ++total; }, delay1);
  std::this_thread::sleep_for(std::chrono::milliseconds(delay1 + 10));
  timer.setTimeout([&]() { ++total; }, delay2);
  std::this_thread::sleep_for(std::chrono::milliseconds(delay2 + 10));

  CHECK_EQ(total, expected_result);
}