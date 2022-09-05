//
// Created by Jacopo Gasparetto on 31/08/22.
//

#ifndef COREGAME_MAIN_H
#define COREGAME_MAIN_H

#include <Godot.hpp>
#include <Node.hpp>
#include "Player.h"
#include "Map.h"
#include "VisibilityPolygon.h"

class Main : public godot::Node {
  GODOT_CLASS(Main, godot::Node)

  Player * m_player;
  Map * m_map;
  VisibilityPolygon * m_visibility_polygon;

public:

  void _init() { godot::Godot::print("Main init"); }
  void _ready();
  static void _register_methods();
};

#endif // COREGAME_MAIN_H
