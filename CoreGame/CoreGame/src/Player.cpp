//
// Created by Jacopo Gasparetto on 30/08/22.
//
#include "Player.h"
#include <algorithm>
#include <iostream>

void Player::_ready() { godot::Godot::print("Player ready"); }

std::ostream &operator<<(std::ostream &os, const godot::Vector2 &v) { return os << "x: " << v.x << ", y: " << v.y; }

void Player::_process(const real_t p_delta) {
  auto input = godot::Input::get_singleton();
  godot::Vector2 velocity{0, 0};
  velocity.x = input->get_action_strength("move_right") - input->get_action_strength("move_left");
  velocity.y = input->get_action_strength("move_down") - input->get_action_strength("move_up");
  float inputMagnitude = std::clamp(velocity.length(), 0.0f, 1.0f);
  move_and_collide(velocity * speed * inputMagnitude * p_delta);
  if (velocity.length() > 0.0f) {
    float angle = velocity.angle();
    set_global_rotation(godot::Math::lerp_angle(get_global_rotation(), angle, 0.1f));
  }
  update();
}

void Player::_on_VisibilityNotifier2D_screen_exited() { queue_free(); }

godot::Vector2 Player::get_direction() const {
  godot::Vector2 v{};
  v.set_rotation(get_global_rotation());
  return v;
}

void Player::_register_methods() {
  godot::register_property("speed", &Player::speed, (real_t)200.0f);
  godot::register_method("_ready", &Player::_ready);
  godot::register_method("_process", &Player::_process);
  godot::register_method("_on_VisibilityNotifier2D_screen_exited", &Player::_on_VisibilityNotifier2D_screen_exited);
}