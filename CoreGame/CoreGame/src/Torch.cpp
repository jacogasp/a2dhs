//
// Created by Jacopo Gasparetto on 09/10/22.
//

#include "Torch.h"
#include <string>

void Torch::_register_methods() {
  godot::register_property("intensity", &Torch::magnitude, 1.0f);
  godot::register_property("dischargeRate", &Torch::dischargeRate, 1.0f);
  godot::register_method("_ready", &Torch::_ready);
  godot::register_method("_process", &Torch::_process);
}

void Torch::_ready() {
  full_charge();
  m_shadowCaster = get_node<godot::Light2D>("ShadowCaster");
  m_AmbientLight = get_node<godot::Light2D>("AmbientLight");
  set_energy(magnitude * m_intensity);
  std::string m = "Torch ready. Light energy: " + std::to_string(get_energy());
  m += ", discharge rate: " + std::to_string(dischargeRate);
  godot::Godot::print(m.c_str());
}

void Torch::_process() {
  discharge();
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
    auto m = "Light intensity: " + std::to_string(m_intensity);
    m += ", energy: " + std::to_string(get_energy());
    godot::Godot::print(m.c_str());
    m_prev_intensity = m_intensity;
  }
}

void Torch::full_charge() {
  m_intensity = 1.0f;
  m_prev_intensity = 1.0f;
}
