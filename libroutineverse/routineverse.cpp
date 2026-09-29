#include "routineverse.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <fstream>
#include <random>
#include <sstream>

namespace {

std::mt19937& rng() {
  static std::mt19937 gen(std::random_device{}());
  return gen;
}

int rand_int(int min_v, int max_v) {
  if (max_v <= min_v) return min_v;
  std::uniform_int_distribution<int> dist(min_v, max_v);
  return dist(rng());
}

// Standard OSRS / Melvor Idle XP table for levels 1..99
const std::array<int, MAX_SKILL_LEVEL + 1>& xp_table() {
  static const auto table = []() {
    std::array<int, MAX_SKILL_LEVEL + 1> t{};
    t[0] = 0;
    t[1] = 0;
    double points = 0.0;
    for (int lvl = 1; lvl < MAX_SKILL_LEVEL; ++lvl) {
      points += std::floor(lvl + 300.0 * std::pow(2.0, lvl / 7.0));
      t[lvl + 1] = static_cast<int>(std::floor(points / 4.0));
    }
    return t;
  }();
  return table;
}

}  // namespace

const std::array<ItemInfo, ITEM_COUNT> item_info = {{
    // Logs (0..8)
    {ITEM_NORMAL_LOGS, "Normal Logs", ItemCategory::Logs, 2, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_OAK_LOGS, "Oak Logs", ItemCategory::Logs, 5, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_WILLOW_LOGS, "Willow Logs", ItemCategory::Logs, 10, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_TEAK_LOGS, "Teak Logs", ItemCategory::Logs, 18, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_MAPLE_LOGS, "Maple Logs", ItemCategory::Logs, 30, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_MAHOGANY_LOGS, "Mahogany Logs", ItemCategory::Logs, 45, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_YEW_LOGS, "Yew Logs", ItemCategory::Logs, 70, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_MAGIC_LOGS, "Magic Logs", ItemCategory::Logs, 120, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_REDWOOD_LOGS, "Redwood Logs", ItemCategory::Logs, 200, 0, EquipSlot::None, 1, 0, 0, 0, 0},

    // Raw Fish (9..16)
    {ITEM_RAW_SHRIMP, "Raw Shrimp", ItemCategory::RawFish, 3, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_RAW_SARDINE, "Raw Sardine", ItemCategory::RawFish, 6, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_RAW_TROUT, "Raw Trout", ItemCategory::RawFish, 14, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_RAW_SALMON, "Raw Salmon", ItemCategory::RawFish, 24, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_RAW_LOBSTER, "Raw Lobster", ItemCategory::RawFish, 45, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_RAW_SWORDFISH, "Raw Swordfish", ItemCategory::RawFish, 75, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_RAW_SHARK, "Raw Shark", ItemCategory::RawFish, 140, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_RAW_WHALE, "Raw Whale", ItemCategory::RawFish, 260, 0, EquipSlot::None, 1, 0, 0, 0, 0},

    // Cooked Food & Burnt (17..26)
    {ITEM_COOKED_SHRIMP, "Cooked Shrimp", ItemCategory::CookedFood, 8, 30, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_COOKED_SARDINE, "Cooked Sardine", ItemCategory::CookedFood, 15, 50, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_COOKED_TROUT, "Cooked Trout", ItemCategory::CookedFood, 32, 80, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_COOKED_SALMON, "Cooked Salmon", ItemCategory::CookedFood, 55, 110, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_COOKED_LOBSTER, "Cooked Lobster", ItemCategory::CookedFood, 100, 160, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_COOKED_SWORDFISH, "Cooked Swordfish", ItemCategory::CookedFood, 165, 220, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_COOKED_SHARK, "Cooked Shark", ItemCategory::CookedFood, 300, 320, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_COOKED_WHALE, "Cooked Whale", ItemCategory::CookedFood, 550, 480, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_COOKED_BEEF, "Cooked Beef", ItemCategory::CookedFood, 6, 25, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_BURNT_FISH, "Burnt Fish", ItemCategory::BurntFood, 1, 0, EquipSlot::None, 1, 0, 0, 0, 0},

    // Ores (27..36)
    {ITEM_COPPER_ORE, "Copper Ore", ItemCategory::Ore, 4, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_TIN_ORE, "Tin Ore", ItemCategory::Ore, 4, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_IRON_ORE, "Iron Ore", ItemCategory::Ore, 12, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_COAL_ORE, "Coal Ore", ItemCategory::Ore, 18, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_SILVER_ORE, "Silver Ore", ItemCategory::Ore, 30, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_GOLD_ORE, "Gold Ore", ItemCategory::Ore, 50, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_MITHRIL_ORE, "Mithril Ore", ItemCategory::Ore, 70, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_ADAMANTITE_ORE, "Adamantite Ore", ItemCategory::Ore, 120, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_RUNITE_ORE, "Runite Ore", ItemCategory::Ore, 220, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_DRAGONITE_ORE, "Dragonite Ore", ItemCategory::Ore, 400, 0, EquipSlot::None, 1, 0, 0, 0, 0},

    // Bars (37..45)
    {ITEM_BRONZE_BAR, "Bronze Bar", ItemCategory::Bar, 15, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_IRON_BAR, "Iron Bar", ItemCategory::Bar, 32, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_STEEL_BAR, "Steel Bar", ItemCategory::Bar, 65, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_SILVER_BAR, "Silver Bar", ItemCategory::Bar, 80, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_GOLD_BAR, "Gold Bar", ItemCategory::Bar, 130, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_MITHRIL_BAR, "Mithril Bar", ItemCategory::Bar, 175, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_ADAMANT_BAR, "Adamant Bar", ItemCategory::Bar, 310, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_RUNE_BAR, "Rune Bar", ItemCategory::Bar, 580, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_DRAGON_BAR, "Dragon Bar", ItemCategory::Bar, 1100, 0, EquipSlot::None, 1, 0, 0, 0, 0},

    // Gems (46..50)
    {ITEM_TOPAZ, "Topaz", ItemCategory::Gem, 150, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_SAPPHIRE, "Sapphire", ItemCategory::Gem, 250, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_RUBY, "Ruby", ItemCategory::Gem, 450, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_EMERALD, "Emerald", ItemCategory::Gem, 750, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_DIAMOND, "Diamond", ItemCategory::Gem, 1500, 0, EquipSlot::None, 1, 0, 0, 0, 0},

    // Weapons (51..57)
    {ITEM_BRONZE_SCIMITAR, "Bronze Scimitar", ItemCategory::Weapon, 45, 0, EquipSlot::Weapon, 1, 10, 12, 0, 0},
    {ITEM_IRON_SCIMITAR, "Iron Scimitar", ItemCategory::Weapon, 100, 0, EquipSlot::Weapon, 5, 18, 20, 0, 0},
    {ITEM_STEEL_SCIMITAR, "Steel Scimitar", ItemCategory::Weapon, 220, 0, EquipSlot::Weapon, 10, 28, 32, 0, 0},
    {ITEM_MITHRIL_SCIMITAR, "Mithril Scimitar", ItemCategory::Weapon, 550, 0, EquipSlot::Weapon, 20, 42, 48, 0, 0},
    {ITEM_ADAMANT_SCIMITAR, "Adamant Scimitar", ItemCategory::Weapon, 1100, 0, EquipSlot::Weapon, 30, 60, 66, 0, 0},
    {ITEM_RUNE_SCIMITAR, "Rune Scimitar", ItemCategory::Weapon, 2400, 0, EquipSlot::Weapon, 40, 84, 92, 0, 0},
    {ITEM_DRAGON_SCIMITAR, "Dragon Scimitar", ItemCategory::Weapon, 6000, 0, EquipSlot::Weapon, 60, 120, 130, 0, 0},

    // Helmets (58..64)
    {ITEM_BRONZE_HELMET, "Bronze Helmet", ItemCategory::Helmet, 40, 0, EquipSlot::Helmet, 1, 0, 0, 6, 1},
    {ITEM_IRON_HELMET, "Iron Helmet", ItemCategory::Helmet, 90, 0, EquipSlot::Helmet, 5, 0, 0, 11, 2},
    {ITEM_STEEL_HELMET, "Steel Helmet", ItemCategory::Helmet, 200, 0, EquipSlot::Helmet, 10, 0, 0, 18, 3},
    {ITEM_MITHRIL_HELMET, "Mithril Helmet", ItemCategory::Helmet, 500, 0, EquipSlot::Helmet, 20, 0, 0, 27, 4},
    {ITEM_ADAMANT_HELMET, "Adamant Helmet", ItemCategory::Helmet, 950, 0, EquipSlot::Helmet, 30, 0, 0, 38, 5},
    {ITEM_RUNE_HELMET, "Rune Helmet", ItemCategory::Helmet, 2100, 0, EquipSlot::Helmet, 40, 0, 0, 52, 7},
    {ITEM_DRAGON_HELMET, "Dragon Helmet", ItemCategory::Helmet, 5200, 0, EquipSlot::Helmet, 60, 0, 0, 72, 10},

    // Platebodies (65..71)
    {ITEM_BRONZE_PLATEBODY, "Bronze Platebody", ItemCategory::Platebody, 85, 0, EquipSlot::Platebody, 1, 0, 0, 14, 2},
    {ITEM_IRON_PLATEBODY, "Iron Platebody", ItemCategory::Platebody, 180, 0, EquipSlot::Platebody, 5, 0, 0, 24, 3},
    {ITEM_STEEL_PLATEBODY, "Steel Platebody", ItemCategory::Platebody, 400, 0, EquipSlot::Platebody, 10, 0, 0, 36, 5},
    {ITEM_MITHRIL_PLATEBODY, "Mithril Platebody", ItemCategory::Platebody, 950, 0, EquipSlot::Platebody, 20, 0, 0, 52, 7},
    {ITEM_ADAMANT_PLATEBODY, "Adamant Platebody", ItemCategory::Platebody, 1900, 0, EquipSlot::Platebody, 30, 0, 0, 74, 9},
    {ITEM_RUNE_PLATEBODY, "Rune Platebody", ItemCategory::Platebody, 4200, 0, EquipSlot::Platebody, 40, 0, 0, 102, 12},
    {ITEM_DRAGON_PLATEBODY, "Dragon Platebody", ItemCategory::Platebody, 9800, 0, EquipSlot::Platebody, 60, 0, 0, 140, 16},

    // Shields (72..78)
    {ITEM_BRONZE_SHIELD, "Bronze Shield", ItemCategory::Shield, 55, 0, EquipSlot::Shield, 1, 0, 0, 9, 1},
    {ITEM_IRON_SHIELD, "Iron Shield", ItemCategory::Shield, 120, 0, EquipSlot::Shield, 5, 0, 0, 16, 2},
    {ITEM_STEEL_SHIELD, "Steel Shield", ItemCategory::Shield, 260, 0, EquipSlot::Shield, 10, 0, 0, 25, 3},
    {ITEM_MITHRIL_SHIELD, "Mithril Shield", ItemCategory::Shield, 620, 0, EquipSlot::Shield, 20, 0, 0, 36, 5},
    {ITEM_ADAMANT_SHIELD, "Adamant Shield", ItemCategory::Shield, 1250, 0, EquipSlot::Shield, 30, 0, 0, 50, 6},
    {ITEM_RUNE_SHIELD, "Rune Shield", ItemCategory::Shield, 2800, 0, EquipSlot::Shield, 40, 0, 0, 68, 8},
    {ITEM_DRAGON_SHIELD, "Dragon Shield", ItemCategory::Shield, 6800, 0, EquipSlot::Shield, 60, 0, 0, 96, 12},

    // Monster Loot (79..83)
    {ITEM_BONES, "Bones", ItemCategory::Loot, 8, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_BIG_BONES, "Big Bones", ItemCategory::Loot, 30, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_DRAGON_BONES, "Dragon Bones", ItemCategory::Loot, 180, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_FEATHER, "Feather", ItemCategory::Loot, 3, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_LEATHER, "Leather", ItemCategory::Loot, 16, 0, EquipSlot::None, 1, 0, 0, 0, 0},
}};

