//
// Created by Jacopo Gasparetto on 30/08/22.
//
#include "Player.h"

void Player::_ready() {
  _screen_size = get_viewport_rect().size;
  godot::Godot::print("Player ready");
}

void Player::_register_methods() {
  godot::register_property("speed", &Player::speed, (real_t)200.0f);
  godot::register_method("_ready", &Player::_ready);
  godot::register_method("_process", &Player::_process);
  godot::register_method("_on_VisibilityNotifier2D_screen_exited", &Player::_on_VisibilityNotifier2D_screen_exited);
}

void Player::_process(const double p_delta) {
  auto position = get_viewport()->get_mouse_position();
  set_position(position);
}

void Player::_on_VisibilityNotifier2D_screen_exited() { queue_free(); }