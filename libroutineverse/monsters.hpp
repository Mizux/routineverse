#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>

#include "items.hpp"

#define RV_MONSTER_ID_LIST(X) \
  X(StrayServoDrone, "Stray Servo-Drone") \
  X(BioVatHound, "Bio-Vat Hound") \
  X(StreetScavenger, "Street Scavenger") \
  X(ChromeGangPunk, "Chrome Gang Punk") \
  X(RiotEnforcerBot, "Riot Enforcer Bot") \
  X(ChemMutantBrute, "Chem-Mutant Brute") \
  X(CryoSecMech, "Cryo-Sec Mech") \
  X(CorpShadowOp, "Corp Shadow-Op") \
  X(CobaltCyberNinja, "Cobalt Cyber-Ninja") \
  X(NeutroniumCyborg, "Neutronium Cyborg") \
  X(ApexCyberWyrm, "Apex Cyber-Wyrm") \
  X(Nexus9RogueOvermind, "NEXUS-9, Rogue Overmind")

enum class MonsterId : uint8_t {
#define X(id, name) id,
  RV_MONSTER_ID_LIST(X)
#undef X
};

std::string monster_name(MonsterId monster);

inline constexpr std::array all_monster_ids{std::to_array<MonsterId>({
#define X(id, name) MonsterId::id,
    RV_MONSTER_ID_LIST(X)
#undef X
})};

inline constexpr int MONSTER_COUNT = static_cast<int>(all_monster_ids.size());

inline constexpr std::optional<MonsterId> monster_id_from_int(int val) noexcept {
  if (val >= 0 && static_cast<size_t>(val) < all_monster_ids.size()) {
    return all_monster_ids[val];
  }
  return std::nullopt;
}

inline constexpr MonsterId monster_id_or_default(int val) noexcept {
  if (val >= 0 && static_cast<size_t>(val) < all_monster_ids.size()) {
    return all_monster_ids[val];
  }
  return MonsterId::StrayServoDrone;
}

inline constexpr int monster_id_to_int(MonsterId id) noexcept {
  return static_cast<int>(id);
}

struct MonsterDrop {
  ItemId item_id = ItemId::None;
  int chance_pct = 0;  // 1..100
  int min_qty = 0;
  int max_qty = 0;
};

struct MonsterInfo {
  MonsterId id;
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

extern const std::array<MonsterInfo, MONSTER_COUNT> monster_info;

const MonsterInfo& get_monster_info(MonsterId id);
std::string monster_summary(MonsterId id);
