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
using namespace std::chrono_literals;
using sc = std::chrono::system_clock;

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
  auto startTime = sc::now();

  timer.setInterval(
      [&]() {
        lastTime = sc::now();
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

TEST_CASE("Interrupt Timeout") {
  Timer timer1, timer2;
  int delay = 5000;
  int interruptDelay = 2500;
  timer1.setTimeout([&] { std::cout << "Task 1 executed"; }, delay);
  timer2.setTimeout([&] { std::cout << "Task 2 executed"; }, delay);
  CHECK(timer1.is_running());
  CHECK(timer2.is_running());

  std::this_thread::sleep_for(std::chrono::milliseconds(interruptDelay));

  timer1.stop();
  CHECK(!timer1.is_running());
  CHECK(timer2.is_running());
}

TEST_CASE("Fast timer") {
  Timer timer;
  int delay = 200;
  int deltaT = 20;
  auto startTime = sc::now();

  timer.setTimeout(
      [&] {
        auto duration = (double)(sc::now() - startTime).count() / 1000.0;
        std::cout << "Fast timeout! " << duration << " ms\n";
      },
      delay);

  CHECK(timer.is_running());
  std::this_thread::sleep_for(std::chrono::milliseconds(delay + deltaT));
  CHECK(!timer.is_running());

  delay = 20;
  deltaT = 5;
  startTime = sc::now();
  timer.setTimeout(
      [&] {
        auto duration = (double)(sc::now() - startTime).count() / 1000.0;
        std::cout << "Faster timeout! " << duration << " ms\n";
      },
      delay);

  CHECK(timer.is_running());
  std::this_thread::sleep_for(std::chrono::milliseconds(delay + deltaT));
  CHECK(!timer.is_running());
}

TEST_CASE("Fast Interrupt Timeout") {
  Timer timer1, timer2;
  int delay = 200;
  int interruptDelay = 20;

  auto startTime = sc::now();
  timer1.setTimeout([&] { std::cout << "Fast timeout 1"; }, delay);
  timer2.setTimeout([&] { std::cout << "Fast timeout 2"; }, delay);
  CHECK(timer1.is_running());
  CHECK(timer2.is_running());

  std::this_thread::sleep_for(std::chrono::milliseconds(interruptDelay));
  timer1.stop();
  CHECK(!timer1.is_running());
  CHECK(timer2.is_running());
  auto duration = static_cast<double>((sc::now() - startTime).count()) / 1000;
  std::cout << "Fast interrupt " << duration << " ms\n";
}

TEST_CASE("Timeout stress") {
  Timer timer;
  int delay = 20;
  int n_iterations = 100;
  double total_duration = 0;
  double expected_duration = delay * n_iterations;

  for (int i = 0; i < n_iterations; ++i) {
    auto startTime = sc::now();
    timer.setTimeout(
        [&] {
          std::cout << "Repetition " << i << "\n";
          total_duration += static_cast<double>((sc::now() - startTime).count()) / 1000;
        },
        delay);
    std::this_thread::sleep_for(std::chrono::milliseconds(delay + 5));
  }
  CHECK(!timer.is_running());
  std::cout << "Total duration: " << total_duration << " ms\n";
  CHECK_LE(std::abs(total_duration - expected_duration), expected_duration * 0.2);
}

TEST_CASE("Timeout stress no wait") {
  Timer timer;
  int delay = 20;
  int n_iterations = 100;
  double total_duration = 0;

  for (std::atomic_int i = 0; i < n_iterations; i++) {
    auto startTime = sc::now();
    timer.setTimeout(
        [&] {
          std::cout << "Iteration " << i << "\n";
          total_duration += static_cast<double>((sc::now() - startTime).count()) / 1000;
        },
        delay);
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  CHECK(!timer.is_running());
  std::cout << "Total duration: " << total_duration << " ms\n";
  CHECK_LE(std::abs(total_duration - delay), delay * 0.25);
}

TEST_CASE("Timeout stress stop") {
  Timer timer;
  int delay = 20;
  int n_iterations = 100;
  double total_duration = 0;

  for (std::atomic_int i = 0; i < n_iterations; i++) {
    auto startTime = sc::now();
    timer.setTimeout(
        [&] {
          std::cout << "Iteration " << i << "\n";
          total_duration += static_cast<double>((sc::now() - startTime).count()) / 1000;
        },
        delay);
    timer.stop();
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  CHECK(!timer.is_running());
  std::cout << "Total duration: " << total_duration << " ms\n";
  CHECK_LE(std::abs(total_duration), 0.01);
}