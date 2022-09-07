//
// Created by Jacopo Gasparetto on 07/09/22.
//

#include "LightFilter.h"

void LightFilter::_ready() {
  godot::Godot::print("Light Filter ready");
  godot::Ref<godot::ShaderMaterial> mat = get_material();
  auto screen_size = get_viewport()->get_size();
  mat->set_shader_param("aspect_ratio", screen_size.y / screen_size.x);
}

void LightFilter::_process() {
  godot::Ref<godot::ShaderMaterial> mat = get_material();
  auto pos = m_player->get_position();
  auto screen_size = get_viewport()->get_size();
  pos.x /= screen_size.x;
  pos.y /= screen_size.y;
  mat->set_shader_param("mouse_position", pos);
}

void LightFilter::set_player(const godot::Node2D *player) {
  m_player = player;
}

void LightFilter::_register_methods() {
  godot::register_method("_ready", &LightFilter::_ready);
  godot::register_method("_process", &LightFilter::_process);
}