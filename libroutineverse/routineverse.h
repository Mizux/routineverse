#pragma once

#include <sys/types.h>

#include <array>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

inline constexpr uint8_t MAX_HISTORY_POINTS = 60;
inline constexpr uint8_t MAX_SKILL_LEVEL = 99;

enum class SkillType : uint8_t {
  Salvaging,
  BioHarvest,
  Farming,
  Recycling,
  SynthCook,
  DeepMining,
  Smithing,
  CyberFab,
  Attack,
  Strength,
  Defence,
  Hitpoints,
  Bounty,
};

inline constexpr ssize_t SKILL_COUNT = 13;
inline constexpr ssize_t NON_COMBAT_SKILL_COUNT = 8;

enum class ItemCategory : uint8_t {
  Scrap,
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

enum class EquipSlot : int8_t {
  None = -1,
  Weapon,
  Visor,
  ExoSuit,
  HoloShield,
};

enum class CombatStyle : uint8_t {
  Accurate,    // Trains Attack (+accuracy)
  Aggressive,  // Trains Strength (+max hit)
  Defensive,   // Trains Defence (+evasion)
};

enum class ActiveActivityType : uint8_t {
  None,
  Skill,
  Combat,
};

enum class ItemId : uint16_t {
  None,
  // Scrap & Tech Nodes
  CopperWireScrap,
  PlasteelShards,
  CarbonNanotubes,
  OpticFiberBundle,
  PositronicRelays,
  CryoCellCore,
  PlasmaConduit,
  QuantumNode,
  AiMainframeCore,

  // Recycled Basic / Raw Materials
  CopperFilament,
  PlasteelPolymer,
  CarbonFiberWeave,
  OpticSilicaGlass,
  PositronicWafer,
  CryoCoolantGel,
  PlasmaCoil,
  QuantumLattice,
  NeuralMatrix,

  // Raw Synth-Biota
  RawKrillBiomass,
  RawNeonEel,
  RawSynthCarp,
  RawChromeSalmon,
  RawCyberLobster,
  RawPlasmaRay,
  RawApexShark,
  RawLeviathanCell,
  RawCyberKraken,

  // Hydro-Farmed Crops & Synth-Noodles
  HydroWheat,
  SoyPods,
  NeonScallion,
  GlowNori,
  BioBamboo,
  CyberShiitake,
  PlasmaChili,
  ChronoLotus,
  QuantumTruffle,
  SynthNoodles,

  // Synthesized Stims, Cyber-Ramen & Toxic Slag
  KrillRation,
  NeonEelSkewer,
  SynthCarpPack,
  ChromeSalmonStim,
  CyberLobsterMeal,
  PlasmaRayInfusion,
  ApexSharkBooster,
  LeviathanNanomed,
  KrakenBioElixir,
  ShoyuRamen,
  ScallionRamen,
  NoriRamen,
  BambooRamen,
  ShiitakeRamen,
  PlasmaChiliRamen,
  ChronoLotusRamen,
  TruffleRamen,
  QuantumKrakenRamen,
  SynthProteinBar,
  ToxicSlag,

  // Deep-Mined Ores & Cells
  CopperOre,
  SiliconOre,
  TitaniumOre,
  CarbonCell,
  SilverOre,
  GoldOre,
  CobaltOre,
  TungstenOre,
  NeutroniumOre,
  ChronoOre,
  QuantumOre,

  // Refined Alloys & Conductors
  ScrapAlloy,
  TitaniumAlloy,
  DurasteelAlloy,
  SilverConductor,
  GoldSuperconductor,
  CobaltAlloy,
  TungstenAlloy,
  NeutroniumAlloy,
  ChronoAlloy,
  QuantumAlloy,

  // Data Crystals
  AmberDatachip,
  SapphireCortex,
  RubyLaserCore,
  EmeraldCryptokey,
  QuantumDiamond,

  // Weapons - Mono-Blades
  ScrapBlade,
  TitaniumBlade,
  DurasteelBlade,
  CobaltBlade,
  TungstenBlade,
  NeutroniumBlade,
  ChronoBlade,
  QuantumBlade,

