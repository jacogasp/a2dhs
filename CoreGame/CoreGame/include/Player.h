//
// Created by Jacopo Gasparetto on 30/08/22.
//

#ifndef COREGAME_PLAYER_H
#define COREGAME_PLAYER_H

#include "CoreMath.h"
#include <AnimatedSprite.hpp>
#include <CollisionShape2D.hpp>
#include <Godot.hpp>
#include <Input.hpp>
#include <InputEvent.hpp>
#include <InputEventMouse.hpp>
#include <KinematicBody2D.hpp>
#include <KinematicCollision2D.hpp>
#include <Viewport.hpp>

class Player : public godot::KinematicBody2D {
  GODOT_CLASS(Player, godot::KinematicBody2D)
  godot::Vector2 m_velocity;
  godot::Vector2 m_acceleration;

public:
  real_t speed = 200.0f;
  static void _register_methods();

  void _init(){};
  void _ready();
  void _process(real_t p_delta);
  [[nodiscard]] godot::Vector2 get_direction() const;
  void _on_VisibilityNotifier2D_screen_exited();
};

#endif // COREGAME_PLAYER_H
