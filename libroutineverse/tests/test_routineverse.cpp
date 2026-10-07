#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <iostream>

#include "routineverse.hpp"

#define TEST_CHECK(cond)                                                       \
  do {                                                                         \
    if (!(cond)) {                                                             \
      std::cerr << "Assertion failed: " #cond " at " << __FILE__ << ":"        \
                << __LINE__ << std::endl;                                      \
      std::exit(1);                                                            \
    }                                                                          \
  } while (0)

void test_enum_safety() {
  std::cout << "[RUNNING] test_enum_safety..." << std::endl;

  // Item bounds and validity
  TEST_CHECK(!all_item_ids().empty());
  TEST_CHECK(!is_valid_item(ItemId::None));
  TEST_CHECK(is_valid_item(ItemId::CopperWireScrap));
  TEST_CHECK(is_valid_item(ItemId::SynthWeaveHide));
  TEST_CHECK(!is_valid_item(static_cast<ItemId>(999)));
  TEST_CHECK(!is_valid_item(static_cast<ItemId>(152)));
  TEST_CHECK(!is_valid_item(static_cast<ItemId>(65535)));

  // Safe fallback in get_item_info
  const auto& invalid_info = get_item_info(static_cast<ItemId>(999));
  TEST_CHECK(invalid_info.id == ItemId::None);

  // EquipSlot & upgrade helpers
  TEST_CHECK(!all_equip_slots().empty());
  TEST_CHECK(!cutter_upgrades().empty());
  TEST_CHECK(!harvester_upgrades().empty());
  TEST_CHECK(!drill_upgrades().empty());
  TEST_CHECK(!reactor_upgrades().empty());
  TEST_CHECK(!auto_stim_upgrades().empty());

  // CombatStyle helpers
  TEST_CHECK(!all_combat_styles().empty());
  TEST_CHECK(next_combat_style(CombatStyle::Accurate) == CombatStyle::Aggressive);
  TEST_CHECK(next_combat_style(CombatStyle::Aggressive) == CombatStyle::Defensive);
  TEST_CHECK(next_combat_style(CombatStyle::Defensive) == CombatStyle::Accurate);

  // ActivityType helpers
  TEST_CHECK(!all_activity_types().empty());

  // Item name helper
  TEST_CHECK(item_name(ItemId::CopperWireScrap) == "Copper Wire Scrap");

  // MonsterId & ZoneId helpers
  TEST_CHECK(!all_monster_ids().empty());
  TEST_CHECK(!all_zone_ids().empty());
  TEST_CHECK(monster_name(MonsterId::StrayServoDrone) == "Stray Servo-Drone");
  TEST_CHECK(!monster_summary(MonsterId::StrayServoDrone).empty());
  TEST_CHECK(get_monster_info(MonsterId::StrayServoDrone).combat_level == 1);
  TEST_CHECK(zone_name(get_monster_info(MonsterId::StrayServoDrone).zone) ==
             "Neon Slums");

  // Skill helpers
  TEST_CHECK(!all_skills().empty());
  TEST_CHECK(!all_actions().empty());
  TEST_CHECK(skill_name(SkillType::Salvaging) == "Salvaging");
  TEST_CHECK(skill_short_name(SkillType::Salvaging) == "SLV");

  // Activity type name helper
  TEST_CHECK(activity_type_name(ActiveActivityType::Skill) == "Skill");

  std::cout << "[PASSED] test_enum_safety" << std::endl;
}

