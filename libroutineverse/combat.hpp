#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>

enum class CombatStyle : uint8_t {
  Accurate,    // Trains Precision (+accuracy)
  Aggressive,  // Trains Strength (+max hit)
  Defensive,   // Trains Defence (+evasion)
};

std::string combat_style_name(CombatStyle style);

inline constexpr std::array all_combat_styles{std::to_array<CombatStyle>({
    CombatStyle::Accurate,
    CombatStyle::Aggressive,
    CombatStyle::Defensive,
})};

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

enum class ActiveActivityType : int8_t {
  None = -1,
  Skill,
  Combat,
};
inline constexpr std::array all_activity_types{std::to_array<ActiveActivityType>(
    {ActiveActivityType::None, ActiveActivityType::Skill, ActiveActivityType::Combat})};

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

