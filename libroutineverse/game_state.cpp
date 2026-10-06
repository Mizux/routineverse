#include "game_state.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <fstream>
#include <string_view>

#include "utils.hpp"

void GameState::Skills::reset() {
  xp.clear();
  for (SkillType sk : all_skills) xp[sk] = 0;
  // Hitpoints starts at Level 10 (1,154 XP)
  xp[SkillType::Hitpoints] = xp_for_level(10);
  action_mastery_xp.assign(skill_actions.size(), 0);
}

int GameState::Skills::level(SkillType skill) const {
  return level_for_xp(skill_xp(skill));
}

uint64_t GameState::Skills::skill_xp(SkillType skill) const {
  auto it = xp.find(skill);
  return it != xp.end() ? it->second : 0;
}

int GameState::Skills::total_level() const {
  int sum = 0;
  for (SkillType sk : all_skills) {
    sum += level(sk);
  }
  return sum;
}

uint64_t GameState::Skills::total_xp() const {
  uint64_t sum = 0;
  for (const auto& [sk, v] : xp) sum += v;
  return sum;
}

int GameState::Skills::mastery_level(int global_action_id) const {
  if (global_action_id < 0 ||
      global_action_id >= static_cast<int>(action_mastery_xp.size())) {
    return 1;
  }
  return level_for_xp(action_mastery_xp[global_action_id]);
}

void GameState::Equipment::reset() {
  items = {
      {EquipSlot::Weapon, ItemId::ScrapBlade},
      {EquipSlot::Head, ItemId::None},
      {EquipSlot::Armor, ItemId::None},
      {EquipSlot::Shield, ItemId::None},
      {EquipSlot::Cutter, ItemId::ScrapCutter},
      {EquipSlot::Harvester, ItemId::ScrapHarvester},
      {EquipSlot::Drill, ItemId::ScrapDrill},
      {EquipSlot::Reactor, ItemId::BasicReactor},
      {EquipSlot::AutoStim, ItemId::None},
  };
  food_item = ItemId::None;
  food_qty = 0;
}

int GameState::Equipment::cutter_tier() const {
  return find_upgrade_tier(cutter_upgrades, items.at(EquipSlot::Cutter));
}

int GameState::Equipment::harvester_tier() const {
  return find_upgrade_tier(harvester_upgrades, items.at(EquipSlot::Harvester));
}

int GameState::Equipment::drill_tier() const {
  return find_upgrade_tier(drill_upgrades, items.at(EquipSlot::Drill));
}

int GameState::Equipment::reactor_tier() const {
  return find_upgrade_tier(reactor_upgrades, items.at(EquipSlot::Reactor));
}

int GameState::Equipment::auto_stim_tier() const {
  return find_upgrade_tier(auto_stim_upgrades, items.at(EquipSlot::AutoStim));
}

int GameState::Equipment::attack_bonus() const {
  int bonus = 0;
  for (const auto& [slot, id] : items) {
    if (is_valid_item(id)) bonus += get_item_info(id).bonus.attack;
  }
  return bonus;
}

int GameState::Equipment::strength_bonus() const {
  int bonus = 0;
  for (const auto& [slot, id] : items) {
    if (is_valid_item(id)) bonus += get_item_info(id).bonus.strength;
  }
  return bonus;
}

int GameState::Equipment::defence_bonus() const {
  int bonus = 0;
  for (const auto& [slot, id] : items) {
    if (is_valid_item(id)) bonus += get_item_info(id).bonus.defence;
  }
  return bonus;
}

int GameState::Equipment::damage_reduction() const {
  int dr = 0;
  for (const auto& [slot, id] : items) {
    if (is_valid_item(id)) dr += get_item_info(id).bonus.damage_reduction;
  }
  return std::clamp(dr, 0, 75);
}

int GameState::Equipment::speed_bonus_pct(EquipSlot slot) const {
  ItemId id = items.at(slot);
  return is_valid_item(id) ? get_item_info(id).bonus.speed_bonus_pct : 0;
}

void GameState::CombatState::reset(int initial_hp) {
  style = CombatStyle::Accurate;
  player_hp = initial_hp;
  active_monster_id = 0;
  monster_hp = monster_info[0].max_hp;
  player_attack_timer_ms = 0;
  monster_attack_timer_ms = 0;
  hp_regen_timer_ms = 0;

  bounty_target_id = 0;
  bounty_remaining = 8;
  bounties_completed = 0;
}

void GameState::Stats::reset() {
  monster_kills.fill(0);
  total_items_gathered = 0;
  total_monsters_killed = 0;
  total_credits_earned = 250;
  player_deaths = 0;
}

GameState::GameState() { new_game(); }

void GameState::new_game() {
  credits = 250;
  bounty_tokens = 0;
  total_ticks_ms = 0;

  skills.reset();

  bank.clear();
  bank.capacity = 24;

  // Give starter Scrap Vibro-Knife and starter tools equipped
  equipment.reset();

  add_item(ItemId::CopperWireScrap, 5, false);
  add_item(ItemId::KrillRation, 5, false);

  active_type = ActiveActivityType::Skill;
  active_action_id = 0;  // Start stripping Copper Wiring by default!
  active_progress_ms = 0;
  active_target_ms = action_effective_interval_ms(0);

  combat.reset(max_hp());
  stats.reset();

  game_log.clear();
  history.clear();
  history_timer_ms_ = 0;

  status_banner = "Strip Copper Wiring (Salvaging)";
  add_log(
      "Welcome to Routineverse! You jack into Neo-Sector with a Scrap "
      "Vibro-Knife, 10 Krill Rations, and 250 Cr.");
  add_log(
      "Active protocol: Strip Copper Wiring. Select any skill or hostile "
      "target to begin!");
  record_history_snapshot();
}

void GameState::add_log(const std::string& entry) {
  game_log.push_back(entry);
  if (game_log.size() > 120) {
    game_log.erase(game_log.begin(), game_log.begin() + (game_log.size() - 120));
  }
}

