//
// Created by Jacopo Gasparetto on 30/08/22.
//
#include "Player.h"

void Player::_ready() {
  _screen_size = get_viewport_rect().size;
  godot::Godot::print("Player ready");
}

void Player::_process() {
  auto target = get_viewport()->get_mouse_position();
  auto desired = target - get_position();
  if (desired.length() < 1) return;
  float distance = desired.length();
  desired.normalize();

  if (distance < 100) {
    desired *= (distance * .5f);
    desired = desired.clamped(maxSpeed);
  } else {
    desired *= maxSpeed;
  }
  auto steer = desired - m_velocity;
  steer = steer.clamped(maxForce);
  applyForce(steer);
  m_velocity += m_acceleration;
  set_position(get_position() + m_velocity);
  m_acceleration *= 0;
  update();
}

void Player::applyForce(const godot::Vector2 &force) { m_acceleration += force; }

void Player::_draw() {
  godot::Vector2 origin{0.0, 0.0};
  origin -= get_global_position();
  godot::Vector2 dir{};
  dir.set_rotation(m_velocity.angle());
  dir.normalize();
  dir *= 200;
  godot::Vector2 A = get_position();
  godot::Vector2 B = A + dir;

  A -= get_global_position();
  B -= get_global_position();
  draw_line(A, B, {255, 0, 0});
}

void Player::_on_VisibilityNotifier2D_screen_exited() { queue_free(); }

float Player::get_heading() const { return m_heading; }

void Player::_register_methods() {
  godot::register_property("speed", &Player::speed, (real_t)200.0f);
  godot::register_method("_ready", &Player::_ready);
  godot::register_method("_process", &Player::_process);
  godot::register_method("_draw", &Player::_draw);
  godot::register_method("_on_VisibilityNotifier2D_screen_exited", &Player::_on_VisibilityNotifier2D_screen_exited);
}