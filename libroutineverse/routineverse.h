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
  Farming = 2,
  Recycling = 3,
  SynthCook = 4,
  DeepMining = 5,
  Smithing = 6,
  CyberFab = 7,
  Attack = 8,
  Strength = 9,
  Defence = 10,
  Hitpoints = 11,
  Bounty = 12,
};

inline constexpr int SKILL_COUNT = 13;
inline constexpr int NON_COMBAT_SKILL_COUNT = 8;

enum class ItemCategory : int {
  Scrap = 0,
  RawMaterial,
  RawBiota,
  Crop,
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

enum class ItemId : int {
  None = -1,

  // Scrap & Tech Nodes (0..8)
  CopperWireScrap = 0,
  PlasteelShards = 1,
  CarbonNanotubes = 2,
  OpticFiberBundle = 3,
  PositronicRelays = 4,
  CryoCellCore = 5,
  PlasmaConduit = 6,
  QuantumNode = 7,
  AiMainframeCore = 8,

  // Recycled Basic / Raw Materials (9..17)
  CopperFilament = 9,
  PlasteelPolymer = 10,
  CarbonFiberWeave = 11,
  OpticSilicaGlass = 12,
  PositronicWafer = 13,
  CryoCoolantGel = 14,
  PlasmaCoil = 15,
  QuantumLattice = 16,
  NeuralMatrix = 17,

  // Raw Synth-Biota (18..26)
  RawKrillBiomass = 18,
  RawNeonEel = 19,
  RawSynthCarp = 20,
  RawChromeSalmon = 21,
  RawCyberLobster = 22,
  RawPlasmaRay = 23,
  RawApexShark = 24,
  RawLeviathanCell = 25,
  RawCyberKraken = 26,

  // Hydro-Farmed Crops & Synth-Noodles (27..36)
  HydroWheat = 27,
  SoyPods = 28,
  NeonScallion = 29,
  GlowNori = 30,
  BioBamboo = 31,
  CyberShiitake = 32,
  PlasmaChili = 33,
  ChronoLotus = 34,
  QuantumTruffle = 35,
  SynthNoodles = 36,

  // Synthesized Stims, Cyber-Ramen & Toxic Slag (37..56)
  KrillRation = 37,
  NeonEelSkewer = 38,
  SynthCarpPack = 39,
  ChromeSalmonStim = 40,
  CyberLobsterMeal = 41,
  PlasmaRayInfusion = 42,
  ApexSharkBooster = 43,
  LeviathanNanomed = 44,
  KrakenBioElixir = 45,
  ShoyuRamen = 46,
  ScallionRamen = 47,
  NoriRamen = 48,
  BambooRamen = 49,
  ShiitakeRamen = 50,
  PlasmaChiliRamen = 51,
  ChronoLotusRamen = 52,
  TruffleRamen = 53,
  QuantumKrakenRamen = 54,
  SynthProteinBar = 55,
  ToxicSlag = 56,

  // Deep-Mined Ores & Cells (57..67)
  CopperOre = 57,
  SiliconOre = 58,
  TitaniumOre = 59,
  CarbonCell = 60,
  SilverOre = 61,
  GoldOre = 62,
  CobaltOre = 63,
  TungstenOre = 64,
  NeutroniumOre = 65,
  ChronoOre = 66,
  QuantumOre = 67,

  // Refined Alloys & Conductors (68..77)
  ScrapAlloy = 68,
  TitaniumAlloy = 69,
  DurasteelAlloy = 70,
  SilverConductor = 71,
  GoldSuperconductor = 72,
  CobaltAlloy = 73,
  TungstenAlloy = 74,
  NeutroniumAlloy = 75,
  ChronoAlloy = 76,
  QuantumAlloy = 77,

  // Data Crystals (78..82)
  AmberDatachip = 78,
  SapphireCortex = 79,
  RubyLaserCore = 80,
  EmeraldCryptokey = 81,
  QuantumDiamond = 82,

  // Weapons - Mono-Blades (83..90)
  ScrapBlade = 83,
  TitaniumBlade = 84,
  DurasteelBlade = 85,
  CobaltBlade = 86,
  TungstenBlade = 87,
  NeutroniumBlade = 88,
  ChronoBlade = 89,
  QuantumBlade = 90,

  // Visors (91..98)
  ScrapVisor = 91,
  TitaniumVisor = 92,
  DurasteelVisor = 93,
  CobaltVisor = 94,
  TungstenVisor = 95,
  NeutroniumVisor = 96,
  ChronoVisor = 97,
  QuantumVisor = 98,

  // Exo-Suits (99..106)
  ScrapExoSuit = 99,
  TitaniumExoSuit = 100,
  DurasteelExoSuit = 101,
  CobaltExoSuit = 102,
  TungstenExoSuit = 103,
  NeutroniumExoSuit = 104,
  ChronoExoSuit = 105,
  QuantumExoSuit = 106,

  // Holo-Shields (107..114)
  ScrapShield = 107,
  TitaniumShield = 108,
  DurasteelShield = 109,
  CobaltShield = 110,
  TungstenShield = 111,
  NeutroniumShield = 112,
  ChronoShield = 113,
  QuantumShield = 114,

  // Enemy Salvage Loot (115..119)
  ServoParts = 115,
  HeavyChassis = 116,
  ApexCyberCore = 117,
  Microchip = 118,
  SynthWeaveHide = 119,
};

inline constexpr int ITEM_COUNT = 120;

struct ItemInfo {
  ItemId id;
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
  ItemId product_item;
  int product_qty;
  ItemId input_item_1;
  int input_qty_1;
  ItemId input_item_2;
  int input_qty_2;
};

struct MonsterDrop {
  ItemId item_id;
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
  ItemId item_id = ItemId::None;
  int qty = 0;
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

inline constexpr bool is_valid_item(ItemId id) {
  int idx = static_cast<int>(id);
  return idx >= 0 && idx < ITEM_COUNT;
}

inline const ItemInfo& get_item_info(ItemId id) {
  return item_info[static_cast<int>(id)];
}

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
  int item_qty(ItemId item_id) const;
  int used_bank_slots() const;
  long long total_bank_value() const;
  bool can_store_item(ItemId item_id) const;
  bool add_item(ItemId item_id, int qty, bool log_drop = false);
  bool remove_item(ItemId item_id, int qty);
  bool sell_item(ItemId item_id, int qty);
  long long sell_all_non_equipped();

  // Equipment & Stims
  bool equip_item(ItemId item_id);
  bool unequip_slot(EquipSlot slot);
  bool equip_food(ItemId item_id);
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

  std::array<ItemId, EQUIP_SLOT_COUNT> equipped_items{
      ItemId::None, ItemId::None, ItemId::None, ItemId::None};
  ItemId equipped_food_item = ItemId::None;
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
