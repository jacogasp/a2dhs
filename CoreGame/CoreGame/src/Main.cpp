//
// Created by Jacopo Gasparetto on 31/08/22.
//

#include "Main.h"
#include "DialogueTrigger.h"
#include <File.hpp>
#include <Input.hpp>
#include <JSON.hpp>
#include <JSONParseResult.hpp>
#include <SceneTree.hpp>

namespace KCE {
class File {
  godot::File *m_file;

public:
  explicit File(const char *path, godot::File::ModeFlags flags = godot::File::READ) : m_file{godot::File::_new()} {
    m_file->open(path, flags);
    if (!m_file->is_open()) {
      std::cerr << "Cannot find file at " << path << ". Exit" << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }
  ~File() {
    m_file->close();
    m_file->free();
  }
  auto get_as_text() { return m_file->get_as_text(); }
};
} // namespace KCE
void load_dialogues(const char *path, godot::Dictionary &d) {
  KCE::File file{path};
  auto json = godot::JSON::get_singleton()->parse(file.get_as_text());
  d = json->get_result();
}

void Main::_ready() {
  load_dialogues("res://Assets/dialogues.json", m_dialogues);

  m_player = get_node<Player>("Player");
  m_hud = get_node<HUD>("HUD");
  m_darknessLayer = get_node<godot::CanvasModulate>("Darkness");
  m_player->disableInteraction();
  m_hud->showStart();
  m_hud->hideGameOver();
  m_hud->hideDialogue();
  m_hud->hideGameCompleted();
  if (!m_darknessLayer->is_visible()) m_darknessLayer->set_visible(true);
  godot::Godot::print("Main scene ready");
}

void Main::_process() {}

void Main::win() {
  m_gameOver = true;
  m_hud->showGameCompleted();
  m_player->disableInteraction();
}

void Main::gameOver() {
  m_gameOver = true;
  m_hud->showGameOver();
  m_player->disableInteraction();
}

void Main::resetGame() {
  get_tree()->call_group("batteries", "show");
  m_player->resetPlayer();
  m_player->enableInteraction();
  m_hud->hideGameOver();
  m_hud->hideGameCompleted();
  m_gameOver = false;
}

void Main::displayDialogue(const godot::String &dialogueKey) const {
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
    if (m_firstGame) {
      m_firstGame = false;
      m_player->enableInteraction();
      m_hud->hideStart();
      return;
    }

    if (m_gameOver) {
      resetGame();
      return;
    }

    m_hud->hideDialogue();
    m_player->enableInteraction();
  }
}

void Main::_on_dialogue(godot::NodePath nodePath) {
  godot::Godot::print(nodePath);
  auto trigger = get_node<DialogueTrigger>(nodePath);
  if (trigger) {
    displayDialogue(trigger->dialogueKey);
    trigger->queue_free();
  }
}

void Main::_register_methods() {
  godot::register_method("_ready", &Main::_ready);
  godot::register_method("_process", &Main::_process);
  godot::register_method("_input", &Main::_input);
  godot::register_method("_on_dialogue", &Main::_on_dialogue);
  godot::register_method("gameOver", &Main::gameOver);
  godot::register_method("win", &Main::win);
}