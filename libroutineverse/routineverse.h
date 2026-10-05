#pragma once

#include <array>
#include <cstdint>
#include <list>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

// Items
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

  // Tools - Salvaging Cutters
  ScrapCutter,
  TitaniumCutter,
  DurasteelCutter,
  CobaltCutter,
  TungstenCutter,
  NeutroniumCutter,
  ChronoCutter,

  // Tools - Bio-Harvesters
  ScrapHarvester,
  TitaniumHarvester,
  DurasteelHarvester,
  CobaltHarvester,
  TungstenHarvester,
  NeutroniumHarvester,
  ChronoHarvester,

  // Tools - Mining Drills
  ScrapDrill,
  TitaniumDrill,
  DurasteelDrill,
  CobaltDrill,
  TungstenDrill,
  NeutroniumDrill,
  ChronoDrill,

  // Tools - Synth-Reactors
  BasicReactor,
  PlasteelReactor,
  NanotubeReactor,
  PositronicReactor,
  PlasmaReactor,
  QuantumReactor,
  MainframeReactor,

  // Cyberware - Auto-Stim Injectors
  AutoStimMk1,
  AutoStimMk2,
  AutoStimMk3,

  // Enemy Salvage Loot
  ServoParts,
  HeavyChassis,
  ApexCyberCore,
  Microchip,
  SynthWeaveHide,
};
inline constexpr bool is_valid_item(ItemId id) {
  return id == ItemId::None ? false : true;
}

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
  Head,
  Armor,
  Shield,
  Cutter,
  Harvester,
  Drill,
  Reactor,
  AutoStim,
  CyberLoot,
};
std::string item_category_name(ItemCategory cat);

struct Bonus {
  int attack = 0;            // Accuracy bonus
  int strength = 0;          // Max hit bonus
  int defence = 0;           // Evasion bonus
  int damage_reduction = 0;  // Damage reduction %
  int speed_bonus_pct = 0;   // Tool interval reduction % (or Auto-Stim threshold %)
};

struct ItemInfo {
  ItemId id;
  const char* name;
  ItemCategory category;
  int price;
  int heal_amount;  // > 0 if usable stim/ration
  int req_level;    // Required skill level to equip
  Bonus bonus{};
};

const ItemInfo& get_item_info(ItemId id);
std::string item_equip_summary(ItemId id);

// Monsters
struct MonsterDrop {
  ItemId item_id = ItemId::None;
  int chance_pct = 0;  // 1..100
  int min_qty = 0;
  int max_qty = 0;
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
  std::array<MonsterDrop, 4> drops;
};
inline constexpr int MONSTER_COUNT = 12;
extern const std::array<MonsterInfo, MONSTER_COUNT> monster_info;

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

// Skills
inline constexpr uint8_t MAX_SKILL_LEVEL = 99;

enum class SkillType : uint8_t {
  // Logistic Skill
  Salvaging,
  BioHarvest,
  Farming,
  Recycling,
  SynthCook,
  DeepMining,
  Smithing,
  CyberFab,
  // Combat Skill
  Attack,
  Strength,
  Defence,
  Hitpoints,
  Bounty,
  // Hacking Skill
};
std::string skill_name(SkillType skill);
std::string skill_short_name(SkillType skill);

inline constexpr std::array all_skills = {
    SkillType::Salvaging, SkillType::BioHarvest, SkillType::Farming,
    SkillType::Recycling, SkillType::SynthCook,  SkillType::DeepMining,
    SkillType::Smithing,  SkillType::CyberFab,   SkillType::Attack,
    SkillType::Strength,  SkillType::Defence,    SkillType::Hitpoints,
    SkillType::Bounty,
};
inline constexpr bool is_combat_skill(SkillType skill) {
  switch (skill) {
    case SkillType::Attack:
    case SkillType::Strength:
    case SkillType::Defence:
    case SkillType::Hitpoints:
    case SkillType::Bounty:
      return true;
    default:
      return false;
  }
}
inline constexpr int max_total_skill_level() {
  return static_cast<int>(all_skills.size()) * MAX_SKILL_LEVEL;
}

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
extern const std::vector<SkillAction> skill_actions;
std::string action_recipe(const SkillAction& act);
std::vector<int> actions_for_skill(SkillType skill);

// Combat
enum class CombatStyle : uint8_t {
  Accurate,    // Trains Precision (+accuracy)
  Aggressive,  // Trains Strength (+max hit)
  Defensive,   // Trains Defence (+evasion)
};
std::string combat_style_name(CombatStyle style);

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

// Utility & Formatting functions
std::string money_string(uint64_t value);
std::string number_string(uint64_t value);

// XP & Level utility
uint64_t xp_for_level(int level);
int level_for_xp(uint64_t xp);
double level_progress_ratio(uint64_t xp);

enum class ActiveActivityType : uint8_t {
  None,
  Skill,
  Combat,
};

// todo split into smaller classes e.g. PlayerState have instance of
// InventoryClass, BankClass, SkillClass, CombatClass, etc.
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
  uint64_t next_bank_slot_cost() const;
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

  int cutter_tier() const;
  int harvester_tier() const;
  int drill_tier() const;
  int reactor_tier() const;
  int auto_stim_tier() const;

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

  std::unordered_map<SkillType, uint64_t> xp;
  std::vector<uint64_t> action_mastery_xp;

  struct Bank {
    struct Slot {
      ItemId item_id = ItemId::None;
      int qty = 0;
    };

    explicit Bank(int cap = 24) : capacity(cap) {}

    int capacity = 24;
    std::vector<Slot> items;

    void clear();
    bool empty() const { return items.empty(); }
    size_t size() const { return items.size(); }
    int used_slots() const { return static_cast<int>(items.size()); }
    int item_qty(ItemId item_id) const;
    uint64_t total_value() const;
    bool can_store_item(ItemId item_id) const;
    bool add_item(ItemId item_id, int qty);
    bool remove_item(ItemId item_id, int qty);
    uint64_t next_slot_cost() const;

    const Slot& operator[](size_t idx) const { return items[idx]; }
    Slot& operator[](size_t idx) { return items[idx]; }
    auto begin() { return items.begin(); }
    auto end() { return items.end(); }
    auto begin() const { return items.begin(); }
    auto end() const { return items.end(); }
  };
  Bank bank;

  std::map<EquipSlot, ItemId> equipped_items = {
      {EquipSlot::Weapon, ItemId::None},  {EquipSlot::Head, ItemId::None},
      {EquipSlot::Armor, ItemId::None},   {EquipSlot::Shield, ItemId::None},
      {EquipSlot::Cutter, ItemId::None},  {EquipSlot::Harvester, ItemId::None},
      {EquipSlot::Drill, ItemId::None},   {EquipSlot::Reactor, ItemId::None},
      {EquipSlot::AutoStim, ItemId::None}};
  ItemId equipped_food_item = ItemId::None;
  int equipped_food_qty = 0;

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
  struct History {
    explicit History(size_t max_entries = 60) : max_entries(max_entries) {}

    size_t max_entries = 60;
    std::list<uint64_t> credits;
    std::list<uint64_t> bank_value;
    std::list<int> total_level;
    std::list<uint64_t> total_xp;
    std::list<int> hp;
    std::unordered_map<SkillType, std::list<uint64_t>> skill_xp;

    void clear();
    void add_record(const GameState& state);
  };
  History history;

 private:
  void complete_skill_action(int global_action_id);
  void step_combat_tick(int elapsed_ms);
  void on_monster_defeated(int monster_id);
  void on_player_defeated();
  void gain_xp(SkillType skill, uint64_t amount);

  int history_timer_ms_ = 0;
};
