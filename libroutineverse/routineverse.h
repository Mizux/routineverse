#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

inline constexpr int MAX_HISTORY_POINTS = 60;
inline constexpr int MAX_SKILL_LEVEL = 99;

enum class SkillType : int {
  Salvaging = 0,
  BioHarvest = 1,
  Overclock = 2,
  SynthCook = 3,
  DeepMining = 4,
  CyberFab = 5,
  Attack = 6,
  Strength = 7,
  Defence = 8,
  Hitpoints = 9,
  Bounty = 10,
};

inline constexpr int SKILL_COUNT = 11;
inline constexpr int NON_COMBAT_SKILL_COUNT = 6;

enum class ItemCategory : int {
  Scrap = 0,
  RawBiota,
  StimFood,
  ToxicWaste,
  RawOre,
  Alloy,
  DataCrystal,
  Weapon,
  Visor,
  ExoSuit,
  HoloShield,
  CyberLoot,
};

enum class EquipSlot : int {
  None = -1,
  Weapon = 0,
  Visor = 1,
  ExoSuit = 2,
  HoloShield = 3,
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
  int heal_amount;       // > 0 if usable stim/ration
  EquipSlot equip_slot;  // Weapon, Visor, ExoSuit, HoloShield, or None
  int req_level;         // Attack level for weapons, Defence level for cyber-armor
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
  int credits_min;
  int credits_max;
  int bounty_req;
  bool is_boss;
  std::array<MonsterDrop, 3> drops;
};

struct ShopUpgradeInfo {
  int tier;
  const char* name;
  const char* description;
  int req_skill_level;
  int cost_credits;
  int speed_bonus_pct;
};

struct BankSlot {
  int item_id = -1;
  int qty = 0;
};

// Item ID constants
enum ItemId : int {
  // Scrap & Tech Nodes (0..8)
  ITEM_COPPER_WIRE_SCRAP = 0,
  ITEM_PLASTEEL_SHARDS = 1,
  ITEM_CARBON_NANOTUBES = 2,
  ITEM_OPTIC_FIBER_BUNDLE = 3,
  ITEM_POSITRONIC_RELAYS = 4,
  ITEM_CRYO_CELL_CORE = 5,
  ITEM_PLASMA_CONDUIT = 6,
  ITEM_QUANTUM_NODE = 7,
  ITEM_AI_MAINFRAME_CORE = 8,

  // Raw Synth-Biota (9..16)
  ITEM_RAW_KRILL_BIOMASS = 9,
  ITEM_RAW_NEON_EEL = 10,
  ITEM_RAW_SYNTH_CARP = 11,
  ITEM_RAW_CHROME_SALMON = 12,
  ITEM_RAW_CYBER_LOBSTER = 13,
  ITEM_RAW_PLASMA_RAY = 14,
  ITEM_RAW_APEX_SHARK = 15,
  ITEM_RAW_LEVIATHAN_CELL = 16,

  // Synthesized Stims / Rations & Toxic Slag (17..26)
  ITEM_KRILL_RATION = 17,
  ITEM_NEON_EEL_SKEWER = 18,
  ITEM_SYNTH_CARP_PACK = 19,
  ITEM_CHROME_SALMON_STIM = 20,
  ITEM_CYBER_LOBSTER_MEAL = 21,
  ITEM_PLASMA_RAY_INFUSION = 22,
  ITEM_APEX_SHARK_BOOSTER = 23,
  ITEM_LEVIATHAN_NANOMED = 24,
  ITEM_SYNTH_PROTEIN_BAR = 25,
  ITEM_TOXIC_SLAG = 26,

  // Deep-Mined Ores & Cells (27..36)
  ITEM_COPPER_ORE = 27,
  ITEM_SILICON_ORE = 28,
  ITEM_TITANIUM_ORE = 29,
  ITEM_CARBON_CELL = 30,
  ITEM_SILVER_ORE = 31,
  ITEM_GOLD_ORE = 32,
  ITEM_COBALT_ORE = 33,
  ITEM_TUNGSTEN_ORE = 34,
  ITEM_NEUTRONIUM_ORE = 35,
  ITEM_CHRONO_ORE = 36,

  // Refined Alloys & Conductors (37..45)
  ITEM_SCRAP_ALLOY = 37,
  ITEM_TITANIUM_ALLOY = 38,
  ITEM_DURASTEEL_ALLOY = 39,
  ITEM_SILVER_CONDUCTOR = 40,
  ITEM_GOLD_SUPERCONDUCTOR = 41,
  ITEM_COBALT_ALLOY = 42,
  ITEM_TUNGSTEN_ALLOY = 43,
  ITEM_NEUTRONIUM_ALLOY = 44,
  ITEM_CHRONO_ALLOY = 45,

  // Data Crystals (46..50)
  ITEM_AMBER_DATACHIP = 46,
  ITEM_SAPPHIRE_CORTEX = 47,
  ITEM_RUBY_LASER_CORE = 48,
  ITEM_EMERALD_CRYPTOKEY = 49,
  ITEM_QUANTUM_DIAMOND = 50,

