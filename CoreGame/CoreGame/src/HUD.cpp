//
// Created by Jacopo Gasparetto on 31/08/22.
//

#include "HUD.h"

void HUD::_ready() {
  _dialogueSprite = get_node<godot::Sprite>("Dialogue");
  _messageLabel = get_node<godot::Label>("Dialogue/MessageLabel");
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

void HUD::_register_methods() {
  godot::register_method("_ready", &HUD::_ready);
}
