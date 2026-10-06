#include "combat.hpp"

std::string combat_style_name(CombatStyle style) {
  switch (style) {
    case CombatStyle::Accurate:
      return "Precision (Accuracy)";
    case CombatStyle::Aggressive:
      return "Overdrive (Strength)";
    case CombatStyle::Defensive:
      return "Evasive (Defence)";
  }
  return "Unknown";
}

