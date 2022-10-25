//
// Created by Jacopo Gasparetto on 31/08/22.
//

#ifndef COREGAME_MAIN_H
#define COREGAME_MAIN_H

#include <Array.hpp>
#include <CanvasModulate.hpp>
#include <Dictionary.hpp>
#include <Godot.hpp>
#include <InputEvent.hpp>
#include <Node2D.hpp>

#include <functional>

#include "HUD.h"
#include "Player.h"

class Main : public godot::Node2D {
  GODOT_CLASS(Main, godot::Node2D)
  godot::CanvasModulate *m_darknessLayer = nullptr;
  godot::Dictionary m_dialogues;
  Player *m_player = nullptr;
  HUD *m_hud = nullptr;
  bool m_firstGame = true;
  bool m_gameOver = false;
  godot::Array m_batteries;

public:
  void _init() {}
  void _ready();
  void _process();
  void _input();
  void gameOver();
  void resetGame();
  void displayDialogue(const godot::String &dialogueKey) const;
  static void _register_methods();
};

#endif // COREGAME_MAIN_H
