//
// Created by Jacopo Gasparetto on 20/10/22.
//

#ifndef COREGAME_DIALOGUETRIGGER_H
#define COREGAME_DIALOGUETRIGGER_H

#include <Area2D.hpp>
#include <Godot.hpp>
#include <String.hpp>

class DialogueTrigger : public godot::Area2D {
  GODOT_CLASS(DialogueTrigger, godot::Area2D);

public:
  godot::String dialogueKey;
  void _init(){};
  static void _register_methods() {
    godot::register_property("DialogueKey", &DialogueTrigger::dialogueKey, godot::String{""});
  };
};

#endif // COREGAME_DIALOGUETRIGGER_H
