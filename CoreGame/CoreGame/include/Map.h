//
// Created by Jacopo Gasparetto on 04/09/22.
//

#ifndef A2DHS_MAP_H
#define A2DHS_MAP_H

#include <Array.hpp>
#include <Godot.hpp>
#include <Input.hpp>
#include <InputEvent.hpp>
#include <InputEventMouseButton.hpp>
#include <Node.hpp>
#include <Polygon2D.hpp>
#include <PoolArrays.hpp>
#include <SceneTree.hpp>
#include <Viewport.hpp>
#include <vector>
#include <string>

class Map : public godot::Node {
  GODOT_CLASS(Map, godot::Node)
  godot::Polygon2D *m_new_polygon;
  godot::Array _walls;
  std::vector<const godot::Polygon2D *> m_walls;

public:
  void _init() {}
  void _ready();
  void _process();
  void _input(godot::InputEvent *inputEvent);
  [[nodiscard]] std::vector<const godot::Polygon2D *>& get_walls();
  static void _register_methods();
};

#endif // A2DHS_MAP_H
