//
// Created by Jacopo Gasparetto on 31/08/22.
//

#include "Main.h"
#include <Input.hpp>
#include <string>
#include <vector>

std::vector<std::string> dialogues{
    "What is it? Could be useful to charge torch! (Press Enter to continue)",
    "Look! It seems that someone has been here.. (Press Enter to continue)"};

void Main::_ready() {
  m_player = get_node<Player>("Player");
  m_player->setOnShowDialogueCallback([&] { displayDialogue(); });
  m_hud = get_node<HUD>("HUD");
  m_darknessLayer = get_node<godot::CanvasModulate>("Darkness");
  if (!m_darknessLayer->is_visible()) m_darknessLayer->set_visible(true);
  godot::Godot::print("Main scene ready");
}

void Main::_process() {}

void Main::displayDialogue() {
  auto msg = dialogues[dialogue++].c_str();
  godot::Godot::print(msg);
  m_hud->displayDialogue(msg);
  m_player->setUserInteraction(false);
}

void Main::_input() {
  if (godot::Input::get_singleton()->is_action_just_released("ui_accept")) {
    m_hud->hideDialogue();
    m_player->setUserInteraction(true);
  }
}

void Main::_register_methods() {
  godot::register_method("_ready", &Main::_ready);
  godot::register_method("_process", &Main::_process);
  godot::register_method("_input", &Main::_input);
}