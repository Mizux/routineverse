#include "skills.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <format>
#include <vector>

namespace {
constexpr uint8_t MAX_SKILL_LEVEL = 99;
}  // namespace

std::span<const uint64_t> xp_table() noexcept {
  static const auto table = []() {
    std::array<uint64_t, MAX_SKILL_LEVEL + 1> t{};
    t[0] = 0;
    t[1] = 0;
    double points = 0.0;
    for (size_t lvl = 1; lvl < MAX_SKILL_LEVEL; ++lvl) {
      points += std::floor(lvl + 300.0 * std::pow(2.0, lvl / 7.0));
      t[lvl + 1] = static_cast<uint64_t>(std::floor(points / 4.0));
    }
    return t;
  }();
  return table;
}

std::string skill_action_name(SkillActionId id) {
  switch (id) {
#define X(id, skill, name, req_lvl, ms, xp, prod, ...) \
  case SkillActionId::id:                              \
    return name;
    RV_SKILL_ACTION_ID_LIST(X)
#undef X
  }
  return "Unknown";
}

std::span<const SkillActionId> all_skill_action_ids() noexcept {
  static constexpr std::array ids{std::to_array<SkillActionId>({
#define X(id, skill, name, req_lvl, ms, xp, prod, ...) SkillActionId::id,
      RV_SKILL_ACTION_ID_LIST(X)
#undef X
  })};
  return ids;
}

const SkillAction& get_skill_action(SkillActionId id) {
  static constexpr std::array actions{std::to_array<SkillAction>({
#define RV_ITEM_QTY(item, qty) ItemId::item, qty
#define RV_INPUTS_1(in1) RV_ITEM_QTY in1
#define RV_INPUTS_2(in1, in2) RV_ITEM_QTY in1, RV_ITEM_QTY in2
#define RV_GET_INPUTS_MACRO(_1, _2, NAME, ...) NAME
#define RV_INPUTS(...) \
  RV_GET_INPUTS_MACRO(__VA_ARGS__, RV_INPUTS_2, RV_INPUTS_1)(__VA_ARGS__)
#define X(id, skill, name, req_lvl, ms, xp, prod, ...) \
  {SkillActionId::id,                                  \
   SkillType::skill,                                   \
   name,                                               \
   req_lvl,                                            \
   ms,                                                 \
   xp,                                                 \
   RV_ITEM_QTY prod __VA_OPT__(, RV_INPUTS(__VA_ARGS__))},
      RV_SKILL_ACTION_ID_LIST(X)
#undef X
#undef RV_INPUTS
#undef RV_GET_INPUTS_MACRO
#undef RV_INPUTS_2
#undef RV_INPUTS_1
#undef RV_ITEM_QTY
  })};
  size_t idx = static_cast<size_t>(id);
  if (idx < actions.size()) {
    return actions[idx];
  }
  return actions[0];
}

std::span<const SkillType> all_skills() noexcept {
  static constexpr std::array skills{std::to_array<SkillType>({
#define X(id, name, short_name, is_combat) SkillType::id,
      RV_SKILL_TYPE_LIST(X)
#undef X
  })};
  return skills;
}

bool is_combat_skill(SkillType skill) noexcept {
  switch (skill) {
#define X(id, name, short_name, is_combat) \
  case SkillType::id:                      \
    return is_combat;
    RV_SKILL_TYPE_LIST(X)
#undef X
  }
  return false;
}

int max_total_skill_level() noexcept {
  return static_cast<int>(all_skills().size()) * MAX_SKILL_LEVEL;
}

std::string skill_name(SkillType skill) {
  switch (skill) {
#define X(id, name, short_name, is_combat) \
  case SkillType::id:                      \
    return name;
    RV_SKILL_TYPE_LIST(X)
#undef X
  }
  return "Unknown";
}

std::string skill_short_name(SkillType skill) {
  switch (skill) {
#define X(id, name, short_name, is_combat) \
  case SkillType::id:                      \
    return short_name;
    RV_SKILL_TYPE_LIST(X)
#undef X
  }
  return "???";
}

uint64_t xp_for_level(int level) {
  const auto tbl = xp_table();
  int clamped = std::clamp(level, 1, static_cast<int>(MAX_SKILL_LEVEL));
  return tbl[clamped];
}

int level_for_xp(uint64_t xp) {
  const auto tbl = xp_table();
  for (int lvl = MAX_SKILL_LEVEL; lvl >= 1; --lvl) {
    if (xp >= tbl[lvl]) return lvl;
  }
  return 1;
}

double level_progress_ratio(uint64_t xp) {
  int lvl = level_for_xp(xp);
  if (lvl >= MAX_SKILL_LEVEL) return 1.0;
  uint64_t cur_base = xp_for_level(lvl);
  uint64_t next_base = xp_for_level(lvl + 1);
  if (next_base <= cur_base) return 1.0;
  return std::clamp(
      static_cast<double>(xp - cur_base) / static_cast<double>(next_base - cur_base),
      0.0, 1.0);
}

std::span<const SkillActionId> actions_for_skill(SkillType skill) noexcept {
  static const auto table = []() {
    std::vector<std::vector<SkillActionId>> map(all_skills().size());
    for (SkillActionId id : all_skill_action_ids()) {
      const auto& act = get_skill_action(id);
      size_t idx = static_cast<size_t>(act.skill);
      if (idx < map.size()) {
        map[idx].push_back(id);
      }
    }
    return map;
  }();
  size_t idx = static_cast<size_t>(skill);
  if (idx >= table.size()) {
    return {};
  }
  return table[idx];
}

std::string action_recipe(const SkillAction& act) {
  std::string io_str;
  if (is_valid_item(act.input_item_1)) {
    io_str +=
        std::format("{}x {}", act.input_qty_1, get_item_info(act.input_item_1).name);
  }
  if (is_valid_item(act.input_item_2)) {
    io_str +=
        std::format(" + {}x {}", act.input_qty_2, get_item_info(act.input_item_2).name);
  }
  if (is_valid_item(act.product_item)) {
    if (!io_str.empty()) io_str += " -> ";
    io_str +=
        std::format("{}x {}", act.product_qty, get_item_info(act.product_item).name);
  } else if (io_str.empty()) {
    io_str = "XP+Cell+Cr";
  }
  return io_str;
}
