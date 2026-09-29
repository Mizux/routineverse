#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

inline constexpr int MAX_HISTORY_POINTS = 60;
inline constexpr int MAX_SKILL_LEVEL = 99;

enum class SkillType : int {
  Woodcutting = 0,
  Fishing = 1,
  Firemaking = 2,
  Cooking = 3,
  Mining = 4,
  Smithing = 5,
  Attack = 6,
  Strength = 7,
  Defence = 8,
  Hitpoints = 9,
  Slayer = 10,
};

inline constexpr int SKILL_COUNT = 11;
inline constexpr int NON_COMBAT_SKILL_COUNT = 6;

enum class ItemCategory : int {
  Logs = 0,
  RawFish,
  CookedFood,
  BurntFood,
  Ore,
  Bar,
  Gem,
  Weapon,
  Helmet,
  Platebody,
  Shield,
  Loot,
};

enum class EquipSlot : int {
  None = -1,
  Weapon = 0,
  Helmet = 1,
  Platebody = 2,
  Shield = 3,
};

inline constexpr int EQUIP_SLOT_COUNT = 4;

enum class AttackStyle : int {
  Accurate = 0,    // Trains Attack (+accuracy)
  Aggressive = 1,  // Trains Strength (+max hit)
  Defensive = 2,   // Trains Defence (+evasion)
};

enum class ActiveActivityType : int {
  None = 0,
  Skill = 1,
  Combat = 2,
};

struct ItemInfo {
  int id;
  const char* name;
  ItemCategory category;
  int price;
  int heal_amount;       // > 0 if edible food
  EquipSlot equip_slot;  // Weapon, Helmet, Platebody, Shield, or None
  int req_level;         // Attack level for weapons, Defence level for armor
  int attack_bonus;      // Accuracy bonus
  int strength_bonus;    // Max hit bonus
  int defence_bonus;     // Evasion bonus
  int damage_reduction;  // Damage reduction %
};

struct SkillAction {
  int id;
  SkillType skill;
  const char* name;
  int req_level;
  int base_interval_ms;
  int xp;
  int product_item;
  int product_qty;
  int input_item_1;
  int input_qty_1;
  int input_item_2;
  int input_qty_2;
};

struct MonsterDrop {
  int item_id;
  int chance_pct;  // 1..100
  int min_qty;
  int max_qty;
};

struct MonsterInfo {
  int id;
  const char* name;
  const char* zone_name;
  int combat_level;
  int max_hp;
  int attack_interval_ms;
  int max_hit;
  int accuracy;
  int evasion;
  int xp_reward;
  int gp_min;
  int gp_max;
  int slayer_req;
  bool is_boss;
  std::array<MonsterDrop, 3> drops;
};

struct ShopUpgradeInfo {
  int tier;
  const char* name;
  const char* description;
  int req_skill_level;
  int cost_gp;
  int speed_bonus_pct;
};

struct BankSlot {
  int item_id = -1;
  int qty = 0;
};

// Item ID constants
enum ItemId : int {
  // Logs (0..8)
  ITEM_NORMAL_LOGS = 0,
  ITEM_OAK_LOGS = 1,
  ITEM_WILLOW_LOGS = 2,
  ITEM_TEAK_LOGS = 3,
  ITEM_MAPLE_LOGS = 4,
  ITEM_MAHOGANY_LOGS = 5,
  ITEM_YEW_LOGS = 6,
  ITEM_MAGIC_LOGS = 7,
  ITEM_REDWOOD_LOGS = 8,

  // Raw Fish (9..16)
  ITEM_RAW_SHRIMP = 9,
  ITEM_RAW_SARDINE = 10,
  ITEM_RAW_TROUT = 11,
  ITEM_RAW_SALMON = 12,
  ITEM_RAW_LOBSTER = 13,
  ITEM_RAW_SWORDFISH = 14,
  ITEM_RAW_SHARK = 15,
  ITEM_RAW_WHALE = 16,

  // Cooked Food & Burnt (17..26)
  ITEM_COOKED_SHRIMP = 17,
  ITEM_COOKED_SARDINE = 18,
  ITEM_COOKED_TROUT = 19,
  ITEM_COOKED_SALMON = 20,
  ITEM_COOKED_LOBSTER = 21,
  ITEM_COOKED_SWORDFISH = 22,
  ITEM_COOKED_SHARK = 23,
  ITEM_COOKED_WHALE = 24,
  ITEM_COOKED_BEEF = 25,
  ITEM_BURNT_FISH = 26,