const std::vector<SkillAction> skill_actions = {
    // Woodcutting (0..8)
    {0, SkillType::Woodcutting, "Chop Normal Tree", 1, 3000, 15, ITEM_NORMAL_LOGS, 1, -1, 0, -1, 0},
    {1, SkillType::Woodcutting, "Chop Oak Tree", 10, 3500, 30, ITEM_OAK_LOGS, 1, -1, 0, -1, 0},
    {2, SkillType::Woodcutting, "Chop Willow Tree", 25, 4000, 55, ITEM_WILLOW_LOGS, 1, -1, 0, -1, 0},
    {3, SkillType::Woodcutting, "Chop Teak Tree", 35, 4500, 85, ITEM_TEAK_LOGS, 1, -1, 0, -1, 0},
    {4, SkillType::Woodcutting, "Chop Maple Tree", 45, 5000, 120, ITEM_MAPLE_LOGS, 1, -1, 0, -1, 0},
    {5, SkillType::Woodcutting, "Chop Mahogany Tree", 55, 5500, 165, ITEM_MAHOGANY_LOGS, 1, -1, 0, -1, 0},
    {6, SkillType::Woodcutting, "Chop Yew Tree", 60, 6000, 220, ITEM_YEW_LOGS, 1, -1, 0, -1, 0},
    {7, SkillType::Woodcutting, "Chop Magic Tree", 75, 7500, 340, ITEM_MAGIC_LOGS, 1, -1, 0, -1, 0},
    {8, SkillType::Woodcutting, "Chop Redwood Tree", 90, 9000, 500, ITEM_REDWOOD_LOGS, 1, -1, 0, -1, 0},

    // Fishing (9..16)
    {9, SkillType::Fishing, "Catch Raw Shrimp", 1, 3000, 12, ITEM_RAW_SHRIMP, 1, -1, 0, -1, 0},
    {10, SkillType::Fishing, "Catch Raw Sardine", 5, 3400, 24, ITEM_RAW_SARDINE, 1, -1, 0, -1, 0},
    {11, SkillType::Fishing, "Catch Raw Trout", 20, 4000, 55, ITEM_RAW_TROUT, 1, -1, 0, -1, 0},
    {12, SkillType::Fishing, "Catch Raw Salmon", 35, 4500, 90, ITEM_RAW_SALMON, 1, -1, 0, -1, 0},
    {13, SkillType::Fishing, "Catch Raw Lobster", 45, 5200, 135, ITEM_RAW_LOBSTER, 1, -1, 0, -1, 0},
    {14, SkillType::Fishing, "Catch Raw Swordfish", 55, 6000, 195, ITEM_RAW_SWORDFISH, 1, -1, 0, -1, 0},
    {15, SkillType::Fishing, "Catch Raw Shark", 70, 7200, 310, ITEM_RAW_SHARK, 1, -1, 0, -1, 0},
    {16, SkillType::Fishing, "Catch Raw Whale", 85, 8500, 480, ITEM_RAW_WHALE, 1, -1, 0, -1, 0},

    // Firemaking (17..25)
    {17, SkillType::Firemaking, "Burn Normal Logs", 1, 2200, 22, -1, 0, ITEM_NORMAL_LOGS, 1, -1, 0},
    {18, SkillType::Firemaking, "Burn Oak Logs", 10, 2400, 42, -1, 0, ITEM_OAK_LOGS, 1, -1, 0},
    {19, SkillType::Firemaking, "Burn Willow Logs", 25, 2600, 75, -1, 0, ITEM_WILLOW_LOGS, 1, -1, 0},
    {20, SkillType::Firemaking, "Burn Teak Logs", 35, 2800, 110, -1, 0, ITEM_TEAK_LOGS, 1, -1, 0},
    {21, SkillType::Firemaking, "Burn Maple Logs", 45, 3000, 155, -1, 0, ITEM_MAPLE_LOGS, 1, -1, 0},
    {22, SkillType::Firemaking, "Burn Mahogany Logs", 55, 3200, 210, -1, 0, ITEM_MAHOGANY_LOGS, 1, -1, 0},
    {23, SkillType::Firemaking, "Burn Yew Logs", 60, 3500, 280, -1, 0, ITEM_YEW_LOGS, 1, -1, 0},
    {24, SkillType::Firemaking, "Burn Magic Logs", 75, 3800, 410, -1, 0, ITEM_MAGIC_LOGS, 1, -1, 0},
    {25, SkillType::Firemaking, "Burn Redwood Logs", 90, 4200, 600, -1, 0, ITEM_REDWOOD_LOGS, 1, -1, 0},

    // Cooking (26..33)
    {26, SkillType::Cooking, "Cook Shrimp (+30 HP)", 1, 2600, 18, ITEM_COOKED_SHRIMP, 1, ITEM_RAW_SHRIMP, 1, -1, 0},
    {27, SkillType::Cooking, "Cook Sardine (+50 HP)", 5, 2800, 34, ITEM_COOKED_SARDINE, 1, ITEM_RAW_SARDINE, 1, -1, 0},
    {28, SkillType::Cooking, "Cook Trout (+80 HP)", 20, 3000, 70, ITEM_COOKED_TROUT, 1, ITEM_RAW_TROUT, 1, -1, 0},
    {29, SkillType::Cooking, "Cook Salmon (+110 HP)", 35, 3200, 115, ITEM_COOKED_SALMON, 1, ITEM_RAW_SALMON, 1, -1, 0},
    {30, SkillType::Cooking, "Cook Lobster (+160 HP)", 45, 3400, 175, ITEM_COOKED_LOBSTER, 1, ITEM_RAW_LOBSTER, 1, -1, 0},
    {31, SkillType::Cooking, "Cook Swordfish (+220 HP)", 55, 3600, 240, ITEM_COOKED_SWORDFISH, 1, ITEM_RAW_SWORDFISH, 1, -1, 0},
    {32, SkillType::Cooking, "Cook Shark (+320 HP)", 70, 3800, 360, ITEM_COOKED_SHARK, 1, ITEM_RAW_SHARK, 1, -1, 0},
    {33, SkillType::Cooking, "Cook Whale (+480 HP)", 85, 4200, 540, ITEM_COOKED_WHALE, 1, ITEM_RAW_WHALE, 1, -1, 0},

    // Mining (34..43)
    {34, SkillType::Mining, "Mine Copper Ore", 1, 2800, 14, ITEM_COPPER_ORE, 1, -1, 0, -1, 0},
    {35, SkillType::Mining, "Mine Tin Ore", 1, 2800, 14, ITEM_TIN_ORE, 1, -1, 0, -1, 0},
    {36, SkillType::Mining, "Mine Iron Ore", 15, 3200, 35, ITEM_IRON_ORE, 1, -1, 0, -1, 0},
    {37, SkillType::Mining, "Mine Coal Ore", 30, 3500, 55, ITEM_COAL_ORE, 1, -1, 0, -1, 0},
    {38, SkillType::Mining, "Mine Silver Ore", 35, 3800, 75, ITEM_SILVER_ORE, 1, -1, 0, -1, 0},
    {39, SkillType::Mining, "Mine Gold Ore", 40, 4200, 105, ITEM_GOLD_ORE, 1, -1, 0, -1, 0},
    {40, SkillType::Mining, "Mine Mithril Ore", 50, 4800, 150, ITEM_MITHRIL_ORE, 1, -1, 0, -1, 0},
    {41, SkillType::Mining, "Mine Adamantite Ore", 70, 5800, 230, ITEM_ADAMANTITE_ORE, 1, -1, 0, -1, 0},
    {42, SkillType::Mining, "Mine Runite Ore", 80, 7000, 350, ITEM_RUNITE_ORE, 1, -1, 0, -1, 0},
    {43, SkillType::Mining, "Mine Dragonite Ore", 92, 8500, 520, ITEM_DRAGONITE_ORE, 1, -1, 0, -1, 0},

    // Smithing - Smelting Bars & Forging Gear (44..78)
    {44, SkillType::Smithing, "Smelt Bronze Bar", 1, 2200, 16, ITEM_BRONZE_BAR, 1, ITEM_COPPER_ORE, 1, ITEM_TIN_ORE, 1},
    {45, SkillType::Smithing, "Forge Bronze Scimitar", 1, 2500, 35, ITEM_BRONZE_SCIMITAR, 1, ITEM_BRONZE_BAR, 2, -1, 0},
    {46, SkillType::Smithing, "Forge Bronze Helmet", 2, 2500, 35, ITEM_BRONZE_HELMET, 1, ITEM_BRONZE_BAR, 2, -1, 0},
    {47, SkillType::Smithing, "Forge Bronze Shield", 3, 2600, 50, ITEM_BRONZE_SHIELD, 1, ITEM_BRONZE_BAR, 3, -1, 0},
    {48, SkillType::Smithing, "Forge Bronze Platebody", 5, 2800, 80, ITEM_BRONZE_PLATEBODY, 1, ITEM_BRONZE_BAR, 5, -1, 0},

    {49, SkillType::Smithing, "Smelt Iron Bar", 15, 2400, 32, ITEM_IRON_BAR, 1, ITEM_IRON_ORE, 1, -1, 0},
    {50, SkillType::Smithing, "Forge Iron Scimitar", 15, 2600, 65, ITEM_IRON_SCIMITAR, 1, ITEM_IRON_BAR, 2, -1, 0},
    {51, SkillType::Smithing, "Forge Iron Helmet", 16, 2600, 65, ITEM_IRON_HELMET, 1, ITEM_IRON_BAR, 2, -1, 0},
    {52, SkillType::Smithing, "Forge Iron Shield", 18, 2700, 95, ITEM_IRON_SHIELD, 1, ITEM_IRON_BAR, 3, -1, 0},
    {53, SkillType::Smithing, "Forge Iron Platebody", 20, 2900, 150, ITEM_IRON_PLATEBODY, 1, ITEM_IRON_BAR, 5, -1, 0},

    {54, SkillType::Smithing, "Smelt Steel Bar", 30, 2600, 55, ITEM_STEEL_BAR, 1, ITEM_IRON_ORE, 1, ITEM_COAL_ORE, 2},
    {55, SkillType::Smithing, "Forge Steel Scimitar", 30, 2800, 110, ITEM_STEEL_SCIMITAR, 1, ITEM_STEEL_BAR, 2, -1, 0},
    {56, SkillType::Smithing, "Forge Steel Helmet", 32, 2800, 110, ITEM_STEEL_HELMET, 1, ITEM_STEEL_BAR, 2, -1, 0},
    {57, SkillType::Smithing, "Forge Steel Shield", 34, 2900, 160, ITEM_STEEL_SHIELD, 1, ITEM_STEEL_BAR, 3, -1, 0},
    {58, SkillType::Smithing, "Forge Steel Platebody", 36, 3100, 260, ITEM_STEEL_PLATEBODY, 1, ITEM_STEEL_BAR, 5, -1, 0},

    {59, SkillType::Smithing, "Smelt Silver Bar", 35, 2500, 68, ITEM_SILVER_BAR, 1, ITEM_SILVER_ORE, 1, -1, 0},
    {60, SkillType::Smithing, "Smelt Gold Bar", 40, 2600, 95, ITEM_GOLD_BAR, 1, ITEM_GOLD_ORE, 1, -1, 0},

    {61, SkillType::Smithing, "Smelt Mithril Bar", 50, 2800, 115, ITEM_MITHRIL_BAR, 1, ITEM_MITHRIL_ORE, 1, ITEM_COAL_ORE, 4},
    {62, SkillType::Smithing, "Forge Mithril Scimitar", 50, 3000, 220, ITEM_MITHRIL_SCIMITAR, 1, ITEM_MITHRIL_BAR, 2, -1, 0},
    {63, SkillType::Smithing, "Forge Mithril Helmet", 52, 3000, 220, ITEM_MITHRIL_HELMET, 1, ITEM_MITHRIL_BAR, 2, -1, 0},
    {64, SkillType::Smithing, "Forge Mithril Shield", 54, 3100, 320, ITEM_MITHRIL_SHIELD, 1, ITEM_MITHRIL_BAR, 3, -1, 0},
    {65, SkillType::Smithing, "Forge Mithril Platebody", 56, 3300, 520, ITEM_MITHRIL_PLATEBODY, 1, ITEM_MITHRIL_BAR, 5, -1, 0},

    {66, SkillType::Smithing, "Smelt Adamant Bar", 70, 3000, 175, ITEM_ADAMANT_BAR, 1, ITEM_ADAMANTITE_ORE, 1, ITEM_COAL_ORE, 6},
    {67, SkillType::Smithing, "Forge Adamant Scimitar", 70, 3200, 340, ITEM_ADAMANT_SCIMITAR, 1, ITEM_ADAMANT_BAR, 2, -1, 0},
    {68, SkillType::Smithing, "Forge Adamant Helmet", 72, 3200, 340, ITEM_ADAMANT_HELMET, 1, ITEM_ADAMANT_BAR, 2, -1, 0},
    {69, SkillType::Smithing, "Forge Adamant Shield", 74, 3300, 500, ITEM_ADAMANT_SHIELD, 1, ITEM_ADAMANT_BAR, 3, -1, 0},
    {70, SkillType::Smithing, "Forge Adamant Platebody", 76, 3500, 820, ITEM_ADAMANT_PLATEBODY, 1, ITEM_ADAMANT_BAR, 5, -1, 0},

    {71, SkillType::Smithing, "Smelt Rune Bar", 80, 3200, 260, ITEM_RUNE_BAR, 1, ITEM_RUNITE_ORE, 1, ITEM_COAL_ORE, 8},
    {72, SkillType::Smithing, "Forge Rune Scimitar", 80, 3400, 520, ITEM_RUNE_SCIMITAR, 1, ITEM_RUNE_BAR, 2, -1, 0},
    {73, SkillType::Smithing, "Forge Rune Helmet", 82, 3400, 520, ITEM_RUNE_HELMET, 1, ITEM_RUNE_BAR, 2, -1, 0},
    {74, SkillType::Smithing, "Forge Rune Shield", 84, 3500, 760, ITEM_RUNE_SHIELD, 1, ITEM_RUNE_BAR, 3, -1, 0},
    {75, SkillType::Smithing, "Forge Rune Platebody", 86, 3700, 1250, ITEM_RUNE_PLATEBODY, 1, ITEM_RUNE_BAR, 5, -1, 0},

    {76, SkillType::Smithing, "Smelt Dragon Bar", 92, 3600, 400, ITEM_DRAGON_BAR, 1, ITEM_DRAGONITE_ORE, 1, ITEM_RUNITE_ORE, 2},
    {77, SkillType::Smithing, "Forge Dragon Scimitar", 92, 3800, 800, ITEM_DRAGON_SCIMITAR, 1, ITEM_DRAGON_BAR, 2, -1, 0},
    {78, SkillType::Smithing, "Forge Dragon Helmet", 94, 3800, 800, ITEM_DRAGON_HELMET, 1, ITEM_DRAGON_BAR, 2, -1, 0},
    {79, SkillType::Smithing, "Forge Dragon Shield", 96, 3900, 1150, ITEM_DRAGON_SHIELD, 1, ITEM_DRAGON_BAR, 3, -1, 0},
    {80, SkillType::Smithing, "Forge Dragon Platebody", 98, 4100, 1900, ITEM_DRAGON_PLATEBODY, 1, ITEM_DRAGON_BAR, 5, -1, 0},
};

