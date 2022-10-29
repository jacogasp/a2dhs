//
// Created by Jacopo Gasparetto on 30/08/22.
//

#ifndef COREGAME_PLAYER_H
#define COREGAME_PLAYER_H

#include "CoreMath.h"
#include <AnimatedSprite.hpp>
#include <Area2D.hpp>
#include <AudioStreamPlayer.hpp>
#include <CollisionShape2D.hpp>
#include <Godot.hpp>
#include <Input.hpp>
#include <InputEvent.hpp>
#include <InputEventMouse.hpp>
#include <KinematicBody2D.hpp>
#include <KinematicCollision2D.hpp>
#include <Viewport.hpp>

#include <functional>

#include "Torch.h"

class Player : public godot::KinematicBody2D {
  GODOT_CLASS(Player, godot::KinematicBody2D)
  godot::Vector2 m_initialPosition{};
  real_t m_initialRotation{};
  godot::Vector2 m_velocity{};
  godot::Vector2 m_acceleration{};
  godot::AnimatedSprite *m_animatedSprite = nullptr;
  godot::AudioStreamPlayer *m_walkSoundPlayer = nullptr;
  Torch *m_torch = nullptr;
  bool m_userInteractionEnabled = true;

public:
  real_t speed = 200.0f;
  real_t rotation_weight = 0.5f;
  real_t detection_radius = 45;
  static void _register_methods();

  void _init(){};
  void _ready();
  void _physics_process(real_t p_delta);
  void resetPlayer();
  void playWalkingSound();
  void stopWalkingSound();
  void setUserInteraction(bool enabled);
  void enableInteraction() { setUserInteraction(true); }
  void disableInteraction() { setUserInteraction(false); }
  [[nodiscard]] real_t getDetectionRadius() const { return detection_radius; }

  [[nodiscard]] godot::Vector2 get_direction() const;
  void _on_Player_area_entered(godot::Area2D *area);
  void _on_battery_run_out();
};

#endif // COREGAME_PLAYER_H
