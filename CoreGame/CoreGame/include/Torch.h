//
// Created by Jacopo Gasparetto on 09/10/22.
//

#ifndef COREGAME_TORCH_H
#define COREGAME_TORCH_H

#include "Godot.hpp"
#include "Light2D.hpp"
#include "Node2D.hpp"

class Torch : public godot::Node2D {

  GODOT_CLASS(Torch, godot::Node2D);

  godot::Light2D *m_shadowCaster = nullptr;
  godot::Light2D *m_AmbientLight = nullptr;
  float m_energy{};
  float m_intensity{};
  float m_prev_intensity{};

public:
  float magnitude = 1.0f;
  float dischargeRate = 1.0f;
  static void _register_methods();
  void _init() {}
  void _ready();
  void _process();
  void set_energy(float energy);
  [[nodiscard]] float get_energy() const;
  inline void discharge();
  void full_charge();
};

#endif // COREGAME_TORCH_H
