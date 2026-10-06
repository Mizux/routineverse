#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "items.hpp"

// Skills
inline constexpr uint8_t MAX_SKILL_LEVEL = 99;

enum class SkillType : uint8_t {
  // Logistic Skill
  Salvaging,
  Fishing,
  Farming,
  Recycling,
  SynthCook,
  DeepMining,
  Smithing,
  CyberFab,
  // Combat Skill
  Attack,
  Strength,
  Defence,
  Hitpoints,
  Bounty,
};
std::string skill_name(SkillType skill);
std::string skill_short_name(SkillType skill);

inline constexpr std::array all_skills = {
    SkillType::Salvaging, SkillType::Fishing,  SkillType::Farming,
    SkillType::Recycling, SkillType::SynthCook, SkillType::DeepMining,
    SkillType::Smithing,  SkillType::CyberFab,  SkillType::Attack,
    SkillType::Strength,  SkillType::Defence,   SkillType::Hitpoints,
    SkillType::Bounty,
};
inline constexpr bool is_combat_skill(SkillType skill) {
  switch (skill) {
    case SkillType::Attack:
    case SkillType::Strength:
    case SkillType::Defence:
    case SkillType::Hitpoints:
    case SkillType::Bounty:
      return true;
    default:
      return false;
  }
}
inline constexpr int max_total_skill_level() {
  return static_cast<int>(all_skills.size()) * MAX_SKILL_LEVEL;
}

struct SkillAction {
  SkillType skill;
  const char* name;
  int req_level;
  int base_interval_ms;
  int xp;
  ItemId product_item;
  int product_qty;
  ItemId input_item_1;
  int input_qty_1;
  ItemId input_item_2;
  int input_qty_2;
};
extern const std::vector<SkillAction> skill_actions;
std::string action_recipe(const SkillAction& act);
std::vector<int> actions_for_skill(SkillType skill);

// XP & Level utility
uint64_t xp_for_level(int level);
int level_for_xp(uint64_t xp);
double level_progress_ratio(uint64_t xp);
const std::array<uint64_t, MAX_SKILL_LEVEL + 1>& xp_table();