const std::array<MonsterInfo, MONSTER_COUNT> monster_info = {{
    {0,
     "Chicken",
     "Farmlands",
     1,
     30,
     2600,
     6,
     15,
     10,
     20,
     3,
     10,
     1,
     false,
     {{{ITEM_FEATHER, 90, 2, 6},
       {ITEM_BONES, 100, 1, 1},
       {ITEM_COOKED_SHRIMP, 25, 1, 2}}}},
    {1,
     "Cow",
     "Farmlands",
     4,
     65,
     2800,
     12,
     25,
     18,
     42,
     8,
     22,
     1,
     false,
     {{{ITEM_LEATHER, 85, 1, 2},
       {ITEM_COOKED_BEEF, 70, 1, 2},
       {ITEM_BONES, 100, 1, 1}}}},
    {2,
     "Junior Farmer",
     "Farmlands",
     9,
     110,
     2600,
     20,
     40,
     32,
     72,
     18,
     45,
     1,
     false,
     {{{ITEM_OAK_LOGS, 50, 2, 5},
       {ITEM_COOKED_TROUT, 40, 1, 2},
       {ITEM_BONES, 100, 1, 1}}}},
    {3,
     "Goblin",
     "Goblin Village",
     14,
     160,
     2500,
     28,
     55,
     45,
     105,
     28,
     70,
     1,
     false,
     {{{ITEM_BRONZE_SCIMITAR, 15, 1, 1},
       {ITEM_IRON_ORE, 45, 2, 4},
       {ITEM_BONES, 100, 1, 1}}}},
    {4,
     "Hill Giant",
     "Giant Caves",
     24,
     280,
     3000,
     45,
     85,
     72,
     185,
     55,
     130,
     10,
     false,
     {{{ITEM_BIG_BONES, 100, 1, 2},
       {ITEM_STEEL_SCIMITAR, 12, 1, 1},
       {ITEM_COAL_ORE, 40, 3, 6}}}},
    {5,
     "Moss Giant",
     "Giant Caves",
     36,
     450,
     3000,
     68,
     120,
     105,
     295,
     95,
     220,
     20,
     false,
     {{{ITEM_BIG_BONES, 100, 1, 2},
       {ITEM_MITHRIL_ORE, 45, 2, 5},
       {ITEM_COOKED_LOBSTER, 35, 2, 4}}}},
    {6,
     "Ice Giant",
     "Giant Caves",
     48,
     650,
     2900,
     92,
     165,
     145,
     430,
     150,
     340,
     30,
     false,
     {{{ITEM_BIG_BONES, 100, 2, 3},
       {ITEM_MITHRIL_PLATEBODY, 10, 1, 1},
       {ITEM_SAPPHIRE, 25, 1, 2}}}},
    {7,
     "Black Knight",
     "Castle of Routineverse",
     60,
     880,
     2700,
     125,
     215,
     195,
     580,
     230,
     520,
     40,
     false,
     {{{ITEM_ADAMANT_SCIMITAR, 12, 1, 1},
       {ITEM_ADAMANTITE_ORE, 45, 2, 5},
       {ITEM_COOKED_SWORDFISH, 40, 2, 4}}}},
    {8,
     "Mithril Knight",
     "Castle of Routineverse",
     74,
     1150,
     2600,
     160,
     270,
     250,
     780,
     350,
     780,
     50,
     false,
     {{{ITEM_ADAMANT_PLATEBODY, 10, 1, 1},
       {ITEM_RUBY, 30, 1, 2},
       {ITEM_COOKED_SHARK, 35, 2, 4}}}},
    {9,
     "Rune Knight",
     "Castle of Routineverse",
     88,
     1500,
     2500,
     205,
     340,
     320,
     1050,
     550,
     1200,
     65,
     false,
     {{{ITEM_RUNE_SCIMITAR, 10, 1, 1},
       {ITEM_RUNE_PLATEBODY, 8, 1, 1},
       {ITEM_EMERALD, 30, 1, 2}}}},
    {10,
     "Red Dragon",
     "Dragon Valley",
     110,
     2150,
     2600,
     270,
     430,
     410,
     1500,
     900,
     2000,
     75,
     false,
     {{{ITEM_DRAGON_BONES, 100, 1, 2},
       {ITEM_DRAGONITE_ORE, 35, 2, 4},
       {ITEM_DIAMOND, 25, 1, 2}}}},
    {11,
     "Malcs, Guardian of Routineverse",
     "Volcanic Cave [BOSS]",
     150,
     3500,
     2400,
     360,
     550,
     520,
     3000,
     2500,
     5500,
     85,
     true,
     {{{ITEM_DRAGON_SCIMITAR, 20, 1, 1},
       {ITEM_DRAGON_PLATEBODY, 15, 1, 1},
       {ITEM_COOKED_WHALE, 60, 4, 8}}}},
}};

