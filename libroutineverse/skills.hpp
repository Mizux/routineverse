#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "items.hpp"

// Skills
inline constexpr uint8_t MAX_SKILL_LEVEL = 99;

#define RV_SKILL_TYPE_LIST(X) \
  X(Salvaging, "Salvaging", "SLV", false) \
  X(Fishing, "Fishing", "FSH", false) \
  X(Farming, "Farming", "FRM", false) \
  X(Recycling, "Recycling", "REC", false) \
  X(SynthCook, "Synth-Cook", "SYN", false) \
  X(DeepMining, "Deep-Mining", "MIN", false) \
  X(Smithing, "Smithing", "SMT", false) \
  X(CyberFab, "Cyber-Fab", "FAB", false) \
  X(Attack, "Attack", "ATK", true) \
  X(Strength, "Strength", "STR", true) \
  X(Defence, "Defence", "DEF", true) \
  X(Hitpoints, "Hitpoints", "HP", true) \
  X(Bounty, "Bounty", "BNT", true)

enum class SkillType : uint8_t {
#define X(id, name, short_name, is_combat) id,
  RV_SKILL_TYPE_LIST(X)
#undef X
};

std::string skill_name(SkillType skill);
std::string skill_short_name(SkillType skill);

inline constexpr std::array all_skills{std::to_array<SkillType>({
#define X(id, name, short_name, is_combat) SkillType::id,
    RV_SKILL_TYPE_LIST(X)
#undef X
})};

inline constexpr bool is_combat_skill(SkillType skill) {
  switch (skill) {
#define X(id, name, short_name, is_combat) \
    case SkillType::id: \
      return is_combat;
    RV_SKILL_TYPE_LIST(X)
#undef X
  }
  return false;
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
