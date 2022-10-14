//
// Created by Jacopo Gasparetto on 09/10/22.
//

#include "Torch.h"
#include "CoreMath.h"
#include <iostream>
#include <string>

void Torch::_register_methods() {
  godot::register_property("intensity", &Torch::magnitude, 1.0f);
  godot::register_property("dischargeRate", &Torch::dischargeRate, defaultDischargeRate);
  godot::register_method("_ready", &Torch::_ready);
  godot::register_method("_process", &Torch::_process);
  godot::register_method("_on_PlayAnimationTimer_timeout", &Torch::_on_PlayAnimationTimer_timeout);
  godot::register_method("_on_BlinkTimer_timeout", &Torch::_on_BlinkTimer_timeout);
  godot::register_method("_on_StopAnimationTimer_timeout", &Torch::_on_StopAnimationTimer_timeout);
  godot::register_method("_on_StopBlinkTimer_timeout", &Torch::_on_StopBlinkTimer_timeout);
}

void Torch::get_nodes() {
  m_shadowCaster = get_node<godot::Light2D>("ShadowCaster");
  m_AmbientLight = get_node<godot::Light2D>("AmbientLight");
  m_playAnimationTimer = get_node<godot::Timer>("PlayAnimationTimer");
  m_stopAnimationTimer = get_node<godot::Timer>("StopAnimationTimer");
  m_blinkTimer = get_node<godot::Timer>("BlinkTimer");
  m_stopBlinkTimer = get_node<godot::Timer>("StopBlinkTimer");
}

void Torch::_ready() {
  get_nodes();
  full_charge();
  set_energy(magnitude * m_intensity);
  std::cout << "Torch ready. Light magnitude: " << magnitude << ", energy: " << get_energy()
            << ", discharge rate: " << dischargeRate << std::endl;
}

void Torch::_process(real_t delta) {
  discharge();
  if (m_intensity <= 0.5f && !m_playAnimation)
    m_stopAnimationTimer->start();

  if (m_blink) {
    static real_t t{0};
    t += delta;
    playAnimation(t / m_stopBlinkTimer->get_wait_time());
  }
  if (m_intensity <= 0.25f) full_charge();
}

void Torch::set_energy(float energy) {
  m_energy = energy;
  if (m_shadowCaster && m_AmbientLight) {
    m_shadowCaster->set_energy(m_energy);
    m_AmbientLight->set_energy(m_energy);
  }
}

float Torch::get_energy() const { return m_energy; }

void Torch::discharge() {
  m_intensity -= dischargeRate / 10000.f;
  if (m_intensity <= 0) {
    m_intensity = 0.0f;
    return;
  }

  set_energy(magnitude * m_intensity);

  if (m_prev_intensity - m_intensity >= 0.05) {
    std::cout << "Light intensity: " << m_intensity << ", energy: " << get_energy() << std::endl;
    m_prev_intensity = m_intensity;
  }
}

void Torch::full_charge() {
  m_intensity = 1.0f;
  m_prev_intensity = 1.0f;
}

void Torch::_on_PlayAnimationTimer_timeout() {
#ifdef DEBUG
  std::cout << "Start Animation Timer" << std::endl;
#endif
  m_playAnimation = true;
  m_blinkTimer->start();
  m_stopAnimationTimer->start();
}

void Torch::_on_BlinkTimer_timeout() {
  m_blink = true;
  m_stopBlinkTimer->start();
}

void Torch::_on_StopAnimationTimer_timeout() {
#ifdef DEBUG
  std::cout << "Stop animation timer" << std::endl;
#endif
  m_playAnimation = false;
  if (!m_blinkTimer->is_stopped()) m_blinkTimer->stop();
  m_stopBlinkTimer->start();
}

void Torch::_on_StopBlinkTimer_timeout() {
  m_blink = false;
  m_blinkTimer->start();
}

void Torch::playAnimation(real_t t) {
  auto intensity = 1.0f - 0.5f * CoreGame::crossfade(CoreGame::smoothStep3(t), 1 - CoreGame::smoothStep3(t), t);
  set_energy(magnitude * intensity);
}
