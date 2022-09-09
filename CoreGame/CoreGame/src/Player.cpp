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
  if (desired.length() < 20) {
    m_acceleration *= 0;
    return;
  }
  float distance = desired.length();
  desired.normalize();

  if (distance < 20) {
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

void Player::_on_VisibilityNotifier2D_screen_exited() { queue_free(); }

const godot::Vector2 &Player::get_velocity() const { return m_velocity; }

void Player::_register_methods() {
  godot::register_property("speed", &Player::speed, (real_t)200.0f);
  godot::register_method("_ready", &Player::_ready);
  godot::register_method("_process", &Player::_process);
  godot::register_method("_on_VisibilityNotifier2D_screen_exited", &Player::_on_VisibilityNotifier2D_screen_exited);
}