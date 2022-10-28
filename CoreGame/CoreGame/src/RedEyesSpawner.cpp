//
// Created by Jacopo Gasparetto on 23/10/22.
//

#include "RedEyesSpawner.h"
#include <SceneTree.hpp>
#include <cassert>

void RedEyes::_register_methods() {
  godot::register_method("_process", &RedEyes::_process);
  godot::register_method("_ready", &RedEyes::_ready);
  godot::register_method("onDisappearTimeout", &RedEyes::onDisappearTimeout);
}

void RedEyes::_ready() {
  m_timer = get_node<godot::Timer>("LifeTimer");
  m_timer->start();
  m_player = get_tree()->get_root()->get_node<Player>("MainScene/Player");
}

void RedEyes::_process() {
  assert(m_player);
  this->look_at(m_player->get_global_position());
}

// ************* Spawner *************

void RedEyesSpawner::_register_methods() {
  godot::register_property("red_eys", &RedEyesSpawner::redEyes, static_cast<godot::Ref<godot::PackedScene>>(nullptr));
  godot::register_method("spawnTimeout", &RedEyesSpawner::spawn);
  godot::register_method("_ready", &RedEyesSpawner::_ready);
}

void RedEyesSpawner::_ready() {
  m_spawnTimer = get_node<godot::Timer>("SpawnTimer");
  _random = static_cast<godot::Ref<godot::RandomNumberGenerator>>(godot::RandomNumberGenerator::_new());
  _random->randomize();
  godot::Godot::print("Start spawning");
  startSpawning();
}

void RedEyesSpawner::spawn() {
  godot::Node *red_eyes = redEyes->instance();
  godot::Vector2 position{get_global_position()};
  position.x += _random->randf() * 1280.0f;
  position.y += _random->randf() * 600.0f;
  red_eyes->set("global_position", position);
  get_parent()->get_parent()->add_child(red_eyes);
}

void RedEyesSpawner::startSpawning() { m_spawnTimer->start(); }
void RedEyesSpawner::stopSpawning() { m_spawnTimer->stop(); }