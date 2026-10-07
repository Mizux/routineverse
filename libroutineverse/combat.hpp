#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>

#define RV_COMBAT_STYLE_LIST(X) \
  X(Accurate, "Precision (Accuracy)") \
  X(Aggressive, "Overdrive (Strength)") \
  X(Defensive, "Evasive (Defence)")

enum class CombatStyle : uint8_t {
#define X(name, str) name,
  RV_COMBAT_STYLE_LIST(X)
#undef X
};

std::string combat_style_name(CombatStyle style);

inline const auto& all_combat_styles() noexcept {
  static constexpr std::array styles{std::to_array<CombatStyle>({
#define X(name, str) CombatStyle::name,
      RV_COMBAT_STYLE_LIST(X)
#undef X
  })};
  return styles;
}

inline std::optional<CombatStyle> combat_style_from_int(int val) noexcept {
  if (val >= 0 && static_cast<size_t>(val) < all_combat_styles().size()) {
    return all_combat_styles()[val];
  }
  return std::nullopt;
}

inline CombatStyle combat_style_or_default(int val) noexcept {
  if (val >= 0 && static_cast<size_t>(val) < all_combat_styles().size()) {
    return all_combat_styles()[val];
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

#define RV_ACTIVE_ACTIVITY_TYPE_LIST(X) \
  X(None, -1, "None") \
  X(Skill, 0, "Skill") \
  X(Combat, 1, "Combat")

enum class ActiveActivityType : int8_t {
#define X(name, val, str) name = val,
  RV_ACTIVE_ACTIVITY_TYPE_LIST(X)
#undef X
};

std::string activity_type_name(ActiveActivityType type);

inline constexpr std::array all_activity_types{std::to_array<ActiveActivityType>({
#define X(name, val, str) ActiveActivityType::name,
    RV_ACTIVE_ACTIVITY_TYPE_LIST(X)
#undef X
})};

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
