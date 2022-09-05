//
// Created by Jacopo Gasparetto on 31/08/22.
//

#include "Main.h"

void Main::_ready() {
  m_player = get_node<Player>("Player");
  m_map = get_node<Map>("Map");
  m_visibility_polygon = VisibilityPolygon::_new();
  m_visibility_polygon->set_player(m_player);
  m_visibility_polygon->set_walls(m_map->get_walls());
  add_child(m_visibility_polygon);
  godot::Godot::print("Main scene ready");
}

void Main::_register_methods() { godot::register_method("_ready", &Main::_ready); }