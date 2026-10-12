#include "monsters.hpp"

#include <array>
#include <cstddef>
#include <format>

std::string zone_name(ZoneId zone) {
  switch (zone) {
#define X(id, name) \
  case ZoneId::id:  \
    return name;
    RV_ZONE_ID_LIST(X)
#undef X
  }
  return "Unknown";
}

std::span<const ZoneId> all_zone_ids() noexcept {
  static constexpr std::array ids{std::to_array<ZoneId>({
#define X(id, name) ZoneId::id,
      RV_ZONE_ID_LIST(X)
#undef X
  })};
  return ids;
}

std::string monster_name(MonsterId monster) {
  switch (monster) {
#define X(id, name, zone, lvl, hp, intg, atk_ms, hit, ice_hit, ice_pool, acc, eva, xp, \
          cr_min, cr_max, bounty, boss, d1, d2, d3, d4)                                \
  case MonsterId::id:                                                                  \
    return name;
    RV_MONSTER_ID_LIST(X)
#undef X
  }
  return "Unknown";
}

std::span<const MonsterId> all_monster_ids() noexcept {
  static constexpr std::array ids{std::to_array<MonsterId>({
#define X(id, name, zone, lvl, hp, intg, atk_ms, hit, ice_hit, ice_pool, acc, eva, xp, \
          cr_min, cr_max, bounty, boss, d1, d2, d3, d4)                                \
  MonsterId::id,
      RV_MONSTER_ID_LIST(X)
#undef X
  })};
  return ids;
}

const MonsterInfo& get_monster_info(MonsterId id) {
  static constexpr std::array monster_info{std::to_array<MonsterInfo>({
#define RV_DROP(item, chance, min_q, max_q) \
  MonsterDrop { ItemId::item, chance, min_q, max_q }
#define X(id, name, zone, lvl, hp, intg, atk_ms, hit, ice_hit, ice_pool, acc, eva, xp, \
          cr_min, cr_max, bounty, boss, d1, d2, d3, d4)                                \
  {MonsterId::id,                                                                      \
   name,                                                                               \
   ZoneId::zone,                                                                       \
   lvl,                                                                                \
   hp,                                                                                 \
   intg,                                                                               \
   atk_ms,                                                                             \
   hit,                                                                                \
   ice_hit,                                                                            \
   ice_pool,                                                                           \
   acc,                                                                                \
   eva,                                                                                \
   xp,                                                                                 \
   cr_min,                                                                             \
   cr_max,                                                                             \
   bounty,                                                                             \
   boss,                                                                               \
   {{RV_DROP d1, RV_DROP d2, RV_DROP d3, RV_DROP d4}}},
      RV_MONSTER_ID_LIST(X)
#undef X
#undef RV_DROP
  })};
  size_t idx = static_cast<size_t>(id);
  if (idx < monster_info.size()) {
    return monster_info[idx];
  }
  return monster_info[0];
}

std::string monster_summary(MonsterId id) {
  const auto& mon = get_monster_info(id);
  return std::format("{} (Lv {}, {} HP / {} INT, MaxHit {}/{}, {})", mon.name,
                     mon.combat_level, mon.max_hp, mon.max_integrity, mon.max_hit,
                     mon.ice_max_hit, zone_name(mon.zone));
}
