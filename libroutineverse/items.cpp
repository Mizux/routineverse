#include "items.hpp"

#include <array>
#include <format>
#include <vector>

const ItemInfo& get_item_info(ItemId id) {
  static constexpr std::array item_info{std::to_array<ItemInfo>({
#define RV_BONUS(...) Bonus{__VA_ARGS__}
#define X(id, name, cat, req_lvl, price, ...)    \
  {ItemId::id, name, ItemCategory::cat, req_lvl, \
   price __VA_OPT__(, RV_BONUS __VA_ARGS__)},
      RV_ITEM_ID_LIST(X)
#undef X
#undef RV_BONUS
  })};
  size_t idx = static_cast<size_t>(id);
  if (idx >= item_info.size()) {
    return item_info[0];
  }
  return item_info[idx];
}

std::string item_category_name(ItemCategory cat) {
  switch (cat) {
    case ItemCategory::Scrap:
      return "Scrap";
    case ItemCategory::RawMaterial:
      return "Raw Material";
    case ItemCategory::RawBiota:
      return "Raw Biota";
    case ItemCategory::Crop:
      return "Crop/Ingr";
    case ItemCategory::StimFood:
      return "Stim/Ration";
    case ItemCategory::ToxicWaste:
      return "Slag";
    case ItemCategory::RawOre:
      return "Ore";
    case ItemCategory::Alloy:
      return "Alloy";
    case ItemCategory::DataCrystal:
      return "Crystal";
    case ItemCategory::Weapon:
      return "Weapon";
    case ItemCategory::Head:
      return "Head";
    case ItemCategory::Armor:
      return "Armor";
    case ItemCategory::Shield:
      return "Shield";
    case ItemCategory::Cutter:
      return "Cutter";
    case ItemCategory::Harvester:
      return "Harvester";
    case ItemCategory::Drill:
      return "Drill";
    case ItemCategory::Reactor:
      return "Reactor";
    case ItemCategory::AutoStim:
      return "Auto-Stim";
    case ItemCategory::CyberLoot:
      return "Salvage";
    case ItemCategory::Hardware:
      return "CPU/RAM";
    case ItemCategory::Firewall:
      return "Firewall";
    case ItemCategory::AttackIce:
      return "Atk-ICE";
    case ItemCategory::DefenseIce:
      return "Def-ICE";
    case ItemCategory::IntegrityPatch:
      return "INT-Patch";
  }
  return "Item";
}

std::span<const ItemId> items_by_category(ItemCategory cat) noexcept {
  static const auto table = []() {
    constexpr size_t num_cats = static_cast<size_t>(ItemCategory::IntegrityPatch) + 1;
    std::array<std::vector<ItemId>, num_cats> map{};
    for (ItemId id : all_item_ids()) {
      if (!is_valid_item(id)) continue;
      size_t idx = static_cast<size_t>(get_item_info(id).category);
      if (idx < map.size()) {
        map[idx].push_back(id);
      }
    }
    return map;
  }();
  size_t idx = static_cast<size_t>(cat);
  if (idx >= table.size()) {
    return {};
  }
  return table[idx];
}

std::string item_equip_summary(ItemId id) {
  if (!is_valid_item(id)) return "Empty";
  const auto& info = get_item_info(id);
  switch (info.category) {
    case ItemCategory::Cutter:
    case ItemCategory::Harvester:
    case ItemCategory::Drill:
      return std::format("{} (-{}% Cycle)", info.name, info.bonus.speed_bonus_pct);
    case ItemCategory::Reactor:
      return std::format("{} (-{}% Cycle, +{}% XP)", info.name,
                         info.bonus.speed_bonus_pct, info.bonus.speed_bonus_pct);
    case ItemCategory::AutoStim:
      return std::format("{} (Auto-Stim <= {}% HP)", info.name,
                         info.bonus.speed_bonus_pct);
    case ItemCategory::Firewall:
      return std::format("{} (+{}MaxINT, +{}CDef, {}%IDR)", info.name,
                         info.bonus.strength, info.bonus.defence,
                         info.bonus.damage_reduction);
    case ItemCategory::AttackIce:
      return std::format("{} (+{}CAtk, +{}CStr)", info.name, info.bonus.attack,
                         info.bonus.strength);
    case ItemCategory::DefenseIce:
      return std::format("{} (+{}INT, +{}CDef, {}%IDR)", info.name,
                         info.bonus.recover_amount, info.bonus.defence,
                         info.bonus.damage_reduction);
    case ItemCategory::IntegrityPatch:
      if (info.bonus.speed_bonus_pct > 0) {
        return std::format("{} (+{}INT, +{}XP)", info.name, info.bonus.recover_amount,
                           info.bonus.speed_bonus_pct);
      }
      return std::format("{} (+{}INT)", info.name, info.bonus.recover_amount);
    default:
      return std::format("{} (+{}Atk, +{}Str, +{}Def, {}%DR)", info.name,
                         info.bonus.attack, info.bonus.strength, info.bonus.defence,
                         info.bonus.damage_reduction);
  }
}

std::string item_name(ItemId id) { return get_item_info(id).name; }

std::span<const ItemId> all_item_ids() noexcept {
  static constexpr std::array ids{std::to_array<ItemId>({
#define X(id, name, cat, req_lvl, price, ...) ItemId::id,
      RV_ITEM_ID_LIST(X)
#undef X
  })};
  return ids;
}

bool is_valid_item(ItemId id) noexcept {
  return id != ItemId::None && static_cast<size_t>(id) < all_item_ids().size();
}

std::span<const ItemId> data_crystal_ids() noexcept {
  return items_by_category(ItemCategory::DataCrystal);
}