const std::array<ShopUpgradeInfo, TOOL_TIER_COUNT> axe_upgrades = {{
    {0, "Bronze Axe", "Starter Woodcutting Axe", 1, 0, 0},
    {1, "Iron Axe", "-6% Woodcutting Interval", 10, 200, 6},
    {2, "Steel Axe", "-12% Woodcutting Interval", 25, 750, 12},
    {3, "Mithril Axe", "-18% Woodcutting Interval", 40, 2500, 18},
    {4, "Adamant Axe", "-24% Woodcutting Interval", 55, 8000, 24},
    {5, "Rune Axe", "-30% Woodcutting Interval", 70, 25000, 30},
    {6, "Dragon Axe", "-38% Woodcutting Interval", 85, 80000, 38},
}};

const std::array<ShopUpgradeInfo, TOOL_TIER_COUNT> rod_upgrades = {{
    {0, "Bronze Fishing Rod", "Starter Fishing Rod", 1, 0, 0},
    {1, "Iron Fishing Rod", "-6% Fishing Interval", 10, 200, 6},
    {2, "Steel Fishing Rod", "-12% Fishing Interval", 25, 750, 12},
    {3, "Mithril Fishing Rod", "-18% Fishing Interval", 40, 2500, 18},
    {4, "Adamant Fishing Rod", "-24% Fishing Interval", 55, 8000, 24},
    {5, "Rune Fishing Rod", "-30% Fishing Interval", 70, 25000, 30},
    {6, "Dragon Fishing Rod", "-38% Fishing Interval", 85, 80000, 38},
}};

const std::array<ShopUpgradeInfo, TOOL_TIER_COUNT> pickaxe_upgrades = {{
    {0, "Bronze Pickaxe", "Starter Mining Pickaxe", 1, 0, 0},
    {1, "Iron Pickaxe", "-6% Mining Interval", 10, 200, 6},
    {2, "Steel Pickaxe", "-12% Mining Interval", 25, 750, 12},
    {3, "Mithril Pickaxe", "-18% Mining Interval", 40, 2500, 18},
    {4, "Adamant Pickaxe", "-24% Mining Interval", 55, 8000, 24},
    {5, "Rune Pickaxe", "-30% Mining Interval", 70, 25000, 30},
    {6, "Dragon Pickaxe", "-38% Mining Interval", 85, 80000, 38},
}};

const std::array<ShopUpgradeInfo, TOOL_TIER_COUNT> fire_upgrades = {{
    {0, "Basic Campfire", "Starter Cooking Fire", 1, 0, 0},
    {1, "Oak Cooking Fire", "-5% Cooking Interval & +5% XP", 10, 250, 5},
    {2, "Willow Cooking Fire", "-10% Cooking Interval & +10% XP", 25, 900, 10},
    {3, "Maple Cooking Fire", "-15% Cooking Interval & +15% XP", 45, 3000, 15},
    {4, "Yew Cooking Fire", "-20% Cooking Interval & +20% XP", 60, 10000, 20},
    {5, "Magic Cooking Fire", "-26% Cooking Interval & +26% XP", 75, 30000, 26},
    {6, "Redwood Cooking Fire", "-34% Cooking Interval & +34% XP", 90, 95000, 34},
}};

const std::array<ShopUpgradeInfo, AUTO_EAT_TIER_COUNT> auto_eat_upgrades = {{
    {0, "No Auto-Eat", "Manual food eating only", 1, 0, 0},
    {1, "Auto-Eat — Tier I", "Auto-eats equipped food below 25% HP", 1, 1500, 25},
    {2, "Auto-Eat — Tier II", "Auto-eats equipped food below 40% HP", 1, 12000, 40},
    {3, "Auto-Eat — Tier III", "Auto-eats equipped food below 55% HP", 1, 50000, 55},
}};

std::string skill_name(SkillType skill) {
  switch (skill) {
    case SkillType::Woodcutting:
      return "Woodcutting";
    case SkillType::Fishing:
      return "Fishing";
    case SkillType::Firemaking:
      return "Firemaking";
    case SkillType::Cooking:
      return "Cooking";
    case SkillType::Mining:
      return "Mining";
    case SkillType::Smithing:
      return "Smithing";
    case SkillType::Attack:
      return "Attack";
    case SkillType::Strength:
      return "Strength";
    case SkillType::Defence:
      return "Defence";
    case SkillType::Hitpoints:
      return "Hitpoints";
    case SkillType::Slayer:
      return "Slayer";
  }
  return "Unknown";
}

std::string skill_short_name(SkillType skill) {
  switch (skill) {
    case SkillType::Woodcutting:
      return "WC";
    case SkillType::Fishing:
      return "FSH";
    case SkillType::Firemaking:
      return "FM";
    case SkillType::Cooking:
      return "CK";
    case SkillType::Mining:
      return "MIN";
    case SkillType::Smithing:
      return "SMT";
    case SkillType::Attack:
      return "ATK";
    case SkillType::Strength:
      return "STR";
    case SkillType::Defence:
      return "DEF";
    case SkillType::Hitpoints:
      return "HP";
    case SkillType::Slayer:
      return "SLY";
  }
  return "???";
}

std::string item_category_name(ItemCategory cat) {
  switch (cat) {
    case ItemCategory::Logs:
      return "Logs";
    case ItemCategory::RawFish:
      return "Raw Fish";
    case ItemCategory::CookedFood:
      return "Food";
    case ItemCategory::BurntFood:
      return "Junk";
    case ItemCategory::Ore:
      return "Ore";
    case ItemCategory::Bar:
      return "Bar";
    case ItemCategory::Gem:
      return "Gem";
    case ItemCategory::Weapon:
      return "Weapon";
    case ItemCategory::Helmet:
      return "Helmet";
    case ItemCategory::Platebody:
      return "Platebody";
    case ItemCategory::Shield:
      return "Shield";
    case ItemCategory::Loot:
      return "Loot";
  }
  return "Item";
}

std::string equip_slot_name(EquipSlot slot) {
  switch (slot) {
    case EquipSlot::Weapon:
      return "Weapon";
    case EquipSlot::Helmet:
      return "Helmet";
    case EquipSlot::Platebody:
      return "Platebody";
    case EquipSlot::Shield:
      return "Shield";
    case EquipSlot::None:
      return "None";
  }
  return "None";
}

std::string attack_style_name(AttackStyle style) {
  switch (style) {
    case AttackStyle::Accurate:
      return "Accurate (Attack)";
    case AttackStyle::Aggressive:
      return "Aggressive (Strength)";
    case AttackStyle::Defensive:
      return "Defensive (Defence)";
  }
  return "Accurate";
}

std::string number_string(long long value) {
  bool neg = value < 0;
  unsigned long long v = neg ? static_cast<unsigned long long>(-value)
                             : static_cast<unsigned long long>(value);
  std::string raw = std::to_string(v);
  std::string out;
  int count = 0;
  for (auto it = raw.rbegin(); it != raw.rend(); ++it) {
    if (count > 0 && count % 3 == 0) out.push_back(',');
    out.push_back(*it);
    ++count;
  }
  if (neg) out.push_back('-');
  std::reverse(out.begin(), out.end());
  return out;
}

std::string money_string(long long value) {
  return number_string(value) + " GP";
}

int xp_for_level(int level) {
  const auto& tbl = xp_table();
  int clamped = std::clamp(level, 1, MAX_SKILL_LEVEL);
  return tbl[clamped];
}

int level_for_xp(long long xp) {
  const auto& tbl = xp_table();
  for (int lvl = MAX_SKILL_LEVEL; lvl >= 1; --lvl) {
    if (xp >= tbl[lvl]) return lvl;
  }
  return 1;
}

