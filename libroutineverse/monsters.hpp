#pragma once

#include <array>
#include <cstdint>
#include <span>
#include <string>

#include "items.hpp"

#define RV_ZONE_ID_LIST(X)                 \
  X(NeonSlums, "Neon Slums")               \
  X(BackAlleySector, "Back-Alley Sector")  \
  X(IndustrialSector, "Industrial Sector") \
  X(MegacorpPlaza, "Megacorp Plaza")       \
  X(OrbitalSpire, "Orbital Spire")         \
  X(MainframeCore, "Mainframe Core [BOSS]")

enum class ZoneId : uint8_t {
#define X(id, name) id,
  RV_ZONE_ID_LIST(X)
#undef X
};
std::string zone_name(ZoneId zone);
std::span<const ZoneId> all_zone_ids() noexcept;

#define RV_MONSTER_ID_LIST(X)               \
  X(StrayServoDrone, "Stray Servo-Drone")   \
  X(BioVatHound, "Bio-Vat Hound")           \
  X(StreetScavenger, "Street Scavenger")    \
  X(ChromeGangPunk, "Chrome Gang Punk")     \
  X(RiotEnforcerBot, "Riot Enforcer Bot")   \
  X(ChemMutantBrute, "Chem-Mutant Brute")   \
  X(CryoSecMech, "Cryo-Sec Mech")           \
  X(CorpShadowOp, "Corp Shadow-Op")         \
  X(CobaltCyberNinja, "Cobalt Cyber-Ninja") \
  X(NeutroniumCyborg, "Neutronium Cyborg")  \
  X(ApexCyberWyrm, "Apex Cyber-Wyrm")       \
  X(Nexus9, "NEXUS-9, Rogue Overmind")

enum class MonsterId : uint8_t {
#define X(id, name) id,
  RV_MONSTER_ID_LIST(X)
#undef X
};
std::string monster_name(MonsterId monster);
std::span<const MonsterId> all_monster_ids() noexcept;

struct MonsterDrop {
  ItemId item_id = ItemId::None;
  int chance_pct = 0;  // 1..100
  int min_qty = 0;
  int max_qty = 0;
};

struct MonsterInfo {
  MonsterId id;
  ZoneId zone;
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
const MonsterInfo& get_monster_info(MonsterId id);
std::string monster_summary(MonsterId id);
