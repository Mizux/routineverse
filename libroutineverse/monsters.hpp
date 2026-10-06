#pragma once

#include <array>
#include <cstdint>

#include "items.hpp"

struct MonsterDrop {
  ItemId item_id = ItemId::None;
  int chance_pct = 0;  // 1..100
  int min_qty = 0;
  int max_qty = 0;
};

struct MonsterInfo {
  const char* name;
  const char* zone_name;
  int combat_level;
  int max_hp;
  int attack_interval_ms;
  int max_hit;
  int accuracy;
  int evasion;
  int xp_reward;
  uint64_t credits_min;
  uint64_t credits_max;
  int bounty_req;
  bool is_boss;
  std::array<MonsterDrop, 4> drops;
};

inline constexpr int MONSTER_COUNT = 12;
extern const std::array<MonsterInfo, MONSTER_COUNT> monster_info;

