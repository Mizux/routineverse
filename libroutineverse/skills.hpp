#pragma once

#include <cstdint>
#include <span>
#include <string>

#include "items.hpp"

// Skills

// enum, name, short_name, is_combat
#define RV_SKILL_TYPE_LIST(X)                \
  X(Salvaging, "Salvaging", "SLV", false)    \
  X(Fishing, "Fishing", "FSH", false)        \
  X(Farming, "Farming", "FRM", false)        \
  X(Recycling, "Recycling", "REC", false)    \
  X(SynthCook, "Synth-Cook", "SYN", false)   \
  X(DeepMining, "Deep-Mining", "MIN", false) \
  X(Smithing, "Smithing", "SMT", false)      \
  X(CyberFab, "Cyber-Fab", "FAB", false)     \
  X(Hacking, "Hacking", "HCK", false)        \
  X(Attack, "Attack", "ATK", true)           \
  X(Strength, "Strength", "STR", true)       \
  X(Defence, "Defence", "DEF", true)         \
  X(Hitpoints, "Hitpoints", "HP", true)      \
  X(Integrity, "Integrity", "INT", true)     \
  X(Bounty, "Bounty", "BNT", true)

enum class SkillType : uint8_t {
#define X(id, name, short_name, is_combat) id,
  RV_SKILL_TYPE_LIST(X)
#undef X
};
std::string skill_name(SkillType skill);
std::string skill_short_name(SkillType skill);

std::span<const SkillType> all_skills() noexcept;

bool is_combat_skill(SkillType skill) noexcept;

int max_total_skill_level() noexcept;

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

std::span<const SkillAction> all_actions() noexcept;
std::string action_recipe(const SkillAction& act);
std::span<const int> actions_for_skill(SkillType skill) noexcept;

// XP & Level utility
uint64_t xp_for_level(int level);
int level_for_xp(uint64_t xp);
double level_progress_ratio(uint64_t xp);

std::span<const uint64_t> xp_table() noexcept;
