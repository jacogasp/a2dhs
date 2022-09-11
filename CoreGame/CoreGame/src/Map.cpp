//
// Created by Jacopo Gasparetto on 04/09/22.
//

#include "Map.h"

void Map::_ready() {
  auto walls = get_children();

  for (int i = 0; i < walls.size(); ++i) {
    auto wall = static_cast<godot::Polygon2D *>(walls[i]);
    auto collisionPolygon2D = godot::CollisionPolygon2D::_new();
    collisionPolygon2D->set_polygon(wall->get_polygon());
    auto staticBody = godot::StaticBody2D::_new();
    staticBody->add_child(collisionPolygon2D);
    add_child(staticBody);
    m_walls.push_back(wall);
  }
  godot::String m {"Map ready: "};
  m += godot::String(std::to_string(m_walls.size()).c_str());
  m += " walls";
  godot::Godot::print(m);
}

void Map::_process() {}


std::vector<const godot::Polygon2D *> &Map::get_walls() { return m_walls; }

void Map::_register_methods() {
  godot::register_method("_ready", &Map::_ready);
  godot::register_method("_process", &Map::_process);
}
