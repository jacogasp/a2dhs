//
// Created by Jacopo Gasparetto on 04/09/22.
//

#include "VisibilityPolygon.h"

void VisibilityPolygon::_ready() {
  screen_size = get_viewport()->get_size();

  // Add vertices at screen corners
  godot::Vector2 topLeft{0, 0};
  godot::Vector2 topRight{screen_size.width, 0};
  godot::Vector2 bottomRight{screen_size.width, screen_size.height};
  godot::Vector2 bottomLeft{0, screen_size.height};

  m_vertices = {topLeft, topRight, bottomRight, bottomLeft};

  // Add four edges around the screen for hit test
  Edge top{topLeft, topRight};
  Edge right{topRight, bottomRight};
  Edge bottom{bottomRight, bottomLeft};
  Edge left{bottomLeft, topLeft};

  m_edges.push_back(top);
  m_edges.push_back(right);
  m_edges.push_back(bottom);
  m_edges.push_back(left);

  for (auto &w : m_walls) {
    auto vertices = w->get_polygon();
    for (int i = 0; i < vertices.size(); ++i)
      m_vertices.push_back(vertices[i]);
  }
  // We are going to draw N * 2 rays
  m_intersections.resize(m_vertices.size() * 2);

  godot::String m{"VisibilityPolygon ready, walls: "};
  m += godot::String(std::to_string(m_walls.size()).c_str());
  m += godot::String(", vertices: ");
  m += godot::String(std::to_string(m_vertices.size()).c_str());
  godot::Godot::print(m);
}

void VisibilityPolygon::_process() {
  // For each vertex, hit test every polygon between the player and vertex.
  for (size_t i = 0; i < m_vertices.size(); i++) {
    auto &target = m_vertices[i];
    auto ray = target - m_player->get_position();
    ray = m_player->get_position() + ray.normalized() * 1000;
    const auto r1 = ray.rotated(0.0001);
    const auto r2 = ray.rotated(-0.0001);
    const auto h1 = hitTest(r1);
    const auto h2 = hitTest(r2);
    m_intersections.at(2 * i) = h1;
    m_intersections.at(2 * i + 1) = h2;
  }

  // Sort by angle respect to the player
  std::sort(m_intersections.begin(), m_intersections.end(), [&](godot::Vector2 const &a, godot::Vector2 const &b) {
    auto d_a = a - m_player->get_position();
    auto d_b = b - m_player->get_position();
    d_a.normalize();
    d_b.normalize();
    return d_a.angle() < d_b.angle();
  });

  // Create the vertex pool
  godot::PoolVector2Array pool;
  pool.resize((int)m_intersections.size());
  for (int i = 0; i < pool.size(); ++i) {
    pool.set(i, m_intersections[i]);
  }

  // Upload mouse normalized coordinates to shader
  auto playerPosition = m_player->get_position();
  playerPosition.x /= screen_size.x;
  playerPosition.y /= screen_size.y;
  godot::Ref<godot::ShaderMaterial> shader_material = get_material();
  shader_material->set_shader_param("mouse_position", playerPosition);
  set_polygon(pool);
  update();
}

godot::Vector2 VisibilityPolygon::hitTest(const godot::Vector2 &ray) {
  auto target = ray;
  float min_t = 1000;
  for (auto &e : m_edges) {
    if (!CoreGame::intersect(m_player->get_position(), ray, e.a, e.b)) continue;
    float T;
    const auto p = CoreGame::intersectionPoint(m_player->get_position(), ray, e.a, e.b, T);
    if (T < min_t) {
      min_t = T;
      target = p;
    }
  }
  return target;
}

void VisibilityPolygon::set_walls(std::vector<const godot::Polygon2D *> &walls) {
  m_walls = walls;
  for (auto &w : m_walls) {
    auto vertices = w->get_polygon();
    vertices.push_back(vertices[0]);
    if (vertices.size() < 2) continue;
    for (int i = 0; i < vertices.size() - 1; ++i) {
      m_edges.emplace_back(vertices[i + 1], vertices[i]);
    }
  }
}

void VisibilityPolygon::set_player(const Player *player) { m_player = player; }

void VisibilityPolygon::_register_methods() {
  godot::register_method("_ready", &VisibilityPolygon::_ready);
  godot::register_method("_process", &VisibilityPolygon::_process);
}