double level_progress_ratio(long long xp) {
  int lvl = level_for_xp(xp);
  if (lvl >= MAX_SKILL_LEVEL) return 1.0;
  long long cur_base = xp_for_level(lvl);
  long long next_base = xp_for_level(lvl + 1);
  if (next_base <= cur_base) return 1.0;
  return std::clamp(
      static_cast<double>(xp - cur_base) / static_cast<double>(next_base - cur_base),
      0.0, 1.0);
}

std::vector<int> actions_for_skill(SkillType skill) {
  std::vector<int> res;
  for (const auto& act : skill_actions) {
    if (act.skill == skill) res.push_back(act.id);
  }
  return res;
}

// ============================================================================
// GameState Implementation
// ============================================================================

GameState::GameState() { new_game(); }

void GameState::new_game() {
  gp = 250;
  slayer_coins = 0;
  total_ticks_ms = 0;

  xp.fill(0);
  // Hitpoints starts at Level 10 (1,154 XP) just like OSRS / Melvor Idle!
  xp[static_cast<int>(SkillType::Hitpoints)] = xp_for_level(10);

  action_mastery_xp.assign(skill_actions.size(), 0);

  bank_capacity = 24;
  bank.clear();

  equipped_items = {-1, -1, -1, -1};
  equipped_food_item = ITEM_COOKED_SHRIMP;
  equipped_food_qty = 10;

  // Give starter Bronze Scimitar equipped and a few logs/shrimp
  equipped_items[static_cast<int>(EquipSlot::Weapon)] = ITEM_BRONZE_SCIMITAR;
  add_item(ITEM_NORMAL_LOGS, 5, false);
  add_item(ITEM_COOKED_SHRIMP, 5, false);

  axe_tier = 0;
  rod_tier = 0;
  pickaxe_tier = 0;
  fire_tier = 0;
  auto_eat_tier = 0;

  active_type = ActiveActivityType::Skill;
  active_action_id = 0;  // Start chopping Normal Tree by default!
  active_progress_ms = 0;
  active_target_ms = action_effective_interval_ms(0);

  attack_style = AttackStyle::Accurate;
  player_hp = max_hp();
  active_monster_id = 0;
  monster_hp = monster_info[0].max_hp;
  player_attack_timer_ms = 0;
  monster_attack_timer_ms = 0;
  hp_regen_timer_ms = 0;

  slayer_task_monster_id = 0;
  slayer_task_remaining = 8;
  slayer_tasks_completed = 0;

  monster_kills.fill(0);
  total_items_gathered = 0;
  total_monsters_killed = 0;
  total_gp_earned = 250;
  player_deaths = 0;

  game_log.clear();
  gp_history.clear();
  bank_value_history.clear();
  total_level_history.clear();
  total_xp_history.clear();
  hp_history.clear();
  for (auto& vec : skill_xp_history) vec.clear();
  history_timer_ms_ = 0;

  status_banner = "Chopping Normal Tree (Woodcutting)";
  add_log("Welcome to Routineverse! You begin your adventure with a Bronze Scimitar, 10 Cooked Shrimp, and 250 GP.");
  add_log("Active task: Chop Normal Tree. Select any skill or monster to train!");
  record_history_snapshot();
}

void GameState::add_log(const std::string& entry) {
  game_log.push_back(entry);
  if (game_log.size() > 120) {
    game_log.erase(game_log.begin(),
                   game_log.begin() + (game_log.size() - 120));
  }
}

void GameState::record_history_snapshot() {
  auto push_capped = [](auto& vec, auto val) {
    vec.push_back(val);
    if (static_cast<int>(vec.size()) > MAX_HISTORY_POINTS) {
      vec.erase(vec.begin());
    }
  };

  push_capped(gp_history, gp);
  push_capped(bank_value_history, total_bank_value());
  push_capped(total_level_history, total_skill_level());
  push_capped(total_xp_history, total_skill_xp());
  push_capped(hp_history, player_hp);
  for (int i = 0; i < SKILL_COUNT; ++i) {
    push_capped(skill_xp_history[i], xp[i]);
  }
}

int GameState::skill_level(SkillType skill) const {
  return level_for_xp(xp[static_cast<int>(skill)]);
}

long long GameState::skill_xp(SkillType skill) const {
  return xp[static_cast<int>(skill)];
}

int GameState::total_skill_level() const {
  int sum = 0;
  for (int i = 0; i < SKILL_COUNT; ++i) {
    sum += level_for_xp(xp[i]);
  }
  return sum;
}

long long GameState::total_skill_xp() const {
  long long sum = 0;
  for (long long v : xp) sum += v;
  return sum;
}

int GameState::mastery_level(int global_action_id) const {
  if (global_action_id < 0 ||
      global_action_id >= static_cast<int>(action_mastery_xp.size())) {
    return 1;
  }
  return level_for_xp(action_mastery_xp[global_action_id]);
}

int GameState::action_effective_interval_ms(int global_action_id) const {
  if (global_action_id < 0 ||
      global_action_id >= static_cast<int>(skill_actions.size())) {
    return 2000;
  }
  const auto& act = skill_actions[global_action_id];
  int bonus_pct = 0;
  switch (act.skill) {
    case SkillType::Woodcutting:
      bonus_pct = axe_upgrades[std::clamp(axe_tier, 0, TOOL_TIER_COUNT - 1)]
                      .speed_bonus_pct;
      break;
    case SkillType::Fishing:
      bonus_pct = rod_upgrades[std::clamp(rod_tier, 0, TOOL_TIER_COUNT - 1)]
                      .speed_bonus_pct;
      break;
    case SkillType::Mining:
      bonus_pct =
          pickaxe_upgrades[std::clamp(pickaxe_tier, 0, TOOL_TIER_COUNT - 1)]
              .speed_bonus_pct;
      break;
    case SkillType::Cooking:
      bonus_pct = fire_upgrades[std::clamp(fire_tier, 0, TOOL_TIER_COUNT - 1)]
                      .speed_bonus_pct;
      break;
    default:
      break;
  }
  // Mastery also reduces interval slightly (up to -10% at Mastery 99)
  int m_lvl = mastery_level(global_action_id);
  bonus_pct += (m_lvl / 10);

  int eff = (act.base_interval_ms * std::max(35, 100 - bonus_pct)) / 100;
  return std::max(400, eff);
}

bool GameState::can_perform_action(int global_action_id) const {
  if (global_action_id < 0 ||
      global_action_id >= static_cast<int>(skill_actions.size())) {
    return false;
  }
  const auto& act = skill_actions[global_action_id];
  if (skill_level(act.skill) < act.req_level) return false;
  if (act.input_item_1 >= 0 && item_qty(act.input_item_1) < act.input_qty_1) {
    return false;
  }
  if (act.input_item_2 >= 0 && item_qty(act.input_item_2) < act.input_qty_2) {
    return false;
  }
  return true;
}

bool GameState::start_skill_action(int global_action_id) {
  if (global_action_id < 0 ||
      global_action_id >= static_cast<int>(skill_actions.size())) {
    return false;
  }
  const auto& act = skill_actions[global_action_id];
  if (skill_level(act.skill) < act.req_level) {
    add_log(std::format("Requires {} Level {} to perform {}.",
                        skill_name(act.skill), act.req_level, act.name));
    return false;
  }
  if (!can_perform_action(global_action_id)) {
    std::string req_str;
    if (act.input_item_1 >= 0) {
      req_str = std::format("{}x {}", act.input_qty_1,
                            item_info[act.input_item_1].name);
    }
    if (act.input_item_2 >= 0) {
      req_str += std::format(" + {}x {}", act.input_qty_2,
                             item_info[act.input_item_2].name);
    }
    add_log(std::format("Missing ingredients for {}: need {}.", act.name,
                        req_str));
    return false;
  }

  active_type = ActiveActivityType::Skill;
  active_action_id = global_action_id;
  active_progress_ms = 0;
  active_target_ms = action_effective_interval_ms(global_action_id);
  status_banner = std::format("{} ({})", act.name, skill_name(act.skill));
  add_log(std::format("Started {} ({:.2f}s interval).", act.name,
                      active_target_ms / 1000.0));
  return true;
}

bool GameState::start_combat(int monster_id) {
  if (monster_id < 0 || monster_id >= MONSTER_COUNT) return false;
  const auto& mon = monster_info[monster_id];
  if (skill_level(SkillType::Slayer) < mon.slayer_req) {
    add_log(std::format("Requires Slayer Level {} to fight {}.",
                        mon.slayer_req, mon.name));
    return false;
  }
  active_type = ActiveActivityType::Combat;
  active_monster_id = monster_id;
  monster_hp = mon.max_hp;
  player_attack_timer_ms = 0;
  monster_attack_timer_ms = 0;
  status_banner = std::format("Fighting {} (Lv {}) in {}", mon.name,
                              mon.combat_level, mon.zone_name);
  add_log(std::format("Entered combat with {} ({} HP) in {}.", mon.name,
                      mon.max_hp, mon.zone_name));
  return true;
}

void GameState::stop_activity() {
  active_type = ActiveActivityType::None;
  active_progress_ms = 0;
  status_banner = "Idle — Select a Skill or Monster";
  add_log("Stopped current activity.");
}

void GameState::gain_xp(SkillType skill, long long amount) {
  if (amount <= 0) return;
  int idx = static_cast<int>(skill);
  int old_lvl = level_for_xp(xp[idx]);
  // Firemaking tier grants a small global XP bonus
  long long bonus = (amount * fire_tier * 2) / 100;
  xp[idx] += (amount + bonus);
  int new_lvl = level_for_xp(xp[idx]);
  if (new_lvl > old_lvl) {
    add_log(std::format("LEVEL UP! Your {} level is now {}!",
                        skill_name(skill), new_lvl));
    if (skill == SkillType::Hitpoints) {
      player_hp += (new_lvl - old_lvl) * 10;
      player_hp = std::min(player_hp, max_hp());
    }
  }
}

