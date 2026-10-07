#include "combat.hpp"

#include <array>

std::string combat_style_name(CombatStyle style) {
  switch (style) {
#define X(name, str) \
    case CombatStyle::name: \
      return str;
    RV_COMBAT_STYLE_LIST(X)
#undef X
  }
  return "Unknown";
}

std::span<const CombatStyle> all_combat_styles() noexcept {
  static constexpr std::array styles{std::to_array<CombatStyle>({
#define X(name, str) CombatStyle::name,
      RV_COMBAT_STYLE_LIST(X)
#undef X
  })};
  return styles;
}

CombatStyle next_combat_style(CombatStyle style) noexcept {
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

std::string activity_type_name(ActiveActivityType type) {
  switch (type) {
#define X(name, val, str) \
    case ActiveActivityType::name: \
      return str;
    RV_ACTIVE_ACTIVITY_TYPE_LIST(X)
#undef X
  }
  return "Unknown";
}

std::span<const ActiveActivityType> all_activity_types() noexcept {
  static constexpr std::array types{std::to_array<ActiveActivityType>({
#define X(name, val, str) ActiveActivityType::name,
      RV_ACTIVE_ACTIVITY_TYPE_LIST(X)
#undef X
  })};
  return types;
}
