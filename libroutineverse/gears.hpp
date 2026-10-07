#pragma once

#include <cstdint>
#include <span>
#include <string>

#include "items.hpp"

// Gears & Tools
#define RV_EQUIP_SLOT_LIST(X, Y) \
  Y(None, -1, "None")            \
  X(Weapon, 0, "Weapon")         \
  X(Head, 1, "Head")             \
  X(Armor, 2, "Armor")           \
  X(Shield, 3, "Shield")         \
  X(Cutter, 4, "Cutter")         \
  X(Harvester, 5, "Harvester")   \
  X(Drill, 6, "Drill")           \
  X(Reactor, 7, "Reactor")       \
  X(AutoStim, 8, "Auto-Stim")

enum class EquipSlot : int8_t {
#define X(id, val, name) id = val,
  RV_EQUIP_SLOT_LIST(X, X)
#undef X
};
std::string equip_slot_name(EquipSlot slot);
std::span<const EquipSlot> all_equip_slots() noexcept;
EquipSlot equip_slot(ItemCategory cat);

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

std::span<const ShopUpgradeInfo> cutter_upgrades() noexcept;
std::span<const ShopUpgradeInfo> harvester_upgrades() noexcept;
std::span<const ShopUpgradeInfo> drill_upgrades() noexcept;
std::span<const ShopUpgradeInfo> reactor_upgrades() noexcept;
std::span<const ShopUpgradeInfo> auto_stim_upgrades() noexcept;

int find_upgrade_tier(std::span<const ShopUpgradeInfo> upgrades, ItemId id);