  // Visors
  ScrapVisor,
  TitaniumVisor,
  DurasteelVisor,
  CobaltVisor,
  TungstenVisor,
  NeutroniumVisor,
  ChronoVisor,
  QuantumVisor,

  // Exo-Suits
  ScrapExoSuit,
  TitaniumExoSuit,
  DurasteelExoSuit,
  CobaltExoSuit,
  TungstenExoSuit,
  NeutroniumExoSuit,
  ChronoExoSuit,
  QuantumExoSuit,

  // Holo-Shields
  ScrapShield,
  TitaniumShield,
  DurasteelShield,
  CobaltShield,
  TungstenShield,
  NeutroniumShield,
  ChronoShield,
  QuantumShield,

  // Enemy Salvage Loot
  ServoParts,
  HeavyChassis,
  ApexCyberCore,
  Microchip,
  SynthWeaveHide,
};

inline constexpr ssize_t ITEM_COUNT = 121;

//! @todo remove equip_slop since ItemCategory already have the information
//! @todo move all bonus in a struct
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
  const char* name;
  const char* zone_name;
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
  std::array<MonsterDrop, 3> drops;
};

struct ShopUpgradeInfo {
  uint8_t tier;
  const char* name;
  const char* description;
  uint8_t req_skill_level;
  uint64_t cost_credits;
  uint8_t speed_bonus_pct;
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
std::string attack_style_name(CombatStyle style);
std::string money_string(uint64_t value);
std::string number_string(uint64_t value);

// XP & Level utility
uint64_t xp_for_level(int level);
int level_for_xp(uint64_t xp);
double level_progress_ratio(uint64_t xp);

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
  uint64_t total_bank_value() const;
  bool can_store_item(ItemId item_id) const;
  bool add_item(ItemId item_id, int qty, bool log_drop = false);
  bool remove_item(ItemId item_id, int qty);
  bool sell_item(ItemId item_id, int qty);
  uint64_t sell_all_non_equipped();

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
  uint64_t skill_xp(SkillType skill) const;
  int total_skill_level() const;
  uint64_t total_skill_xp() const;
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
  uint64_t credits = 250;
  uint64_t bounty_tokens = 0;
  uint64_t total_ticks_ms = 0;

  std::array<uint64_t, SKILL_COUNT> xp{};
  std::vector<uint64_t> action_mastery_xp;

  int bank_capacity = 24;
  std::vector<BankSlot> bank;

  std::map<EquipSlot, ItemId> equipped_items = {
      {EquipSlot::Weapon, ItemId::None},
      {EquipSlot::Visor, ItemId::None},
      {EquipSlot::ExoSuit, ItemId::None},
      {EquipSlot::HoloShield, ItemId::None}};
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
  CombatStyle combat_style = CombatStyle::Accurate;
  int player_hp = 100;
  int active_monster_id = 0;
  int monster_hp = 30;
  int player_attack_timer_ms = 0;
  int monster_attack_timer_ms = 0;
  int hp_regen_timer_ms = 0;

  // Bounty contract
  uint8_t bounty_target_id = 0;
  uint8_t bounty_remaining = 10;
  uint16_t bounties_completed = 0;

  // Statistics
  std::array<uint16_t, MONSTER_COUNT> monster_kills{};
  uint64_t total_items_gathered = 0;
  uint64_t total_monsters_killed = 0;
  uint64_t total_credits_earned = 250;
  uint16_t player_deaths = 0;

  bool sound_enabled = false;
  std::string status_banner;
  std::vector<std::string> game_log;

  // History for Charts
  //! @todo use an History struct with all this fields
  std::vector<uint64_t> credits_history;
  std::vector<uint64_t> bank_value_history;
  std::vector<int> total_level_history;
  std::vector<uint64_t> total_xp_history;
  std::vector<int> hp_history;
  std::array<std::vector<uint64_t>, SKILL_COUNT> skill_xp_history{};

 private:
  void complete_skill_action(int global_action_id);
  void step_combat_tick(int elapsed_ms);
  void on_monster_defeated(int monster_id);
  void on_player_defeated();
  void gain_xp(SkillType skill, uint64_t amount);

  int history_timer_ms_ = 0;
};
