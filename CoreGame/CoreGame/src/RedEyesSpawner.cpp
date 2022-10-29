//
// Created by Jacopo Gasparetto on 23/10/22.
//

#include "RedEyesSpawner.h"
#include "CoreMath.h"
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
  cos_theta = std::cos(CoreGame::deg2rad(m_player->getDetectionRadius()));
}

void RedEyes::_process() {
  assert(m_player);
  auto direction = get_global_position() - m_player->get_global_position();
  direction.normalize();
  auto angle = direction.dot(m_player->get_direction());
  if (angle > cos_theta) queue_free();
  this->look_at(m_player->get_global_position());
}

// ************* Spawner *************

void RedEyesSpawner::_register_methods() {
  godot::register_property("red_eys", &RedEyesSpawner::redEyes, static_cast<godot::Ref<godot::PackedScene>>(nullptr));
  godot::register_method("spawnTimeout", &RedEyesSpawner::spawn);
  godot::register_method("_ready", &RedEyesSpawner::_ready);
  godot::register_property("SpawningRadius", &RedEyesSpawner::radius, 160.0f);
  godot::register_property("SpawningSpread", &RedEyesSpawner::spread, 200.0f);
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
  auto player = get_tree()->get_root()->get_node<Player>("MainScene/Player");
  godot::Vector2 position{player->get_position()};
  const float t = _random->randf();
  position.x += radius * std::cos(2 * CoreGame::PI * t);
  position.x += _random->randf() * spread - spread * .5f;
  position.y += radius * std::sin(2 * CoreGame::PI * t);
  position.y += _random->randf() * spread - spread * .5f;
  red_eyes->set("position", position);
  get_parent()->get_parent()->add_child(red_eyes);
}

void RedEyesSpawner::startSpawning() { m_spawnTimer->start(); }
void RedEyesSpawner::stopSpawning() { m_spawnTimer->stop(); }