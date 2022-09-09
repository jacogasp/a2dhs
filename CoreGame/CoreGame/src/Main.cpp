//
// Created by Jacopo Gasparetto on 31/08/22.
//

#include "Main.h"

void Main::_ready() {
  m_screen_size = get_viewport()->get_size();
  auto loader = godot::ResourceLoader::get_singleton();
  m_player = get_node<Player>("Player");
  m_map = get_node<Map>("Map");

  m_lightFilter = get_node<godot::ColorRect>("LightFilter");
  godot::Ref<godot::ShaderMaterial> lightFilter_mat = m_lightFilter->get_material();
  lightFilter_mat->set_shader_param("fall_off", m_lightFalloff);
  lightFilter_mat->set_shader_param("aspect_ratio", m_screen_size.y / m_screen_size.x);

  m_visibility_polygon = VisibilityPolygon::_new();
  m_visibility_polygon->set_player(m_player);
  m_visibility_polygon->set_walls(m_map->get_walls());

  godot::Ref<godot::Shader> shader = loader->load("res://Shaders/LightCaster.gdshader");
  auto mat = godot::ShaderMaterial::_new();
  mat->set_shader(shader);
  mat->set_shader_param("node_size", m_screen_size);
  mat->set_shader_param("aspect_ratio", m_screen_size.y / m_screen_size.x);
  mat->set_shader_param("fall_off", m_lightFalloff);
  m_visibility_polygon->set_material(mat);

  add_child(m_visibility_polygon);
  godot::Godot::print("Main scene ready");
}

void Main::_process() {
  // Upload mouse normalized coordinates to shader
  auto playerPosition = m_player->get_position();
  auto playerDirection = m_player->get_direction();
  playerPosition.x /= m_screen_size.x;
  playerPosition.y /= m_screen_size.y;

  godot::Ref<godot::ShaderMaterial> visibility_polygon_mat = m_visibility_polygon->get_material();
  visibility_polygon_mat->set_shader_param("light_position", playerPosition);
  visibility_polygon_mat->set_shader_param("heading", playerDirection);


  godot::Ref<godot::ShaderMaterial> lightFilter_mat = m_lightFilter->get_material();
  lightFilter_mat->set_shader_param("light_position", playerPosition);
  lightFilter_mat->set_shader_param("heading", playerDirection);

//  godot::Godot::print(std::to_string(m_player->get_global_rotation()).c_str());
}

void Main::_register_methods() {
  godot::register_method("_ready", &Main::_ready);
  godot::register_method("_process", &Main::_process);
  godot::register_property("_lightFalloff", &Main::m_lightFalloff, 1.0f);
}