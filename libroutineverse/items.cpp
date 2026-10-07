#include "items.hpp"

#include <array>
#include <format>

const ItemInfo& get_item_info(ItemId id) {
  static constexpr std::array item_info{std::to_array<ItemInfo>({
#define X(id, name, cat, price, heal, req_lvl, atk, str, def, dr, spd) \
  {ItemId::id, name, ItemCategory::cat, price, heal, req_lvl, {atk, str, def, dr, spd}},
      RV_ITEM_ID_LIST(X)
#undef X
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
  }
  return "Item";
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
    default:
      return std::format("{} (+{}Atk, +{}Str, +{}Def, {}%DR)", info.name,
                         info.bonus.attack, info.bonus.strength, info.bonus.defence,
                         info.bonus.damage_reduction);
  }
}

std::string item_name(ItemId id) { return get_item_info(id).name; }

std::span<const ItemId> all_item_ids() noexcept {
  static constexpr std::array ids{std::to_array<ItemId>({
#define X(id, name, cat, price, heal, req_lvl, atk, str, def, dr, spd) ItemId::id,
      RV_ITEM_ID_LIST(X)
#undef X
  })};
  return ids;
}

bool is_valid_item(ItemId id) noexcept {
  return id != ItemId::None && static_cast<size_t>(id) < all_item_ids().size();
}

std::span<const ItemId> data_crystal_ids() noexcept {
  static constexpr std::array ids{std::to_array<ItemId>({
      ItemId::AmberDatachip,
      ItemId::RubyLaserCore,
      ItemId::EmeraldCryptokey,
      ItemId::SapphireCortex,
      ItemId::QuantumDiamond,
  })};
  return ids;
}
