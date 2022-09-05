//
// Created by Jacopo Gasparetto on 05/09/22.
//

#ifndef COREGAME_COREMATH_H
#define COREGAME_COREMATH_H

#include <Godot.hpp>
#include <Vector2.hpp>

namespace CoreGame {
inline bool ccw(godot::Vector2 A, godot::Vector2 B, godot::Vector2 C) {
  return (C.x - A.x) * (B.y - A.y) < (B.x - A.x) * (C.y - A.y);
}

inline bool intersect(godot::Vector2 v1_0, godot::Vector2 v1_1, godot::Vector2 v2_0, godot::Vector2 v2_1) {
  const auto &A = v1_0.x;
  const auto &B = v1_1.y;
  const auto &C = v2_0.x;
  const auto &D = v2_1.y;
  return CoreGame::ccw(v1_0, v1_1, v2_0) != CoreGame::ccw(v1_0, v1_1, v2_1) &&
         CoreGame::ccw(v1_0, v2_0, v2_1) != CoreGame::ccw(v1_1, v2_0, v2_1);
}

inline godot::Vector2 intersectionPoint(godot::Vector2 v1_0, godot::Vector2 v1_1, godot::Vector2 v2_0,
                                        godot::Vector2 v2_1, float &T) {
  auto d1 = (v1_1 - v1_0).normalized();
  auto d2 = (v2_1 - v2_0).normalized();
  const auto V = ((v1_0.x - v2_0.x) * d1.y + (v2_0.y - v1_0.y) * d1.x) / (d2.x * d1.y - d1.x * d2.y);
  T = (v2_0.x + V * d2.x - v1_0.x) / d1.x;
  d2 *= V;
  return v2_0 + d2;
}
} // namespace CoreGame

#endif // COREGAME_COREMATH_H
