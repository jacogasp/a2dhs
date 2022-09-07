//
// Created by Jacopo Gasparetto on 31/08/22.
//

#ifndef COREGAME_MAIN_H
#define COREGAME_MAIN_H

#include "LightFilter.h"
#include "Map.h"
#include "Player.h"
#include "VisibilityPolygon.h"
#include <Godot.hpp>
#include <Node.hpp>
#include <PackedScene.hpp>
#include <ResourceLoader.hpp>

class Main : public godot::Node {
  GODOT_CLASS(Main, godot::Node)

  Player *m_player;
  Map *m_map;
  VisibilityPolygon *m_visibility_polygon;
  LightFilter *m_lightFilter;
  float m_lightFalloff = 1.0f;

public:
  void _init() {}
  void _ready();
  static void _register_methods();
};

#endif // COREGAME_MAIN_H
