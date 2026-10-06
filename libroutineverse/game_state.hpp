#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <list>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "combat.hpp"
#include "gears.hpp"
#include "items.hpp"
#include "monsters.hpp"
#include "skills.hpp"

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

  struct Skills {
    std::unordered_map<SkillType, uint64_t> xp;
    std::vector<uint64_t> action_mastery_xp;

    void reset();
    int level(SkillType skill) const;
    uint64_t skill_xp(SkillType skill) const;
    int total_level() const;
    uint64_t total_xp() const;
    int mastery_level(int global_action_id) const;
  };
  Skills skills;

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

  struct Equipment {
    std::map<EquipSlot, ItemId> items = {
        {EquipSlot::Weapon, ItemId::None},  {EquipSlot::Head, ItemId::None},
        {EquipSlot::Armor, ItemId::None},   {EquipSlot::Shield, ItemId::None},
        {EquipSlot::Cutter, ItemId::None},  {EquipSlot::Harvester, ItemId::None},
        {EquipSlot::Drill, ItemId::None},   {EquipSlot::Reactor, ItemId::None},
        {EquipSlot::AutoStim, ItemId::None}};
    ItemId food_item = ItemId::None;
    int food_qty = 0;

    void reset();
    ItemId at(EquipSlot slot) const {
      auto it = items.find(slot);
      return it != items.end() ? it->second : ItemId::None;
    }
    ItemId& operator[](EquipSlot slot) { return items[slot]; }
    size_t size() const { return items.size(); }
    auto begin() const { return items.begin(); }
    auto end() const { return items.end(); }

    int cutter_tier() const;
    int harvester_tier() const;
    int drill_tier() const;
    int reactor_tier() const;
    int auto_stim_tier() const;

    int attack_bonus() const;
    int strength_bonus() const;
    int defence_bonus() const;
    int damage_reduction() const;
    int speed_bonus_pct(EquipSlot slot) const;
  };
  Equipment equipment;

  // Active activity state
  ActiveActivityType active_type = ActiveActivityType::None;
  int active_action_id = -1;
  int active_progress_ms = 0;
  int active_target_ms = 2000;

  // Combat & Bounty state
  struct CombatState {
    CombatStyle style = CombatStyle::Accurate;
    int player_hp = 100;
    int active_monster_id = 0;
    int monster_hp = 30;
    int player_attack_timer_ms = 0;
    int monster_attack_timer_ms = 0;
    int hp_regen_timer_ms = 0;

    uint8_t bounty_target_id = 0;
    uint8_t bounty_remaining = 10;
    uint16_t bounties_completed = 0;

    void reset(int initial_hp);
  };
  CombatState combat;

  // Statistics
  struct Stats {
    std::array<uint16_t, MONSTER_COUNT> monster_kills{};
    uint64_t total_items_gathered = 0;
    uint64_t total_monsters_killed = 0;
    uint64_t total_credits_earned = 250;
    uint16_t player_deaths = 0;

    void reset();
  };
  Stats stats;

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