void GameState::complete_skill_action(int global_action_id) {
  if (!can_perform_action(global_action_id)) {
    add_log("Out of materials! Stopping action.");
    stop_activity();
    return;
  }
  const auto& act = skill_actions[global_action_id];
  int m_lvl = mastery_level(global_action_id);

  // Resource preservation chance for Artisan skills (5% + 0.2% per mastery level)
  bool preserved = false;
  if (act.input_item_1 >= 0 &&
      (act.skill == SkillType::Smithing || act.skill == SkillType::Cooking ||
       act.skill == SkillType::Firemaking)) {
    int pres_chance = 5 + (m_lvl / 5);
    if (rand_int(1, 100) <= pres_chance) {
      preserved = true;
    }
  }

  if (!preserved) {
    if (act.input_item_1 >= 0) remove_item(act.input_item_1, act.input_qty_1);
    if (act.input_item_2 >= 0) remove_item(act.input_item_2, act.input_qty_2);
  }

  // Gain Skill XP & Mastery XP
  gain_xp(act.skill, act.xp);
  action_mastery_xp[global_action_id] += std::max(10, act.xp / 2);

  // Double reward chance (5% + 0.3% per mastery level)
  int qty = act.product_qty;
  if (qty > 0 && rand_int(1, 100) <= (5 + m_lvl / 3)) {
    qty *= 2;
  }

  if (act.skill == SkillType::Cooking && act.product_item >= 0) {
    // Cooking success chance (75% base + mastery/level bonus up to 99%)
    int cook_chance =
        std::min(99, 74 + (skill_level(SkillType::Cooking) - act.req_level) / 2 +
                         m_lvl / 4);
    if (rand_int(1, 100) <= cook_chance) {
      add_item(act.product_item, qty, false);
      total_items_gathered += qty;
    } else {
      add_item(ITEM_BURNT_FISH, 1, false);
      add_log(std::format("You accidentally burnt the {}!",
                          item_info[act.input_item_1].name));
    }
  } else if (act.product_item >= 0 && qty > 0) {
    add_item(act.product_item, qty, false);
    total_items_gathered += qty;
  }

  // Bonus procs by skill
  if (act.skill == SkillType::Firemaking) {
    // 25% chance to receive Coal Ore from burning logs, plus small GP ash bonus
    if (rand_int(1, 100) <= 25) {
      add_item(ITEM_COAL_ORE, 1, false);
    }
    gp += 2 + act.req_level / 5;
    total_gp_earned += 2 + act.req_level / 5;
  } else if (act.skill == SkillType::Mining) {
    // 8% chance to find a random Gem while mining!
    if (rand_int(1, 100) <= 8) {
      int gem_id = ITEM_TOPAZ + rand_int(0, 4);
      if (add_item(gem_id, 1, false)) {
        add_log(std::format("While mining, you found a sparkling {}!",
                            item_info[gem_id].name));
      }
    }
  } else if (act.skill == SkillType::Fishing) {
    // 5% chance to fish up a sunken treasure chest (GP or Gem)
    if (rand_int(1, 100) <= 5) {
      int bonus_gp = 25 + act.req_level * 8;
      gp += bonus_gp;
      total_gp_earned += bonus_gp;
      add_log(std::format("You fished up a Sunken Treasure worth {}!",
                          money_string(bonus_gp)));
    }
  }

  // Stop if materials ran out after this action
  if (!can_perform_action(global_action_id)) {
    add_log(std::format("Finished {}: no more input materials remaining.",
                        act.name));
    stop_activity();
  }
}

void GameState::tick(int elapsed_ms) {
  if (elapsed_ms <= 0) return;
  total_ticks_ms += elapsed_ms;

  // Passive HP regeneration outside/inside combat (+1% max HP every 5 seconds)
  hp_regen_timer_ms += elapsed_ms;
  while (hp_regen_timer_ms >= 5000) {
    hp_regen_timer_ms -= 5000;
    if (player_hp < max_hp()) {
      int regen = std::max(1, max_hp() / 100);
      player_hp = std::min(max_hp(), player_hp + regen);
    }
  }

  if (active_type == ActiveActivityType::Skill && active_action_id >= 0) {
    active_target_ms = action_effective_interval_ms(active_action_id);
    active_progress_ms += elapsed_ms;
    while (active_type == ActiveActivityType::Skill &&
           active_progress_ms >= active_target_ms) {
      active_progress_ms -= active_target_ms;
      complete_skill_action(active_action_id);
      if (active_type == ActiveActivityType::Skill) {
        active_target_ms = action_effective_interval_ms(active_action_id);
      }
    }
  } else if (active_type == ActiveActivityType::Combat) {
    step_combat_tick(elapsed_ms);
  }

  history_timer_ms_ += elapsed_ms;
  if (history_timer_ms_ >= 5000) {
    history_timer_ms_ %= 5000;
    record_history_snapshot();
  }
}

void GameState::fast_forward_seconds(int seconds) {
  if (seconds <= 0) return;
  int remaining_ms = seconds * 1000;
  const int step = 250;
  while (remaining_ms > 0) {
    int dt = std::min(step, remaining_ms);
    tick(dt);
    remaining_ms -= dt;
  }
  record_history_snapshot();
}

void GameState::step_combat_tick(int elapsed_ms) {
  if (active_monster_id < 0 || active_monster_id >= MONSTER_COUNT) {
    stop_activity();
    return;
  }
  const auto& mon = monster_info[active_monster_id];
  int plr_interval = player_attack_interval_ms();

  player_attack_timer_ms += elapsed_ms;
  monster_attack_timer_ms += elapsed_ms;

  // Player attacks
  while (active_type == ActiveActivityType::Combat &&
         player_attack_timer_ms >= plr_interval) {
    player_attack_timer_ms -= plr_interval;
    if (rand_int(1, 100) <= player_hit_chance_pct(active_monster_id)) {
      int dmg = rand_int(std::max(1, player_max_hit() / 4), player_max_hit());
      dmg = std::min(dmg, monster_hp);
      monster_hp -= dmg;

      // Grant combat XP based on damage dealt
      long long c_xp = std::max(4, dmg / 2);
      if (attack_style == AttackStyle::Accurate) {
        gain_xp(SkillType::Attack, c_xp);
      } else if (attack_style == AttackStyle::Aggressive) {
        gain_xp(SkillType::Strength, c_xp);
      } else {
        gain_xp(SkillType::Defence, c_xp);
      }
      gain_xp(SkillType::Hitpoints, std::max(2LL, c_xp / 3));
    }

    if (monster_hp <= 0) {
      on_monster_defeated(active_monster_id);
      break;
    }
  }

  // Monster attacks
  while (active_type == ActiveActivityType::Combat &&
         monster_attack_timer_ms >= mon.attack_interval_ms) {
    monster_attack_timer_ms -= mon.attack_interval_ms;
    if (rand_int(1, 100) <= monster_hit_chance_pct(active_monster_id)) {
      int raw_dmg = rand_int(1, mon.max_hit);
      int dr = player_damage_reduction();
      int dmg = std::max(1, (raw_dmg * (100 - dr)) / 100);
      player_hp -= dmg;
      check_auto_eat();
      if (player_hp <= 0) {
        on_player_defeated();
        break;
      }
    }
  }
}

void GameState::on_monster_defeated(int monster_id) {
  const auto& mon = monster_info[monster_id];
  monster_kills[monster_id]++;
  total_monsters_killed++;

  int gp_drop = rand_int(mon.gp_min, mon.gp_max);
  gp += gp_drop;
  total_gp_earned += gp_drop;

  // Bonus XP on kill
  if (attack_style == AttackStyle::Accurate) {
    gain_xp(SkillType::Attack, mon.xp_reward);
  } else if (attack_style == AttackStyle::Aggressive) {
    gain_xp(SkillType::Strength, mon.xp_reward);
  } else {
    gain_xp(SkillType::Defence, mon.xp_reward);
  }
  gain_xp(SkillType::Hitpoints, mon.xp_reward / 3);

  // Slayer task check
  if (monster_id == slayer_task_monster_id && slayer_task_remaining > 0) {
    slayer_task_remaining--;
    gain_xp(SkillType::Slayer, mon.xp_reward / 2 + 15);
    int sc = std::max(5, mon.combat_level * 2);
    slayer_coins += sc;
    if (slayer_task_remaining <= 0) {
      slayer_tasks_completed++;
      int bonus_sc = 50 + slayer_tasks_completed * 15;
      slayer_coins += bonus_sc;
      gain_xp(SkillType::Slayer, 120 + mon.xp_reward);
      add_log(std::format(
          "SLAYER TASK COMPLETE! Earned +{} Slayer Coins! Assigning new task...",
          bonus_sc));
      assign_new_slayer_task();
    }
  } else if (mon.slayer_req > 1) {
    gain_xp(SkillType::Slayer, mon.xp_reward / 4);
  }

  // Roll monster drop table
  std::string loot_str;
  for (const auto& drop : mon.drops) {
    if (drop.item_id >= 0 && rand_int(1, 100) <= drop.chance_pct) {
      int q = rand_int(drop.min_qty, drop.max_qty);
      if (add_item(drop.item_id, q, false)) {
        if (!loot_str.empty()) loot_str += ", ";
        loot_str += std::format("{}x {}", q, item_info[drop.item_id].name);
      }
    }
  }

  if (loot_str.empty()) {
    add_log(std::format("Defeated {}! Looted {}.", mon.name,
                        money_string(gp_drop)));
  } else {
    add_log(std::format("Defeated {}! Looted {} and {}.", mon.name,
                        money_string(gp_drop), loot_str));
  }

  // Respawn monster
  monster_hp = mon.max_hp;
  player_attack_timer_ms = 0;
  monster_attack_timer_ms = 0;
}

void GameState::on_player_defeated() {
  player_deaths++;
  player_hp = max_hp();
  long long lost_gp = std::min(gp, std::max(10LL, gp / 10));
  gp -= lost_gp;
  add_log(std::format(
      "YOU DIED fighting {}! You respawned in town at full HP and dropped {}.",
      monster_info[active_monster_id].name, money_string(lost_gp)));
  stop_activity();
}

