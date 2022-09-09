//
// Created by Jacopo Gasparetto on 30/08/22.
//

#ifndef COREGAME_PLAYER_H
#define COREGAME_PLAYER_H

#include <AnimatedSprite.hpp>
#include <CollisionShape2D.hpp>
#include <Godot.hpp>
#include <Input.hpp>
#include <InputEvent.hpp>
#include <InputEventMouse.hpp>
#include <Node2D.hpp>
#include <Viewport.hpp>
#include "CoreMath.h"

class Player : public godot::Node2D {
  GODOT_CLASS(Player, godot::Node2D)
  godot::Vector2 _screen_size;
  godot::Vector2 m_velocity;
  godot::Vector2 m_acceleration;
  godot::Vector2 m_prev_pos;
  float maxSpeed = 5.0f;
  float maxForce = 1.0f;

public:
  real_t speed = 400;
  static void _register_methods();

  void _init(){};
  void _ready();
  void _process();
  void applyForce(const godot::Vector2 &force);
  [[nodiscard]] const godot::Vector2& get_velocity() const;
  void _on_VisibilityNotifier2D_screen_exited();
};

#endif // COREGAME_PLAYER_H
