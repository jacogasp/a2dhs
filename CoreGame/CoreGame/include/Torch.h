//
// Created by Jacopo Gasparetto on 09/10/22.
//

#ifndef COREGAME_TORCH_H
#define COREGAME_TORCH_H

#include "Godot.hpp"
#include "Light2D.hpp"
#include "Node2D.hpp"
#include "Timer.hpp"
#include <functional>
#include <iostream>
#include <random>
#include <string>

#include "Timer.h"

static std::random_device rnd_device;
static std::mt19937 rnd_engine{rnd_device()};

static constexpr int k_minFlickeringDuration = 250;  // ms
static constexpr int k_maxFlickeringDuration = 750;  // ms
static constexpr int k_minSpikeDuration = 20;        // ms
static constexpr int k_maxSpikeDuration = 200;       // ms
static constexpr int k_maxAnimationDelayTime = 3000; // ms

class Torch : public godot::Node2D {
  GODOT_CLASS(Torch, godot::Node2D);
  godot::Light2D *m_shadowCaster = nullptr;
  godot::Light2D *m_AmbientLight = nullptr;
  KCE::Timer m_playAnimationTimer;
  KCE::Timer m_stopAnimationTimer;
  KCE::Timer m_startBlinkTimer;
  KCE::Timer m_stopBlinkTimer;

  float m_energy{1.0f};
  float m_intensity{1.0f};
  int m_lastBatteryCharge{100};

  bool m_animationAutoPlaying = false;
  float m_currentEnergy{0.f};
  float m_batteryCurrentTime{0.0f};
  bool m_isPaused = false;
  std::function<void(void)> m_onBatteryRunOut;

  void get_nodes();

public:
  float magnitude{1.0f};
  float batteryLifeTime{60}; // 2 min
  int minFlickeringDuration = k_minFlickeringDuration;
  int maxFlickeringDuration = k_maxFlickeringDuration;
  int minSpikeDuration = k_minSpikeDuration;
  int maxSpikeDuration = k_maxSpikeDuration;
  int maxAnimationDelayTime = k_maxAnimationDelayTime;

  std::uniform_int_distribution<int> animationDistribution{minFlickeringDuration, maxFlickeringDuration};
  std::uniform_int_distribution<int> blinkingDistribution{minSpikeDuration, maxSpikeDuration};

  Torch() { std::cout << "Light Intensity: " << m_intensity << ", default magnitude: " << magnitude << std::endl; }
  ~Torch();

  static void _register_methods();
  void _init() {}
  void _ready();
  void _process(real_t delta);
  void set_energy(float energy);
  [[nodiscard]] float get_energy() const;
  [[nodiscard]] inline int batteryCharge() const;
  inline void printBatteryCharge() const;
  void discharge(real_t t);
  void setOnBatterRunOutCallback(const std::function<void(void)> &&callback) { m_onBatteryRunOut = callback; }
  void full_charge();
  void pause();
  void resume();
  void startAnimation();
  void stopAnimation();
  void startBlinking();
  void stopBlinking();
};

#endif // COREGAME_TORCH_H