void GameState::History::clear() {
  credits.clear();
  bank_value.clear();
  total_level.clear();
  total_xp.clear();
  hp.clear();
  skill_xp.clear();
  for (SkillType sk : all_skills) skill_xp[sk] = {};
}

void GameState::History::add_record(const GameState& state) {
  auto push_capped = [this](auto& list, auto val) {
    list.push_back(val);
    if (list.size() > max_entries) {
      list.pop_front();
    }
  };

  push_capped(credits, state.credits);
  push_capped(bank_value, state.total_bank_value());
  push_capped(total_level, state.total_skill_level());
  push_capped(total_xp, state.total_skill_xp());
  push_capped(hp, state.combat.player_hp);
  for (SkillType sk : all_skills) {
    push_capped(skill_xp[sk], state.skill_xp(sk));
  }
}

void GameState::record_history_snapshot() { history.add_record(*this); }

int GameState::skill_level(SkillType skill) const { return skills.level(skill); }

uint64_t GameState::skill_xp(SkillType skill) const { return skills.skill_xp(skill); }

int GameState::total_skill_level() const { return skills.total_level(); }

uint64_t GameState::total_skill_xp() const { return skills.total_xp(); }

int GameState::mastery_level(int global_action_id) const {
  return skills.mastery_level(global_action_id);
}

int GameState::cutter_tier() const { return equipment.cutter_tier(); }

int GameState::harvester_tier() const { return equipment.harvester_tier(); }

int GameState::drill_tier() const { return equipment.drill_tier(); }

int GameState::reactor_tier() const { return equipment.reactor_tier(); }

int GameState::auto_stim_tier() const { return equipment.auto_stim_tier(); }

