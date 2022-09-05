//
// Created by Jacopo Gasparetto on 04/09/22.
//

#include "Map.h"

void Map::_ready() {
  auto walls = get_tree()->get_nodes_in_group("walls");
  for (int i = 0; i < walls.size(); ++i) {
    m_walls.push_back(static_cast<godot::Polygon2D *>(walls[i]));
  }
  godot::String m {"Map ready: "};
  m += godot::String(std::to_string(m_walls.size()).c_str());
  m += " walls";
  godot::Godot::print(m);
}

void Map::_process() {}

void Map::_input(godot::InputEvent *inputEvent) {
  if (inputEvent->is_action_released(godot::String("click_left"))) {
    auto mouseEvent = reinterpret_cast<godot::InputEventMouseButton *>(inputEvent);
    if (m_new_polygon) {
      godot::Godot::print("add vertex!");
      godot::Godot::print(godot::Variant(mouseEvent->get_position()));
      auto polygon = m_new_polygon->get_polygon();
      polygon.push_back(mouseEvent->get_position());
      m_new_polygon->set_polygon(polygon);
    } else {
      godot::Godot::print("new poly!");
      godot::Godot::print(godot::Variant(mouseEvent->get_position()));
      godot::PoolVector2Array polygon;
      polygon.push_back(mouseEvent->get_position());
      m_new_polygon = godot::Polygon2D::_new();
      m_new_polygon->set_polygon(polygon);
      add_child(m_new_polygon);
    }
  }
  if (inputEvent->is_action_released(godot::String("jump"))) {
    m_walls.push_back(m_new_polygon);
    m_new_polygon = nullptr;
    godot::Godot::print("Save poly");
  }
}

std::vector<const godot::Polygon2D *> &Map::get_walls() { return m_walls; }

void Map::_register_methods() {
  godot::register_method("_ready", &Map::_ready);
  godot::register_method("_process", &Map::_process);
  godot::register_method("_input", &Map::_input);
}
