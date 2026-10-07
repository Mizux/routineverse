#pragma once

#include <cstdint>
#include <span>
#include <string>

#define RV_COMBAT_STYLE_LIST(X)         \
  X(Accurate, "Precision (Accuracy)")   \
  X(Aggressive, "Overdrive (Strength)") \
  X(Defensive, "Evasive (Defence)")

enum class CombatStyle : uint8_t {
#define X(name, str) name,
  RV_COMBAT_STYLE_LIST(X)
#undef X
};
std::string combat_style_name(CombatStyle style);
std::span<const CombatStyle> all_combat_styles() noexcept;
CombatStyle next_combat_style(CombatStyle style) noexcept;

#define RV_ACTIVE_ACTIVITY_TYPE_LIST(X) \
  X(None, -1, "None")                   \
  X(Skill, 0, "Skill")                  \
  X(Combat, 1, "Combat")

enum class ActiveActivityType : int8_t {
#define X(name, val, str) name = val,
  RV_ACTIVE_ACTIVITY_TYPE_LIST(X)
#undef X
};
std::string activity_type_name(ActiveActivityType type);
std::span<const ActiveActivityType> all_activity_types() noexcept;
