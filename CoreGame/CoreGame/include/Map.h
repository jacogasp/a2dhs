//
// Created by Jacopo Gasparetto on 04/09/22.
//

#ifndef COREGAME_MAP_H
#define COREGAME_MAP_H

#include <Area2D.hpp>
#include <Array.hpp>
#include <CollisionPolygon2D.hpp>
#include <Godot.hpp>
#include <Input.hpp>
#include <Node2D.hpp>
#include <Polygon2D.hpp>
#include <PoolArrays.hpp>
#include <StaticBody2D.hpp>
#include <string>
#include <vector>

class Map : public godot::Node2D {
  GODOT_CLASS(Map, godot::Node2D)
  std::vector<const godot::Polygon2D *> m_walls;

public:
  void _init() {}
  void _ready();
  void _process();
  [[nodiscard]] std::vector<const godot::Polygon2D *> &get_walls();
  static void _register_methods();
};

#endif // COREGAME_MAP_H
