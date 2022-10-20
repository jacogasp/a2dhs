//
// Created by Jacopo Gasparetto on 31/08/22.
//

#include "Main.h"
#include <File.hpp>
#include <Input.hpp>
#include <JSON.hpp>
#include <JSONParseResult.hpp>

void load_dialogues(const char *path, godot::Dictionary &d) {
  auto file = godot::File::_new();
  file->open(path, godot::File::READ);
  if (!file->is_open()) {
    std::cerr << "Cannot find file at " << path << ". Exit" << std::endl;
    std::exit(EXIT_FAILURE);
  }
  godot::String buff{file->get_as_text()};
  auto json = godot::JSON::get_singleton()->parse(buff);
  d = json->get_result();
  file->close();
}

void Main::_ready() {
  m_player = get_node<Player>("Player");
  m_player->setOnShowDialogueCallback([&](const godot::String &dialogueKey) { displayDialogue(dialogueKey); });
  m_hud = get_node<HUD>("HUD");
  m_darknessLayer = get_node<godot::CanvasModulate>("Darkness");
  if (!m_darknessLayer->is_visible()) m_darknessLayer->set_visible(true);
  load_dialogues("res://Assets/dialogues.json", m_dialogues);
  godot::Godot::print("Main scene ready");
}

void Main::_process() {}

void Main::displayDialogue(const godot::String &dialogueKey) {
  godot::String msg;
  if (m_dialogues.has(dialogueKey)) {
    msg = m_dialogues[dialogueKey];
  } else {
    msg = "Dialogue not found";
  }

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