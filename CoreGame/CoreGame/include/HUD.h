//
// Created by Jacopo Gasparetto on 31/08/22.
//

#ifndef COREGAME_HUD_H
#define COREGAME_HUD_H

#include <Button.hpp>
#include <CanvasLayer.hpp>
#include <Godot.hpp>
#include <Label.hpp>
#include <Sprite.hpp>
#include <Timer.hpp>

class HUD : public godot::CanvasLayer {
  GODOT_CLASS(HUD, godot::CanvasLayer)
  godot::Sprite *_dialogueSprite;
  godot::Label *_messageLabel;
  godot::CanvasLayer *_startGameHUD;
  godot::CanvasLayer *_gameOverHUD;
  bool m_dialogue_visible = false;

public:
  void _init() {}
  void _ready();
  bool is_dialogue_visible() const { return m_dialogue_visible; }
  void showStart();
  void hideStart();
  void showGameOver();
  void hideGameOver();
  void displayDialogue(const godot::String &message);
  void hideDialogue();
  static void _register_methods();
};

#endif // COREGAME_HUD_H
