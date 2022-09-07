//
// Created by Jacopo Gasparetto on 31/08/22.
//

#include "Main.h"

void Main::_ready() {
  auto loader = godot::ResourceLoader::get_singleton();
  m_player = get_node<Player>("Player");
  m_map = get_node<Map>("Map");

  m_lightFilter = get_node<LightFilter>("Map/LightFilter");
  m_lightFilter->set_player(m_player);
  godot::Ref<godot::ShaderMaterial> lightFilter_mat = m_lightFilter->get_material();
  lightFilter_mat->set_shader_param("fall_off", m_lightFalloff);

  m_visibility_polygon = VisibilityPolygon::_new();
  m_visibility_polygon->set_player(m_player);
  m_visibility_polygon->set_walls(m_map->get_walls());

  auto screen_size = get_viewport()->get_size();

  godot::Ref<godot::Shader> shader = loader->load("res://Shaders/Gradient.gdshader");
  auto mat = godot::ShaderMaterial::_new();
  mat->set_shader(shader);
  mat->set_shader_param("node_size", screen_size);
  mat->set_shader_param("aspect_ratio", screen_size.y / screen_size.x);
  mat->set_shader_param("fall_off", m_lightFalloff);
  m_visibility_polygon->set_material(mat);

  add_child(m_visibility_polygon);
  godot::Godot::print("Main scene ready");
}

void Main::_register_methods() {
  godot::register_method("_ready", &Main::_ready);
  godot::register_property("_lightFalloff", &Main::m_lightFalloff, 1.0f);
}