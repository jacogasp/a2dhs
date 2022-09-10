//
// Created by Jacopo Gasparetto on 31/08/22.
//

#ifndef COREGAME_MAIN_H
#define COREGAME_MAIN_H

#include "Map.h"
#include "Player.h"
#include "VisibilityPolygon.h"
#include <ColorRect.hpp>
#include <Godot.hpp>
#include <Node2D.hpp>
#include <PackedScene.hpp>
#include <ResourceLoader.hpp>
#include <Viewport.hpp>

class Main : public godot::Node2D {
  GODOT_CLASS(Main, godot::Node2D)

  Player *m_player;
  Map *m_map;

    VisibilityPolygon *m_visibility_polygon;
    godot::ColorRect *m_lightFilter;
    godot::Vector2 m_screen_size;

public:
    float lightFalloff = 5.0f;
   // ~Main() {m_visibility_polygon->queue_free();}
  void _init() {}
  void _ready();
  void _process();
  static void _register_methods();
};

#endif // COREGAME_MAIN_H
