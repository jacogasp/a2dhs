//
// Created by Jacopo Gasparetto on 31/08/22.
//

#ifndef COREGAME_MAIN_H
#define COREGAME_MAIN_H

#include <CanvasModulate.hpp>
#include <Godot.hpp>
#include <InputEvent.hpp>
#include <Node2D.hpp>

#include "HUD.h"
#include "Player.h"

class Main : public godot::Node2D {
  GODOT_CLASS(Main, godot::Node2D)
  godot::CanvasModulate *m_darknessLayer = nullptr;
  Player *m_player = nullptr;
  HUD *m_hud = nullptr;

  int dialogue = 0;

public:
  void _init() {}
  void _ready();
  void _process();
  void _input();
  void displayDialogue();
  static void _register_methods();
};

#endif // COREGAME_MAIN_H
