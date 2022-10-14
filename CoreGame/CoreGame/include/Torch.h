//
// Created by Jacopo Gasparetto on 09/10/22.
//

#ifndef COREGAME_TORCH_H
#define COREGAME_TORCH_H

#include "Godot.hpp"
#include "Light2D.hpp"
#include "Node2D.hpp"
#include "Timer.hpp"
#include <iostream>
#include <string>

class Torch : public godot::Node2D {
  GODOT_CLASS(Torch, godot::Node2D);
  static constexpr float defaultDischargeRate = {1.0f};
  godot::Light2D *m_shadowCaster = nullptr;
  godot::Light2D *m_AmbientLight = nullptr;
  godot::Timer *m_playAnimationTimer = nullptr;
  godot::Timer *m_stopAnimationTimer = nullptr;
  godot::Timer *m_blinkTimer = nullptr;
  godot::Timer *m_stopBlinkTimer = nullptr;
  float m_energy{1.0f};
  float m_intensity{1.0f};
  float m_prev_intensity{1.0f};
  bool m_playAnimation{};
  bool m_blink{};

  void get_nodes();

public:
  float magnitude = 1.0f;
  float dischargeRate = 1.0f;
  Torch() {
    std::cout << "Light Intensity: " << m_intensity << ", default magnitude: " << magnitude <<  std::endl; }
  static void _register_methods();
  void _init() {}
  void _ready();
  void _process(real_t delta);
  void set_energy(float energy);
  [[nodiscard]] float get_energy() const;
  inline void discharge();
  void full_charge();
  void _on_PlayAnimationTimer_timeout();
  void _on_BlinkTimer_timeout();
  void _on_StopAnimationTimer_timeout();
  void _on_StopBlinkTimer_timeout();
  void playAnimation(real_t t);
};

#endif // COREGAME_TORCH_H