void GameState::assign_new_slayer_task() {
  std::vector<int> eligible;
  int s_lvl = skill_level(SkillType::Slayer);
  int c_lvl = combat_level();
  for (int i = 0; i < MONSTER_COUNT; ++i) {
    if (monster_info[i].slayer_req <= s_lvl &&
        monster_info[i].combat_level <= c_lvl + 15 &&
        !monster_info[i].is_boss) {
      eligible.push_back(i);
    }
  }
  if (eligible.empty()) eligible.push_back(0);
  slayer_task_monster_id = eligible[rand_int(0, static_cast<int>(eligible.size()) - 1)];
  slayer_task_remaining = rand_int(6, 15);
  add_log(std::format("New Slayer Task: Defeat {}x {} ({}).",
                      slayer_task_remaining,
                      monster_info[slayer_task_monster_id].name,
                      monster_info[slayer_task_monster_id].zone_name));
}

int GameState::item_qty(int item_id) const {
  for (const auto& s : bank) {
    if (s.item_id == item_id) return s.qty;
  }
  return 0;
}

int GameState::used_bank_slots() const {
  return static_cast<int>(bank.size());
}

long long GameState::total_bank_value() const {
  long long total = 0;
  for (const auto& s : bank) {
    if (s.item_id >= 0 && s.item_id < ITEM_COUNT) {
      total += static_cast<long long>(s.qty) * item_info[s.item_id].price;
    }
  }
  return total;
}

bool GameState::can_store_item(int item_id) const {
  for (const auto& s : bank) {
    if (s.item_id == item_id) return true;
  }
  return static_cast<int>(bank.size()) < bank_capacity;
}

bool GameState::add_item(int item_id, int qty, bool log_drop) {
  if (item_id < 0 || item_id >= ITEM_COUNT || qty <= 0) return false;
  // If this is the currently equipped food, top up food stack directly or bank
  for (auto& s : bank) {
    if (s.item_id == item_id) {
      s.qty += qty;
      if (log_drop) {
        add_log(std::format("Added {}x {} to Bank.", qty,
                            item_info[item_id].name));
      }
      return true;
    }
  }
  if (static_cast<int>(bank.size()) >= bank_capacity) {
    add_log(std::format("Bank is full ({}/{})! Could not store {}!",
                        bank.size(), bank_capacity, item_info[item_id].name));
    return false;
  }
  bank.push_back(BankSlot{item_id, qty});
  if (log_drop) {
    add_log(std::format("Added {}x {} to Bank.", qty, item_info[item_id].name));
  }
  return true;
}

bool GameState::remove_item(int item_id, int qty) {
  if (qty <= 0) return true;
  for (auto it = bank.begin(); it != bank.end(); ++it) {
    if (it->item_id == item_id) {
      if (it->qty < qty) return false;
      it->qty -= qty;
      if (it->qty == 0) bank.erase(it);
      return true;
    }
  }
  return false;
}

bool GameState::sell_item(int item_id, int qty) {
  if (item_id < 0 || item_id >= ITEM_COUNT || qty <= 0) return false;
  int have = item_qty(item_id);
  int sell_q = std::min(have, qty);
  if (sell_q <= 0) return false;
  long long value = static_cast<long long>(sell_q) * item_info[item_id].price;
  remove_item(item_id, sell_q);
  gp += value;
  total_gp_earned += value;
  add_log(std::format("Sold {}x {} for {}.", sell_q, item_info[item_id].name,
                      money_string(value)));
  return true;
}

long long GameState::sell_all_non_equipped() {
  long long gained = 0;
  int items_sold = 0;
  for (const auto& s : bank) {
    if (s.item_id >= 0 && s.item_id < ITEM_COUNT) {
      gained += static_cast<long long>(s.qty) * item_info[s.item_id].price;
      items_sold += s.qty;
    }
  }
  bank.clear();
  if (gained > 0) {
    gp += gained;
    total_gp_earned += gained;
    add_log(std::format("Sold all {} Bank items for {}!", items_sold,
                        money_string(gained)));
  }
  return gained;
}

bool GameState::equip_item(int item_id) {
  if (item_id < 0 || item_id >= ITEM_COUNT) return false;
  const auto& info = item_info[item_id];
  if (info.equip_slot == EquipSlot::None) {
    if (info.heal_amount > 0) {
      return equip_food(item_id);
    }
    return false;
  }

  SkillType req_skill = (info.equip_slot == EquipSlot::Weapon)
                            ? SkillType::Attack
                            : SkillType::Defence;
  if (skill_level(req_skill) < info.req_level) {
    add_log(std::format("Requires {} Level {} to equip {}.",
                        skill_name(req_skill), info.req_level, info.name));
    return false;
  }
  if (item_qty(item_id) <= 0) return false;

  int slot_idx = static_cast<int>(info.equip_slot);
  int old_item = equipped_items[slot_idx];
  remove_item(item_id, 1);
  if (old_item >= 0) {
    add_item(old_item, 1, false);
  }
  equipped_items[slot_idx] = item_id;
  add_log(std::format("Equipped {} in {} slot.", info.name,
                      equip_slot_name(info.equip_slot)));
  return true;
}

bool GameState::unequip_slot(EquipSlot slot) {
  int slot_idx = static_cast<int>(slot);
  if (slot_idx < 0 || slot_idx >= EQUIP_SLOT_COUNT) return false;
  int cur = equipped_items[slot_idx];
  if (cur < 0) return false;
  if (!can_store_item(cur)) {
    add_log("Bank is full! Cannot unequip item.");
    return false;
  }
  add_item(cur, 1, false);
  equipped_items[slot_idx] = -1;
  add_log(std::format("Unequipped {}.", item_info[cur].name));
  return true;
}

bool GameState::equip_food(int item_id) {
  if (item_id < 0 || item_id >= ITEM_COUNT) return false;
  if (item_info[item_id].heal_amount <= 0) return false;
  int have = item_qty(item_id);
  if (have <= 0) return false;

  if (equipped_food_item == item_id) {
    remove_item(item_id, have);
    equipped_food_qty += have;
    add_log(std::format("Added {}x {} to equipped food ({} total).", have,
                        item_info[item_id].name, equipped_food_qty));
    return true;
  }

  // Return old equipped food to bank if any
  if (equipped_food_item >= 0 && equipped_food_qty > 0) {
    if (!can_store_item(equipped_food_item)) {
      add_log("Bank is full! Cannot swap equipped food.");
      return false;
    }
    add_item(equipped_food_item, equipped_food_qty, false);
  }
  remove_item(item_id, have);
  equipped_food_item = item_id;
  equipped_food_qty = have;
  add_log(std::format("Equipped {}x {} (+{} HP each).", have,
                      item_info[item_id].name, item_info[item_id].heal_amount));
  return true;
}

bool GameState::eat_food() {
  if (equipped_food_item < 0 || equipped_food_qty <= 0) {
    add_log("No food equipped!");
    return false;
  }
  if (player_hp >= max_hp()) {
    add_log("You are already at full Hitpoints!");
    return false;
  }
  int heal = item_info[equipped_food_item].heal_amount;
  equipped_food_qty--;
  int before = player_hp;
  player_hp = std::min(max_hp(), player_hp + heal);
  add_log(std::format("Ate {} and restored +{} HP ({}/{} HP).",
                      item_info[equipped_food_item].name, player_hp - before,
                      player_hp, max_hp()));
  if (equipped_food_qty == 0) {
    equipped_food_item = -1;
  }
  return true;
}

void GameState::check_auto_eat() {
  if (auto_eat_tier <= 0) return;
  int threshold = auto_eat_threshold_hp();
  while (player_hp > 0 && player_hp <= threshold && equipped_food_item >= 0 &&
         equipped_food_qty > 0) {
    int heal = item_info[equipped_food_item].heal_amount;
    equipped_food_qty--;
    player_hp = std::min(max_hp(), player_hp + heal);
    if (equipped_food_qty == 0) {
      equipped_food_item = -1;
      break;
    }
  }
}

int GameState::next_bank_slot_cost() const {
  int extra = std::max(0, (bank_capacity - 24) / 4);
  return 150 + extra * extra * 120 + extra * 150;
}

bool GameState::buy_axe_upgrade() {
  if (axe_tier + 1 >= TOOL_TIER_COUNT) return false;
  const auto& upg = axe_upgrades[axe_tier + 1];
  if (skill_level(SkillType::Woodcutting) < upg.req_skill_level) {
    add_log(std::format("Requires Woodcutting Level {} to buy {}.",
                        upg.req_skill_level, upg.name));
    return false;
  }
  if (gp < upg.cost_gp) {
    add_log(std::format("Not enough GP for {} (need {}).", upg.name,
                        money_string(upg.cost_gp)));
    return false;
  }
  gp -= upg.cost_gp;
  axe_tier++;
  add_log(std::format("Purchased {} ({})!", upg.name, upg.description));
  return true;
}

bool GameState::buy_rod_upgrade() {
  if (rod_tier + 1 >= TOOL_TIER_COUNT) return false;
  const auto& upg = rod_upgrades[rod_tier + 1];
  if (skill_level(SkillType::Fishing) < upg.req_skill_level) {
    add_log(std::format("Requires Fishing Level {} to buy {}.",
                        upg.req_skill_level, upg.name));
    return false;
  }
  if (gp < upg.cost_gp) {
    add_log(std::format("Not enough GP for {} (need {}).", upg.name,
                        money_string(upg.cost_gp)));
    return false;
  }
  gp -= upg.cost_gp;
  rod_tier++;
  add_log(std::format("Purchased {} ({})!", upg.name, upg.description));
  return true;
}

