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

class Player : public godot::Node2D {
  GODOT_CLASS(Player, godot::Node2D)
  godot::Vector2 _screen_size;
  godot::Vector2 m_velocity;
  godot::Vector2 m_acceleration;
  godot::Vector2 m_prev_pos;
  float m_heading = 0;
  float maxSpeed = 15.0f;
  float maxForce = 5.0f;

public:
  real_t speed = 400;
  static void _register_methods();

  void _init(){};
  void _ready();
  void _process();
  void _draw();
  void applyForce(const godot::Vector2 &force);
  [[nodiscard]] float get_heading() const;
  void _on_VisibilityNotifier2D_screen_exited();
};

#endif // COREGAME_PLAYER_H
