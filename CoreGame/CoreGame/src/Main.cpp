//
// Created by Jacopo Gasparetto on 31/08/22.
//

#include "Main.h"

void Main::_ready() {
  m_darknessLayer = get_node<godot::CanvasModulate>("Darkness");
  if (!m_darknessLayer->is_visible()) m_darknessLayer->set_visible(true);
  godot::Godot::print("Main scene ready");
}

void Main::_process() {}

void Main::_register_methods() {
  godot::register_method("_ready", &Main::_ready);
  godot::register_method("_process", &Main::_process);
}