  // Ores (27..36)
  ITEM_COPPER_ORE = 27,
  ITEM_TIN_ORE = 28,
  ITEM_IRON_ORE = 29,
  ITEM_COAL_ORE = 30,
  ITEM_SILVER_ORE = 31,
  ITEM_GOLD_ORE = 32,
  ITEM_MITHRIL_ORE = 33,
  ITEM_ADAMANTITE_ORE = 34,
  ITEM_RUNITE_ORE = 35,
  ITEM_DRAGONITE_ORE = 36,

  // Bars (37..45)
  ITEM_BRONZE_BAR = 37,
  ITEM_IRON_BAR = 38,
  ITEM_STEEL_BAR = 39,
  ITEM_SILVER_BAR = 40,
  ITEM_GOLD_BAR = 41,
  ITEM_MITHRIL_BAR = 42,
  ITEM_ADAMANT_BAR = 43,
  ITEM_RUNE_BAR = 44,
  ITEM_DRAGON_BAR = 45,

  // Gems & Special (46..50)
  ITEM_TOPAZ = 46,
  ITEM_SAPPHIRE = 47,
  ITEM_RUBY = 48,
  ITEM_EMERALD = 49,
  ITEM_DIAMOND = 50,

  // Weapons (51..57)
  ITEM_BRONZE_SCIMITAR = 51,
  ITEM_IRON_SCIMITAR = 52,
  ITEM_STEEL_SCIMITAR = 53,
  ITEM_MITHRIL_SCIMITAR = 54,
  ITEM_ADAMANT_SCIMITAR = 55,
  ITEM_RUNE_SCIMITAR = 56,
  ITEM_DRAGON_SCIMITAR = 57,

  // Helmets (58..64)
  ITEM_BRONZE_HELMET = 58,
  ITEM_IRON_HELMET = 59,
  ITEM_STEEL_HELMET = 60,
  ITEM_MITHRIL_HELMET = 61,
  ITEM_ADAMANT_HELMET = 62,
  ITEM_RUNE_HELMET = 63,
  ITEM_DRAGON_HELMET = 64,

  // Platebodies (65..71)
  ITEM_BRONZE_PLATEBODY = 65,
  ITEM_IRON_PLATEBODY = 66,
  ITEM_STEEL_PLATEBODY = 67,
  ITEM_MITHRIL_PLATEBODY = 68,
  ITEM_ADAMANT_PLATEBODY = 69,
  ITEM_RUNE_PLATEBODY = 70,
  ITEM_DRAGON_PLATEBODY = 71,

  // Shields (72..78)
  ITEM_BRONZE_SHIELD = 72,
  ITEM_IRON_SHIELD = 73,
  ITEM_STEEL_SHIELD = 74,
  ITEM_MITHRIL_SHIELD = 75,
  ITEM_ADAMANT_SHIELD = 76,
  ITEM_RUNE_SHIELD = 77,
  ITEM_DRAGON_SHIELD = 78,

  // Monster Loot (79..83)
  ITEM_BONES = 79,
  ITEM_BIG_BONES = 80,
  ITEM_DRAGON_BONES = 81,
  ITEM_FEATHER = 82,
  ITEM_LEATHER = 83,

  ITEM_COUNT = 84
};

inline constexpr int MONSTER_COUNT = 12;
inline constexpr int TOOL_TIER_COUNT = 7;
inline constexpr int AUTO_EAT_TIER_COUNT = 4;

extern const std::array<ItemInfo, ITEM_COUNT> item_info;
extern const std::vector<SkillAction> skill_actions;
extern const std::array<MonsterInfo, MONSTER_COUNT> monster_info;
extern const std::array<ShopUpgradeInfo, TOOL_TIER_COUNT> axe_upgrades;
extern const std::array<ShopUpgradeInfo, TOOL_TIER_COUNT> rod_upgrades;
extern const std::array<ShopUpgradeInfo, TOOL_TIER_COUNT> pickaxe_upgrades;
extern const std::array<ShopUpgradeInfo, TOOL_TIER_COUNT> fire_upgrades;
extern const std::array<ShopUpgradeInfo, AUTO_EAT_TIER_COUNT> auto_eat_upgrades;

// Utility & Formatting functions
std::string skill_name(SkillType skill);
std::string skill_short_name(SkillType skill);
std::string item_category_name(ItemCategory cat);
std::string equip_slot_name(EquipSlot slot);
std::string attack_style_name(AttackStyle style);
std::string money_string(long long value);
std::string number_string(long long value);