void test_equip_rollback_on_full_vault() {
  std::cout << "[RUNNING] test_equip_rollback_on_full_vault..." << std::endl;
  GameState gs;
  gs.bank.clear();
  gs.bank.capacity = 24;

  // Fill 23 slots with distinct items
  const auto items = all_item_ids();
  for (int i = 1; i <= 23; ++i) {
    auto id = items[i];
    TEST_CHECK(is_valid_item(id));
    gs.bank.add_item(id, 10);
  }
  TEST_CHECK(gs.bank.used_slots() == 23);

  // Put 2x TitaniumBlade in the 24th slot
  gs.bank.add_item(ItemId::TitaniumBlade, 2);
  TEST_CHECK(gs.bank.used_slots() == 24);

  // Player has ScrapBlade equipped by default
  TEST_CHECK(gs.equipment.at(EquipSlot::Weapon) == ItemId::ScrapBlade);
  // ScrapBlade is not currently in the bank
  TEST_CHECK(gs.bank.item_qty(ItemId::ScrapBlade) == 0);

  // Grant player Attack level 5 to satisfy TitaniumBlade requirement
  while (gs.skill_level(SkillType::Attack) < 5) {
    gs.skills.xp[SkillType::Attack] += 1000;
  }

  // Attempting to equip TitaniumBlade should fail because vault is 24/24,
  // removing 1 TitaniumBlade leaves 1 in the bank (so slot is not freed),
  // and ScrapBlade has nowhere to go!
  bool res = gs.equip_item(ItemId::TitaniumBlade);
  TEST_CHECK(!res);
  // ScrapBlade must NOT have been destroyed!
  TEST_CHECK(gs.equipment.at(EquipSlot::Weapon) == ItemId::ScrapBlade);
  // TitaniumBlade in bank must remain 2
  TEST_CHECK(gs.bank.item_qty(ItemId::TitaniumBlade) == 2);

  // Now reduce TitaniumBlade quantity to 1 so that equipping it WILL free a slot
  gs.bank.remove_item(ItemId::TitaniumBlade, 1);
  TEST_CHECK(gs.bank.item_qty(ItemId::TitaniumBlade) == 1);

  // Now equipping TitaniumBlade should succeed and place ScrapBlade into the freed slot
  res = gs.equip_item(ItemId::TitaniumBlade);
  TEST_CHECK(res);
  TEST_CHECK(gs.equipment.at(EquipSlot::Weapon) == ItemId::TitaniumBlade);
  TEST_CHECK(gs.bank.item_qty(ItemId::ScrapBlade) == 1);
  TEST_CHECK(gs.bank.item_qty(ItemId::TitaniumBlade) == 0);

  std::cout << "[PASSED] test_equip_rollback_on_full_vault" << std::endl;
}

void test_save_load_roundtrip_v3() {
  std::cout << "[RUNNING] test_save_load_roundtrip_v3..." << std::endl;

  GameState gs1;
  gs1.credits = 123456;
  gs1.bounty_tokens = 789;
  gs1.combat.player_hp = 45;
  gs1.stats.monster_kills[MonsterId::StrayServoDrone] = 111;
  gs1.stats.monster_kills[MonsterId::Nexus9RogueOvermind] = 42; // Nexus-9 boss kills
  gs1.stats.total_monsters_killed = 153;
  gs1.equipment[EquipSlot::Weapon] = ItemId::NeutroniumBlade;

  std::filesystem::path test_save =
      std::filesystem::temp_directory_path() / "routineverse_test_v3.save";

  bool saved = gs1.save_to_file(test_save.string());
  TEST_CHECK(saved);

  GameState gs2;
  bool loaded = gs2.load_from_file(test_save.string());
  TEST_CHECK(loaded);

  TEST_CHECK(gs2.credits == 123456);
  TEST_CHECK(gs2.bounty_tokens == 789);
  TEST_CHECK(gs2.combat.player_hp == 45);
  TEST_CHECK(gs2.stats.monster_kills[MonsterId::StrayServoDrone] == 111);
  TEST_CHECK(gs2.stats.monster_kills[MonsterId::Nexus9RogueOvermind] == 42);
  TEST_CHECK(gs2.stats.total_monsters_killed == 153);
  TEST_CHECK(gs2.equipment.at(EquipSlot::Weapon) == ItemId::NeutroniumBlade);

  std::error_code ec;
  std::filesystem::remove(test_save, ec);

  std::cout << "[PASSED] test_save_load_roundtrip_v3" << std::endl;
}

