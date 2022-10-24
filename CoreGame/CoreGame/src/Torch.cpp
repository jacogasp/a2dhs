//
// Created by Jacopo Gasparetto on 09/10/22.
//

#include "Torch.h"
#include "CoreMath.h"
#include <cmath>
#include <iostream>

Torch::~Torch() {
  m_stopAnimationTimer.stop();
  m_stopBlinkTimer.stop();
  m_playAnimationTimer.stop();
  m_startBlinkTimer.stop();
}

void Torch::_register_methods() {
  godot::register_property("intensity", &Torch::magnitude, 1.0f);
  godot::register_property("batteryLifeTime", &Torch::batteryLifeTime, 60.0f);
  godot::register_property("minFlickeringDuration", &Torch::minFlickeringDuration, k_minFlickeringDuration);
  godot::register_property("maxFlickeringDuration", &Torch::maxFlickeringDuration, k_maxFlickeringDuration);
  godot::register_property("minSpikeDuration", &Torch::minSpikeDuration, k_minSpikeDuration);
  godot::register_property("maxSpikeDuration", &Torch::maxSpikeDuration, k_maxSpikeDuration);
  godot::register_property("maxAnimationDelayTime", &Torch::maxAnimationDelayTime, k_maxAnimationDelayTime);
  godot::register_method("_ready", &Torch::_ready);
  godot::register_method("_process", &Torch::_process);
}

void Torch::get_nodes() {
  m_shadowCaster = get_node<godot::Light2D>("ShadowCaster");
  m_AmbientLight = get_node<godot::Light2D>("AmbientLight");
}

void Torch::_ready() {
  get_nodes();
  full_charge();
  set_energy(magnitude * m_intensity);
  animationDistribution = std::uniform_int_distribution{minFlickeringDuration, maxFlickeringDuration};
  blinkingDistribution = std::uniform_int_distribution{minSpikeDuration, maxSpikeDuration};
  std::cout << "Torch ready. Light magnitude: " << magnitude << ", energy: " << get_energy()
            << ", battery lifetime: " << batteryLifeTime << " s" << std::endl;
}

void Torch::_process(real_t delta) {
  if (m_isPaused) return;
  m_batteryCurrentTime += delta;
  discharge(m_batteryCurrentTime / batteryLifeTime);
  if (m_intensity <= 0.75f && !m_animationAutoPlaying) startAnimation();

  if (m_intensity <= 0.1f) {
    if (m_onBatteryRunOut) m_onBatteryRunOut();
    pause();
  }
}

void Torch::set_energy(float energy) {
  m_energy = energy;
  if (m_shadowCaster && m_AmbientLight) {
    m_shadowCaster->set_energy(m_energy);
    m_AmbientLight->set_energy(m_energy);
  }
}

float Torch::get_energy() const { return m_energy; }

int Torch::batteryCharge() const { return std::floor(m_intensity * 100.0f); }

void Torch::printBatteryCharge() const {
  godot::Godot::print("Battery charge: " + godot::String::num_int64(batteryCharge()) + "%");
}

void Torch::discharge(real_t t) {
  m_intensity = CoreGame::flip(CoreGame::smoothStart2(t));

#ifdef DEBUG
  auto bc = batteryCharge();
  if (bc % 10 == 0 && bc != m_lastBatteryCharge) {
    printBatteryCharge();
    m_lastBatteryCharge = bc;
  }
#endif
  if (m_intensity <= 0) {
    m_intensity = 0.0f;
    return;
  }
  set_energy(magnitude * m_intensity);
}

void Torch::full_charge() {
  m_intensity = 1.0f;
  m_lastBatteryCharge = 100;
  m_batteryCurrentTime = 0.0f;
  m_animationAutoPlaying = false;
  m_stopAnimationTimer.stop();
  m_stopBlinkTimer.stop();
  m_playAnimationTimer.stop();
  m_startBlinkTimer.stop();

  godot::Godot::print("Battery fully charged!");
#ifdef DEBUG
  printBatteryCharge();
#endif
}

void Torch::pause() {
  m_isPaused = true;
  m_stopAnimationTimer.stop();
  m_stopBlinkTimer.stop();
  m_playAnimationTimer.stop();
  m_startBlinkTimer.stop();
}

void Torch::resume() {
  m_isPaused = false;
  if (m_intensity <= 0.75f && !m_animationAutoPlaying) startAnimation();
}

void Torch::startAnimation() {
#ifdef DEBUG
  std::cout << "Start blinking animation" << std::endl;
#endif
  m_animationAutoPlaying = true;
  startBlinking();
  auto delay = animationDistribution(rnd_engine);
  m_stopAnimationTimer.setTimeout([&]() { stopAnimation(); }, delay);
}

void Torch::stopAnimation() {
#ifdef DEBUG
  std::cout << "Stop blinking animation" << std::endl;
#endif
  m_startBlinkTimer.stop();
  m_stopBlinkTimer.stop();
  auto nextAnimationDelay = static_cast<int>((float)maxAnimationDelayTime * m_intensity);
  m_playAnimationTimer.setTimeout([&]() { startAnimation(); }, nextAnimationDelay);
}

void Torch::startBlinking() {
  m_currentEnergy = get_energy();
  set_energy(m_currentEnergy * 0.75f);
  auto delay = blinkingDistribution(rnd_engine);
  m_stopBlinkTimer.setTimeout([&]() { stopBlinking(); }, delay);
}

void Torch::stopBlinking() {
  set_energy(m_currentEnergy);
  auto delay = blinkingDistribution(rnd_engine);
  m_startBlinkTimer.setTimeout([&]() { startBlinking(); }, delay);
}