bool GameState::buy_pickaxe_upgrade() {
  if (pickaxe_tier + 1 >= TOOL_TIER_COUNT) return false;
  const auto& upg = pickaxe_upgrades[pickaxe_tier + 1];
  if (skill_level(SkillType::Mining) < upg.req_skill_level) {
    add_log(std::format("Requires Mining Level {} to buy {}.",
                        upg.req_skill_level, upg.name));
    return false;
  }
  if (gp < upg.cost_gp) {
    add_log(std::format("Not enough GP for {} (need {}).", upg.name,
                        money_string(upg.cost_gp)));
    return false;
  }
  gp -= upg.cost_gp;
  pickaxe_tier++;
  add_log(std::format("Purchased {} ({})!", upg.name, upg.description));
  return true;
}

bool GameState::buy_fire_upgrade() {
  if (fire_tier + 1 >= TOOL_TIER_COUNT) return false;
  const auto& upg = fire_upgrades[fire_tier + 1];
  if (skill_level(SkillType::Firemaking) < upg.req_skill_level) {
    add_log(std::format("Requires Firemaking Level {} to buy {}.",
                        upg.req_skill_level, upg.name));
    return false;
  }
  if (gp < upg.cost_gp) {
    add_log(std::format("Not enough GP for {} (need {}).", upg.name,
                        money_string(upg.cost_gp)));
    return false;
  }
  gp -= upg.cost_gp;
  fire_tier++;
  add_log(std::format("Purchased {} ({})!", upg.name, upg.description));
  return true;
}

bool GameState::buy_auto_eat_upgrade() {
  if (auto_eat_tier + 1 >= AUTO_EAT_TIER_COUNT) return false;
  const auto& upg = auto_eat_upgrades[auto_eat_tier + 1];
  if (gp < upg.cost_gp) {
    add_log(std::format("Not enough GP for {} (need {}).", upg.name,
                        money_string(upg.cost_gp)));
    return false;
  }
  gp -= upg.cost_gp;
  auto_eat_tier++;
  add_log(std::format("Purchased {} ({})!", upg.name, upg.description));
  return true;
}

bool GameState::buy_bank_slot() {
  int cost = next_bank_slot_cost();
  if (gp < cost) {
    add_log(std::format("Not enough GP for +4 Bank Slots (need {}).",
                        money_string(cost)));
    return false;
  }
  gp -= cost;
  bank_capacity += 4;
  add_log(std::format("Purchased +4 Bank Slots! Bank capacity is now {}.",
                      bank_capacity));
  return true;
}

int GameState::combat_level() const {
  int atk = skill_level(SkillType::Attack);
  int str = skill_level(SkillType::Strength);
  int def = skill_level(SkillType::Defence);
  int hp = skill_level(SkillType::Hitpoints);
  double base = 0.25 * (def + hp);
  double melee = 0.325 * (atk + str);
  return std::max(3, static_cast<int>(std::floor(base + melee)));
}

int GameState::max_hp() const {
  return skill_level(SkillType::Hitpoints) * 10;
}

int GameState::player_attack_interval_ms() const {
  return 2400;
}

int GameState::player_max_hit() const {
  int str_lvl = skill_level(SkillType::Strength);
  if (attack_style == AttackStyle::Aggressive) str_lvl += 3;
  int str_bonus = 0;
  for (int id : equipped_items) {
    if (id >= 0 && id < ITEM_COUNT) str_bonus += item_info[id].strength_bonus;
  }
  return 12 + str_lvl * 3 + (str_bonus * (10 + str_lvl)) / 12;
}

int GameState::player_accuracy() const {
  int atk_lvl = skill_level(SkillType::Attack);
  if (attack_style == AttackStyle::Accurate) atk_lvl += 3;
  int atk_bonus = 0;
  for (int id : equipped_items) {
    if (id >= 0 && id < ITEM_COUNT) atk_bonus += item_info[id].attack_bonus;
  }
  return 25 + atk_lvl * 5 + atk_bonus * 3;
}

int GameState::player_evasion() const {
  int def_lvl = skill_level(SkillType::Defence);
  if (attack_style == AttackStyle::Defensive) def_lvl += 3;
  int def_bonus = 0;
  for (int id : equipped_items) {
    if (id >= 0 && id < ITEM_COUNT) def_bonus += item_info[id].defence_bonus;
  }
  return 20 + def_lvl * 5 + def_bonus * 3;
}

int GameState::player_damage_reduction() const {
  int dr = 0;
  for (int id : equipped_items) {
    if (id >= 0 && id < ITEM_COUNT) dr += item_info[id].damage_reduction;
  }
  return std::clamp(dr, 0, 75);
}

int GameState::player_hit_chance_pct(int monster_id) const {
  if (monster_id < 0 || monster_id >= MONSTER_COUNT) return 50;
  int acc = player_accuracy();
  int eva = monster_info[monster_id].evasion;
  int pct = (acc * 100) / std::max(1, acc + eva / 2);
  return std::clamp(pct, 15, 97);
}

int GameState::monster_hit_chance_pct(int monster_id) const {
  if (monster_id < 0 || monster_id >= MONSTER_COUNT) return 50;
  int acc = monster_info[monster_id].accuracy;
  int eva = player_evasion();
  int pct = (acc * 100) / std::max(1, acc + eva / 2);
  return std::clamp(pct, 10, 92);
}

int GameState::auto_eat_threshold_hp() const {
  if (auto_eat_tier <= 0 || auto_eat_tier >= AUTO_EAT_TIER_COUNT) return 0;
  int pct = auto_eat_upgrades[auto_eat_tier].speed_bonus_pct;
  return (max_hp() * pct) / 100;
}

std::string GameState::default_save_path() {
  const char* home = std::getenv("HOME");
  if (!home || std::string_view(home).empty()) {
    return "routineverse.save";
  }
  std::filesystem::path dir =
      std::filesystem::path(home) / ".config" / "Mizux";
  std::error_code ec;
  std::filesystem::create_directories(dir, ec);
  return (dir / "routineverse.save").string();
}

bool GameState::save_to_file(const std::string& path) const {
  std::ofstream out(path);
  if (!out.is_open()) return false;

  out << "ROUTINEVERSE_SAVE_V1\n";
  out << gp << " " << slayer_coins << " " << total_ticks_ms << "\n";
  for (int i = 0; i < SKILL_COUNT; ++i) {
    out << xp[i] << (i + 1 == SKILL_COUNT ? "\n" : " ");
  }
  out << action_mastery_xp.size() << "\n";
  for (size_t i = 0; i < action_mastery_xp.size(); ++i) {
    out << action_mastery_xp[i]
        << (i + 1 == action_mastery_xp.size() ? "\n" : " ");
  }
  out << bank_capacity << " " << bank.size() << "\n";
  for (const auto& s : bank) {
    out << s.item_id << " " << s.qty << "\n";
  }
  for (int i = 0; i < EQUIP_SLOT_COUNT; ++i) {
    out << equipped_items[i] << (i + 1 == EQUIP_SLOT_COUNT ? "\n" : " ");
  }
  out << equipped_food_item << " " << equipped_food_qty << "\n";
  out << axe_tier << " " << rod_tier << " " << pickaxe_tier << " " << fire_tier
      << " " << auto_eat_tier << "\n";
  out << static_cast<int>(active_type) << " " << active_action_id << " "
      << active_monster_id << " " << player_hp << " " << monster_hp << " "
      << static_cast<int>(attack_style) << "\n";
  out << slayer_task_monster_id << " " << slayer_task_remaining << " "
      << slayer_tasks_completed << "\n";
  out << total_items_gathered << " " << total_monsters_killed << " "
      << total_gp_earned << " " << player_deaths << "\n";
  return out.good();
}

bool GameState::load_from_file(const std::string& path) {
  std::ifstream in(path);
  if (!in.is_open()) return false;

  std::string header;
  if (!(in >> header) || header != "ROUTINEVERSE_SAVE_V1") return false;

  in >> gp >> slayer_coins >> total_ticks_ms;
  for (int i = 0; i < SKILL_COUNT; ++i) in >> xp[i];

  size_t m_sz = 0;
  in >> m_sz;
  action_mastery_xp.assign(skill_actions.size(), 0);
  for (size_t i = 0; i < m_sz; ++i) {
    long long val = 0;
    in >> val;
    if (i < action_mastery_xp.size()) action_mastery_xp[i] = val;
  }

  size_t b_sz = 0;
  in >> bank_capacity >> b_sz;
  bank.clear();
  for (size_t i = 0; i < b_sz; ++i) {
    BankSlot s{};
    in >> s.item_id >> s.qty;
    if (s.item_id >= 0 && s.item_id < ITEM_COUNT && s.qty > 0) {
      bank.push_back(s);
    }
  }

  for (int i = 0; i < EQUIP_SLOT_COUNT; ++i) in >> equipped_items[i];
  in >> equipped_food_item >> equipped_food_qty;
  in >> axe_tier >> rod_tier >> pickaxe_tier >> fire_tier >> auto_eat_tier;

  int act_t = 0;
  int style_t = 0;
  in >> act_t >> active_action_id >> active_monster_id >> player_hp >>
      monster_hp >> style_t;
  active_type = static_cast<ActiveActivityType>(std::clamp(act_t, 0, 2));
  attack_style = static_cast<AttackStyle>(std::clamp(style_t, 0, 2));

  in >> slayer_task_monster_id >> slayer_task_remaining >>
      slayer_tasks_completed;
  in >> total_items_gathered >> total_monsters_killed >> total_gp_earned >>
      player_deaths;

  if (active_type == ActiveActivityType::Skill && active_action_id >= 0 &&
      active_action_id < static_cast<int>(skill_actions.size())) {
    active_target_ms = action_effective_interval_ms(active_action_id);
    status_banner =
        std::format("{} ({})", skill_actions[active_action_id].name,
                    skill_name(skill_actions[active_action_id].skill));
  } else if (active_type == ActiveActivityType::Combat &&
             active_monster_id >= 0 && active_monster_id < MONSTER_COUNT) {
    status_banner =
        std::format("Fighting {}", monster_info[active_monster_id].name);
  } else {
    status_banner = "Idle — Select a Skill or Monster";
  }

  record_history_snapshot();
  add_log("Loaded saved game state.");
  return true;
}
