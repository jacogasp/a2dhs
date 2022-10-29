//
// Created by Jacopo Gasparetto on 23/10/22.
//

#ifndef COREGAME_REDEYES_SPAWNER_H
#define COREGAME_REDEYES_SPAWNER_H

#include <Godot.hpp>
#include <Node2D.hpp>
#include <PackedScene.hpp>
#include <RandomNumberGenerator.hpp>
#include <Sprite.hpp>
#include <Timer.hpp>

#include "Player.h"


class RedEyes : public godot::Node2D {
  GODOT_CLASS(RedEyes, godot::Node2D);
  godot::Timer *m_timer = nullptr;
  Player *m_player = nullptr;

public:
  static void _register_methods();
  void _init(){};
  void _ready();
  void _process();
  void onDisappearTimeout() { queue_free(); }
};

class RedEyesSpawner : public godot::Node2D {
  GODOT_CLASS(RedEyesSpawner, godot::Node2D);
  godot::Timer *m_spawnTimer;
  godot::Ref<godot::RandomNumberGenerator> _random;

public:
  godot::Ref<godot::PackedScene> redEyes;
  static void _register_methods();
  void _init(){};
  void _ready();
  void _process();
  void spawn();
  void startSpawning();
  void stopSpawning();
};

#endif // COREGAME_REDEYES_SPAWNER_H
