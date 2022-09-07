//
// Created by Jacopo Gasparetto on 07/09/22.
//

#ifndef COREGAME_LIGHTFILTER_H
#define COREGAME_LIGHTFILTER_H

#include <ColorRect.hpp>
#include <Godot.hpp>
#include <Node2D.hpp>
#include <ShaderMaterial.hpp>
#include <Viewport.hpp>

class LightFilter : public godot::ColorRect {
  GODOT_CLASS(LightFilter, godot::ColorRect)

  const godot::Node2D *m_player;

public:
  void _init() {}
  void _ready();
  void _process();
  void set_player(const godot::Node2D *player);

  static void _register_methods();
};

#endif // COREGAME_LIGHTFILTER_H
