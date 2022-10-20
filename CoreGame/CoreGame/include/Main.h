//
// Created by Jacopo Gasparetto on 31/08/22.
//

#ifndef COREGAME_MAIN_H
#define COREGAME_MAIN_H

#include <CanvasModulate.hpp>
#include <Godot.hpp>
#include <Node2D.hpp>

class Main : public godot::Node2D {
  GODOT_CLASS(Main, godot::Node2D)
  godot::CanvasModulate *m_darknessLayer = nullptr;

public:
  void _init() {}
  void _ready();
  void _process();
  static void _register_methods();
};

#endif // COREGAME_MAIN_H
