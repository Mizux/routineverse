#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>

// Combat
enum class CombatStyle : uint8_t {
  Accurate,    // Trains Precision (+accuracy)
  Aggressive,  // Trains Strength (+max hit)
  Defensive,   // Trains Defence (+evasion)
};
inline constexpr size_t COMBAT_STYLE_COUNT = 3;
inline constexpr std::array<CombatStyle, COMBAT_STYLE_COUNT> all_combat_styles = {
    CombatStyle::Accurate,
    CombatStyle::Aggressive,
    CombatStyle::Defensive,
};

inline constexpr std::optional<CombatStyle> combat_style_from_int(int val) noexcept {
  if (val >= 0 && static_cast<size_t>(val) < all_combat_styles.size()) {
    return all_combat_styles[val];
  }
  return std::nullopt;
}

inline constexpr CombatStyle combat_style_or_default(int val) noexcept {
  if (val >= 0 && static_cast<size_t>(val) < all_combat_styles.size()) {
    return all_combat_styles[val];
  }
  return CombatStyle::Accurate;
}

inline constexpr int combat_style_to_int(CombatStyle style) noexcept {
  return static_cast<int>(style);
}

inline constexpr CombatStyle next_combat_style(CombatStyle style) noexcept {
  switch (style) {
    case CombatStyle::Accurate:
      return CombatStyle::Aggressive;
    case CombatStyle::Aggressive:
      return CombatStyle::Defensive;
    case CombatStyle::Defensive:
      return CombatStyle::Accurate;
  }
  return CombatStyle::Accurate;
}

std::string combat_style_name(CombatStyle style);

enum class ActiveActivityType : uint8_t {
  None,
  Skill,
  Combat,
};
inline constexpr size_t ACTIVITY_TYPE_COUNT = 3;
inline constexpr std::array<ActiveActivityType, ACTIVITY_TYPE_COUNT> all_activity_types = {
    ActiveActivityType::None,
    ActiveActivityType::Skill,
    ActiveActivityType::Combat,
};

inline constexpr std::optional<ActiveActivityType> activity_type_from_int(int val) noexcept {
  if (val >= 0 && static_cast<size_t>(val) < all_activity_types.size()) {
    return all_activity_types[val];
  }
  return std::nullopt;
}

inline constexpr ActiveActivityType activity_type_or_none(int val) noexcept {
  if (val >= 0 && static_cast<size_t>(val) < all_activity_types.size()) {
    return all_activity_types[val];
  }
  return ActiveActivityType::None;
}

inline constexpr int activity_type_to_int(ActiveActivityType type) noexcept {
  return static_cast<int>(type);
}

