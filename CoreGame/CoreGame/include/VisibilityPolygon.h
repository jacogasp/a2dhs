//
// Created by Jacopo Gasparetto on 04/09/22.
//

#ifndef A2DHS_VISIBILITYPOLYGON_H
#define A2DHS_VISIBILITYPOLYGON_H

#include <Godot.hpp>
#include <Gradient.hpp>
#include <GradientTexture2D.hpp>
#include <Material.hpp>
#include <Polygon2D.hpp>
#include <Shader.hpp>
#include <ShaderMaterial.hpp>
#include <Texture.hpp>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "CoreMath.h"
#include "Map.h"
#include "Player.h"

struct Edge {
  godot::Vector2 a;
  godot::Vector2 b;
  Edge(const godot::Vector2 &_a, const godot::Vector2 &_b) : a(_a), b(_b) {}
};

class VisibilityPolygon : public godot::Node2D {
  GODOT_CLASS(VisibilityPolygon, godot::Node2D)

  const Player *m_player;
  godot::Vector2 m_screen_size;
  std::vector<const godot::Polygon2D *> m_walls{};
  std::vector<godot::Vector2> m_vertices{};
  std::vector<Edge> m_edges{};
  std::vector<godot::Vector2> m_intersections;
  godot::Polygon2D *m_visibilityPolygon{};
  godot::Ref<godot::ShaderMaterial> m_material;

public:
  float lightFalloff = 5.0f;

  void _init(){};
  void _ready();
  void _process();
  godot::Vector2 hitTest(godot::Vector2 const &ray);
  void set_walls(std::vector<const godot::Polygon2D *> &walls);
  void set_player(const Player *player);
  void set_light_falloff(float f);
  static void _register_methods();
};

#endif // A2DHS_VISIBILITYPOLYGON_H
