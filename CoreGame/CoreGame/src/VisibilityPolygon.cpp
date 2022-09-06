//
// Created by Jacopo Gasparetto on 04/09/22.
//

#include "VisibilityPolygon.h"

void VisibilityPolygon::_ready() {
  screen_size = get_viewport()->get_size();

  godot::Vector2 topLeft{0, 0};
  godot::Vector2 topRight{screen_size.width, 0};
  godot::Vector2 bottomRight{screen_size.width, screen_size.height};
  godot::Vector2 bottomLeft{0, screen_size.height};

  m_vertices = {topLeft, topRight, bottomRight, bottomLeft};

  Edge top{topLeft, topRight};
  Edge right{topRight, bottomRight};
  Edge bottom{bottomRight, bottomLeft};
  Edge left{bottomLeft, topLeft};

  m_edges.push_back(top);
  m_edges.push_back(right);
  m_edges.push_back(bottom);
  m_edges.push_back(left);

  m_vertices.emplace_back(0.0f, 0.0f);
  for (auto &w : m_walls) {
    auto vertices = w->get_polygon();
    for (int i = 0; i < vertices.size(); ++i)
      m_vertices.push_back(vertices[i]);
  }
  m_intersections.resize(m_vertices.size() * 2);

  m_gradient = static_cast<godot::Ref<godot::Gradient>>(godot::Gradient::_new());
  m_gradient->add_point(0, {0, 0, 0});
  m_gradient->add_point(1, {255, 0, 0});


  godot::String m{"VisibilityPolygon ready, walls: "};
  m += godot::String(std::to_string(m_walls.size()).c_str());
  m += godot::String(", vertices: ");
  m += godot::String(std::to_string(m_vertices.size()).c_str());
  godot::Godot::print(m);
}

void VisibilityPolygon::_process() {
  for (int i = 0; i < m_vertices.size(); i++) {
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

  std::sort(m_intersections.begin(), m_intersections.end(), [&](godot::Vector2 const &a, godot::Vector2 const &b) {
    auto d_a = a - m_player->get_position();
    auto d_b = b - m_player->get_position();
    d_a.normalize();
    d_b.normalize();
    return d_a.angle() < d_b.angle();
  });
  godot::PoolVector2Array pool;
  pool.resize((int)m_intersections.size());
  for (int i = 0; i < pool.size(); ++i) {
    pool.set(i, m_intersections[i]);
  }
  set_polygon(pool);
  m_texture = static_cast<godot::Ref<godot::GradientTexture2D>>(godot::GradientTexture2D::_new());
  m_texture->set_gradient(m_gradient);
  m_texture->set_fill_from({1, 0});
  m_texture->set_fill_to({0, 1.0});
  m_texture->set_fill(1);
  set_texture(m_texture);
  show();
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

void VisibilityPolygon::_draw() {
  //  for (auto &poly : m_walls) {
  //    auto vertices = poly->get_polygon();
  //    for (int i = 0; i < vertices.size(); i += 1) {
  //      draw_circle(vertices[i], 5, {255, 0, 0});
  //      draw_line(m_player->get_position(), vertices[i], {255, 0, 0});
  //    }
  //  }

  for (auto &p : m_intersections) {
    draw_circle(p, 5, {0, 255, 0});
    draw_line(m_player->get_position(), p, {0, 255, 0});
  }
}

void VisibilityPolygon::add_wall(const godot::Polygon2D *wall) { m_walls.push_back(wall); }

void VisibilityPolygon::set_walls(std::vector<const Polygon2D *> &walls) {
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
  godot::register_method("_draw", &VisibilityPolygon::_draw);
}