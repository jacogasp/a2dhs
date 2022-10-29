//
// Created by Jacopo Gasparetto on 05/09/22.
//

#ifndef COREGAME_COREMATH_H
#define COREGAME_COREMATH_H

#include <Godot.hpp>
#include <Vector2.hpp>
#include <cmath>

namespace CoreGame {
inline bool ccw(godot::Vector2 A, godot::Vector2 B, godot::Vector2 C) {
  return (C.x - A.x) * (B.y - A.y) < (B.x - A.x) * (C.y - A.y);
}

inline bool intersect(godot::Vector2 v1_0, godot::Vector2 v1_1, godot::Vector2 v2_0, godot::Vector2 v2_1) {
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

static constexpr float PI = 3.1415926536f;
template <typename T> inline constexpr T deg2rad(T x) { return x * PI / 360; }
template <typename T> inline constexpr T flip(T x) { return 1 - x; }
template <typename T> inline constexpr T flip2(T x) { return flip(x) * flip(x); }
template <typename T> inline constexpr T flip3(T x) { return flip(x) * flip2(x); }
template <typename T> inline constexpr T flip4(T x) { return flip(x) * flip3(x); }
template <typename T> inline constexpr T flip5(T x) { return flip(x) * flip4(x); }
template <typename T> inline constexpr T flip6(T x) { return flip(x) * flip5(x); }
template <typename T> inline constexpr T crossfade(T a, T b, T t) { return a + (b - a) * t; }
template <typename T> inline constexpr T scale(T x, T t) { return x * t; }
template <typename T> inline constexpr T reverse_scale(T x, T t) { return (1 - t) * x; }
template <typename T> inline constexpr T smoothStart2(T x) { return x * x; }
template <typename T> inline constexpr T smoothStart3(T x) { return smoothStart2(x) * x; }
template <typename T> inline constexpr T smoothStart4(T x) { return smoothStart3(x) * x; }
template <typename T> inline constexpr T smoothStart5(T x) { return smoothStart4(x) * x; }
template <typename T> inline constexpr T smoothStart6(T x) { return smoothStart5(x) * x; }
template <typename T> inline constexpr T smoothStop2(T x) { return 1 - flip2(x); }
template <typename T> inline constexpr T smoothStop3(T x) { return 1 - flip3(x); }
template <typename T> inline constexpr T smoothStop4(T x) { return 1 - flip4(x); }
template <typename T> inline constexpr T smoothStop5(T x) { return 1 - flip5(x); }
template <typename T> inline constexpr T smoothStop6(T x) { return 1 - flip6(x); }
template <typename T> inline constexpr T smoothStep2(T x) { return crossfade(smoothStart2(x), smoothStop2(x), x); }
template <typename T> inline constexpr T smoothStep3(T x) { return crossfade(smoothStart3(x), smoothStop3(x), x); }
template <typename T> inline constexpr T smoothStep4(T x) { return crossfade(smoothStart4(x), smoothStop4(x), x); }
template <typename T> inline constexpr T smoothStep5(T x) { return crossfade(smoothStart5(x), smoothStop5(x), x); }
template <typename T> inline constexpr T smoothStep6(T x) { return crossfade(smoothStart6(x), smoothStop6(x), x); }
template <typename T> inline constexpr T arch2(T x) { return scale(flip(x), x) * 4.0; }
template <typename T> inline constexpr T smoothStartArch3(T x) { return scale(arch2(x), x) / 0.66; }

} // namespace CoreGame

#endif // COREGAME_COREMATH_H
