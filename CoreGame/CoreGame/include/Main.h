//
// Created by Jacopo Gasparetto on 31/08/22.
//

#ifndef COREGAME_MAIN_H
#define COREGAME_MAIN_H

#include <CanvasModulate.hpp>
#include <Dictionary.hpp>
#include <Godot.hpp>
#include <InputEvent.hpp>
#include <Node2D.hpp>
#include <NodePath.hpp>

#include "HUD.h"
#include "Player.h"
#include "RedEyesSpawner.h"

class Main : public godot::Node2D {
  GODOT_CLASS(Main, godot::Node2D)
  godot::CanvasModulate *m_darknessLayer = nullptr;
  godot::Dictionary m_dialogues;
  Player *m_player = nullptr;
  HUD *m_hud = nullptr;
  RedEyesSpawner *m_redEyesSpawner = nullptr;
  bool m_firstGame = true;
  bool m_gameOver = false;

public:
  void _init() {}
  void _ready();
  void _process();
  void _input();
  void gameOver();
  void resetGame();
  void displayDialogue(const godot::String &dialogueKey) const;
  void _on_dialogue(godot::NodePath nodePath);
  void win();
  static void _register_methods();
};

#endif // COREGAME_MAIN_H
