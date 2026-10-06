#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>

#include "items.hpp"

// Gears & Tools
enum class EquipSlot : int8_t {
  None = -1,
  Weapon,
  Head,
  Armor,
  Shield,
  Cutter,
  Harvester,
  Drill,
  Reactor,
  AutoStim,
};
inline constexpr size_t EQUIP_SLOT_COUNT = 9;
inline constexpr std::array<EquipSlot, EQUIP_SLOT_COUNT> all_equip_slots = {
    EquipSlot::Weapon,    EquipSlot::Head,  EquipSlot::Armor,
    EquipSlot::Shield,    EquipSlot::Cutter, EquipSlot::Harvester,
    EquipSlot::Drill,     EquipSlot::Reactor, EquipSlot::AutoStim,
};

inline constexpr std::optional<EquipSlot> equip_slot_from_int(int val) noexcept {
  if (val >= 0 && static_cast<size_t>(val) < all_equip_slots.size()) {
    return all_equip_slots[val];
  }
  return std::nullopt;
}

inline constexpr EquipSlot equip_slot_or_none(int val) noexcept {
  if (val >= 0 && static_cast<size_t>(val) < all_equip_slots.size()) {
    return all_equip_slots[val];
  }
  return EquipSlot::None;
}

inline constexpr int equip_slot_to_int(EquipSlot slot) noexcept {
  return static_cast<int>(slot);
}

std::string equip_slot_name(EquipSlot slot);
inline constexpr EquipSlot equip_slot(ItemCategory cat) {
  switch (cat) {
    case ItemCategory::Weapon:
      return EquipSlot::Weapon;
    case ItemCategory::Head:
      return EquipSlot::Head;
    case ItemCategory::Armor:
      return EquipSlot::Armor;
    case ItemCategory::Shield:
      return EquipSlot::Shield;
    case ItemCategory::Cutter:
      return EquipSlot::Cutter;
    case ItemCategory::Harvester:
      return EquipSlot::Harvester;
    case ItemCategory::Drill:
      return EquipSlot::Drill;
    case ItemCategory::Reactor:
      return EquipSlot::Reactor;
    case ItemCategory::AutoStim:
      return EquipSlot::AutoStim;
    default:
      return EquipSlot::None;
  }
}

// Shop
struct ShopUpgradeInfo {
  uint8_t tier;
  const char* name;
  const char* description;
  uint8_t req_skill_level;
  uint64_t cost_credits;
  uint8_t speed_bonus_pct;
  ItemId item_id = ItemId::None;
};
inline constexpr int TOOL_TIER_COUNT = 7;
extern const std::array<ShopUpgradeInfo, TOOL_TIER_COUNT> cutter_upgrades;
extern const std::array<ShopUpgradeInfo, TOOL_TIER_COUNT> harvester_upgrades;
extern const std::array<ShopUpgradeInfo, TOOL_TIER_COUNT> drill_upgrades;
extern const std::array<ShopUpgradeInfo, TOOL_TIER_COUNT> reactor_upgrades;
inline constexpr int AUTO_STIM_TIER_COUNT = 4;
extern const std::array<ShopUpgradeInfo, AUTO_STIM_TIER_COUNT> auto_stim_upgrades;

template <size_t N>
int find_upgrade_tier(const std::array<ShopUpgradeInfo, N>& upgrades, ItemId id) {
  if (!is_valid_item(id)) return 0;
  for (size_t i = 0; i < N; ++i) {
    if (upgrades[i].item_id == id) return static_cast<int>(upgrades[i].tier);
  }
  return 0;
}