int xp_for_level(int level);
int level_for_xp(long long xp);
double level_progress_ratio(long long xp);

std::vector<int> actions_for_skill(SkillType skill);

class GameState {
 public:
  GameState();

  void new_game();

  // Simulation tick
  void tick(int elapsed_ms);
  void fast_forward_seconds(int seconds);

  // Skill & Combat control
  bool start_skill_action(int global_action_id);
  bool start_combat(int monster_id);
  void stop_activity();

  // Inventory / Bank management
  int item_qty(int item_id) const;
  int used_bank_slots() const;
  long long total_bank_value() const;
  bool can_store_item(int item_id) const;
  bool add_item(int item_id, int qty, bool log_drop = false);
  bool remove_item(int item_id, int qty);
  bool sell_item(int item_id, int qty);
  long long sell_all_non_equipped();

  // Equipment & Food
  bool equip_item(int item_id);
  bool unequip_slot(EquipSlot slot);
  bool equip_food(int item_id);
  bool eat_food();
  void check_auto_eat();

  // Slayer
  void assign_new_slayer_task();

  // Shop upgrades
  int next_bank_slot_cost() const;
  bool buy_axe_upgrade();
  bool buy_rod_upgrade();
  bool buy_pickaxe_upgrade();
  bool buy_fire_upgrade();
  bool buy_auto_eat_upgrade();
  bool buy_bank_slot();

  // Computed Stats
  int skill_level(SkillType skill) const;
  long long skill_xp(SkillType skill) const;
  int total_skill_level() const;
  long long total_skill_xp() const;
  int mastery_level(int global_action_id) const;

  int action_effective_interval_ms(int global_action_id) const;
  bool can_perform_action(int global_action_id) const;

  int combat_level() const;
  int max_hp() const;
  int player_attack_interval_ms() const;
  int player_max_hit() const;
  int player_accuracy() const;
  int player_evasion() const;
  int player_damage_reduction() const;
  int player_hit_chance_pct(int monster_id) const;
  int monster_hit_chance_pct(int monster_id) const;
  int auto_eat_threshold_hp() const;

  // History & Logging
  void record_history_snapshot();
  void add_log(const std::string& entry);

  // Save / Load
  static std::string default_save_path();
  bool save_to_file(const std::string& path = default_save_path()) const;
  bool load_from_file(const std::string& path = default_save_path());

  // Persistent Game State
  long long gp = 250;
  long long slayer_coins = 0;
  long long total_ticks_ms = 0;

  std::array<long long, SKILL_COUNT> xp{};
  std::vector<long long> action_mastery_xp;

  int bank_capacity = 24;
  std::vector<BankSlot> bank;

  std::array<int, EQUIP_SLOT_COUNT> equipped_items{-1, -1, -1, -1};
  int equipped_food_item = -1;
  int equipped_food_qty = 0;

  // Shop upgrade tiers
  int axe_tier = 0;
  int rod_tier = 0;
  int pickaxe_tier = 0;
  int fire_tier = 0;
  int auto_eat_tier = 0;

  // Active activity state
  ActiveActivityType active_type = ActiveActivityType::None;
  int active_action_id = -1;
  int active_progress_ms = 0;
  int active_target_ms = 2000;

  // Combat state
  AttackStyle attack_style = AttackStyle::Accurate;
  int player_hp = 100;
  int active_monster_id = 0;
  int monster_hp = 30;
  int player_attack_timer_ms = 0;
  int monster_attack_timer_ms = 0;
  int hp_regen_timer_ms = 0;

  // Slayer task
  int slayer_task_monster_id = 0;
  int slayer_task_remaining = 10;
  int slayer_tasks_completed = 0;

  // Statistics
  std::array<int, MONSTER_COUNT> monster_kills{};
  long long total_items_gathered = 0;
  long long total_monsters_killed = 0;
  long long total_gp_earned = 250;
  int player_deaths = 0;

  bool sound_enabled = false;
  std::string status_banner;
  std::vector<std::string> game_log;

  // History for Charts
  std::vector<long long> gp_history;
  std::vector<long long> bank_value_history;
  std::vector<int> total_level_history;
  std::vector<long long> total_xp_history;
  std::vector<int> hp_history;
  std::array<std::vector<long long>, SKILL_COUNT> skill_xp_history{};

 private:
  void complete_skill_action(int global_action_id);
  void step_combat_tick(int elapsed_ms);
  void on_monster_defeated(int monster_id);
  void on_player_defeated();
  void gain_xp(SkillType skill, long long amount);

  int history_timer_ms_ = 0;
};