int GameState::action_effective_interval_ms(int global_action_id) const {
  if (global_action_id < 0 ||
      global_action_id >= static_cast<int>(skill_actions.size())) {
    return 2000;
  }
  const auto& act = skill_actions[global_action_id];
  int bonus_pct = 0;
  switch (act.skill) {
    case SkillType::Salvaging:
      bonus_pct = equipment.speed_bonus_pct(EquipSlot::Cutter);
      break;
    case SkillType::Fishing:
    case SkillType::Farming:
      bonus_pct = equipment.speed_bonus_pct(EquipSlot::Harvester);
      break;
    case SkillType::DeepMining:
      bonus_pct = equipment.speed_bonus_pct(EquipSlot::Drill);
      break;
    case SkillType::Recycling:
    case SkillType::SynthCook:
      bonus_pct = equipment.speed_bonus_pct(EquipSlot::Reactor);
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
  if (is_valid_item(act.input_item_1) && item_qty(act.input_item_1) < act.input_qty_1) {
    return false;
  }
  if (is_valid_item(act.input_item_2) && item_qty(act.input_item_2) < act.input_qty_2) {
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
    add_log(std::format("Requires {} Level {} to execute {}.", skill_name(act.skill),
                        act.req_level, act.name));
    return false;
  }
  if (!can_perform_action(global_action_id)) {
    std::string req_str;
    if (is_valid_item(act.input_item_1)) {
      req_str =
          std::format("{}x {}", act.input_qty_1, get_item_info(act.input_item_1).name);
    }
    if (is_valid_item(act.input_item_2)) {
      req_str += std::format(" + {}x {}", act.input_qty_2,
                             get_item_info(act.input_item_2).name);
    }
    add_log(std::format("Missing components for {}: need {}.", act.name, req_str));
    return false;
  }

  active_type = ActiveActivityType::Skill;
  active_action_id = global_action_id;
  active_progress_ms = 0;
  active_target_ms = action_effective_interval_ms(global_action_id);
  status_banner = std::format("{} ({})", act.name, skill_name(act.skill));
  add_log(
      std::format("Started {} ({:.2f}s cycle).", act.name, active_target_ms / 1000.0));
  return true;
}

bool GameState::start_combat(int monster_id) {
  if (monster_id < 0 || monster_id >= MONSTER_COUNT) return false;
  const auto& mon = monster_info[monster_id];
  if (skill_level(SkillType::Bounty) < mon.bounty_req) {
    add_log(std::format("Requires Bounty Level {} clearance to engage {}.",
                        mon.bounty_req, monster_name(mon.id)));
    return false;
  }
  check_auto_eat();
  active_type = ActiveActivityType::Combat;
  combat.active_monster_id = monster_id;
  combat.monster_hp = mon.max_hp;
  combat.player_attack_timer_ms = 0;
  combat.monster_attack_timer_ms = 0;
  status_banner = std::format("Engaging {} (Lv {}) in {}", monster_name(mon.id), mon.combat_level,
                              mon.zone_name);
  add_log(std::format("Engaged hostile {} ({} HP) in {}.", monster_name(mon.id), mon.max_hp,
                      mon.zone_name));
  return true;
}

void GameState::stop_activity() {
  active_type = ActiveActivityType::None;
  active_progress_ms = 0;
  status_banner = "Standby — Select a Skill or Hostile Target";
  add_log("Paused active protocol.");
}

void GameState::gain_xp(SkillType skill, uint64_t amount) {
  if (amount <= 0) return;
  int old_lvl = skill_level(skill);
  uint64_t bonus = 0;
  if (skill == SkillType::Recycling || skill == SkillType::SynthCook) {
    bonus = (amount * equipment.speed_bonus_pct(EquipSlot::Reactor)) / 100;
  }
  skills.xp[skill] += (amount + bonus);
  int new_lvl = skill_level(skill);
  if (new_lvl > old_lvl) {
    add_log(std::format("NEURAL UPGRADE! Your {} skill is now Level {}!",
                        skill_name(skill), new_lvl));
    if (skill == SkillType::Hitpoints) {
      combat.player_hp += (new_lvl - old_lvl) * 10;
      combat.player_hp = std::min(combat.player_hp, max_hp());
    }
  }
}

void GameState::complete_skill_action(int global_action_id) {
  if (!can_perform_action(global_action_id)) {
    add_log("Out of input components! Halting protocol.");
    stop_activity();
    return;
  }
  const auto& act = skill_actions[global_action_id];
  int m_lvl = mastery_level(global_action_id);

  // Resource preservation chance for
  // Fabrication/Synthesis/Smithing/Recycling/Farming skills (5% + 0.2% per
  // mastery level)
  bool preserved = false;
  if (is_valid_item(act.input_item_1) &&
      (act.skill == SkillType::Smithing || act.skill == SkillType::CyberFab ||
       act.skill == SkillType::SynthCook || act.skill == SkillType::Recycling ||
       act.skill == SkillType::Farming)) {
    int pres_chance = 5 + (m_lvl / 5);
    if (rand_int(1, 100) <= pres_chance) {
      preserved = true;
    }
  }

  if (!preserved) {
    if (is_valid_item(act.input_item_1)) {
      remove_item(act.input_item_1, act.input_qty_1);
    }
    if (is_valid_item(act.input_item_2)) {
      remove_item(act.input_item_2, act.input_qty_2);
    }
  }

  // Gain Skill XP & Mastery XP
  gain_xp(act.skill, act.xp);
  skills.action_mastery_xp[global_action_id] += std::max(10, act.xp / 2);

  // Double output chance (5% + 0.3% per mastery level)
  int qty = act.product_qty;
  if (qty > 0 && rand_int(1, 100) <= (5 + m_lvl / 3)) {
    qty *= 2;
  }

  if (act.skill == SkillType::SynthCook && is_valid_item(act.product_item) &&
      get_item_info(act.product_item).heal_amount > 0) {
    // Synthesis success chance (75% base + mastery/level bonus up to 99%)
    int cook_chance = std::min(
        99, 74 + (skill_level(SkillType::SynthCook) - act.req_level) / 2 + m_lvl / 4);
    if (rand_int(1, 100) <= cook_chance) {
      add_item(act.product_item, qty, false);
      stats.total_items_gathered += qty;
    } else {
      add_item(ItemId::ToxicSlag, 1, false);
      add_log(std::format("Synthesis contaminated! Ruined {}.",
                          get_item_info(act.product_item).name));
    }
  } else if (is_valid_item(act.product_item) && qty > 0) {
    add_item(act.product_item, qty, false);
    stats.total_items_gathered += qty;
  }

  // Bonus procs by skill
  if (act.skill == SkillType::Recycling) {
    // 25% chance to recover a Carbon Cell while recycling scrap, plus minor
    // credit yield
    if (rand_int(1, 100) <= 25) {
      add_item(ItemId::CarbonCell, 1, false);
    }
    credits += 2 + act.req_level / 5;
    stats.total_credits_earned += 2 + act.req_level / 5;
  } else if (act.skill == SkillType::Farming && !is_valid_item(act.input_item_1)) {
    // 20% chance to harvest bonus Hydro-Wheat alongside hydroponic crops!
    if (rand_int(1, 100) <= 20) {
      add_item(ItemId::HydroWheat, 1, false);
    }
  } else if (act.skill == SkillType::DeepMining) {
    // 8% chance to unearth a rare Data Crystal while deep-mining!
    if (rand_int(1, 100) <= 8) {
      auto gem_id =
          data_crystal_ids[rand_int(0, static_cast<int>(data_crystal_ids.size()) - 1)];
      if (add_item(gem_id, 1, false)) {
        add_log(std::format("While deep-mining, you extracted a rare {}!",
                            get_item_info(gem_id).name));
      }
    }
  } else if (act.skill == SkillType::Fishing) {
    // 5% chance to recover a submerged Corp Data-Cache (Credits)
    if (rand_int(1, 100) <= 5) {
      uint64_t bonus_cr = 25 + act.req_level * 8;
      credits += bonus_cr;
      stats.total_credits_earned += bonus_cr;
      add_log(std::format("Recovered a submerged Corp Data-Cache worth {}!",
                          money_string(bonus_cr)));
    }
  }

  // Stop if materials ran out after this action
  if (!can_perform_action(global_action_id)) {
    add_log(std::format("Completed {}: input components depleted.", act.name));
    stop_activity();
  }
}

void GameState::tick(int elapsed_ms) {
  if (elapsed_ms <= 0) return;
  total_ticks_ms += elapsed_ms;

  // Passive nanite HP regeneration outside/inside combat (+1% max HP every 5
  // seconds)
  combat.hp_regen_timer_ms += elapsed_ms;
  while (combat.hp_regen_timer_ms >= 5000) {
    combat.hp_regen_timer_ms -= 5000;
    if (combat.player_hp < max_hp()) {
      int regen = std::max(1, max_hp() / 100);
      combat.player_hp = std::min(max_hp(), combat.player_hp + regen);
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
  if (combat.active_monster_id < 0 || combat.active_monster_id >= MONSTER_COUNT) {
    stop_activity();
    return;
  }
  const auto& mon = monster_info[combat.active_monster_id];
  int plr_interval = player_attack_interval_ms();

  combat.player_attack_timer_ms += elapsed_ms;
  combat.monster_attack_timer_ms += elapsed_ms;

  // Player attacks
  while (active_type == ActiveActivityType::Combat &&
         combat.player_attack_timer_ms >= plr_interval) {
    combat.player_attack_timer_ms -= plr_interval;
    if (rand_int(1, 100) <= player_hit_chance_pct(combat.active_monster_id)) {
      int dmg = rand_int(std::max(1, player_max_hit() / 4), player_max_hit());
      dmg = std::min(dmg, combat.monster_hp);
      combat.monster_hp -= dmg;

      // Grant combat XP based on damage dealt
      uint64_t c_xp = std::max(4, dmg / 2);
      if (combat.style == CombatStyle::Accurate) {
        gain_xp(SkillType::Attack, c_xp);
      } else if (combat.style == CombatStyle::Aggressive) {
        gain_xp(SkillType::Strength, c_xp);
      } else {
        gain_xp(SkillType::Defence, c_xp);
      }
      gain_xp(SkillType::Hitpoints, std::max(uint64_t{2}, c_xp / 3));
    }

    if (combat.monster_hp <= 0) {
      on_monster_defeated(combat.active_monster_id);
      break;
    }
  }

  // Monster attacks
  while (active_type == ActiveActivityType::Combat &&
         combat.monster_attack_timer_ms >= mon.attack_interval_ms) {
    combat.monster_attack_timer_ms -= mon.attack_interval_ms;
    if (rand_int(1, 100) <= monster_hit_chance_pct(combat.active_monster_id)) {
      int raw_dmg = rand_int(1, mon.max_hit);
      int dr = player_damage_reduction();
      int dmg = std::max(1, (raw_dmg * (100 - dr)) / 100);
      combat.player_hp -= dmg;
      check_auto_eat();
      if (combat.player_hp <= 0) {
        on_player_defeated();
        break;
      }
    }
  }
}

void GameState::on_monster_defeated(int monster_id) {
  const auto& mon = monster_info[monster_id];
  stats.monster_kills[monster_id]++;
  stats.total_monsters_killed++;

  uint64_t cr_drop = rand_int(mon.credits_min, mon.credits_max);
  credits += cr_drop;
  stats.total_credits_earned += cr_drop;

  // Bonus XP on kill
  if (combat.style == CombatStyle::Accurate) {
    gain_xp(SkillType::Attack, mon.xp_reward);
  } else if (combat.style == CombatStyle::Aggressive) {
    gain_xp(SkillType::Strength, mon.xp_reward);
  } else {
    gain_xp(SkillType::Defence, mon.xp_reward);
  }
  gain_xp(SkillType::Hitpoints, mon.xp_reward / 3);

  // Bounty contract check
  if (monster_id == combat.bounty_target_id && combat.bounty_remaining > 0) {
    combat.bounty_remaining--;
    gain_xp(SkillType::Bounty, mon.xp_reward / 2 + 15);
    int bt = std::max(5, mon.combat_level * 2);
    bounty_tokens += bt;
    if (combat.bounty_remaining <= 0) {
      combat.bounties_completed++;
      int bonus_bt = 50 + combat.bounties_completed * 15;
      bounty_tokens += bonus_bt;
      gain_xp(SkillType::Bounty, 120 + mon.xp_reward);
      add_log(
          std::format("BOUNTY CONTRACT COMPLETE! Earned +{} Bounty Tokens! "
                      "Assigning new target...",
                      bonus_bt));
      assign_new_bounty_contract();
    }
  } else if (mon.bounty_req > 1) {
    gain_xp(SkillType::Bounty, mon.xp_reward / 4);
  }

  // Roll monster drop table
  std::string loot_str;
  for (const auto& drop : mon.drops) {
    if (is_valid_item(drop.item_id) && rand_int(1, 100) <= drop.chance_pct) {
      int q = rand_int(drop.min_qty, drop.max_qty);
      if (add_item(drop.item_id, q, false)) {
        if (!loot_str.empty()) loot_str += ", ";
        loot_str += std::format("{}x {}", q, get_item_info(drop.item_id).name);
      }
    }
  }

  if (loot_str.empty()) {
    add_log(
        std::format("Neutralized {}! Siphoned {}.", monster_name(mon.id), money_string(cr_drop)));
  } else {
    add_log(std::format("Neutralized {}! Siphoned {} and salvaged {}.", monster_name(mon.id),
                        money_string(cr_drop), loot_str));
  }

  // Respawn monster
  combat.monster_hp = mon.max_hp;
  combat.player_attack_timer_ms = 0;
  combat.monster_attack_timer_ms = 0;
}

void GameState::on_player_defeated() {
  stats.player_deaths++;
  combat.player_hp = max_hp();
  uint64_t lost_cr = std::min(credits, std::max(uint64_t{10}, credits / 10));
  credits -= lost_cr;
  add_log(
      std::format("CRITICAL FLATLINE fighting {}! Trauma Team reconstructed "
                  "you in Neo-Sector for {}.",
                  monster_name(monster_info[combat.active_monster_id].id), money_string(lost_cr)));
  stop_activity();
}

void GameState::assign_new_bounty_contract() {
  std::vector<int> eligible;
  int b_lvl = skill_level(SkillType::Bounty);
  int c_lvl = combat_level();
  for (int i = 0; i < MONSTER_COUNT; ++i) {
    if (monster_info[i].bounty_req <= b_lvl &&
        monster_info[i].combat_level <= c_lvl + 15 && !monster_info[i].is_boss) {
      eligible.push_back(i);
    }
  }
  if (eligible.empty()) eligible.push_back(0);
  combat.bounty_target_id =
      eligible[rand_int(0, static_cast<int>(eligible.size()) - 1)];
  combat.bounty_remaining = rand_int(6, 15);
  add_log(std::format("New Bounty Contract: Neutralize {}x {} ({}).",
                      combat.bounty_remaining,
                      monster_name(monster_info[combat.bounty_target_id].id),
                      monster_info[combat.bounty_target_id].zone_name));
}

void GameState::Bank::clear() { items.clear(); }

int GameState::Bank::item_qty(ItemId item_id) const {
  for (const auto& s : items) {
    if (s.item_id == item_id) return s.qty;
  }
  return 0;
}

uint64_t GameState::Bank::total_value() const {
  uint64_t total = 0;
  for (const auto& s : items) {
    if (is_valid_item(s.item_id)) {
      total += static_cast<uint64_t>(s.qty) * get_item_info(s.item_id).price;
    }
  }
  return total;
}

bool GameState::Bank::can_store_item(ItemId item_id) const {
  for (const auto& s : items) {
    if (s.item_id == item_id) return true;
  }
  return static_cast<int>(items.size()) < capacity;
}

bool GameState::Bank::add_item(ItemId item_id, int qty) {
  if (!is_valid_item(item_id) || qty <= 0) return false;
  for (auto& s : items) {
    if (s.item_id == item_id) {
      s.qty += qty;
      return true;
    }
  }
  if (static_cast<int>(items.size()) >= capacity) return false;
  items.push_back(Slot{item_id, qty});
  return true;
}

bool GameState::Bank::remove_item(ItemId item_id, int qty) {
  if (qty <= 0) return true;
  for (auto it = items.begin(); it != items.end(); ++it) {
    if (it->item_id == item_id) {
      if (it->qty < qty) return false;
      it->qty -= qty;
      if (it->qty == 0) items.erase(it);
      return true;
    }
  }
  return false;
}

uint64_t GameState::Bank::next_slot_cost() const {
  int extra = std::max(0, (capacity - 24) / 4);
  return 150 + extra * extra * 120 + extra * 150;
}

int GameState::item_qty(ItemId item_id) const { return bank.item_qty(item_id); }

int GameState::used_bank_slots() const { return bank.used_slots(); }

uint64_t GameState::total_bank_value() const { return bank.total_value(); }

bool GameState::can_store_item(ItemId item_id) const {
  return bank.can_store_item(item_id);
}

bool GameState::add_item(ItemId item_id, int qty, bool log_drop) {
  if (!is_valid_item(item_id) || qty <= 0) return false;
  if (!bank.add_item(item_id, qty)) {
    add_log(std::format("Cyber-Vault is full ({}/{})! Could not store {}!", bank.size(),
                        bank.capacity, get_item_info(item_id).name));
    return false;
  }
  if (log_drop) {
    add_log(
        std::format("Stored {}x {} in Cyber-Vault.", qty, get_item_info(item_id).name));
  }
  return true;
}

bool GameState::remove_item(ItemId item_id, int qty) {
  return bank.remove_item(item_id, qty);
}

bool GameState::sell_item(ItemId item_id, int qty) {
  if (!is_valid_item(item_id) || qty <= 0) return false;
  int have = item_qty(item_id);
  int sell_q = std::min(have, qty);
  if (sell_q <= 0) return false;
  uint64_t value = static_cast<uint64_t>(sell_q) * get_item_info(item_id).price;
  remove_item(item_id, sell_q);
  credits += value;
  stats.total_credits_earned += value;
  add_log(std::format("Liquidated {}x {} for {}.", sell_q, get_item_info(item_id).name,
                      money_string(value)));
  return true;
}

uint64_t GameState::sell_all_non_equipped() {
  uint64_t gained = 0;
  int items_sold = 0;
  for (const auto& s : bank) {
    if (is_valid_item(s.item_id)) {
      gained += static_cast<uint64_t>(s.qty) * get_item_info(s.item_id).price;
      items_sold += s.qty;
    }
  }
  bank.clear();
  if (gained > 0) {
    credits += gained;
    stats.total_credits_earned += gained;
    add_log(std::format("Liquidated all {} Vault items for {}!", items_sold,
                        money_string(gained)));
  }
  return gained;
}

bool GameState::equip_item(ItemId item_id) {
  if (!is_valid_item(item_id)) return false;
  const auto& info = get_item_info(item_id);
  const EquipSlot slot = equip_slot(info.category);
  if (slot == EquipSlot::None) {
    if (info.heal_amount > 0) {
      return equip_food(item_id);
    }
    return false;
  }

  switch (slot) {
    case EquipSlot::Weapon:
      if (skill_level(SkillType::Attack) < info.req_level) {
        add_log(std::format("Requires Attack Level {} to equip {}.", info.req_level,
                            info.name));
        return false;
      }
      break;
    case EquipSlot::Head:
    case EquipSlot::Armor:
    case EquipSlot::Shield:
      if (skill_level(SkillType::Defence) < info.req_level) {
        add_log(std::format("Requires Defence Level {} to equip {}.", info.req_level,
                            info.name));
        return false;
      }
      break;
    case EquipSlot::Cutter:
      if (skill_level(SkillType::Salvaging) < info.req_level) {
        add_log(std::format("Requires Salvaging Level {} to equip {}.", info.req_level,
                            info.name));
        return false;
      }
      break;
    case EquipSlot::Harvester:
      if (std::max(skill_level(SkillType::Fishing),
                   skill_level(SkillType::Farming)) < info.req_level) {
        add_log(std::format("Requires Fishing or Farming Level {} to equip {}.",
                            info.req_level, info.name));
        return false;
      }
      break;
    case EquipSlot::Drill:
      if (skill_level(SkillType::DeepMining) < info.req_level) {
        add_log(std::format("Requires Deep-Mining Level {} to equip {}.",
                            info.req_level, info.name));
        return false;
      }
      break;
    case EquipSlot::Reactor:
      if (std::max(skill_level(SkillType::Recycling),
                   skill_level(SkillType::SynthCook)) < info.req_level) {
        add_log(std::format("Requires Recycling or Synth-Cook Level {} to equip {}.",
                            info.req_level, info.name));
        return false;
      }
      break;
    case EquipSlot::AutoStim:
    case EquipSlot::None:
      break;
  }

  if (item_qty(item_id) <= 0) return false;

  ItemId old_item = equipment.at(slot);
  if (is_valid_item(old_item)) {
    bool removing_frees_slot = (bank.item_qty(item_id) == 1);
    bool can_store_old = removing_frees_slot || bank.can_store_item(old_item);
    if (!can_store_old) {
      add_log("Cyber-Vault is full! Cannot unequip current item.");
      return false;
    }
  }

  remove_item(item_id, 1);
  if (is_valid_item(old_item)) {
    add_item(old_item, 1, false);
  }
  equipment[slot] = item_id;
  add_log(std::format("Installed {} in {} slot.", info.name, equip_slot_name(slot)));
  return true;
}

bool GameState::unequip_slot(EquipSlot slot) {
  if (slot == EquipSlot::None) return false;
  ItemId cur = equipment.at(slot);
  if (!is_valid_item(cur)) return false;
  if (!can_store_item(cur)) {
    add_log("Cyber-Vault is full! Cannot unequip item.");
    return false;
  }
  add_item(cur, 1, false);
  equipment[slot] = ItemId::None;
  add_log(std::format("Unequipped {}.", get_item_info(cur).name));
  return true;
}

bool GameState::equip_food(ItemId item_id) {
  if (!is_valid_item(item_id)) return false;
  const auto& info = get_item_info(item_id);
  if (info.heal_amount <= 0) return false;
  int have = item_qty(item_id);
  if (have <= 0) return false;

  if (equipment.food_item == item_id) {
    remove_item(item_id, have);
    equipment.food_qty += have;
    add_log(std::format("Loaded {}x {} into Stim-Injector ({} total).", have, info.name,
                        equipment.food_qty));
    check_auto_eat();
    return true;
  }

  // Return old equipped stim to vault if any
  if (is_valid_item(equipment.food_item) && equipment.food_qty > 0) {
    bool frees_slot = (have == bank.item_qty(item_id));
    bool can_store = frees_slot || bank.can_store_item(equipment.food_item);
    if (!can_store) {
      add_log("Cyber-Vault is full! Cannot swap equipped stims.");
      return false;
    }
    remove_item(item_id, have);
    add_item(equipment.food_item, equipment.food_qty, false);
  } else {
    remove_item(item_id, have);
  }
  equipment.food_item = item_id;
  equipment.food_qty = have;
  add_log(
      std::format("Loaded {}x {} (+{} HP each).", have, info.name, info.heal_amount));
  check_auto_eat();
  return true;
}

bool GameState::eat_food() {
  if (!is_valid_item(equipment.food_item) || equipment.food_qty <= 0) {
    add_log("No stim-pack or ration loaded!");
    return false;
  }
  if (combat.player_hp >= max_hp()) {
    add_log("You are already at full Hitpoints!");
    return false;
  }
  const auto& food_info = get_item_info(equipment.food_item);
  int heal = food_info.heal_amount;
  equipment.food_qty--;
  int before = combat.player_hp;
  combat.player_hp = std::min(max_hp(), combat.player_hp + heal);
  add_log(std::format("Used {} and restored +{} HP ({}/{} HP).", food_info.name,
                      combat.player_hp - before, combat.player_hp, max_hp()));
  if (equipment.food_qty == 0) {
    equipment.food_item = ItemId::None;
  }
  return true;
}

void GameState::check_auto_eat() {
  if (auto_stim_tier() <= 0) return;
  int threshold = auto_eat_threshold_hp();
  while (combat.player_hp <= threshold &&
         is_valid_item(equipment.food_item) && equipment.food_qty > 0) {
    int heal = get_item_info(equipment.food_item).heal_amount;
    equipment.food_qty--;
    combat.player_hp = std::min(max_hp(), combat.player_hp + heal);
    if (equipment.food_qty == 0) {
      equipment.food_item = ItemId::None;
      break;
    }
  }
}

uint64_t GameState::next_bank_slot_cost() const { return bank.next_slot_cost(); }

bool GameState::buy_cutter_upgrade() {
  int cur_t = cutter_tier();
  if (cur_t + 1 >= TOOL_TIER_COUNT) return false;
  const auto& upg = cutter_upgrades[cur_t + 1];
  if (skill_level(SkillType::Salvaging) < upg.req_skill_level) {
    add_log(std::format("Requires Salvaging Level {} to buy {}.", upg.req_skill_level,
                        upg.name));
    return false;
  }
  if (credits < upg.cost_credits) {
    add_log(std::format("Not enough Credits for {} (need {}).", upg.name,
                        money_string(upg.cost_credits)));
    return false;
  }
  credits -= upg.cost_credits;
  equipment[EquipSlot::Cutter] = upg.item_id;
  add_log(std::format("Purchased & installed {} ({})!", upg.name, upg.description));
  return true;
}

bool GameState::buy_harvester_upgrade() {
  int cur_t = harvester_tier();
  if (cur_t + 1 >= TOOL_TIER_COUNT) return false;
  const auto& upg = harvester_upgrades[cur_t + 1];
  if (std::max(skill_level(SkillType::Fishing), skill_level(SkillType::Farming)) <
      upg.req_skill_level) {
    add_log(std::format("Requires Fishing or Farming Level {} to buy {}.",
                        upg.req_skill_level, upg.name));
    return false;
  }
  if (credits < upg.cost_credits) {
    add_log(std::format("Not enough Credits for {} (need {}).", upg.name,
                        money_string(upg.cost_credits)));
    return false;
  }
  credits -= upg.cost_credits;
  equipment[EquipSlot::Harvester] = upg.item_id;
  add_log(std::format("Purchased & installed {} ({})!", upg.name, upg.description));
  return true;
}

bool GameState::buy_drill_upgrade() {
  int cur_t = drill_tier();
  if (cur_t + 1 >= TOOL_TIER_COUNT) return false;
  const auto& upg = drill_upgrades[cur_t + 1];
  if (skill_level(SkillType::DeepMining) < upg.req_skill_level) {
    add_log(std::format("Requires Deep-Mining Level {} to buy {}.", upg.req_skill_level,
                        upg.name));
    return false;
  }
  if (credits < upg.cost_credits) {
    add_log(std::format("Not enough Credits for {} (need {}).", upg.name,
                        money_string(upg.cost_credits)));
    return false;
  }
  credits -= upg.cost_credits;
  equipment[EquipSlot::Drill] = upg.item_id;
  add_log(std::format("Purchased & installed {} ({})!", upg.name, upg.description));
  return true;
}

bool GameState::buy_reactor_upgrade() {
  int cur_t = reactor_tier();
  if (cur_t + 1 >= TOOL_TIER_COUNT) return false;
  const auto& upg = reactor_upgrades[cur_t + 1];
  if (std::max(skill_level(SkillType::Recycling),
               skill_level(SkillType::SynthCook)) < upg.req_skill_level) {
    add_log(std::format("Requires Recycling or Synth-Cook Level {} to buy {}.",
                        upg.req_skill_level, upg.name));
    return false;
  }
  if (credits < upg.cost_credits) {
    add_log(std::format("Not enough Credits for {} (need {}).", upg.name,
                        money_string(upg.cost_credits)));
    return false;
  }
  credits -= upg.cost_credits;
  equipment[EquipSlot::Reactor] = upg.item_id;
  add_log(std::format("Purchased & installed {} ({})!", upg.name, upg.description));
  return true;
}

bool GameState::buy_auto_stim_upgrade() {
  int cur_t = auto_stim_tier();
  if (cur_t + 1 >= AUTO_STIM_TIER_COUNT) return false;
  const auto& upg = auto_stim_upgrades[cur_t + 1];
  if (credits < upg.cost_credits) {
    add_log(std::format("Not enough Credits for {} (need {}).", upg.name,
                        money_string(upg.cost_credits)));
    return false;
  }
  credits -= upg.cost_credits;
  equipment[EquipSlot::AutoStim] = upg.item_id;
  add_log(std::format("Purchased & installed {} ({})!", upg.name, upg.description));
  return true;
}

bool GameState::buy_bank_slot() {
  uint64_t cost = next_bank_slot_cost();
  if (credits < cost) {
    add_log(std::format("Not enough Credits for +4 Vault Slots (need {}).",
                        money_string(cost)));
    return false;
  }
  credits -= cost;
  bank.capacity += 4;
  add_log(std::format("Purchased +4 Vault Slots! Cyber-Vault capacity is now {}.",
                      bank.capacity));
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

int GameState::max_hp() const { return skill_level(SkillType::Hitpoints) * 10; }

int GameState::player_attack_interval_ms() const { return 2400; }

int GameState::player_max_hit() const {
  int str_lvl = skill_level(SkillType::Strength);
  if (combat.style == CombatStyle::Aggressive) str_lvl += 3;
  int str_bonus = equipment.strength_bonus();
  return 12 + str_lvl * 3 + (str_bonus * (10 + str_lvl)) / 12;
}

int GameState::player_accuracy() const {
  int atk_lvl = skill_level(SkillType::Attack);
  if (combat.style == CombatStyle::Accurate) atk_lvl += 3;
  int atk_bonus = equipment.attack_bonus();
  return 25 + atk_lvl * 5 + atk_bonus * 3;
}

int GameState::player_evasion() const {
  int def_lvl = skill_level(SkillType::Defence);
  if (combat.style == CombatStyle::Defensive) def_lvl += 3;
  int def_bonus = equipment.defence_bonus();
  return 20 + def_lvl * 5 + def_bonus * 3;
}

int GameState::player_damage_reduction() const { return equipment.damage_reduction(); }

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
  int pct = equipment.speed_bonus_pct(EquipSlot::AutoStim);
  return (max_hp() * pct) / 100;
}

std::string GameState::default_save_path() {
  const char* home = std::getenv("HOME");
#if defined(_WIN32)
  if (!home || std::string_view(home).empty()) {
    home = std::getenv("USERPROFILE");
  }
  if (!home || std::string_view(home).empty()) {
    home = std::getenv("APPDATA");
  }
#endif
  if (!home || std::string_view(home).empty()) {
    return "routineverse.save";
  }
  std::filesystem::path dir = std::filesystem::path(home) / ".config" / "Mizux";
  std::error_code ec;
  std::filesystem::create_directories(dir, ec);
  return (dir / "routineverse.save").string();
}

bool GameState::save_to_file(const std::string& path) const {
  std::filesystem::path final_path(path);
  std::filesystem::path tmp_path = final_path;
  tmp_path += ".tmp";

  std::error_code ec;
  if (final_path.has_parent_path()) {
    std::filesystem::create_directories(final_path.parent_path(), ec);
  }

  std::ofstream out(tmp_path);
  if (!out.is_open()) return false;

  out << "ROUTINEVERSE_SAVE_V3\n";
  out << credits << " " << bounty_tokens << " " << total_ticks_ms << "\n";
  for (size_t i = 0; i < all_skills.size(); ++i) {
    out << skill_xp(all_skills[i]) << (i + 1 == all_skills.size() ? "\n" : " ");
  }
  out << skills.action_mastery_xp.size() << "\n";
  for (size_t i = 0; i < skills.action_mastery_xp.size(); ++i) {
    out << skills.action_mastery_xp[i]
        << (i + 1 == skills.action_mastery_xp.size() ? "\n" : " ");
  }
  out << bank.capacity << " " << bank.size() << "\n";
  for (const auto& s : bank) {
    out << item_id_to_int(s.item_id) << " " << s.qty << "\n";
  }
  for (EquipSlot slot : all_equip_slots) {
    out << item_id_to_int(equipment.at(slot)) << " ";
  }
  out << item_id_to_int(equipment.food_item) << " " << equipment.food_qty << "\n";
  out << activity_type_to_int(active_type) << " " << active_action_id << " "
      << combat.active_monster_id << " " << combat.player_hp << " " << combat.monster_hp
      << " " << combat_style_to_int(combat.style) << "\n";
  out << static_cast<unsigned int>(combat.bounty_target_id) << " "
      << static_cast<unsigned int>(combat.bounty_remaining) << " "
      << combat.bounties_completed << "\n";
  out << stats.total_items_gathered << " " << stats.total_monsters_killed << " "
      << stats.total_credits_earned << " " << stats.player_deaths << "\n";
  for (size_t i = 0; i < stats.monster_kills.size(); ++i) {
    out << stats.monster_kills[i] << (i + 1 == stats.monster_kills.size() ? "\n" : " ");
  }

  if (!out.good()) {
    std::filesystem::remove(tmp_path, ec);
    return false;
  }
  out.close();

  std::filesystem::rename(tmp_path, final_path, ec);
  if (ec) {
    std::filesystem::remove(tmp_path, ec);
    return false;
  }
  return true;
}

bool GameState::load_from_file(const std::string& path) {
  std::ifstream in(path);
  if (!in.is_open()) return false;

  std::string header;
  if (!(in >> header) ||
      (header != "ROUTINEVERSE_SAVE_V1" && header != "ROUTINEVERSE_SAVE_V2" &&
       header != "ROUTINEVERSE_SAVE_V3")) {
    return false;
  }

  uint64_t loaded_credits = 0;
  uint64_t loaded_tokens = 0;
  uint64_t loaded_ticks = 0;
  if (!(in >> loaded_credits >> loaded_tokens >> loaded_ticks)) return false;
  credits = loaded_credits;
  bounty_tokens = loaded_tokens;
  total_ticks_ms = loaded_ticks;

  for (SkillType sk : all_skills) {
    uint64_t xp_val = 0;
    if (!(in >> xp_val)) return false;
    skills.xp[sk] = xp_val;
  }

  size_t m_sz = 0;
  if (!(in >> m_sz)) return false;
  skills.action_mastery_xp.assign(skill_actions.size(), 0);
  for (size_t i = 0; i < m_sz; ++i) {
    uint64_t val = 0;
    if (!(in >> val)) return false;
    if (i < skills.action_mastery_xp.size()) skills.action_mastery_xp[i] = val;
  }

  int b_cap = 24;
  size_t b_sz = 0;
  if (!(in >> b_cap >> b_sz)) return false;
  bank.capacity = std::max(24, b_cap);
  bank.clear();
  for (size_t i = 0; i < b_sz; ++i) {
    int raw_id = -1;
    int qty = 0;
    if (!(in >> raw_id >> qty)) return false;
    ItemId id = item_id_or_none(raw_id);
    if (is_valid_item(id) && qty > 0) {
      bank.items.push_back(Bank::Slot{id, qty});
    }
  }

  if (header == "ROUTINEVERSE_SAVE_V2" || header == "ROUTINEVERSE_SAVE_V3") {
    for (EquipSlot slot : all_equip_slots) {
      int raw_id = -1;
      if (!(in >> raw_id)) return false;
      equipment[slot] = item_id_or_none(raw_id);
    }

    int raw_food_id = -1;
    if (!(in >> raw_food_id >> equipment.food_qty)) return false;
    equipment.food_item = item_id_or_none(raw_food_id);
  } else {
    for (const auto& slot :
         {EquipSlot::Weapon, EquipSlot::Head, EquipSlot::Armor, EquipSlot::Shield}) {
      int raw_id = -1;
      if (!(in >> raw_id)) return false;
      equipment[slot] = item_id_or_none(raw_id);
    }

    int raw_food_id = -1;
    if (!(in >> raw_food_id >> equipment.food_qty)) return false;
    equipment.food_item = item_id_or_none(raw_food_id);

    int c_t = 0, h_t = 0, d_t = 0, r_t = 0, a_t = 0;
    if (!(in >> c_t >> h_t >> d_t >> r_t >> a_t)) return false;
    equipment[EquipSlot::Cutter] =
        cutter_upgrades[std::clamp(c_t, 0, TOOL_TIER_COUNT - 1)].item_id;
    equipment[EquipSlot::Harvester] =
        harvester_upgrades[std::clamp(h_t, 0, TOOL_TIER_COUNT - 1)].item_id;
    equipment[EquipSlot::Drill] =
        drill_upgrades[std::clamp(d_t, 0, TOOL_TIER_COUNT - 1)].item_id;
    equipment[EquipSlot::Reactor] =
        reactor_upgrades[std::clamp(r_t, 0, TOOL_TIER_COUNT - 1)].item_id;
    equipment[EquipSlot::AutoStim] =
        auto_stim_upgrades[std::clamp(a_t, 0, AUTO_STIM_TIER_COUNT - 1)].item_id;
  }

  int act_t = 0;
  int style_t = 0;
  if (!(in >> act_t >> active_action_id >> combat.active_monster_id >> combat.player_hp >>
        combat.monster_hp >> style_t)) {
    return false;
  }
  active_type = activity_type_or_none(act_t);
  combat.style = combat_style_or_default(style_t);

  int b_tid = 0, b_rem = 0, b_comp = 0;
  if (!(in >> b_tid >> b_rem >> b_comp)) return false;
  combat.bounty_target_id = static_cast<uint8_t>(std::clamp(b_tid, 0, MONSTER_COUNT - 1));
  combat.bounty_remaining = static_cast<uint8_t>(std::max(0, b_rem));
  combat.bounties_completed = static_cast<uint16_t>(std::max(0, b_comp));

  if (!(in >> stats.total_items_gathered >> stats.total_monsters_killed >>
        stats.total_credits_earned >> stats.player_deaths)) {
    return false;
  }

  if (header == "ROUTINEVERSE_SAVE_V3") {
    for (size_t i = 0; i < stats.monster_kills.size(); ++i) {
      uint16_t kills = 0;
      if (in >> kills) {
        stats.monster_kills[i] = kills;
      }
    }
  }

  if (active_type == ActiveActivityType::Skill && active_action_id >= 0 &&
      active_action_id < static_cast<int>(skill_actions.size())) {
    active_target_ms = action_effective_interval_ms(active_action_id);
    status_banner = std::format("{} ({})", skill_actions[active_action_id].name,
                                skill_name(skill_actions[active_action_id].skill));
  } else if (active_type == ActiveActivityType::Combat &&
             combat.active_monster_id >= 0 &&
             combat.active_monster_id < MONSTER_COUNT) {
    status_banner =
        std::format("Engaging {}", monster_name(monster_info[combat.active_monster_id].id));
  } else {
    status_banner = "Standby — Select a Skill or Hostile Target";
  }

  record_history_snapshot();
  add_log("Loaded saved neural state.");
  return true;
}