  // Weapons - Mono-Blades (51..57)
  ITEM_SCRAP_BLADE = 51,
  ITEM_TITANIUM_BLADE = 52,
  ITEM_DURASTEEL_BLADE = 53,
  ITEM_COBALT_BLADE = 54,
  ITEM_TUNGSTEN_BLADE = 55,
  ITEM_NEUTRONIUM_BLADE = 56,
  ITEM_CHRONO_BLADE = 57,

  // Visors (58..64)
  ITEM_SCRAP_VISOR = 58,
  ITEM_TITANIUM_VISOR = 59,
  ITEM_DURASTEEL_VISOR = 60,
  ITEM_COBALT_VISOR = 61,
  ITEM_TUNGSTEN_VISOR = 62,
  ITEM_NEUTRONIUM_VISOR = 63,
  ITEM_CHRONO_VISOR = 64,

  // Exo-Suits (65..71)
  ITEM_SCRAP_EXOSUIT = 65,
  ITEM_TITANIUM_EXOSUIT = 66,
  ITEM_DURASTEEL_EXOSUIT = 67,
  ITEM_COBALT_EXOSUIT = 68,
  ITEM_TUNGSTEN_EXOSUIT = 69,
  ITEM_NEUTRONIUM_EXOSUIT = 70,
  ITEM_CHRONO_EXOSUIT = 71,

  // Holo-Shields (72..78)
  ITEM_SCRAP_SHIELD = 72,
  ITEM_TITANIUM_SHIELD = 73,
  ITEM_DURASTEEL_SHIELD = 74,
  ITEM_COBALT_SHIELD = 75,
  ITEM_TUNGSTEN_SHIELD = 76,
  ITEM_NEUTRONIUM_SHIELD = 77,
  ITEM_CHRONO_SHIELD = 78,

  // Enemy Salvage Loot (79..83)
  ITEM_SERVO_PARTS = 79,
  ITEM_HEAVY_CHASSIS = 80,
  ITEM_APEX_CYBER_CORE = 81,
  ITEM_MICROCHIP = 82,
  ITEM_SYNTH_WEAVE_HIDE = 83,

  ITEM_COUNT = 84
};

inline constexpr int MONSTER_COUNT = 12;
inline constexpr int TOOL_TIER_COUNT = 7;
inline constexpr int AUTO_STIM_TIER_COUNT = 4;

extern const std::array<ItemInfo, ITEM_COUNT> item_info;
extern const std::vector<SkillAction> skill_actions;
extern const std::array<MonsterInfo, MONSTER_COUNT> monster_info;
extern const std::array<ShopUpgradeInfo, TOOL_TIER_COUNT> cutter_upgrades;
extern const std::array<ShopUpgradeInfo, TOOL_TIER_COUNT> harvester_upgrades;
extern const std::array<ShopUpgradeInfo, TOOL_TIER_COUNT> drill_upgrades;
extern const std::array<ShopUpgradeInfo, TOOL_TIER_COUNT> reactor_upgrades;
extern const std::array<ShopUpgradeInfo, AUTO_STIM_TIER_COUNT> auto_stim_upgrades;

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

  // Equipment & Stims
  bool equip_item(int item_id);
  bool unequip_slot(EquipSlot slot);
  bool equip_food(int item_id);
  bool eat_food();
  void check_auto_eat();

  // Bounty Contracts
  void assign_new_bounty_contract();

  // Black Market / Cyber-Shop upgrades
  int next_bank_slot_cost() const;
  bool buy_cutter_upgrade();
  bool buy_harvester_upgrade();
  bool buy_drill_upgrade();
  bool buy_reactor_upgrade();
  bool buy_auto_stim_upgrade();
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
  long long credits = 250;
  long long bounty_tokens = 0;
  long long total_ticks_ms = 0;

  std::array<long long, SKILL_COUNT> xp{};
  std::vector<long long> action_mastery_xp;

  int bank_capacity = 24;
  std::vector<BankSlot> bank;

  std::array<int, EQUIP_SLOT_COUNT> equipped_items{-1, -1, -1, -1};
  int equipped_food_item = -1;
  int equipped_food_qty = 0;

  // Cyber-Shop upgrade tiers
  int cutter_tier = 0;
  int harvester_tier = 0;
  int drill_tier = 0;
  int reactor_tier = 0;
  int auto_stim_tier = 0;

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

  // Bounty contract
  int bounty_target_id = 0;
  int bounty_remaining = 10;
  int bounties_completed = 0;

  // Statistics
  std::array<int, MONSTER_COUNT> monster_kills{};
  long long total_items_gathered = 0;
  long long total_monsters_killed = 0;
  long long total_credits_earned = 250;
  int player_deaths = 0;

  bool sound_enabled = false;
  std::string status_banner;
  std::vector<std::string> game_log;

  // History for Charts
  std::vector<long long> credits_history;
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
