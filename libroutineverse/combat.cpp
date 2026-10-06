#include "combat.hpp"

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
