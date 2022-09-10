//
// Created by Jacopo Gasparetto on 04/09/22.
//

#include "VisibilityPolygon.h"

void VisibilityPolygon::_ready() {
  m_visibilityPolygon = get_node<godot::Polygon2D>("VisibilityPolygon");
  m_screen_size = get_viewport_rect().get_size();

  // Add vertices at screen corners
  godot::Vector2 topLeft{0, 0};
  godot::Vector2 topRight{m_screen_size.width, 0};
  godot::Vector2 bottomRight{m_screen_size.width, m_screen_size.height};
  godot::Vector2 bottomLeft{0, m_screen_size.height};

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

  godot::Ref<godot::ShaderMaterial> mat = m_visibilityPolygon->get_material();
  mat->set_shader_param("node_size", m_screen_size);
  mat->set_shader_param("aspect_ratio", m_screen_size.y / m_screen_size.x);
  mat->set_shader_param("fall_off", lightFalloff);

  godot::Godot::print("VisibilityPolygon ready");
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
  m_visibilityPolygon->set_polygon(pool);

  // Set shader rotation
  auto playerPosition = m_player->get_position();
  auto playerDirection = m_player->get_direction();
  playerPosition.x /= m_screen_size.x;
  playerPosition.y /= m_screen_size.y;
  godot::Ref<godot::ShaderMaterial> visibility_polygon_mat = m_visibilityPolygon->get_material();
  visibility_polygon_mat->set_shader_param("light_position", playerPosition);
  visibility_polygon_mat->set_shader_param("heading", playerDirection);
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

  for (auto &w : m_walls) {
    auto vertices = w->get_polygon();
    for (int i = 0; i < vertices.size(); ++i)
      m_vertices.push_back(vertices[i]);
  }

  m_intersections.resize(m_vertices.size() * 2);
}

void VisibilityPolygon::set_player(const Player *player) { m_player = player; }

void VisibilityPolygon::set_light_falloff(const float f) { lightFalloff = f; }

void VisibilityPolygon::_register_methods() {
  godot::register_method("_ready", &VisibilityPolygon::_ready);
  godot::register_method("_process", &VisibilityPolygon::_process);
  godot::register_property("_lightFalloff", &VisibilityPolygon::lightFalloff, 1.0f);
}