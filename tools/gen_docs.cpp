#include <algorithm>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "routineverse.hpp"

namespace {

std::string format_sec(int ms) {
  return std::format("{:.1f}s", static_cast<double>(ms) / 1000.0);
}

std::string strip_paren_suffix(std::string_view s) {
  auto pos = s.find(" (");
  if (pos != std::string_view::npos) {
    return std::string(s.substr(0, pos));
  }
  return std::string(s);
}

const SkillAction* find_action_producing(SkillType skill, ItemId item) {
  for (SkillActionId aid : actions_for_skill(skill)) {
    const auto& act = get_skill_action(aid);
    if (act.product_item == item) {
      return &act;
    }
  }
  return nullptr;
}

int action_req_level(SkillType skill, ItemId item) {
  if (const auto* act = find_action_producing(skill, item)) {
    return act->req_level;
  }
  return 1;
}

std::string node_lvl_item(SkillType skill, ItemId item) {
  return std::format("Lv {}: {}", action_req_level(skill, item), item_name(item));
}

void write_file(const std::filesystem::path& path, std::string_view content) {
  std::ofstream out(path, std::ios::binary | std::ios::trunc);
  if (!out) {
    throw std::runtime_error(
        std::format("Failed to open file for writing: {}", path.string()));
  }
  out.write(content.data(), static_cast<std::streamsize>(content.size()));
  std::cout << "Generated " << path.string() << "\n";
}

void generate_readme(const std::filesystem::path& docs_dir) {
  int total_skills = static_cast<int>(all_skills().size());
  int combat_skills = 0;
  int non_combat_skills = 0;
  for (SkillType s : all_skills()) {
    if (is_combat_skill(s)) {
      ++combat_skills;
    } else {
      ++non_combat_skills;
    }
  }

  size_t cutter_tiers = cutter_upgrades().size();
  size_t harvester_tiers = harvester_upgrades().size();
  size_t drill_tiers = drill_upgrades().size();
  size_t reactor_tiers = reactor_upgrades().size();
  size_t autostim_tiers =
      auto_stim_upgrades().size() > 0 ? auto_stim_upgrades().size() - 1 : 0;
  size_t weapon_tiers = items_by_category(ItemCategory::Weapon).size();
  size_t armor_tiers = items_by_category(ItemCategory::Armor).size();
  size_t firewall_tiers = items_by_category(ItemCategory::Firewall).size();
  size_t atk_ice_tiers = items_by_category(ItemCategory::AttackIce).size();

  std::string out;
  out += "# Routineverse Documentation\n\n";
  out +=
      "Welcome to the **Routineverse** documentation! **Routineverse** is a C++20 "
      "cyberpunk idle RPG featuring both a **Qt6 GUI** (`routineverse`) and an "
      "**ncursesw TUI** (`routineverse-tui`) backed by a shared simulation library "
      "(`libroutineverse`).\n\n";
  out += "## Contents\n\n";
  out +=
      "- **[Logistics & Production Chains (`logistics.md`)](./logistics.md)** — "
      "Detailed Mermaid flowcharts and recipe tables for all gathering, recycling, "
      "smelting, smithing, cyber-fabrication (gear, data crystals & CPU/RAM hardware), "
      "hydroponic farming, noodle milling, and cyber-ramen synthesis chains.\n";
  out += std::format(
      "- **[Hacking, ICE Pools & Cyber-Warfare (`hacking.md`)](./hacking.md)** — "
      "Complete guide to the **{} (`{}`)** and **{} (`{}`)** skills, CPU & RAM "
      "hardware components, Attack ICE & Defense/Repair ICE pools, {}-tier "
      "**Firewalls (`EquipSlot::Firewall`)**, Integrity Patches, and Metaverse Grid "
      "cyber-combat formulas.\n",
      skill_name(SkillType::Hacking), skill_short_name(SkillType::Hacking),
      skill_name(SkillType::Integrity), skill_short_name(SkillType::Integrity),
      firewall_tiers);
  out += std::format(
      "- **[Combat, Gear Tiers & Hostile Sectors (`combat.md`)](./combat.md)** — "
      "Dual **{} ({}) + {} ({})** combat mechanics, {}-tier weapon & cyber-armor "
      "progression (`Scrap` through `Quantum`), Auto-Stim & Defense ICE auto-repair "
      "thresholds, Bounty contracts, and all {} hostile drop tables.\n\n",
      skill_name(SkillType::Hitpoints), skill_short_name(SkillType::Hitpoints),
      skill_name(SkillType::Integrity), skill_short_name(SkillType::Integrity),
      weapon_tiers, all_monster_ids().size());
  out += "---\n\n";
  out += "## High-Level Ecosystem & Skill Loop\n\n";
  out += std::format(
      "Routineverse features **{} skills** (**{} non-combat protocols** and **{} "
      "combat/bounty skills**) that form an interconnected resource and progression "
      "loop:\n\n",
      total_skills, non_combat_skills, combat_skills);

  out += R"MD(```mermaid
flowchart LR
  subgraph Extraction["1. Extraction Protocols"]
    SLV["Salvaging (SLV)<br/>Tech Scrap, CPUs, RAM & Cores"]
    MIN["Deep-Mining (MIN)<br/>Ores, Carbon Cells & Crystals"]
    BIO["Fishing (FSH)<br/>Raw Synth-Biota"]
    FRM["Farming (FRM)<br/>Hydroponic Crops & Wheat"]
  end

  subgraph Processing["2. Processing & Refinement"]
    REC["Recycling (REC)<br/>Raw Materials + Carbon Cell Proc"]
    SMT_BAR["Smithing (SMT)<br/>Smelt Alloy Ingots"]
    NOODLE["Farming / Synth-Cook<br/>Mill Synth-Noodles"]
  end

  subgraph Fabrication["3. Fabrication, Hacking & Culinary"]
    SMT_GEAR["Smithing (SMT)<br/>Mono-Blades & Exo-Suits"]
    FAB["Cyber-Fab (FAB)<br/>Visors, Shields, CPUs/RAM & Crystals"]
    HCK["Hacking (HCK)<br/>Attack/Defense ICE, Firewalls & Patches"]
    COOK["Synth-Cook (SYN)<br/>Biota Stims & Cyber-Ramen"]
  end

  subgraph Arena["4. Dual Combat, Bounty & Shop"]
    CMB["Combat & Bounty<br/>ATK / STR / DEF / HP / INT / BNT"]
    SHOP["Cyber-Shop<br/>Tool Tiers, Auto-Stim & Vault"]
  end

  SLV -->|"Scrap"| REC
  SLV -->|"Scrap CPU / RAM"| HCK
  REC -->|"25% Carbon Cell Proc"| SMT_BAR
  MIN -->|"Ores + Carbon Cells"| SMT_BAR
  SMT_BAR -->|"Alloy Ingots"| SMT_GEAR
  SMT_BAR -->|"Alloy Ingots"| FAB
  REC -->|"Recycled Raw Materials"| FAB
  FAB -->|"CPUs + RAM"| HCK

  FRM -->|"Hydro-Wheat"| NOODLE
  NOODLE -->|"Synth-Noodles"| COOK
  FRM -->|"Hydroponic Crops"| COOK
  BIO -->|"Raw Synth-Biota"| COOK

  SMT_GEAR -->|"Weapons & Armor"| CMB
  FAB -->|"Visors & Shields"| CMB
  HCK -->|"ICE Pools, Firewalls & Patches"| CMB
  COOK -->|"Stims & Food"| CMB

  CMB -->|"Credits & Loot"| SHOP
  SHOP -->|"Speed & XP Upgrades"| Extraction
  SHOP -->|"Synth-Reactor Bonus"| Processing
```

---

## Skill Overview

| Skill | Short | Category | Primary Role | Tool / Loadout Upgrade |
|-------|:-----:|----------|--------------|------------------------|
)MD";

  for (SkillType s : all_skills()) {
    std::string cat;
    std::string role;
    std::string upgrade;
    switch (s) {
      case SkillType::Salvaging:
        cat = "Extraction";
        role =
            "Strip tech scrap, Scrap CPUs/RAM, optic fibers, plasma conduits, and AI "
            "cores";
        upgrade = std::format("Salvaging Cutter (T1–T{})", cutter_tiers);
        break;
      case SkillType::Fishing:
        cat = "Extraction";
        role =
            "Harvest aquatic synth-biota from Krill to Cyber-Kraken (+5% Data-Cache Cr "
            "proc)";
        upgrade = std::format("Bio-Harvester (T1–T{})", harvester_tiers);
        break;
      case SkillType::Farming:
        cat = "Extraction";
        role =
            "Cultivate hydroponic crops, mill Synth-Noodles (+20% bonus Hydro-Wheat "
            "proc)";
        upgrade = std::format("Bio-Harvester (T1–T{})", harvester_tiers);
        break;
      case SkillType::Recycling:
        cat = "Processing";
        role = "Recycle scrap into Raw Materials (+25% Carbon Cell proc & Credits)";
        upgrade = std::format("Synth-Reactor (T1–T{})", reactor_tiers);
        break;
      case SkillType::SynthCook:
        cat = "Culinary";
        role = "Prep Synth-Noodles, cook Cyber-Ramen bowls, and synthesize Biota Stims";
        upgrade = std::format("Synth-Reactor (T1–T{})", reactor_tiers);
        break;
      case SkillType::DeepMining:
        cat = "Extraction";
        role = "Mine industrial ores and Carbon Cells (+8% rare Data Crystal proc)";
        upgrade = std::format("Mining Drill (T1–T{})", drill_tiers);
        break;
      case SkillType::Smithing:
        cat = "Fabrication";
        role = "Smelt ores into Alloy Ingots; forge Mono-Blades and Exo-Suits";
        upgrade = "Mastery Preservation & Speed";
        break;
      case SkillType::CyberFab:
        cat = "Fabrication";
        role =
            "Combine Alloy Ingots + Raw Materials into Visors, Shields, CPUs/RAM & "
            "Crystals";
        upgrade = "Mastery Preservation & Speed";
        break;
      case SkillType::Hacking:
        cat = "Cyberware / Net";
        role =
            "Code Attack ICE, Defense/Repair ICE, Firewalls & Integrity Patches; "
            "scales ICE hit";
        upgrade = std::format("Attack ICE Pool (v1.0–v{}.0)", atk_ice_tiers);
        break;
      case SkillType::Attack:
        cat = "Combat";
        role = "Increases melee Accuracy rating and unlocks higher weapon tiers";
        upgrade = std::format("Mono-Blades (T1–T{})", weapon_tiers);
        break;
      case SkillType::Strength:
        cat = "Combat";
        role = "Increases maximum physical hit damage per weapon cycle";
        upgrade = std::format("Mono-Blades (T1–T{})", weapon_tiers);
        break;
      case SkillType::Defence:
        cat = "Combat";
        role = "Increases Evasion rating and unlocks higher cyber-armor tiers";
        upgrade = std::format("Head, Armor, Shield (T1–T{})", armor_tiers);
        break;
      case SkillType::Hitpoints:
        cat = "Combat";
        role =
            "Determines max HP (`Level × 10`, starts at Lv 10 = 100 HP) and nanite "
            "regen";
        upgrade =
            std::format("Auto-Stim Injector (Mk I–{})",
                        autostim_tiers == 3 ? "III" : std::to_string(autostim_tiers));
        break;
      case SkillType::Integrity:
        cat = "Combat";
        role =
            "Determines max Integrity (`Level × 10 + Firewall`, starts at Lv 10 = 100 "
            "INT)";
        upgrade = std::format("Firewalls (T1–T{}) & Defense ICE", firewall_tiers);
        break;
      case SkillType::Bounty:
        cat = "Combat";
        role =
            "Unlocks high-security physical & Metaverse Grid targets and awards Bounty "
            "Tokens (`BT`)";
        upgrade = "Bounty Contracts";
        break;
    }
    out += std::format("| **{}** | `{}` | {} | {} | {} |\n", skill_name(s),
                       skill_short_name(s), cat, role, upgrade);
  }

  out += "\n---\n\n";
  out += "## Item Categories\n\n";
  out += "| Category | Display Name | Produced By | Used By / Purpose |\n";
  out += "|----------|--------------|-------------|-------------------|\n";

  struct CatDoc {
    ItemCategory cat;
    const char* enum_name;
    const char* produced_by;
    const char* purpose;
  };
  const std::vector<CatDoc> cat_docs = {
      {ItemCategory::Scrap, "`Scrap`", "Salvaging, Hostile Drops",
       "Input for **Recycling**"},
      {ItemCategory::RawMaterial, "`RawMaterial`", "Recycling",
       "Input for **Cyber-Fab** and **Hacking**"},
      {ItemCategory::RawBiota, "`RawBiota`", "Fishing",
       "Input for **Synth-Cook** (Stims & Kraken Ramen)"},
      {ItemCategory::Crop, "`Crop`", "Farming, Synth-Cook",
       "Hydroponic crops & **Synth-Noodles** for **Cyber-Ramen**"},
      {ItemCategory::StimFood, "`StimFood`", "Synth-Cook, Hostile Drops",
       "Loaded into **Stim-Injector** to restore HP in combat"},
      {ItemCategory::ToxicWaste, "`ToxicWaste`", "Synth-Cook (failed synthesis)",
       "Can be liquidated for 1 Cr"},
      {ItemCategory::RawOre, "`RawOre`",
       "Deep-Mining, Recycling (`CarbonCell` proc), Drops",
       "Input for **Smithing** (ingots) and **Cyber-Fab** (`SiliconOre`)"},
      {ItemCategory::Alloy, "`Alloy`", "Smithing",
       "Input for **Smithing** (Blades/Exo-Suits) and **Cyber-Fab** "
       "(Visors/Shields/CPUs/Crystals)"},
      {ItemCategory::DataCrystal, "`DataCrystal`",
       "Cyber-Fab, Deep-Mining (8% proc), Drops",
       "High-value tech cores & inputs for high-tier Firewalls/RAM"},
      {ItemCategory::Hardware, "`Hardware`", "Salvaging, Cyber-Fab, Hostile Drops",
       "**CPUs** & **RAM** modules used as inputs for **Hacking**"},
      {ItemCategory::Weapon, "`Weapon`", "Smithing, Hostile Drops",
       "Equippable in `Weapon` slot (+Attack, +Strength)"},
      {ItemCategory::Head, "`Head`", "Cyber-Fab",
       "Equippable in `Head` slot (+Defence, +% DR)"},
      {ItemCategory::Armor, "`Armor`", "Smithing, Hostile Drops",
       "Equippable in `Armor` slot (+Defence, +% DR)"},
      {ItemCategory::Shield, "`Shield`", "Cyber-Fab",
       "Equippable in `Shield` slot (+Defence, +% DR)"},
      {ItemCategory::Firewall, "`Firewall`", "Hacking, Hostile Drops",
       "Equippable in `Firewall` slot (+Max INT, +Cyber Evasion, +ICE DR%)"},
      {ItemCategory::AttackIce, "`AttackIce`", "Hacking, Hostile Drops",
       "Loaded into **Attack ICE Pool** (+Cyber Accuracy, +ICE Max Hit; 1 "
       "consumed/attack)"},
      {ItemCategory::DefenseIce, "`DefenseIce`", "Hacking",
       "Loaded into **Defense ICE Pool** (+Cyber Evasion, +ICE DR%, auto-repairs INT)"},
      {ItemCategory::IntegrityPatch, "`IntegrityPatch`", "Hacking, Hostile Drops",
       "Restores **Integrity** (`+INT`) and/or grants permanent **Integrity XP**"},
  };
  for (const auto& c : cat_docs) {
    out += std::format("| {} | {} | {} | {} |\n", c.enum_name,
                       item_category_name(c.cat), c.produced_by, c.purpose);
  }
  out += std::format(
      "| `Cutter` / `Harvester` / `Drill` / `Reactor` / `AutoStim` | {} / {} / {} / {} "
      "/ {} | Cyber-Shop, Hostile Drops | Equippable in tool slots to reduce skill "
      "cycle time or raise Auto-Stim threshold |\n",
      item_category_name(ItemCategory::Cutter),
      item_category_name(ItemCategory::Harvester),
      item_category_name(ItemCategory::Drill),
      item_category_name(ItemCategory::Reactor),
      item_category_name(ItemCategory::AutoStim));
  out += std::format(
      "| `CyberLoot` | {} | Hostile Drops | Tradeable mechanical salvage for Credits "
      "|\n",
      item_category_name(ItemCategory::CyberLoot));

  write_file(docs_dir / "README.md", out);
}

void generate_combat(const std::filesystem::path& docs_dir) {
  auto weapons = items_by_category(ItemCategory::Weapon);
  auto visors = items_by_category(ItemCategory::Head);
  auto armors = items_by_category(ItemCategory::Armor);
  auto shields = items_by_category(ItemCategory::Shield);
  auto firewalls = items_by_category(ItemCategory::Firewall);

  size_t num_tiers =
      std::min({weapons.size(), visors.size(), armors.size(), shields.size()});

  std::vector<std::string> slum_unconnected;
  std::vector<std::string> other_unconnected;
  std::vector<std::string> pure_grid;
  std::string boss_short_name = "NEXUS-9";
  for (MonsterId mid : all_monster_ids()) {
    const auto& m = get_monster_info(mid);
    if (m.max_hp > 0 && m.max_integrity == 0) {
      if (m.zone == ZoneId::NeonSlums) {
        slum_unconnected.push_back(std::format("`{}`", m.name));
      } else {
        other_unconnected.push_back(std::format("`{}`", m.name));
      }
    } else if (m.max_hp == 0 && m.max_integrity > 0) {
      pure_grid.push_back(std::format("`{}`", m.name));
    }
    if (m.is_boss) {
      std::string_view sv(m.name);
      auto comma = sv.find(',');
      boss_short_name =
          comma != std::string_view::npos ? std::string(sv.substr(0, comma)) : m.name;
    }
  }

  auto join_list = [](const std::vector<std::string>& v) {
    std::string s;
    for (size_t i = 0; i < v.size(); ++i) {
      if (i > 0) s += ", ";
      s += v[i];
    }
    return s;
  };

  std::string out;
  out += "# Combat, Gear Tiers & Hostile Sectors\n\n";
  out += std::format(
      "This document covers the dual **Hitpoints (HP) + Integrity (INT)** combat "
      "simulation loop, the **{} tiers of weapons and cyber-armor** (`Scrap` through "
      "`Quantum`), Bounty contracts, and all **{} hostile targets** across physical "
      "and **Metaverse Grid** sectors.\n\n",
      num_tiers, all_monster_ids().size());
  out +=
      "For **Attack & Defense ICE Pools**, **Firewalls (`EquipSlot::Firewall`)**, and "
      "**Integrity Patches**, see **[Hacking, ICE Pools & Cyber-Warfare "
      "(`hacking.md`)](./hacking.md)**.\n\n";
  out += "---\n\n";
  out += "## 1. Dual Combat (HP & Integrity) & Bounty Loop\n\n";
  out += "Combat in **Routineverse** operates simultaneously on two planes:\n";
  out +=
      "1. **Physical Plane (`HP`)**: Melee weapons strike enemy `HP`, while enemy "
      "physical attacks damage your `HP` (mitigated by Armor/Visor/Shield `DR%` and "
      "healed by the **Auto-Stim Injector**).\n";
  out +=
      "2. **Cyber / Metaverse Plane (`INT`)**: Loaded **Attack ICE** bots strike enemy "
      "`Integrity` (`INT`) and have a **35% chance on hit to destroy 1 enemy ICE "
      "bot**, while enemy ICE bots either self-repair the enemy's Integrity (25% "
      "chance when damaged) or attack your `Integrity` (mitigated by Firewall/Defense "
      "ICE `Cyber DR%` and auto-repaired by **Defense ICE** at $\\le 65\\%$ "
      "Integrity).\n\n";
  out +=
      "- **Dual Victory Condition**: Depleting **either** a target's `HP` (if `max_hp "
      "> 0`) **or** its `Integrity` (if `max_integrity > 0`) immediately neutralizes "
      "the hostile!\n";
  out += "- **Three Hostile Archetypes**:\n";
  out += std::format(
      "  - **Unconnected Hostiles (`max_hp > 0, max_integrity = 0`)**: Found in the "
      "**{}** ({}) and organic mutants ({}). They are not connected to the net—Attack "
      "ICE is never consumed against them, and they must be defeated via physical `HP` "
      "damage.\n",
      zone_name(ZoneId::NeonSlums), join_list(slum_unconnected),
      join_list(other_unconnected));
  out += std::format(
      "  - **Pure Metaverse Terminals & AIs (`max_hp = 0, max_integrity > 0`)**: "
      "Found in the **{}** ({}). They have no physical body and can only be damaged "
      "via **Attack ICE** (or neural deck cyber pulses), while attacking your "
      "`Integrity` directly.\n",
      zone_name(ZoneId::MetaverseGrid), join_list(pure_grid));
  out += std::format(
      "  - **Connected Hybrid Hostiles (`max_hp > 0, max_integrity > 0`)**: Networked "
      "cyborgs, security mechs, and **{}**. They fight with both physical weapons and "
      "an active **ICE Pool**, and can be defeated by zeroing out either their `HP` or "
      "their `Integrity`.\n\n",
      boss_short_name);

  out += R"MD(```mermaid
flowchart TD
  START["Select Hostile Target<br/>(Check Bounty Lv Clearance)"] --> TIMERS["Advance Weapon & Hostile Timers<br/>+ Passive Regen (+1% HP & +1% INT / 5s)"]

  TIMERS --> PLR_ATK{"Player Cycle<br/>(2.4s)?"}
  PLR_ATK -->|"Physical Strike (if Foe HP > 0)"| PHYS_DMG["Hit Roll vs Foe Evasion<br/>Deal HP Damage & Award ATK/STR/DEF + HP XP"]
  PLR_ATK -->|"Cyber Strike (if Foe INT > 0)"| ICE_DMG["Consume 1 Attack ICE Bot<br/>Deal INT Damage, 35% Foe ICE Break,<br/>Award Hacking + Integrity XP"]

  PHYS_DMG & ICE_DMG --> FOE_DEAD{"Foe HP <= 0<br/>OR Foe INT <= 0?"}

  FOE_DEAD -->|"Yes"| LOOT["Award Credits, Roll Drop Table,<br/>Advance Bounty Contract (+BT & BNT XP)"]
  LOOT --> RESPAWN["Respawn Hostile (Full HP, INT & ICE)"] --> TIMERS

  FOE_DEAD -->|"No"| MON_ATK{"Hostile Cycle?"}
  PLR_ATK -->|"No"| MON_ATK

  MON_ATK -->|"Physical Hit (if MaxHit > 0)"| MON_PHYS["Apply HP Damage Reduced by DR%<br/>Trigger Auto-Stim if HP <= Threshold"]
  MON_ATK -->|"Enemy ICE (if ICE Pool > 0 or Grid AI)"| MON_ICE["25% Self-Repair (+10% Foe INT)<br/>or Attack Player INT (Reduced by Cyber DR%)<br/>Trigger Defense ICE if INT <= 65%"]

  MON_PHYS & MON_ICE --> PLR_DEAD{"Player HP <= 0<br/>OR Player INT <= 0?"}

  PLR_DEAD -->|"Flatline / Neural Crash"| TRAUMA["Trauma Team / NetWatch Reconstruction<br/>(-10% Credits, Stop Activity)"]
  PLR_DEAD -->|"Alive"| TIMERS
```

---
)MD";

  out += std::format("\n## 2. {}-Tier Weapon & Cyber-Armor Progression\n\n", num_tiers);
  out += std::format(
      "Weapons (`EquipSlot::Weapon`) and Exo-Suits (`EquipSlot::Armor`) are forged via "
      "**Smithing**, while Visors (`EquipSlot::Head`) and Holo-Shields "
      "(`EquipSlot::Shield`) are fabricated via **Cyber-Fab**. In addition, operatives "
      "can equip {} tiers of **Firewalls** (`EquipSlot::Firewall`) coded via "
      "**Hacking** (see [`hacking.md`](./hacking.md#4-firewalls--barriers-"
      "equipslotfirewall)).\n\n",
      firewalls.size());

  const std::vector<std::string> tier_materials = {"Scrap",  "Titanium", "Durasteel",
                                                   "Cobalt", "Tungsten", "Neutronium",
                                                   "Chrono", "Quantum"};

  out += "```mermaid\nflowchart LR\n";
  for (size_t i = 0; i < num_tiers; ++i) {
    const auto& w = get_item_info(weapons[i]);
    const auto& h = get_item_info(visors[i]);
    const auto& a = get_item_info(armors[i]);
    const auto& s = get_item_info(shields[i]);
    int tot_dr =
        h.bonus.damage_reduction + a.bonus.damage_reduction + s.bonus.damage_reduction;
    std::string mat = i < tier_materials.size() ? tier_materials[i] : w.name;
    if (i == 0) {
      out += std::format("  T1[\"Tier 1: {}<br/>(Req Lv {}, Tot DR: {}%)\"]", mat,
                         w.req_level, tot_dr);
    } else {
      out += std::format(" --> T{}[\"Tier {}: {}<br/>(Req Lv {}, Tot DR: {}%)\"]\n",
                         i + 1, i + 1, mat, w.req_level, tot_dr);
      if (i + 1 < num_tiers) {
        out += std::format("  T{}", i + 1);
      }
    }
  }
  out += "```\n\n";

  out += "### Complete Physical Gear Stats Table\n\n";
  out +=
      "| Tier | Material | Req Lv | Mono-Blade (`Atk / Str`) | Visor (`Def / DR%`) | "
      "Exo-Suit (`Def / DR%`) | Holo-Shield (`Def / DR%`) | Full Set Bonus (`Def / "
      "DR%`) |\n";
  out +=
      "|:----:|----------|:------:|--------------------------|---------------------|---"
      "---------------------|---------------------------|:----------------------------:"
      "|\n";

  for (size_t i = 0; i < num_tiers; ++i) {
    const auto& w = get_item_info(weapons[i]);
    const auto& h = get_item_info(visors[i]);
    const auto& a = get_item_info(armors[i]);
    const auto& s = get_item_info(shields[i]);
    int tot_def = h.bonus.defence + a.bonus.defence + s.bonus.defence;
    int tot_dr =
        h.bonus.damage_reduction + a.bonus.damage_reduction + s.bonus.damage_reduction;
    std::string mat = i < tier_materials.size() ? tier_materials[i] : w.name;
    out += std::format(
        "| **{}** | **{}** | Lv {} | `+{} Atk / +{} Str` | `+{} Def / {}% DR` | "
        "`+{} Def / {}% DR` | `+{} Def / {}% DR` | `+{} Def / {}% DR` |\n",
        i + 1, mat, w.req_level, w.bonus.attack, w.bonus.strength, h.bonus.defence,
        h.bonus.damage_reduction, a.bonus.defence, a.bonus.damage_reduction,
        s.bonus.defence, s.bonus.damage_reduction, tot_def, tot_dr);
  }

  out += "\n---\n\n";
  out += "## 3. Hostile Sectors & Salvage Drop Tables\n\n";
  out +=
      "| Hostile Target | Sector | Lv | Bounty Req | HP | INT | Phys / ICE Hit | ICE "
      "Pool | Credits | Salvage, Hardware, ICE & Gear Drops |\n";
  out +=
      "|----------------|--------|---:|-----------:|---:|----:|:--------------:|:------"
      "--:|--------:|-------------------------------------|\n";

  for (MonsterId mid : all_monster_ids()) {
    const auto& m = get_monster_info(mid);
    std::string sector = zone_name(m.zone);
    auto boss_pos = sector.find("[BOSS]");
    if (boss_pos != std::string::npos) {
      sector.replace(boss_pos, 6, "`[BOSS]`");
    }

    std::string drops_str;
    for (const auto& d : m.drops) {
      if (!is_valid_item(d.item_id)) continue;
      if (!drops_str.empty()) drops_str += ", ";
      drops_str += std::format("`{}` ({}%)", item_name(d.item_id), d.chance_pct);
    }

    out += std::format(
        "| **{}** | {} | {} | Lv {} | {} | {} | `{} / {}` | {} | {}–{} Cr | {} |\n",
        m.name, sector, m.combat_level, m.bounty_req, number_string(m.max_hp),
        number_string(m.max_integrity), m.max_hit, m.ice_max_hit, m.ice_pool,
        number_string(m.credits_min), number_string(m.credits_max), drops_str);
  }

  write_file(docs_dir / "combat.md", out);
}

void generate_hacking(const std::filesystem::path& docs_dir) {
  auto hardware_items = items_by_category(ItemCategory::Hardware);
  auto atk_ice_items = items_by_category(ItemCategory::AttackIce);
  auto def_ice_items = items_by_category(ItemCategory::DefenseIce);
  auto firewall_items = items_by_category(ItemCategory::Firewall);
  auto patch_span = items_by_category(ItemCategory::IntegrityPatch);
  std::vector<ItemId> patch_items(patch_span.begin(), patch_span.end());

  std::sort(patch_items.begin(), patch_items.end(), [](ItemId a, ItemId b) {
    return get_item_info(a).req_level < get_item_info(b).req_level;
  });

  // Separate CPUs and RAMs in tier order
  std::vector<ItemId> cpus;
  std::vector<ItemId> rams;
  for (ItemId id : hardware_items) {
    std::string_view name = get_item_info(id).name;
    if (name.find("CPU") != std::string_view::npos ||
        name.find("Processor") != std::string_view::npos) {
      cpus.push_back(id);
    } else {
      rams.push_back(id);
    }
  }

  const auto& slv_cpu = get_skill_action(SkillActionId::ScavengeLogicCpu);
  const auto& slv_ram = get_skill_action(SkillActionId::PullScrapDramStick);

  std::string out;
  out += "# Hacking, ICE Pools & Cyber-Warfare\n\n";
  out += std::format(
      "This document details the **{} (`{}`)** and **{} (`{}`)** skills, **CPU & RAM** "
      "hardware fabrication, **Attack & Defense ICE** pools, **Firewalls "
      "(`EquipSlot::Firewall`)**, **Integrity Patches**, and **Metaverse Grid** "
      "cyber-combat mechanics in **Routineverse**.\n\n",
      skill_name(SkillType::Hacking), skill_short_name(SkillType::Hacking),
      skill_name(SkillType::Integrity), skill_short_name(SkillType::Integrity));
  out += "---\n\n";
  out += "## 1. Cyber-Warfare & Hacking Ecosystem\n\n";
  out += std::format(
      R"MD(```mermaid
flowchart LR
  subgraph Sources["1. Hardware Sources"]
    SLV["Salvaging (SLV)<br/>Lv {} Scrap CPU & Lv {} Scrap RAM"]
    FAB["Cyber-Fab (FAB)<br/>{} CPU Tiers & {} RAM Tiers"]
    DROPS["Hostile & Grid Drops<br/>CPUs, RAMs & ICE Bots"]
  end

  subgraph HackingSkill["2. Hacking Skill (HCK)"]
    ATK_ICE["Attack ICE (v1.0–v{}.0)<br/>5x Bots per Craft"]
    DEF_ICE["Defense/Repair ICE (v1.0–v{}.0)<br/>5x Bots per Craft"]
    FW["Firewalls & Barriers ({} Tiers)<br/>EquipSlot::Firewall"]
    PATCH["Integrity Patches & Firmware<br/>+INT Recovery & +INT XP"]
  end

  subgraph Loadout["3. Operative Cyber-Loadout"]
    POOL_A["Attack ICE Pool<br/>(Consumable Stack)"]
    POOL_D["Defense ICE Pool<br/>(Consumable Stack)"]
    SLOT_FW["Firewall Slot<br/>(+Max INT, +Cyber Eva, +ICE DR%)"]
  end

  subgraph Combat["4. Dual Combat & Metaverse Grid"]
    HYB["Connected Hybrids<br/>(HP > 0 & INT > 0)"]
    GRID["Metaverse Terminals / AIs<br/>(HP = 0, Pure INT)"]
    INT_SKILL["Integrity Skill (INT)<br/>Base Max INT = Lv × 10"]
  end

  SLV & FAB & DROPS -->|"CPUs + RAM"| HackingSkill
  ATK_ICE --> POOL_A
  DEF_ICE --> POOL_D
  FW --> SLOT_FW
  PATCH -->|"Instant Repair / XP"| INT_SKILL

  POOL_A -->|"1 Bot / Attack Cycle<br/>(35% Foe ICE Break)"| HYB & GRID
  POOL_D -->|"Passive Cyber Eva/DR%<br/>+ Auto-Repair <= 65% INT"| INT_SKILL
  SLOT_FW --> INT_SKILL
```
)MD",
      slv_cpu.req_level, slv_ram.req_level, cpus.size(), rams.size(),
      atk_ice_items.size(), def_ice_items.size(), firewall_items.size());

  out += "\n---\n\n";
  out += "## 2. Hardware Components (`ItemCategory::Hardware`)\n\n";
  out +=
      "Coding ICE bots, Firewalls, and Integrity Patches requires **CPU processors** "
      "and **RAM storage modules**. Low-tier hardware can be scavenged directly via "
      "**Salvaging**, while mid-to-endgame hardware is fabricated in **Cyber-Fab** or "
      "looted from **Metaverse Grid** hostiles.\n\n";
  out +=
      "| Hardware Item | Type | Tier | Value | Salvaging / Drop Source | Cyber-Fab "
      "Recipe (`SkillType::CyberFab`) |\n";
  out +=
      "|---------------|:----:|:----:|------:|-------------------------|---------------"
      "---------------------------|\n";

  auto emit_hw_row = [&](ItemId id, std::string_view type_str, size_t tier_idx) {
    const auto& info = get_item_info(id);
    std::string sources;
    if (const auto* slv_act = find_action_producing(SkillType::Salvaging, id)) {
      sources += std::format("`Salvaging` Lv {} (`{}`, {} XP)", slv_act->req_level,
                             format_sec(slv_act->base_interval_ms),
                             number_string(slv_act->xp));
    }
    for (MonsterId mid : all_monster_ids()) {
      const auto& m = get_monster_info(mid);
      for (const auto& d : m.drops) {
        if (d.item_id == id) {
          if (!sources.empty()) sources += ", ";
          sources += std::format("`{}` ({}%)", m.name, d.chance_pct);
        }
      }
    }

    std::string fab_str = "—";
    if (const auto* fab_act = find_action_producing(SkillType::CyberFab, id)) {
      fab_str =
          std::format("Lv {} (`{}`, {} XP): `{}x {} + {}x {}`", fab_act->req_level,
                      format_sec(fab_act->base_interval_ms), number_string(fab_act->xp),
                      fab_act->input_qty_1, item_name(fab_act->input_item_1),
                      fab_act->input_qty_2, item_name(fab_act->input_item_2));
    }

    out += std::format("| **{}** | {} | T{} | {} Cr | {} | {} |\n", info.name, type_str,
                       tier_idx, number_string(info.price), sources, fab_str);
  };

  size_t hw_tiers = std::max(cpus.size(), rams.size());
  for (size_t i = 0; i < hw_tiers; ++i) {
    if (i < cpus.size()) emit_hw_row(cpus[i], "CPU", i + 1);
    if (i < rams.size()) emit_hw_row(rams[i], "RAM", i + 1);
  }

  out += R"MD(
---

## 3. ICE Pools (`AttackIce` & `DefenseIce`)

Unlike permanent gear slots, **ICE Pools** work as stackable, consumable companion pools (`equipment.attack_ice_item` / `attack_ice_qty` and `equipment.defense_ice_item` / `defense_ice_qty`):
- Equipping an ICE item from your Cyber-Vault loads the **entire stack** into your active Attack ICE or Defense ICE pool (swapping any previously loaded ICE type back to the Vault).
- **Attack ICE Pool**:
  - Activates automatically alongside your weapon cycle whenever fighting a target with **Integrity (`max_integrity > 0`)**.
  - Consumes **1 Attack ICE bot** per attack cycle (never consumed against unconnected `0 INT` enemies in the Neon Slums).
  - Deals cyber damage to the enemy's Integrity (`Hacking` & `Integrity` XP awarded) and has a **35% chance on hit to destroy 1 enemy ICE bot** (`monster_ice_qty`).
- **Defense / Repair ICE Pool**:
  - While at least 1 Defense ICE bot is loaded in the pool, it passively grants **Cyber Evasion (`+Def`)** and **ICE Damage Reduction (`+DR%`)**.
  - Whenever your Integrity drops to **$\le 65\%$ of Max Integrity** (or $\le$ your Auto-Stim threshold), 1 Defense ICE bot is automatically consumed to **repair your Integrity**.

### Attack ICE Progression (`ItemCategory::AttackIce`)

| Tier | Attack ICE Bot | Req Hacking Lv | Cyber Accuracy (`+Atk`) | ICE Max Hit (`+Str`) | Unit Value |
|:----:|----------------|:--------------:|:-----------------------:|:--------------------:|-----------:|
)MD";

  for (size_t i = 0; i < atk_ice_items.size(); ++i) {
    const auto& info = get_item_info(atk_ice_items[i]);
    out += std::format(
        "| **v{}.0** | **{}** | Lv {} | `+{} Cyber Acc` | `+{} ICE Str` | {} Cr |\n",
        i + 1, info.name, info.req_level, info.bonus.attack, info.bonus.strength,
        number_string(info.price));
  }

  out += R"MD(
### Defense & Repair ICE Progression (`ItemCategory::DefenseIce`)

| Tier | Defense ICE Bot | Req Hacking Lv | Integrity Repair (`+INT`) | Passive Cyber Evasion (`+Def`) | Passive ICE Barrier (`+DR%`) | Unit Value |
|:----:|-----------------|:--------------:|:-------------------------:|:------------------------------:|:----------------------------:|-----------:|
)MD";

  for (size_t i = 0; i < def_ice_items.size(); ++i) {
    const auto& info = get_item_info(def_ice_items[i]);
    out += std::format(
        "| **v{}.0** | **{}** | Lv {} | `+{} INT` | `+{} Cyber Eva` | `{}% ICE DR` | "
        "{} Cr |\n",
        i + 1, info.name, info.req_level, info.bonus.recover_amount, info.bonus.defence,
        info.bonus.damage_reduction, number_string(info.price));
  }

  out += R"MD(
---

## 4. Firewalls & Barriers (`EquipSlot::Firewall`)

**Firewalls** (`ItemCategory::Firewall`) are permanent cyberware barriers equipped in the dedicated `Firewall` slot (`EquipSlot::Firewall`). They require **Integrity** skill levels to equip and boost your **Max Integrity**, **Cyber Evasion**, and **ICE Damage Reduction %**.

| Tier | Firewall / Barrier | Req Integrity Lv | Max Integrity Bonus (`+INT`) | Cyber Evasion (`+Def`) | ICE Damage Reduction (`+DR%`) | Value |
|:----:|--------------------|:----------------:|:----------------------------:|:----------------------:|:-----------------------------:|------:|
)MD";

  for (size_t i = 0; i < firewall_items.size(); ++i) {
    const auto& info = get_item_info(firewall_items[i]);
    out += std::format(
        "| **{}** | **{}** | Lv {} | `+{} Max INT` | `+{} Cyber Eva` | `{}% ICE DR` | "
        "{} Cr |\n",
        i + 1, info.name, info.req_level, info.bonus.strength, info.bonus.defence,
        info.bonus.damage_reduction, number_string(info.price));
  }

  out += R"MD(
---

## 5. Integrity Recovery & Firmware Patches (`ItemCategory::IntegrityPatch`)

Using (equipping) an **Integrity Patch** from your Cyber-Vault immediately executes the patch to restore Integrity or upgrade your neural firmware:

| Patch / Script | Req Hacking Lv | Integrity Restored | Firmware Bonus | Value |
|----------------|:--------------:|:------------------:|:--------------:|------:|
)MD";

  for (ItemId id : patch_items) {
    const auto& info = get_item_info(id);
    std::string fw_bonus = info.bonus.speed_bonus_pct > 0
                               ? std::format("**Permanent `+{} Integrity XP`**",
                                             info.bonus.speed_bonus_pct)
                               : "—";
    out += std::format("| **{}** | Lv {} | `+{} INT` | {} | {} Cr |\n", info.name,
                       info.req_level, info.bonus.recover_amount, fw_bonus,
                       number_string(info.price));
  }

  out += R"MD(
---

## 6. Complete Hacking Protocols Table (`SkillType::Hacking`)

All Hacking protocols benefit from **Mastery resource preservation** (`5% + 0.2%` per mastery level) and **double output procs** (`5% + 0.3%` per mastery level).

| Lv | Protocol | Base Cycle | XP | Input 1 | Input 2 | Output | Category |
|---:|----------|:----------:|---:|---------|---------|--------|----------|
)MD";

  for (SkillActionId aid : actions_for_skill(SkillType::Hacking)) {
    const auto& act = get_skill_action(aid);
    const auto& prod_info = get_item_info(act.product_item);
    std::string cat_str;
    switch (prod_info.category) {
      case ItemCategory::AttackIce:
        cat_str = "Attack ICE";
        break;
      case ItemCategory::DefenseIce:
        cat_str = "Defense ICE";
        break;
      case ItemCategory::Firewall:
        cat_str = "Firewall";
        break;
      case ItemCategory::IntegrityPatch:
        cat_str = prod_info.bonus.speed_bonus_pct > 0 ? "Patch (+XP)" : "Patch";
        break;
      default:
        cat_str = item_category_name(prod_info.category);
        break;
    }

    out +=
        std::format("| {} | {} | {} | {} | `{}x {}` | `{}x {}` | `{}x {}` | {} |\n",
                    act.req_level, act.name, format_sec(act.base_interval_ms),
                    number_string(act.xp), act.input_qty_1, item_name(act.input_item_1),
                    act.input_qty_2, item_name(act.input_item_2), act.product_qty,
                    item_name(act.product_item), cat_str);
  }

  out += R"MD(
---

## 7. Cyber-Combat Formulas

- **Max Integrity**:
  $$\text{Max Integrity} = (\text{Integrity Level} \times 10) + \text{Firewall Max INT Bonus}$$
- **ICE Max Hit**:
  - With **Attack ICE** loaded:
    $$\text{ICE Max Hit} = \max\left(2, \left\lfloor 2.0 + \text{Hacking Level} \times 0.35 + \text{ICE Str} \times 0.30 + \frac{\text{Hacking Level} \times \text{ICE Str}}{140.0} \right\rfloor\right)$$
  - Without Attack ICE (base neural deck pulse against pure Metaverse Grid targets):
    $$\text{Base Cyber Pulse} = 1 + \lfloor\text{Hacking Level} / 4\rfloor$$
- **Cyber Accuracy, Evasion & Damage Reduction**:
  - $\text{Cyber Accuracy} = (\text{Hacking Level} + 8) \times (\text{Attack ICE Bonus} + 64) / 10$
  - $\text{Cyber Evasion} = (\text{Integrity Level} + \lfloor\text{Hacking Level}/2\rfloor + 8) \times (\text{Firewall Def} + \text{Defense ICE Def} + 64) / 10$
  - $\text{Cyber DR\%} = \min(80\%, \text{Firewall DR\%} + \text{Defense ICE DR\%} + \lfloor\text{Physical DR\%} / 4\rfloor)$
)MD";

  write_file(docs_dir / "hacking.md", out);
}

void generate_logistics(const std::filesystem::path& docs_dir) {
  std::string out;
  out += R"MD(# Logistics & Production Chains

This document details all resource extraction, refinement, fabrication, and culinary logistics chains in **Routineverse**.

---

## 1. End-to-End Logistics Overview

The non-combat economy is divided into three interconnected industrial branches:
1. **Metallurgy & Cyberware Branch**: `Salvaging` + `Deep-Mining` $\rightarrow$ `Recycling` + `Smithing` $\rightarrow$ `Cyber-Fab` (Weapons, Armor, and Data Crystals).
2. **Hardware & Hacking Branch**: `Salvaging` + `Deep-Mining` + `Recycling` + `Smithing` $\rightarrow$ `Cyber-Fab` (CPUs & RAM) $\rightarrow$ `Hacking` (Attack ICE, Defense/Repair ICE, Firewalls, and Integrity Patches — see [`hacking.md`](./hacking.md)).
3. **Bio-Agri & Culinary Branch**: `Fishing` + `Farming` $\rightarrow$ `Synth-Noodles` $\rightarrow$ `Synth-Cook` (Biota Stims and Cyber-Ramen).

```mermaid
flowchart TD
  subgraph Metallurgy["Metallurgy, Hardware & Hacking Branch"]
    S["Salvaging<br/>(9 Scrap Tiers + Scrap CPU/RAM)"]
    R["Recycling<br/>(9 Raw Materials)"]
    M["Deep-Mining<br/>(10 Ores + Carbon Cell)"]
    SM_ING["Smithing: Smelting<br/>(8 Combat Alloys + 2 Conductors)"]
    SM_EQ["Smithing: Forging<br/>(8 Mono-Blades + 8 Exo-Suits)"]
    CF_EQ["Cyber-Fab: Gear<br/>(8 Visors + 8 Holo-Shields)"]
    CF_CR["Cyber-Fab: Tech Cores<br/>(5 Data Crystals)"]
    CF_HW["Cyber-Fab: Hardware<br/>(4 CPU Tiers + 4 RAM Tiers)"]
    HCK["Hacking<br/>(Attack/Defense ICE, Firewalls & Patches)"]

    S -->|"1x Scrap"| R
    S -->|"Scrap CPU / RAM"| HCK
    R -.->|"25% Bonus Proc:<br/>1x Carbon Cell"| SM_ING
    M -->|"Ores + Carbon Cells"| SM_ING
    M -.->|"8% Bonus Proc"| CF_CR
    SM_ING -->|"2x–5x Alloy Ingot"| SM_EQ
    SM_ING -->|"1x–2x Alloy Ingot"| CF_EQ
    R -->|"1x Raw Material"| CF_EQ
    SM_ING -->|"1x Conductor / Alloy"| CF_CR
    R -->|"1x Raw Material"| CF_CR
    M & R & SM_ING & CF_CR -->|"Silicon / Conductors / Wafers / Lattice"| CF_HW
    CF_HW -->|"CPUs + RAM"| HCK
  end

  subgraph Culinary["Bio-Agri & Culinary Branch"]
    BH["Fishing<br/>(9 Raw Synth-Biota)"]
    FM["Farming<br/>(9 Hydroponic Crops)"]
    HW["Hydro-Wheat<br/>(Lv 1 Crop + 20% Farming Proc)"]
    ND["Synth-Noodles<br/>(1x Hydro-Wheat -> 2x Noodles)"]
    SC_STIM["Synth-Cook: Biota Stims<br/>(9 Healing Rations/Stims)"]
    SC_RAMEN["Synth-Cook: Cyber-Ramen<br/>(9 High-Healing Ramen Bowls)"]

    BH -->|"1x Raw Biota"| SC_STIM
    FM -->|"Cultivate Lv 1<br/>or 20% Bonus Proc"| HW
    HW -->|"Farming Lv 3 or<br/>Synth-Cook Lv 1"| ND
    ND -->|"1x Synth-Noodles"| SC_RAMEN
    FM -->|"1x Crop (Soy..Truffle)"| SC_RAMEN
    BH -->|"1x Raw Cyber-Kraken"| SC_RAMEN
  end
```

---

## 2. Salvaging & Recycling Chain

)MD";

  const auto& slv_cpu = get_skill_action(SkillActionId::ScavengeLogicCpu);
  const auto& slv_ram = get_skill_action(SkillActionId::PullScrapDramStick);
  out += std::format(
      "**Salvaging** extracts raw tech scrap (`ItemCategory::Scrap`) as well as "
      "low-tier **CPU/RAM hardware** (`{}` at Lv {} and `{}` at Lv {} for "
      "**Hacking**). **Recycling** refines scrap into `ItemCategory::RawMaterial` "
      "components used in **Cyber-Fab** and **Hacking**. Every recycling cycle also "
      "grants minor Credits and a **25% chance to recover `1x {}`**, which feeds "
      "directly into **Smithing**.\n\n",
      item_name(slv_cpu.product_item), slv_cpu.req_level,
      item_name(slv_ram.product_item), slv_ram.req_level,
      item_name(ItemId::CarbonCell));

  out +=
      "```mermaid\nflowchart LR\n  subgraph Salvaging[\"Salvaging (Scrap & "
      "Hardware)\"]\n";
  int scrap_idx = 0;
  for (SkillActionId aid : actions_for_skill(SkillType::Salvaging)) {
    const auto& act = get_skill_action(aid);
    if (act.product_item == ItemId::ScrapCpu) {
      out += std::format("    S_CPU[\"Lv {}: {}\"]\n", act.req_level,
                         item_name(act.product_item));
    } else if (act.product_item == ItemId::ScrapRam) {
      out += std::format("    S_RAM[\"Lv {}: {}\"]\n", act.req_level,
                         item_name(act.product_item));
    } else {
      out += std::format("    S{}[\"Lv {}: {}\"]\n", scrap_idx++, act.req_level,
                         item_name(act.product_item));
    }
  }
  out += "  end\n\n  subgraph Recycling[\"Recycling (Raw Materials)\"]\n";
  int rec_idx = 0;
  for (SkillActionId aid : actions_for_skill(SkillType::Recycling)) {
    const auto& act = get_skill_action(aid);
    out += std::format("    R{}[\"Lv {}: {}\"]\n", rec_idx++, act.req_level,
                       item_name(act.product_item));
  }
  out += "  end\n\n";
  for (int i = 0; i < rec_idx; ++i) {
    out += std::format("  S{} --> R{}\n", i, i);
  }
  out += R"MD(
  S_CPU & S_RAM -->|"Hacking"| HCK1["Spike-ICE v1.0 / Watchdog-ICE v1.0<br/>Packet Filter Firewall / Parity Patch"]
  R0 -->|"Cyber-Fab"| CF0["Scrap Visor, Shield & Scrap CPU"]
  R1 -->|"Cyber-Fab / Hacking"| CF1["Titanium Visor/Shield & Firmware Matrix"]
  R2 -->|"Cyber-Fab"| CF2["Durasteel Visor/Shield & Optic-NAND RAM"]
  R3 -->|"Cyber-Fab"| CF3["Amber Datachip & Optic-NAND RAM"]
  R4 -->|"Cyber-Fab"| CF4["Positronic CPU, Sapphire Cortex, Cobalt Visor & Cryo RAM"]
  R5 -->|"Cyber-Fab"| CF5["Cobalt Holo-Aegis & Cryo-Holographic RAM"]
  R6 -->|"Cyber-Fab / Hacking"| CF6["Ruby Core, Tungsten Gear & Neural Restore"]
  R7 -->|"Cyber-Fab"| CF7["Quantum CPU, Quantum RAM, Emerald Key & Neutronium/Chrono Gear"]
  R8 -->|"Cyber-Fab / Hacking"| CF8["Neural CPU, Quantum Diamond, Quantum Gear & Quantum Rollback"]
```
)MD";

  out += std::format(
      R"MD(
---

## 3. Deep-Mining, Smithing & Cyber-Fab Gear & Hardware Chain (8 Tiers)

**Deep-Mining** extracts ores (`ItemCategory::RawOre`), which **Smithing** smelts into `ItemCategory::Alloy` ingots.
- **Smithing** forges offensive **Mono-Blades** (`2x Ingot`) and heavy **Exo-Suits** (`5x Ingot`).
- **Cyber-Fab** combines **Alloy Ingots** and **Ores** with **Recycled Raw Materials** to fabricate:
  - **Visors** (`1x Ingot + 1x Raw Material`) and **Holo-Shields** (`2x Ingot + 1x Raw Material`)
  - **Data Crystals** (`1x Conductor/Ingot + 1x Raw Material`)
  - **CPUs & RAM Hardware** (`{}`, `{}`, `{}`, `{}`, `{}`, `{}`, `{}`, `{}`) for **Hacking** (see [`hacking.md`](./hacking.md#2-hardware-components-itemcategoryhardware)).

### Tier 1–4 Metallurgy & Cyber-Fab Graph

```mermaid
flowchart LR
  subgraph Ores1["Deep-Mining (Tiers 1-4)"]
    O_CU["{}"]
    O_SI["{}"]
    O_TI["{}"]
    O_CC["{}"]
    O_AG["{}"]
    O_AU["{}"]
    O_CO["{}"]
  end

  subgraph Ingots1["Smithing: Smelting"]
    A_SC["{}"]
    A_TI["{}"]
    A_DU["{}"]
    A_AG["{}"]
    A_AU["{}"]
    A_CO["{}"]
  end

  O_CU & O_SI --> A_SC
  O_SI -->|"Cyber-Fab + Copper"| HW_T1["{} (Lv {})<br/>{} (Lv {})"]
  O_TI --> A_TI
  O_TI -->|"1x"| A_DU
  O_CC -->|"2x"| A_DU
  O_AG --> A_AG
  O_AU --> A_AU
  O_CO -->|"1x"| A_CO
  O_CC -->|"4x"| A_CO

  A_SC -->|"Smithing"| G_SC["{} (2x)<br/>{} (5x)"]
  A_SC -->|"Cyber-Fab + {}"| F_SC["{}<br/>{}"]

  A_TI -->|"Smithing"| G_TI["{} (2x)<br/>{} (5x)"]
  A_TI -->|"Cyber-Fab + {}"| F_TI["{}<br/>{}"]

  A_DU -->|"Smithing"| G_DU["{} (2x)<br/>{} (5x)"]
  A_DU -->|"Cyber-Fab + {}"| F_DU["{}<br/>{}"]

  A_AG -->|"Cyber-Fab + {}"| C_AMB["{}"]
  A_AG -->|"Cyber-Fab + {}"| HW_CPU2["{} (Lv {})"]
  A_AU -->|"Cyber-Fab + {}"| C_SAP["{}"]
  A_AU -->|"Cyber-Fab + {}"| HW_CPU3["{} (Lv {})"]
  A_AU -->|"Cyber-Fab + Plasma Coil"| C_RUB["{}"]

  A_CO -->|"Smithing"| G_CO["{} (2x)<br/>{} (5x)"]
  A_CO -->|"Cyber-Fab + Wafer / Cryo-Gel"| F_CO["{}<br/>{}"]
```
)MD",
      item_name(ItemId::ScrapCpu), item_name(ItemId::ScrapRam),
      item_name(ItemId::PositronicCpu), item_name(ItemId::OpticRam),
      item_name(ItemId::QuantumCpu), item_name(ItemId::CryoRam),
      item_name(ItemId::NeuralCpu), item_name(ItemId::QuantumRam),
      node_lvl_item(SkillType::DeepMining, ItemId::CopperOre),
      node_lvl_item(SkillType::DeepMining, ItemId::SiliconOre),
      node_lvl_item(SkillType::DeepMining, ItemId::TitaniumOre),
      node_lvl_item(SkillType::DeepMining, ItemId::CarbonCell),
      node_lvl_item(SkillType::DeepMining, ItemId::SilverOre),
      node_lvl_item(SkillType::DeepMining, ItemId::GoldOre),
      node_lvl_item(SkillType::DeepMining, ItemId::CobaltOre),
      node_lvl_item(SkillType::Smithing, ItemId::ScrapAlloy),
      node_lvl_item(SkillType::Smithing, ItemId::TitaniumAlloy),
      node_lvl_item(SkillType::Smithing, ItemId::DurasteelAlloy),
      node_lvl_item(SkillType::Smithing, ItemId::SilverConductor),
      node_lvl_item(SkillType::Smithing, ItemId::GoldSuperconductor),
      node_lvl_item(SkillType::Smithing, ItemId::CobaltAlloy),
      item_name(ItemId::ScrapCpu),
      action_req_level(SkillType::CyberFab, ItemId::ScrapCpu),
      item_name(ItemId::ScrapRam),
      action_req_level(SkillType::CyberFab, ItemId::ScrapRam),
      item_name(ItemId::ScrapBlade), item_name(ItemId::ScrapExoSuit),
      item_name(ItemId::CopperFilament), item_name(ItemId::ScrapVisor),
      item_name(ItemId::ScrapShield), item_name(ItemId::TitaniumBlade),
      item_name(ItemId::TitaniumExoSuit), item_name(ItemId::PlasteelPolymer),
      item_name(ItemId::TitaniumVisor), item_name(ItemId::TitaniumShield),
      item_name(ItemId::DurasteelBlade), item_name(ItemId::DurasteelExoSuit),
      item_name(ItemId::CarbonFiberWeave), item_name(ItemId::DurasteelVisor),
      item_name(ItemId::DurasteelShield), item_name(ItemId::OpticSilicaGlass),
      item_name(ItemId::AmberDatachip), item_name(ItemId::PositronicWafer),
      item_name(ItemId::PositronicCpu),
      action_req_level(SkillType::CyberFab, ItemId::PositronicCpu),
      item_name(ItemId::PositronicWafer), item_name(ItemId::SapphireCortex),
      item_name(ItemId::QuantumLattice), item_name(ItemId::QuantumCpu),
      action_req_level(SkillType::CyberFab, ItemId::QuantumCpu),
      item_name(ItemId::RubyLaserCore), item_name(ItemId::CobaltBlade),
      item_name(ItemId::CobaltExoSuit), item_name(ItemId::CobaltVisor),
      item_name(ItemId::CobaltShield));

  out += std::format(
      R"MD(
### Tier 5–8 Metallurgy & Cyber-Fab Graph (Endgame & Quantum Tier)

```mermaid
flowchart LR
  subgraph Ores2["Deep-Mining (Tiers 5-8)"]
    O_CC2["{}"]
    O_TU["{}"]
    O_NE["{}"]
    O_CH["{}"]
    O_QU["{}"]
  end

  subgraph Ingots2["Smithing: Smelting"]
    A_TU["{}"]
    A_NE["{}"]
    A_CH["{}"]
    A_QU["{}"]
  end

  O_TU -->|"1x"| A_TU
  O_CC2 -->|"6x"| A_TU

  O_NE -->|"1x"| A_NE
  O_CC2 -->|"8x"| A_NE

  O_CH -->|"1x"| A_CH
  O_NE -->|"2x"| A_CH

  O_QU -->|"1x"| A_QU
  O_CH -->|"2x"| A_QU

  A_TU -->|"Smithing"| G_TU["{} (2x)<br/>{} (5x)"]
  A_TU -->|"Cyber-Fab + Plasma Coil"| F_TU["{}<br/>{}"]

  A_NE -->|"Smithing"| G_NE["{} (2x)<br/>{} (5x)"]
  A_NE -->|"Cyber-Fab + {}"| F_NE["{}<br/>Neutronium Crown & Forcefield"]

  A_CH -->|"Smithing"| G_CH["{} (2x)<br/>{} (5x)"]
  A_CH -->|"Cyber-Fab + Lattice / Matrix"| F_CH["{} (Lv {})<br/>{}, Chrono Visor & Barrier"]

  A_QU -->|"Smithing"| G_QU["{} (2x)<br/>{} (5x)"]
  A_QU -->|"Cyber-Fab + {}"| F_QU["{}<br/>{}"]
```
)MD",
      node_lvl_item(SkillType::DeepMining, ItemId::CarbonCell),
      node_lvl_item(SkillType::DeepMining, ItemId::TungstenOre),
      node_lvl_item(SkillType::DeepMining, ItemId::NeutroniumOre),
      node_lvl_item(SkillType::DeepMining, ItemId::ChronoOre),
      node_lvl_item(SkillType::DeepMining, ItemId::QuantumOre),
      node_lvl_item(SkillType::Smithing, ItemId::TungstenAlloy),
      node_lvl_item(SkillType::Smithing, ItemId::NeutroniumAlloy),
      node_lvl_item(SkillType::Smithing, ItemId::ChronoAlloy),
      node_lvl_item(SkillType::Smithing, ItemId::QuantumAlloy),
      item_name(ItemId::TungstenBlade), item_name(ItemId::TungstenExoSuit),
      item_name(ItemId::TungstenVisor), item_name(ItemId::TungstenShield),
      item_name(ItemId::NeutroniumBlade), item_name(ItemId::NeutroniumExoSuit),
      item_name(ItemId::QuantumLattice), item_name(ItemId::EmeraldCryptokey),
      item_name(ItemId::ChronoBlade), item_name(ItemId::ChronoExoSuit),
      item_name(ItemId::NeuralCpu),
      action_req_level(SkillType::CyberFab, ItemId::NeuralCpu),
      item_name(ItemId::QuantumDiamond), item_name(ItemId::QuantumBlade),
      item_name(ItemId::QuantumExoSuit), item_name(ItemId::NeuralMatrix),
      item_name(ItemId::QuantumVisor), item_name(ItemId::QuantumShield));

  const auto& krill_info = get_item_info(ItemId::KrillRation);
  const auto& kraken_stim_info = get_item_info(ItemId::KrakenBioElixir);
  const auto& shoyu_info = get_item_info(ItemId::ShoyuRamen);
  const auto& kraken_ramen_info = get_item_info(ItemId::QuantumKrakenRamen);

  out += std::format(
      R"MD(
---

## 4. Farming, Synth-Noodles & Cyber-Ramen Culinary Chain

The culinary system offers two parallel paths in **Synth-Cook**:
1. **Single-Input Biota Stims**: Directly synthesize aquatic `RawBiota` from **Fishing** into combat rations (`+{} HP` to `+{} HP`).
2. **Multi-Ingredient Cyber-Ramen**: Cultivate `{}` in **Farming** (also obtained via a **20% bonus proc** on any hydroponic crop harvest), mill it into `2x {}`, and combine `1x {}` with hydroponic crops (or `{}`) to cook high-healing **Cyber-Ramen** bowls (`+{} HP` to `+{} HP`).

```mermaid
flowchart LR
  subgraph Hydroponics["Farming (Hydroponic Crops)"]
    F_WH["{}<br/>(+20% proc on all crops)"]
    F_SOY["{}"]
    F_SC["{}"]
    F_NR["{}"]
    F_BM["{}"]
    F_SH["{}"]
    F_CH["{}"]
    F_LO["{}"]
    F_TR["{}"]
  end

  subgraph Aquaculture["Fishing (Raw Biota)"]
    B_ALL["Lv 1–85: Krill .. Leviathan"]
    B_KR["{}"]
  end

  subgraph NoodlePrep["Noodle Milling (100% Success)"]
    ND["{} (x2)<br/>Farming Lv {} or Synth-Cook Lv {}"]
  end

  subgraph RamenCook["Synth-Cook: Cyber-Ramen & Stims"]
    STIM["Biota Stims (+{} to +{} HP)"]
)MD",
      krill_info.bonus.recover_amount, kraken_stim_info.bonus.recover_amount,
      item_name(ItemId::HydroWheat), item_name(ItemId::SynthNoodles),
      item_name(ItemId::SynthNoodles), item_name(ItemId::RawCyberKraken),
      shoyu_info.bonus.recover_amount, kraken_ramen_info.bonus.recover_amount,
      node_lvl_item(SkillType::Farming, ItemId::HydroWheat),
      node_lvl_item(SkillType::Farming, ItemId::SoyPods),
      node_lvl_item(SkillType::Farming, ItemId::NeonScallion),
      node_lvl_item(SkillType::Farming, ItemId::GlowNori),
      node_lvl_item(SkillType::Farming, ItemId::BioBamboo),
      node_lvl_item(SkillType::Farming, ItemId::CyberShiitake),
      node_lvl_item(SkillType::Farming, ItemId::PlasmaChili),
      node_lvl_item(SkillType::Farming, ItemId::ChronoLotus),
      node_lvl_item(SkillType::Farming, ItemId::QuantumTruffle),
      node_lvl_item(SkillType::Fishing, ItemId::RawCyberKraken),
      item_name(ItemId::SynthNoodles),
      get_skill_action(SkillActionId::MillSynthNoodles).req_level,
      get_skill_action(SkillActionId::PrepSynthNoodles).req_level,
      krill_info.bonus.recover_amount, kraken_stim_info.bonus.recover_amount);

  const std::vector<ItemId> ramen_items = {
      ItemId::ShoyuRamen,       ItemId::ScallionRamen, ItemId::NoriRamen,
      ItemId::BambooRamen,      ItemId::ShiitakeRamen, ItemId::PlasmaChiliRamen,
      ItemId::ChronoLotusRamen, ItemId::TruffleRamen,  ItemId::QuantumKrakenRamen};
  for (size_t i = 0; i < ramen_items.size(); ++i) {
    ItemId rid = ramen_items[i];
    const auto& rinfo = get_item_info(rid);
    out += std::format("    R{}[\"Lv {}: {} (+{} HP)\"]\n", i + 1,
                       action_req_level(SkillType::SynthCook, rid), rinfo.name,
                       rinfo.bonus.recover_amount);
  }

  out += R"MD(  end

  F_WH -->|"1x Hydro-Wheat"| ND
  B_ALL --> STIM
  B_KR -->|"1x Raw Kraken"| STIM

  ND & F_SOY --> R1
  ND & F_SC --> R2
  ND & F_NR --> R3
  ND & F_BM --> R4
  ND & F_SH --> R5
  ND & F_CH --> R6
  ND & F_LO --> R7
  ND & F_TR --> R8
  ND & B_KR --> R9
```

---

## 5. Complete Recipe Reference Tables

### Farming Protocols (`SkillType::Farming`)

| Lv | Protocol | Base Cycle | XP | Inputs | Output | Value |
|---:|----------|:----------:|---:|--------|--------|------:|
)MD";

  for (SkillActionId aid : actions_for_skill(SkillType::Farming)) {
    const auto& act = get_skill_action(aid);
    const auto& prod_info = get_item_info(act.product_item);
    std::string inputs =
        is_valid_item(act.input_item_1)
            ? std::format("`{}x {}`", act.input_qty_1, item_name(act.input_item_1))
            : "—";
    std::string val_str = act.product_qty > 1
                              ? std::format("{} Cr ea", number_string(prod_info.price))
                              : std::format("{} Cr", number_string(prod_info.price));
    out +=
        std::format("| {} | {} | {} | {} | {} | `{}x {}` | {} |\n", act.req_level,
                    act.name, format_sec(act.base_interval_ms), number_string(act.xp),
                    inputs, act.product_qty, prod_info.name, val_str);
  }

  out += R"MD(
### Synth-Cook Protocols (`SkillType::SynthCook`)

| Lv | Protocol | Base Cycle | XP | Input 1 | Input 2 | Output | Heal | Value |
|---:|----------|:----------:|---:|---------|---------|--------|-----:|------:|
)MD";

  for (SkillActionId aid : actions_for_skill(SkillType::SynthCook)) {
    const auto& act = get_skill_action(aid);
    const auto& prod_info = get_item_info(act.product_item);
    std::string in1 =
        is_valid_item(act.input_item_1)
            ? std::format("`{}x {}`", act.input_qty_1, item_name(act.input_item_1))
            : "—";
    std::string in2 =
        is_valid_item(act.input_item_2)
            ? std::format("`{}x {}`", act.input_qty_2, item_name(act.input_item_2))
            : "—";
    std::string heal = prod_info.bonus.recover_amount > 0
                           ? std::format("+{} HP", prod_info.bonus.recover_amount)
                           : "—";
    out += std::format("| {} | {} | {} | {} | {} | {} | `{}x {}` | {} | {} Cr |\n",
                       act.req_level, strip_paren_suffix(act.name),
                       format_sec(act.base_interval_ms), number_string(act.xp), in1,
                       in2, act.product_qty, prod_info.name, heal,
                       number_string(prod_info.price));
  }

  write_file(docs_dir / "logistics.md", out);
}

}  // namespace

int main(int argc, char* argv[]) {
  try {
    std::filesystem::path docs_dir = (argc > 1) ? argv[1] : "docs";
    std::filesystem::create_directories(docs_dir);
    generate_readme(docs_dir);
    generate_combat(docs_dir);
    generate_hacking(docs_dir);
    generate_logistics(docs_dir);
    return 0;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