void test_reactor_upgrade_and_xp() {
  std::cout << "[RUNNING] test_reactor_upgrade_and_xp..." << std::endl;

  GameState gs;
  gs.credits = 10000;
  // Give player SynthCook level 15, but keep Recycling at level 1
  while (gs.skill_level(SkillType::SynthCook) < 15) {
    gs.skills.xp[SkillType::SynthCook] += 1000;
  }
  TEST_CHECK(gs.skill_level(SkillType::Recycling) == 1);
  TEST_CHECK(gs.skill_level(SkillType::SynthCook) >= 15);

  // Buying Tier 1 Plasteel Thermal Unit requires Level 10 Recycling OR SynthCook
  bool bought = gs.buy_reactor_upgrade();
  TEST_CHECK(bought);
  TEST_CHECK(gs.reactor_tier() == 1);
  TEST_CHECK(gs.equipment.at(EquipSlot::Reactor) == ItemId::PlasteelReactor);

  // Tier 1 PlasteelReactor speed_bonus_pct is 5%
  int reactor_bonus_pct = gs.equipment.speed_bonus_pct(EquipSlot::Reactor);
  TEST_CHECK(reactor_bonus_pct == 5);

  // Gain XP in SynthCook: should receive 5% bonus
  uint64_t sc_before = gs.skill_xp(SkillType::SynthCook);
  gs.start_skill_action(0); // stop whatever was running
  gs.stop_activity();

  auto syn_actions = actions_for_skill(SkillType::SynthCook);
  TEST_CHECK(!syn_actions.empty());
  int act_id = syn_actions[0];
  const auto& act = all_actions()[act_id];
  // Add input ingredients if any
  if (is_valid_item(act.input_item_1)) gs.bank.add_item(act.input_item_1, 10);
  if (is_valid_item(act.input_item_2)) gs.bank.add_item(act.input_item_2, 10);

  sc_before = gs.skill_xp(SkillType::SynthCook);
  gs.start_skill_action(act_id);
  gs.tick(gs.info.active_target_ms + 100);

  uint64_t sc_gained = gs.skill_xp(SkillType::SynthCook) - sc_before;
  uint64_t expected_bonus = (act.xp * 5) / 100;
  TEST_CHECK(sc_gained == static_cast<uint64_t>(act.xp) + expected_bonus);

  std::cout << "[PASSED] test_reactor_upgrade_and_xp" << std::endl;
}

void test_auto_eat_behavior() {
  std::cout << "[RUNNING] test_auto_eat_behavior..." << std::endl;

  GameState gs;
  // Install AutoStim Mk 1 (25% threshold)
  gs.equipment[EquipSlot::AutoStim] = ItemId::AutoStimMk1;
  TEST_CHECK(gs.auto_stim_tier() == 1);
  int threshold = gs.auto_eat_threshold_hp();
  TEST_CHECK(threshold == (gs.max_hp() * 25) / 100);

  // Load Krill Rations (+30 HP) into stim injector
  gs.bank.clear();
  gs.bank.add_item(ItemId::KrillRation, 5);
  gs.equip_food(ItemId::KrillRation);
  TEST_CHECK(gs.equipment.food_qty == 5);

  // Reduce HP to below threshold
  gs.combat.player_hp = threshold - 5;
  gs.check_auto_eat();

  // Auto-eat should have injected a ration and healed the player
  TEST_CHECK(gs.combat.player_hp > threshold);
  TEST_CHECK(gs.equipment.food_qty == 4);

  // Test that starting combat when below threshold auto-eats immediately
  gs.combat.player_hp = threshold - 5;
  gs.start_combat(MonsterId::StrayServoDrone);
  TEST_CHECK(gs.combat.player_hp > threshold);
  TEST_CHECK(gs.equipment.food_qty == 3);

  std::cout << "[PASSED] test_auto_eat_behavior" << std::endl;
}

int main() {
  std::cout << "========================================" << std::endl;
  std::cout << "Running Routineverse Test Suite" << std::endl;
  std::cout << "========================================" << std::endl;

  test_enum_safety();
  test_equip_rollback_on_full_vault();
  test_save_load_roundtrip_v3();
  test_reactor_upgrade_and_xp();
  test_auto_eat_behavior();

  std::cout << "========================================" << std::endl;
  std::cout << "All Routineverse tests passed successfully!" << std::endl;
  std::cout << "========================================" << std::endl;
  return 0;
}
