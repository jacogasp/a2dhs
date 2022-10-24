//
// Created by Jacopo Gasparetto on 31/08/22.
//

#include "HUD.h"

void HUD::_register_methods() { godot::register_method("_ready", &HUD::_ready); }

void HUD::_ready() {
  _dialogueSprite = get_node<godot::Sprite>("Dialogue");
  _messageLabel = get_node<godot::Label>("Dialogue/MessageLabel");
  _startGameHUD = get_node<godot::CanvasLayer>("StartGame");
  _gameOverHUD = get_node<godot::CanvasLayer>("GameOver");
  hideDialogue();
}

void HUD::displayDialogue(const godot::String &message) {
  m_dialogue_visible = true;
  _messageLabel->set_text(message);
  _dialogueSprite->show();
  _messageLabel->show();
}

void HUD::hideDialogue() {
  m_dialogue_visible = false;
  _messageLabel->hide();
  _dialogueSprite->hide();
}

void HUD::showStart() { _startGameHUD->show(); }
void HUD::hideStart() { _startGameHUD->hide(); }
void HUD::showGameOver() { _gameOverHUD->show(); }
void HUD::hideGameOver() { _gameOverHUD->hide(); }
