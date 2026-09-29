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

// Standard XP table for levels 1..99
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
    // Scrap & Tech Nodes (0..8)
    {ITEM_COPPER_WIRE_SCRAP, "Copper Wire Scrap", ItemCategory::Scrap, 2, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_PLASTEEL_SHARDS, "Plasteel Shards", ItemCategory::Scrap, 5, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_CARBON_NANOTUBES, "Carbon Nanotubes", ItemCategory::Scrap, 10, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_OPTIC_FIBER_BUNDLE, "Optic Fiber Bundle", ItemCategory::Scrap, 18, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_POSITRONIC_RELAYS, "Positronic Relays", ItemCategory::Scrap, 30, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_CRYO_CELL_CORE, "Cryo-Cell Core", ItemCategory::Scrap, 45, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_PLASMA_CONDUIT, "Plasma Conduit", ItemCategory::Scrap, 70, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_QUANTUM_NODE, "Quantum Node", ItemCategory::Scrap, 120, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_AI_MAINFRAME_CORE, "AI Mainframe Core", ItemCategory::Scrap, 200, 0, EquipSlot::None, 1, 0, 0, 0, 0},

    // Raw Synth-Biota (9..16)
    {ITEM_RAW_KRILL_BIOMASS, "Raw Krill Biomass", ItemCategory::RawBiota, 3, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_RAW_NEON_EEL, "Raw Neon Eel", ItemCategory::RawBiota, 6, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_RAW_SYNTH_CARP, "Raw Synth-Carp", ItemCategory::RawBiota, 14, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_RAW_CHROME_SALMON, "Raw Chrome Salmon", ItemCategory::RawBiota, 24, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_RAW_CYBER_LOBSTER, "Raw Cyber-Lobster", ItemCategory::RawBiota, 45, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_RAW_PLASMA_RAY, "Raw Plasma Ray", ItemCategory::RawBiota, 75, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_RAW_APEX_SHARK, "Raw Apex Shark", ItemCategory::RawBiota, 140, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_RAW_LEVIATHAN_CELL, "Raw Leviathan Cell", ItemCategory::RawBiota, 260, 0, EquipSlot::None, 1, 0, 0, 0, 0},

    // Synthesized Stims / Rations & Toxic Slag (17..26)
    {ITEM_KRILL_RATION, "Krill Ration", ItemCategory::StimFood, 8, 30, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_NEON_EEL_SKEWER, "Neon Eel Skewer", ItemCategory::StimFood, 15, 50, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_SYNTH_CARP_PACK, "Synth-Carp Pack", ItemCategory::StimFood, 32, 80, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_CHROME_SALMON_STIM, "Chrome Salmon Stim", ItemCategory::StimFood, 55, 110, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_CYBER_LOBSTER_MEAL, "Cyber-Lobster Meal", ItemCategory::StimFood, 100, 160, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_PLASMA_RAY_INFUSION, "Plasma Ray Infusion", ItemCategory::StimFood, 165, 220, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_APEX_SHARK_BOOSTER, "Apex Shark Booster", ItemCategory::StimFood, 300, 320, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_LEVIATHAN_NANOMED, "Leviathan Nanomed", ItemCategory::StimFood, 550, 480, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_SYNTH_PROTEIN_BAR, "Synth-Protein Bar", ItemCategory::StimFood, 6, 25, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_TOXIC_SLAG, "Toxic Bio-Slag", ItemCategory::ToxicWaste, 1, 0, EquipSlot::None, 1, 0, 0, 0, 0},

    // Deep-Mined Ores & Cells (27..36)
    {ITEM_COPPER_ORE, "Copper Ore", ItemCategory::RawOre, 4, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_SILICON_ORE, "Silicon Ore", ItemCategory::RawOre, 4, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_TITANIUM_ORE, "Titanium Ore", ItemCategory::RawOre, 12, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_CARBON_CELL, "Carbon Cell", ItemCategory::RawOre, 18, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_SILVER_ORE, "Silver Ore", ItemCategory::RawOre, 30, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_GOLD_ORE, "Gold Ore", ItemCategory::RawOre, 50, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_COBALT_ORE, "Cobalt Ore", ItemCategory::RawOre, 70, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_TUNGSTEN_ORE, "Tungsten Ore", ItemCategory::RawOre, 120, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_NEUTRONIUM_ORE, "Neutronium Ore", ItemCategory::RawOre, 220, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_CHRONO_ORE, "Chrono-Crystal Ore", ItemCategory::RawOre, 400, 0, EquipSlot::None, 1, 0, 0, 0, 0},

    // Refined Alloys & Conductors (37..45)
    {ITEM_SCRAP_ALLOY, "Scrap-Alloy Ingot", ItemCategory::Alloy, 15, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_TITANIUM_ALLOY, "Titanium Ingot", ItemCategory::Alloy, 32, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_DURASTEEL_ALLOY, "Durasteel Ingot", ItemCategory::Alloy, 65, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_SILVER_CONDUCTOR, "Silver Conductor", ItemCategory::Alloy, 80, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_GOLD_SUPERCONDUCTOR, "Gold Superconductor", ItemCategory::Alloy, 130, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_COBALT_ALLOY, "Cobalt-Chrome Ingot", ItemCategory::Alloy, 175, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_TUNGSTEN_ALLOY, "Tungsten Ingot", ItemCategory::Alloy, 310, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_NEUTRONIUM_ALLOY, "Neutronium Ingot", ItemCategory::Alloy, 580, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_CHRONO_ALLOY, "Chrono-Alloy Ingot", ItemCategory::Alloy, 1100, 0, EquipSlot::None, 1, 0, 0, 0, 0},

    // Data Crystals (46..50)
    {ITEM_AMBER_DATACHIP, "Amber Datachip", ItemCategory::DataCrystal, 150, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_SAPPHIRE_CORTEX, "Sapphire Cortex", ItemCategory::DataCrystal, 250, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_RUBY_LASER_CORE, "Ruby Laser Core", ItemCategory::DataCrystal, 450, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_EMERALD_CRYPTOKEY, "Emerald Cryptokey", ItemCategory::DataCrystal, 750, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_QUANTUM_DIAMOND, "Quantum Diamond", ItemCategory::DataCrystal, 1500, 0, EquipSlot::None, 1, 0, 0, 0, 0},

    // Weapons - Mono-Blades (51..57)
    {ITEM_SCRAP_BLADE, "Scrap Vibro-Knife", ItemCategory::Weapon, 45, 0, EquipSlot::Weapon, 1, 10, 12, 0, 0},
    {ITEM_TITANIUM_BLADE, "Titanium Mono-Blade", ItemCategory::Weapon, 100, 0, EquipSlot::Weapon, 5, 18, 20, 0, 0},
    {ITEM_DURASTEEL_BLADE, "Durasteel Katana", ItemCategory::Weapon, 220, 0, EquipSlot::Weapon, 10, 28, 32, 0, 0},
    {ITEM_COBALT_BLADE, "Cobalt Laser-Edge", ItemCategory::Weapon, 550, 0, EquipSlot::Weapon, 20, 42, 48, 0, 0},
    {ITEM_TUNGSTEN_BLADE, "Tungsten Mantis-Blade", ItemCategory::Weapon, 1100, 0, EquipSlot::Weapon, 30, 60, 66, 0, 0},
    {ITEM_NEUTRONIUM_BLADE, "Neutronium Phase-Saber", ItemCategory::Weapon, 2400, 0, EquipSlot::Weapon, 40, 84, 92, 0, 0},
    {ITEM_CHRONO_BLADE, "Chrono-Edge Katana", ItemCategory::Weapon, 6000, 0, EquipSlot::Weapon, 60, 120, 130, 0, 0},

    // Visors (58..64)
    {ITEM_SCRAP_VISOR, "Scrap Optic Visor", ItemCategory::Visor, 40, 0, EquipSlot::Visor, 1, 0, 0, 6, 1},
    {ITEM_TITANIUM_VISOR, "Titanium HUD Visor", ItemCategory::Visor, 90, 0, EquipSlot::Visor, 5, 0, 0, 11, 2},
    {ITEM_DURASTEEL_VISOR, "Durasteel Tac-Helm", ItemCategory::Visor, 200, 0, EquipSlot::Visor, 10, 0, 0, 18, 3},
    {ITEM_COBALT_VISOR, "Cobalt Neural Visor", ItemCategory::Visor, 500, 0, EquipSlot::Visor, 20, 0, 0, 27, 4},
    {ITEM_TUNGSTEN_VISOR, "Tungsten Cyber-Helm", ItemCategory::Visor, 950, 0, EquipSlot::Visor, 30, 0, 0, 38, 5},
    {ITEM_NEUTRONIUM_VISOR, "Neutronium Mind-Crown", ItemCategory::Visor, 2100, 0, EquipSlot::Visor, 40, 0, 0, 52, 7},
    {ITEM_CHRONO_VISOR, "Chrono-Sync Visor", ItemCategory::Visor, 5200, 0, EquipSlot::Visor, 60, 0, 0, 72, 10},

    // Exo-Suits (65..71)
    {ITEM_SCRAP_EXOSUIT, "Scrap Exo-Harness", ItemCategory::ExoSuit, 85, 0, EquipSlot::ExoSuit, 1, 0, 0, 14, 2},
    {ITEM_TITANIUM_EXOSUIT, "Titanium Flak-Jacket", ItemCategory::ExoSuit, 180, 0, EquipSlot::ExoSuit, 5, 0, 0, 24, 3},
    {ITEM_DURASTEEL_EXOSUIT, "Durasteel Exo-Rig", ItemCategory::ExoSuit, 400, 0, EquipSlot::ExoSuit, 10, 0, 0, 36, 5},
    {ITEM_COBALT_EXOSUIT, "Cobalt Subdermal Rig", ItemCategory::ExoSuit, 950, 0, EquipSlot::ExoSuit, 20, 0, 0, 52, 7},
    {ITEM_TUNGSTEN_EXOSUIT, "Tungsten Power-Armor", ItemCategory::ExoSuit, 1900, 0, EquipSlot::ExoSuit, 30, 0, 0, 74, 9},
    {ITEM_NEUTRONIUM_EXOSUIT, "Neutronium Nano-Suit", ItemCategory::ExoSuit, 4200, 0, EquipSlot::ExoSuit, 40, 0, 0, 102, 12},
    {ITEM_CHRONO_EXOSUIT, "Chrono-Weave Exo-Suit", ItemCategory::ExoSuit, 9800, 0, EquipSlot::ExoSuit, 60, 0, 0, 140, 16},

    // Holo-Shields (72..78)
    {ITEM_SCRAP_SHIELD, "Scrap Riot Buckler", ItemCategory::HoloShield, 55, 0, EquipSlot::HoloShield, 1, 0, 0, 9, 1},
    {ITEM_TITANIUM_SHIELD, "Titanium Deflector", ItemCategory::HoloShield, 120, 0, EquipSlot::HoloShield, 5, 0, 0, 16, 2},
    {ITEM_DURASTEEL_SHIELD, "Durasteel Barrier", ItemCategory::HoloShield, 260, 0, EquipSlot::HoloShield, 10, 0, 0, 25, 3},
    {ITEM_COBALT_SHIELD, "Cobalt Holo-Aegis", ItemCategory::HoloShield, 620, 0, EquipSlot::HoloShield, 20, 0, 0, 36, 5},
    {ITEM_TUNGSTEN_SHIELD, "Tungsten Pulse-Shield", ItemCategory::HoloShield, 1250, 0, EquipSlot::HoloShield, 30, 0, 0, 50, 6},
    {ITEM_NEUTRONIUM_SHIELD, "Neutronium Forcefield", ItemCategory::HoloShield, 2800, 0, EquipSlot::HoloShield, 40, 0, 0, 68, 8},
    {ITEM_CHRONO_SHIELD, "Chrono-Phase Barrier", ItemCategory::HoloShield, 6800, 0, EquipSlot::HoloShield, 60, 0, 0, 96, 12},

    // Enemy Salvage Loot (79..83)
    {ITEM_SERVO_PARTS, "Servo Parts", ItemCategory::CyberLoot, 8, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_HEAVY_CHASSIS, "Heavy Mech Chassis", ItemCategory::CyberLoot, 30, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_APEX_CYBER_CORE, "Apex Cyber-Core", ItemCategory::CyberLoot, 180, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_MICROCHIP, "Microchip", ItemCategory::CyberLoot, 3, 0, EquipSlot::None, 1, 0, 0, 0, 0},
    {ITEM_SYNTH_WEAVE_HIDE, "Synth-Weave Hide", ItemCategory::CyberLoot, 16, 0, EquipSlot::None, 1, 0, 0, 0, 0},
}};

const std::vector<SkillAction> skill_actions = {
    // Salvaging (0..8)
    {0, SkillType::Salvaging, "Strip Copper Wiring", 1, 3000, 15, ITEM_COPPER_WIRE_SCRAP, 1, -1, 0, -1, 0},
    {1, SkillType::Salvaging, "Salvage Plasteel Hull", 10, 3500, 30, ITEM_PLASTEEL_SHARDS, 1, -1, 0, -1, 0},
    {2, SkillType::Salvaging, "Extract Nanotubes", 25, 4000, 55, ITEM_CARBON_NANOTUBES, 1, -1, 0, -1, 0},
    {3, SkillType::Salvaging, "Splice Optic Fibers", 35, 4500, 85, ITEM_OPTIC_FIBER_BUNDLE, 1, -1, 0, -1, 0},
    {4, SkillType::Salvaging, "Pull Positronic Relay", 45, 5000, 120, ITEM_POSITRONIC_RELAYS, 1, -1, 0, -1, 0},
    {5, SkillType::Salvaging, "Drain Cryo-Cell Rack", 55, 5500, 165, ITEM_CRYO_CELL_CORE, 1, -1, 0, -1, 0},
    {6, SkillType::Salvaging, "Tap Plasma Conduit", 60, 6000, 220, ITEM_PLASMA_CONDUIT, 1, -1, 0, -1, 0},
    {7, SkillType::Salvaging, "Hack Quantum Node", 75, 7500, 340, ITEM_QUANTUM_NODE, 1, -1, 0, -1, 0},
    {8, SkillType::Salvaging, "Rip AI Mainframe Core", 90, 9000, 500, ITEM_AI_MAINFRAME_CORE, 1, -1, 0, -1, 0},

    // Bio-Harvest (9..16)
    {9, SkillType::BioHarvest, "Culture Krill Biomass", 1, 3000, 12, ITEM_RAW_KRILL_BIOMASS, 1, -1, 0, -1, 0},
    {10, SkillType::BioHarvest, "Net Neon Eel", 5, 3400, 24, ITEM_RAW_NEON_EEL, 1, -1, 0, -1, 0},
    {11, SkillType::BioHarvest, "Harvest Synth-Carp", 20, 4000, 55, ITEM_RAW_SYNTH_CARP, 1, -1, 0, -1, 0},
    {12, SkillType::BioHarvest, "Extract Chrome Salmon", 35, 4500, 90, ITEM_RAW_CHROME_SALMON, 1, -1, 0, -1, 0},
    {13, SkillType::BioHarvest, "Trap Cyber-Lobster", 45, 5200, 135, ITEM_RAW_CYBER_LOBSTER, 1, -1, 0, -1, 0},
    {14, SkillType::BioHarvest, "Snare Plasma Ray", 55, 6000, 195, ITEM_RAW_PLASMA_RAY, 1, -1, 0, -1, 0},
    {15, SkillType::BioHarvest, "Harpoon Apex Shark", 70, 7200, 310, ITEM_RAW_APEX_SHARK, 1, -1, 0, -1, 0},
    {16, SkillType::BioHarvest, "Clone Leviathan Cell", 85, 8500, 480, ITEM_RAW_LEVIATHAN_CELL, 1, -1, 0, -1, 0},

    // Overclock (17..25)
    {17, SkillType::Overclock, "Overclock Copper Scrap", 1, 2200, 22, -1, 0, ITEM_COPPER_WIRE_SCRAP, 1, -1, 0},
    {18, SkillType::Overclock, "Overclock Plasteel", 10, 2400, 42, -1, 0, ITEM_PLASTEEL_SHARDS, 1, -1, 0},
    {19, SkillType::Overclock, "Overclock Nanotubes", 25, 2600, 75, -1, 0, ITEM_CARBON_NANOTUBES, 1, -1, 0},
    {20, SkillType::Overclock, "Overclock Optic Fibers", 35, 2800, 110, -1, 0, ITEM_OPTIC_FIBER_BUNDLE, 1, -1, 0},
    {21, SkillType::Overclock, "Overclock Relay Core", 45, 3000, 155, -1, 0, ITEM_POSITRONIC_RELAYS, 1, -1, 0},
    {22, SkillType::Overclock, "Overclock Cryo-Cell", 55, 3200, 210, -1, 0, ITEM_CRYO_CELL_CORE, 1, -1, 0},
    {23, SkillType::Overclock, "Overclock Plasma Line", 60, 3500, 280, -1, 0, ITEM_PLASMA_CONDUIT, 1, -1, 0},
    {24, SkillType::Overclock, "Overclock Quantum Node", 75, 3800, 410, -1, 0, ITEM_QUANTUM_NODE, 1, -1, 0},
    {25, SkillType::Overclock, "Overclock AI Mainframe", 90, 4200, 600, -1, 0, ITEM_AI_MAINFRAME_CORE, 1, -1, 0},

    // Synth-Cook (26..33)
    {26, SkillType::SynthCook, "Synth Krill Ration (+30 HP)", 1, 2600, 18, ITEM_KRILL_RATION, 1, ITEM_RAW_KRILL_BIOMASS, 1, -1, 0},
    {27, SkillType::SynthCook, "Synth Neon Eel (+50 HP)", 5, 2800, 34, ITEM_NEON_EEL_SKEWER, 1, ITEM_RAW_NEON_EEL, 1, -1, 0},
    {28, SkillType::SynthCook, "Synth Carp Pack (+80 HP)", 20, 3000, 70, ITEM_SYNTH_CARP_PACK, 1, ITEM_RAW_SYNTH_CARP, 1, -1, 0},
    {29, SkillType::SynthCook, "Synth Salmon Stim (+110 HP)", 35, 3200, 115, ITEM_CHROME_SALMON_STIM, 1, ITEM_RAW_CHROME_SALMON, 1, -1, 0},
    {30, SkillType::SynthCook, "Synth Lobster Meal (+160 HP)", 45, 3400, 175, ITEM_CYBER_LOBSTER_MEAL, 1, ITEM_RAW_CYBER_LOBSTER, 1, -1, 0},
    {31, SkillType::SynthCook, "Synth Plasma Ray (+220 HP)", 55, 3600, 240, ITEM_PLASMA_RAY_INFUSION, 1, ITEM_RAW_PLASMA_RAY, 1, -1, 0},
    {32, SkillType::SynthCook, "Synth Shark Boost (+320 HP)", 70, 3800, 360, ITEM_APEX_SHARK_BOOSTER, 1, ITEM_RAW_APEX_SHARK, 1, -1, 0},
    {33, SkillType::SynthCook, "Synth Leviathan Med (+480 HP)", 85, 4200, 540, ITEM_LEVIATHAN_NANOMED, 1, ITEM_RAW_LEVIATHAN_CELL, 1, -1, 0},

    // Deep-Mining (34..43)
    {34, SkillType::DeepMining, "Mine Copper Vein", 1, 2800, 14, ITEM_COPPER_ORE, 1, -1, 0, -1, 0},
    {35, SkillType::DeepMining, "Mine Silicon Deposit", 1, 2800, 14, ITEM_SILICON_ORE, 1, -1, 0, -1, 0},
    {36, SkillType::DeepMining, "Mine Titanium Seam", 15, 3200, 35, ITEM_TITANIUM_ORE, 1, -1, 0, -1, 0},
    {37, SkillType::DeepMining, "Mine Carbon Cell Bed", 30, 3500, 55, ITEM_CARBON_CELL, 1, -1, 0, -1, 0},
    {38, SkillType::DeepMining, "Mine Silver Vein", 35, 3800, 75, ITEM_SILVER_ORE, 1, -1, 0, -1, 0},
    {39, SkillType::DeepMining, "Mine Gold Deposit", 40, 4200, 105, ITEM_GOLD_ORE, 1, -1, 0, -1, 0},
    {40, SkillType::DeepMining, "Mine Cobalt Node", 50, 4800, 150, ITEM_COBALT_ORE, 1, -1, 0, -1, 0},
    {41, SkillType::DeepMining, "Mine Tungsten Core", 70, 5800, 230, ITEM_TUNGSTEN_ORE, 1, -1, 0, -1, 0},
    {42, SkillType::DeepMining, "Mine Neutronium Rift", 80, 7000, 350, ITEM_NEUTRONIUM_ORE, 1, -1, 0, -1, 0},
    {43, SkillType::DeepMining, "Mine Chrono-Crystal", 92, 8500, 520, ITEM_CHRONO_ORE, 1, -1, 0, -1, 0},

    // Cyber-Fab - Refining Alloys & Fabricating Cyber-Gear (44..80)
    {44, SkillType::CyberFab, "Refine Scrap-Alloy Ingot", 1, 2200, 16, ITEM_SCRAP_ALLOY, 1, ITEM_COPPER_ORE, 1, ITEM_SILICON_ORE, 1},
    {45, SkillType::CyberFab, "Fab Scrap Vibro-Knife", 1, 2500, 35, ITEM_SCRAP_BLADE, 1, ITEM_SCRAP_ALLOY, 2, -1, 0},
    {46, SkillType::CyberFab, "Fab Scrap Optic Visor", 2, 2500, 35, ITEM_SCRAP_VISOR, 1, ITEM_SCRAP_ALLOY, 2, -1, 0},
    {47, SkillType::CyberFab, "Fab Scrap Riot Buckler", 3, 2600, 50, ITEM_SCRAP_SHIELD, 1, ITEM_SCRAP_ALLOY, 3, -1, 0},
    {48, SkillType::CyberFab, "Fab Scrap Exo-Harness", 5, 2800, 80, ITEM_SCRAP_EXOSUIT, 1, ITEM_SCRAP_ALLOY, 5, -1, 0},

    {49, SkillType::CyberFab, "Refine Titanium Ingot", 15, 2400, 32, ITEM_TITANIUM_ALLOY, 1, ITEM_TITANIUM_ORE, 1, -1, 0},
    {50, SkillType::CyberFab, "Fab Titanium Mono-Blade", 15, 2600, 65, ITEM_TITANIUM_BLADE, 1, ITEM_TITANIUM_ALLOY, 2, -1, 0},
    {51, SkillType::CyberFab, "Fab Titanium HUD Visor", 16, 2600, 65, ITEM_TITANIUM_VISOR, 1, ITEM_TITANIUM_ALLOY, 2, -1, 0},
    {52, SkillType::CyberFab, "Fab Titanium Deflector", 18, 2700, 95, ITEM_TITANIUM_SHIELD, 1, ITEM_TITANIUM_ALLOY, 3, -1, 0},
    {53, SkillType::CyberFab, "Fab Titanium Flak-Jacket", 20, 2900, 150, ITEM_TITANIUM_EXOSUIT, 1, ITEM_TITANIUM_ALLOY, 5, -1, 0},

    {54, SkillType::CyberFab, "Refine Durasteel Ingot", 30, 2600, 55, ITEM_DURASTEEL_ALLOY, 1, ITEM_TITANIUM_ORE, 1, ITEM_CARBON_CELL, 2},
    {55, SkillType::CyberFab, "Fab Durasteel Katana", 30, 2800, 110, ITEM_DURASTEEL_BLADE, 1, ITEM_DURASTEEL_ALLOY, 2, -1, 0},
    {56, SkillType::CyberFab, "Fab Durasteel Tac-Helm", 32, 2800, 110, ITEM_DURASTEEL_VISOR, 1, ITEM_DURASTEEL_ALLOY, 2, -1, 0},
    {57, SkillType::CyberFab, "Fab Durasteel Barrier", 34, 2900, 160, ITEM_DURASTEEL_SHIELD, 1, ITEM_DURASTEEL_ALLOY, 3, -1, 0},
    {58, SkillType::CyberFab, "Fab Durasteel Exo-Rig", 36, 3100, 260, ITEM_DURASTEEL_EXOSUIT, 1, ITEM_DURASTEEL_ALLOY, 5, -1, 0},

    {59, SkillType::CyberFab, "Refine Silver Conductor", 35, 2500, 68, ITEM_SILVER_CONDUCTOR, 1, ITEM_SILVER_ORE, 1, -1, 0},
    {60, SkillType::CyberFab, "Refine Gold Superconductor", 40, 2600, 95, ITEM_GOLD_SUPERCONDUCTOR, 1, ITEM_GOLD_ORE, 1, -1, 0},

    {61, SkillType::CyberFab, "Refine Cobalt Ingot", 50, 2800, 115, ITEM_COBALT_ALLOY, 1, ITEM_COBALT_ORE, 1, ITEM_CARBON_CELL, 4},
    {62, SkillType::CyberFab, "Fab Cobalt Laser-Edge", 50, 3000, 220, ITEM_COBALT_BLADE, 1, ITEM_COBALT_ALLOY, 2, -1, 0},
    {63, SkillType::CyberFab, "Fab Cobalt Neural Visor", 52, 3000, 220, ITEM_COBALT_VISOR, 1, ITEM_COBALT_ALLOY, 2, -1, 0},
    {64, SkillType::CyberFab, "Fab Cobalt Holo-Aegis", 54, 3100, 320, ITEM_COBALT_SHIELD, 1, ITEM_COBALT_ALLOY, 3, -1, 0},
    {65, SkillType::CyberFab, "Fab Cobalt Subdermal Rig", 56, 3300, 520, ITEM_COBALT_EXOSUIT, 1, ITEM_COBALT_ALLOY, 5, -1, 0},

    {66, SkillType::CyberFab, "Refine Tungsten Ingot", 70, 3000, 175, ITEM_TUNGSTEN_ALLOY, 1, ITEM_TUNGSTEN_ORE, 1, ITEM_CARBON_CELL, 6},
    {67, SkillType::CyberFab, "Fab Tungsten Mantis-Blade", 70, 3200, 340, ITEM_TUNGSTEN_BLADE, 1, ITEM_TUNGSTEN_ALLOY, 2, -1, 0},
    {68, SkillType::CyberFab, "Fab Tungsten Cyber-Helm", 72, 3200, 340, ITEM_TUNGSTEN_VISOR, 1, ITEM_TUNGSTEN_ALLOY, 2, -1, 0},
    {69, SkillType::CyberFab, "Fab Tungsten Pulse-Shield", 74, 3300, 500, ITEM_TUNGSTEN_SHIELD, 1, ITEM_TUNGSTEN_ALLOY, 3, -1, 0},
    {70, SkillType::CyberFab, "Fab Tungsten Power-Armor", 76, 3500, 820, ITEM_TUNGSTEN_EXOSUIT, 1, ITEM_TUNGSTEN_ALLOY, 5, -1, 0},

    {71, SkillType::CyberFab, "Refine Neutronium Ingot", 80, 3200, 260, ITEM_NEUTRONIUM_ALLOY, 1, ITEM_NEUTRONIUM_ORE, 1, ITEM_CARBON_CELL, 8},
    {72, SkillType::CyberFab, "Fab Neutronium Saber", 80, 3400, 520, ITEM_NEUTRONIUM_BLADE, 1, ITEM_NEUTRONIUM_ALLOY, 2, -1, 0},
    {73, SkillType::CyberFab, "Fab Neutronium Mind-Crown", 82, 3400, 520, ITEM_NEUTRONIUM_VISOR, 1, ITEM_NEUTRONIUM_ALLOY, 2, -1, 0},
    {74, SkillType::CyberFab, "Fab Neutronium Forcefield", 84, 3500, 760, ITEM_NEUTRONIUM_SHIELD, 1, ITEM_NEUTRONIUM_ALLOY, 3, -1, 0},
    {75, SkillType::CyberFab, "Fab Neutronium Nano-Suit", 86, 3700, 1250, ITEM_NEUTRONIUM_EXOSUIT, 1, ITEM_NEUTRONIUM_ALLOY, 5, -1, 0},

    {76, SkillType::CyberFab, "Refine Chrono-Alloy Ingot", 92, 3600, 400, ITEM_CHRONO_ALLOY, 1, ITEM_CHRONO_ORE, 1, ITEM_NEUTRONIUM_ORE, 2},
    {77, SkillType::CyberFab, "Fab Chrono-Edge Katana", 92, 3800, 800, ITEM_CHRONO_BLADE, 1, ITEM_CHRONO_ALLOY, 2, -1, 0},
    {78, SkillType::CyberFab, "Fab Chrono-Sync Visor", 94, 3800, 800, ITEM_CHRONO_VISOR, 1, ITEM_CHRONO_ALLOY, 2, -1, 0},
    {79, SkillType::CyberFab, "Fab Chrono-Phase Barrier", 96, 3900, 1150, ITEM_CHRONO_SHIELD, 1, ITEM_CHRONO_ALLOY, 3, -1, 0},
    {80, SkillType::CyberFab, "Fab Chrono-Weave Exo-Suit", 98, 4100, 1900, ITEM_CHRONO_EXOSUIT, 1, ITEM_CHRONO_ALLOY, 5, -1, 0},
};

const std::array<MonsterInfo, MONSTER_COUNT> monster_info = {{
    {0,
     "Stray Servo-Drone",
     "Neon Slums",
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
     {{{ITEM_MICROCHIP, 90, 2, 6},
       {ITEM_SERVO_PARTS, 100, 1, 1},
       {ITEM_KRILL_RATION, 25, 1, 2}}}},
    {1,
     "Bio-Vat Hound",
     "Neon Slums",
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
     {{{ITEM_SYNTH_WEAVE_HIDE, 85, 1, 2},
       {ITEM_SYNTH_PROTEIN_BAR, 70, 1, 2},
       {ITEM_SERVO_PARTS, 100, 1, 1}}}},
    {2,
     "Street Scavenger",
     "Neon Slums",
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
     {{{ITEM_PLASTEEL_SHARDS, 50, 2, 5},
       {ITEM_SYNTH_CARP_PACK, 40, 1, 2},
       {ITEM_SERVO_PARTS, 100, 1, 1}}}},
    {3,
     "Chrome Gang Punk",
     "Back-Alley Sector",
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
     {{{ITEM_SCRAP_BLADE, 15, 1, 1},
       {ITEM_TITANIUM_ORE, 45, 2, 4},
       {ITEM_SERVO_PARTS, 100, 1, 1}}}},
    {4,
     "Riot Enforcer Bot",
     "Industrial Sector",
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
     {{{ITEM_HEAVY_CHASSIS, 100, 1, 2},
       {ITEM_DURASTEEL_BLADE, 12, 1, 1},
       {ITEM_CARBON_CELL, 40, 3, 6}}}},
    {5,
     "Chem-Mutant Brute",
     "Industrial Sector",
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
     {{{ITEM_HEAVY_CHASSIS, 100, 1, 2},
       {ITEM_COBALT_ORE, 45, 2, 5},
       {ITEM_CYBER_LOBSTER_MEAL, 35, 2, 4}}}},
    {6,
     "Cryo-Sec Mech",
     "Industrial Sector",
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
     {{{ITEM_HEAVY_CHASSIS, 100, 2, 3},
       {ITEM_COBALT_EXOSUIT, 10, 1, 1},
       {ITEM_SAPPHIRE_CORTEX, 25, 1, 2}}}},
    {7,
     "Corp Shadow-Op",
     "Megacorp Plaza",
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
     {{{ITEM_TUNGSTEN_BLADE, 12, 1, 1},
       {ITEM_TUNGSTEN_ORE, 45, 2, 5},
       {ITEM_PLASMA_RAY_INFUSION, 40, 2, 4}}}},
    {8,
     "Cobalt Cyber-Ninja",
     "Megacorp Plaza",
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
     {{{ITEM_TUNGSTEN_EXOSUIT, 10, 1, 1},
       {ITEM_RUBY_LASER_CORE, 30, 1, 2},
       {ITEM_APEX_SHARK_BOOSTER, 35, 2, 4}}}},
    {9,
     "Neutronium Cyborg",
     "Megacorp Plaza",
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
     {{{ITEM_NEUTRONIUM_BLADE, 10, 1, 1},
       {ITEM_NEUTRONIUM_EXOSUIT, 8, 1, 1},
       {ITEM_EMERALD_CRYPTOKEY, 30, 1, 2}}}},
    {10,
     "Apex Cyber-Wyrm",
     "Orbital Spire",
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
     {{{ITEM_APEX_CYBER_CORE, 100, 1, 2},
       {ITEM_CHRONO_ORE, 35, 2, 4},
       {ITEM_QUANTUM_DIAMOND, 25, 1, 2}}}},
    {11,
     "NEXUS-9, Rogue Overmind",
     "Mainframe Core [BOSS]",
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
     {{{ITEM_CHRONO_BLADE, 20, 1, 1},
       {ITEM_CHRONO_EXOSUIT, 15, 1, 1},
       {ITEM_LEVIATHAN_NANOMED, 60, 4, 8}}}},
}};

const std::array<ShopUpgradeInfo, TOOL_TIER_COUNT> cutter_upgrades = {{
    {0, "Scrap Cutter", "Starter Salvaging Cutter", 1, 0, 0},
    {1, "Titanium Cutter", "-6% Salvaging Interval", 10, 200, 6},
    {2, "Durasteel Cutter", "-12% Salvaging Interval", 25, 750, 12},
    {3, "Cobalt Laser-Cutter", "-18% Salvaging Interval", 40, 2500, 18},
    {4, "Tungsten Plasma-Torch", "-24% Salvaging Interval", 55, 8000, 24},
    {5, "Neutronium Arc-Splicer", "-30% Salvaging Interval", 70, 25000, 30},
    {6, "Chrono Deconstructor", "-38% Salvaging Interval", 85, 80000, 38},
}};

const std::array<ShopUpgradeInfo, TOOL_TIER_COUNT> harvester_upgrades = {{
    {0, "Scrap Bio-Net", "Starter Bio-Harvester", 1, 0, 0},
    {1, "Titanium Bio-Rig", "-6% Bio-Harvest Interval", 10, 200, 6},
    {2, "Durasteel Bio-Sampler", "-12% Bio-Harvest Interval", 25, 750, 12},
    {3, "Cobalt Gene-Extractor", "-18% Bio-Harvest Interval", 40, 2500, 18},
    {4, "Tungsten Drone-Trawler", "-24% Bio-Harvest Interval", 55, 8000, 24},
    {5, "Neutronium Bio-Harvester", "-30% Bio-Harvest Interval", 70, 25000, 30},
    {6, "Chrono Stasis-Harvester", "-38% Bio-Harvest Interval", 85, 80000, 38},
}};

const std::array<ShopUpgradeInfo, TOOL_TIER_COUNT> drill_upgrades = {{
    {0, "Scrap Rotary Drill", "Starter Mining Drill", 1, 0, 0},
    {1, "Titanium Impact Drill", "-6% Deep-Mining Interval", 10, 200, 6},
    {2, "Durasteel Sonic Drill", "-12% Deep-Mining Interval", 25, 750, 12},
    {3, "Cobalt Laser Bore", "-18% Deep-Mining Interval", 40, 2500, 18},
    {4, "Tungsten Plasma Bore", "-24% Deep-Mining Interval", 55, 8000, 24},
    {5, "Neutronium Quantum Drill", "-30% Deep-Mining Interval", 70, 25000, 30},
    {6, "Chrono Singularity Bore", "-38% Deep-Mining Interval", 85, 80000, 38},
}};

const std::array<ShopUpgradeInfo, TOOL_TIER_COUNT> reactor_upgrades = {{
    {0, "Basic Micro-Reactor", "Starter Synth-Reactor", 1, 0, 0},
    {1, "Plasteel Thermal Unit", "-5% Synth-Cook Interval & +5% XP", 10, 250, 5},
    {2, "Nanotube Induction Core", "-10% Synth-Cook Interval & +10% XP", 25, 900, 10},
    {3, "Positronic Reactor", "-15% Synth-Cook Interval & +15% XP", 45, 3000, 15},
    {4, "Plasma Fusion Furnace", "-20% Synth-Cook Interval & +20% XP", 60, 10000, 20},
    {5, "Quantum Synth-Core", "-26% Synth-Cook Interval & +26% XP", 75, 30000, 26},
    {6, "AI Mainframe Reactor", "-34% Synth-Cook Interval & +34% XP", 90, 95000, 34},
}};

const std::array<ShopUpgradeInfo, AUTO_STIM_TIER_COUNT> auto_stim_upgrades = {{
    {0, "No Auto-Stim", "Manual stim-pack injection only", 1, 0, 0},
    {1, "Auto-Stim — Mk I", "Auto-injects equipped stim below 25% HP", 1, 1500, 25},
    {2, "Auto-Stim — Mk II", "Auto-injects equipped stim below 40% HP", 1, 12000, 40},
    {3, "Auto-Stim — Mk III", "Auto-injects equipped stim below 55% HP", 1, 50000, 55},
}};

std::string skill_name(SkillType skill) {
  switch (skill) {
    case SkillType::Salvaging:
      return "Salvaging";
    case SkillType::BioHarvest:
      return "Bio-Harvest";
    case SkillType::Overclock:
      return "Overclock";
    case SkillType::SynthCook:
      return "Synth-Cook";
    case SkillType::DeepMining:
      return "Deep-Mining";
    case SkillType::CyberFab:
      return "Cyber-Fab";
    case SkillType::Attack:
      return "Attack";
    case SkillType::Strength:
      return "Strength";
    case SkillType::Defence:
      return "Defence";
    case SkillType::Hitpoints:
      return "Hitpoints";
    case SkillType::Bounty:
      return "Bounty";
  }
  return "Unknown";
}

std::string skill_short_name(SkillType skill) {
  switch (skill) {
    case SkillType::Salvaging:
      return "SLV";
    case SkillType::BioHarvest:
      return "BIO";
    case SkillType::Overclock:
      return "OVC";
    case SkillType::SynthCook:
      return "SYN";
    case SkillType::DeepMining:
      return "MIN";
    case SkillType::CyberFab:
      return "FAB";
    case SkillType::Attack:
      return "ATK";
    case SkillType::Strength:
      return "STR";
    case SkillType::Defence:
      return "DEF";
    case SkillType::Hitpoints:
      return "HP";
    case SkillType::Bounty:
      return "BNT";
  }
  return "???";
}

std::string item_category_name(ItemCategory cat) {
  switch (cat) {
    case ItemCategory::Scrap:
      return "Scrap";
    case ItemCategory::RawBiota:
      return "Raw Biota";
    case ItemCategory::StimFood:
      return "Stim/Ration";
    case ItemCategory::ToxicWaste:
      return "Slag";
    case ItemCategory::RawOre:
      return "Ore";
    case ItemCategory::Alloy:
      return "Alloy";
    case ItemCategory::DataCrystal:
      return "Crystal";
    case ItemCategory::Weapon:
      return "Weapon";
    case ItemCategory::Visor:
      return "Visor";
    case ItemCategory::ExoSuit:
      return "Exo-Suit";
    case ItemCategory::HoloShield:
      return "Shield";
    case ItemCategory::CyberLoot:
      return "Salvage";
  }
  return "Item";
}

std::string equip_slot_name(EquipSlot slot) {
  switch (slot) {
    case EquipSlot::Weapon:
      return "Weapon";
    case EquipSlot::Visor:
      return "Visor";
    case EquipSlot::ExoSuit:
      return "Exo-Suit";
    case EquipSlot::HoloShield:
      return "Holo-Shield";
    case EquipSlot::None:
      return "None";
  }
  return "None";
}

std::string attack_style_name(AttackStyle style) {
  switch (style) {
    case AttackStyle::Accurate:
      return "Precision (Attack)";
    case AttackStyle::Aggressive:
      return "Overdrive (Strength)";
    case AttackStyle::Defensive:
      return "Evasive (Defence)";
  }
  return "Precision";
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
  return number_string(value) + " Cr";
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
  credits = 250;
  bounty_tokens = 0;
  total_ticks_ms = 0;

  xp.fill(0);
  // Hitpoints starts at Level 10 (1,154 XP)
  xp[static_cast<int>(SkillType::Hitpoints)] = xp_for_level(10);

  action_mastery_xp.assign(skill_actions.size(), 0);

  bank_capacity = 24;
  bank.clear();

  equipped_items = {-1, -1, -1, -1};
  equipped_food_item = ITEM_KRILL_RATION;
  equipped_food_qty = 10;

  // Give starter Scrap Vibro-Knife equipped and a few scrap/rations in storage
  equipped_items[static_cast<int>(EquipSlot::Weapon)] = ITEM_SCRAP_BLADE;
  add_item(ITEM_COPPER_WIRE_SCRAP, 5, false);
  add_item(ITEM_KRILL_RATION, 5, false);

  cutter_tier = 0;
  harvester_tier = 0;
  drill_tier = 0;
  reactor_tier = 0;
  auto_stim_tier = 0;

  active_type = ActiveActivityType::Skill;
  active_action_id = 0;  // Start stripping Copper Wiring by default!
  active_progress_ms = 0;
  active_target_ms = action_effective_interval_ms(0);

  attack_style = AttackStyle::Accurate;
  player_hp = max_hp();
  active_monster_id = 0;
  monster_hp = monster_info[0].max_hp;
  player_attack_timer_ms = 0;
  monster_attack_timer_ms = 0;
  hp_regen_timer_ms = 0;

  bounty_target_id = 0;
  bounty_remaining = 8;
  bounties_completed = 0;

  monster_kills.fill(0);
  total_items_gathered = 0;
  total_monsters_killed = 0;
  total_credits_earned = 250;
  player_deaths = 0;

  game_log.clear();
  credits_history.clear();
  bank_value_history.clear();
  total_level_history.clear();
  total_xp_history.clear();
  hp_history.clear();
  for (auto& vec : skill_xp_history) vec.clear();
  history_timer_ms_ = 0;

  status_banner = "Strip Copper Wiring (Salvaging)";
  add_log("Welcome to Routineverse! You jack into Neo-Sector with a Scrap Vibro-Knife, 10 Krill Rations, and 250 Cr.");
  add_log("Active protocol: Strip Copper Wiring. Select any skill or hostile target to begin!");
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

  push_capped(credits_history, credits);
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
    case SkillType::Salvaging:
      bonus_pct = cutter_upgrades[std::clamp(cutter_tier, 0, TOOL_TIER_COUNT - 1)]
                      .speed_bonus_pct;
      break;
    case SkillType::BioHarvest:
      bonus_pct =
          harvester_upgrades[std::clamp(harvester_tier, 0, TOOL_TIER_COUNT - 1)]
              .speed_bonus_pct;
      break;
    case SkillType::DeepMining:
      bonus_pct = drill_upgrades[std::clamp(drill_tier, 0, TOOL_TIER_COUNT - 1)]
                      .speed_bonus_pct;
      break;
    case SkillType::SynthCook:
      bonus_pct =
          reactor_upgrades[std::clamp(reactor_tier, 0, TOOL_TIER_COUNT - 1)]
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
    add_log(std::format("Requires {} Level {} to execute {}.",
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
    add_log(std::format("Missing components for {}: need {}.", act.name,
                        req_str));
    return false;
  }

  active_type = ActiveActivityType::Skill;
  active_action_id = global_action_id;
  active_progress_ms = 0;
  active_target_ms = action_effective_interval_ms(global_action_id);
  status_banner = std::format("{} ({})", act.name, skill_name(act.skill));
  add_log(std::format("Started {} ({:.2f}s cycle).", act.name,
                      active_target_ms / 1000.0));
  return true;
}

bool GameState::start_combat(int monster_id) {
  if (monster_id < 0 || monster_id >= MONSTER_COUNT) return false;
  const auto& mon = monster_info[monster_id];
  if (skill_level(SkillType::Bounty) < mon.bounty_req) {
    add_log(std::format("Requires Bounty Level {} clearance to engage {}.",
                        mon.bounty_req, mon.name));
    return false;
  }
  active_type = ActiveActivityType::Combat;
  active_monster_id = monster_id;
  monster_hp = mon.max_hp;
  player_attack_timer_ms = 0;
  monster_attack_timer_ms = 0;
  status_banner = std::format("Engaging {} (Lv {}) in {}", mon.name,
                              mon.combat_level, mon.zone_name);
  add_log(std::format("Engaged hostile {} ({} HP) in {}.", mon.name,
                      mon.max_hp, mon.zone_name));
  return true;
}

void GameState::stop_activity() {
  active_type = ActiveActivityType::None;
  active_progress_ms = 0;
  status_banner = "Standby — Select a Skill or Hostile Target";
  add_log("Paused active protocol.");
}

void GameState::gain_xp(SkillType skill, long long amount) {
  if (amount <= 0) return;
  int idx = static_cast<int>(skill);
  int old_lvl = level_for_xp(xp[idx]);
  // Reactor tier grants a global XP bonus
  long long bonus = (amount * reactor_tier * 2) / 100;
  xp[idx] += (amount + bonus);
  int new_lvl = level_for_xp(xp[idx]);
  if (new_lvl > old_lvl) {
    add_log(std::format("NEURAL UPGRADE! Your {} skill is now Level {}!",
                        skill_name(skill), new_lvl));
    if (skill == SkillType::Hitpoints) {
      player_hp += (new_lvl - old_lvl) * 10;
      player_hp = std::min(player_hp, max_hp());
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

  // Resource preservation chance for Fabrication/Synthesis skills (5% + 0.2% per mastery level)
  bool preserved = false;
  if (act.input_item_1 >= 0 &&
      (act.skill == SkillType::CyberFab || act.skill == SkillType::SynthCook ||
       act.skill == SkillType::Overclock)) {
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

  // Double output chance (5% + 0.3% per mastery level)
  int qty = act.product_qty;
  if (qty > 0 && rand_int(1, 100) <= (5 + m_lvl / 3)) {
    qty *= 2;
  }

  if (act.skill == SkillType::SynthCook && act.product_item >= 0) {
    // Synthesis success chance (75% base + mastery/level bonus up to 99%)
    int cook_chance =
        std::min(99, 74 + (skill_level(SkillType::SynthCook) - act.req_level) / 2 +
                         m_lvl / 4);
    if (rand_int(1, 100) <= cook_chance) {
      add_item(act.product_item, qty, false);
      total_items_gathered += qty;
    } else {
      add_item(ITEM_TOXIC_SLAG, 1, false);
      add_log(std::format("Synthesis contaminated! Ruined {}.",
                          item_info[act.input_item_1].name));
    }
  } else if (act.product_item >= 0 && qty > 0) {
    add_item(act.product_item, qty, false);
    total_items_gathered += qty;
  }

  // Bonus procs by skill
  if (act.skill == SkillType::Overclock) {
    // 25% chance to discharge a Carbon Cell from overclocking scrap, plus crypto-credit yield
    if (rand_int(1, 100) <= 25) {
      add_item(ITEM_CARBON_CELL, 1, false);
    }
    credits += 2 + act.req_level / 5;
    total_credits_earned += 2 + act.req_level / 5;
  } else if (act.skill == SkillType::DeepMining) {
    // 8% chance to unearth a rare Data Crystal while deep-mining!
    if (rand_int(1, 100) <= 8) {
      int gem_id = ITEM_AMBER_DATACHIP + rand_int(0, 4);
      if (add_item(gem_id, 1, false)) {
        add_log(std::format("While deep-mining, you extracted a rare {}!",
                            item_info[gem_id].name));
      }
    }
  } else if (act.skill == SkillType::BioHarvest) {
    // 5% chance to recover a submerged Corp Data-Cache (Credits)
    if (rand_int(1, 100) <= 5) {
      int bonus_cr = 25 + act.req_level * 8;
      credits += bonus_cr;
      total_credits_earned += bonus_cr;
      add_log(std::format("Recovered a submerged Corp Data-Cache worth {}!",
                          money_string(bonus_cr)));
    }
  }

  // Stop if materials ran out after this action
  if (!can_perform_action(global_action_id)) {
    add_log(std::format("Completed {}: input components depleted.",
                        act.name));
    stop_activity();
  }
}

void GameState::tick(int elapsed_ms) {
  if (elapsed_ms <= 0) return;
  total_ticks_ms += elapsed_ms;

  // Passive nanite HP regeneration outside/inside combat (+1% max HP every 5 seconds)
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

  int cr_drop = rand_int(mon.credits_min, mon.credits_max);
  credits += cr_drop;
  total_credits_earned += cr_drop;

  // Bonus XP on kill
  if (attack_style == AttackStyle::Accurate) {
    gain_xp(SkillType::Attack, mon.xp_reward);
  } else if (attack_style == AttackStyle::Aggressive) {
    gain_xp(SkillType::Strength, mon.xp_reward);
  } else {
    gain_xp(SkillType::Defence, mon.xp_reward);
  }
  gain_xp(SkillType::Hitpoints, mon.xp_reward / 3);

  // Bounty contract check
  if (monster_id == bounty_target_id && bounty_remaining > 0) {
    bounty_remaining--;
    gain_xp(SkillType::Bounty, mon.xp_reward / 2 + 15);
    int bt = std::max(5, mon.combat_level * 2);
    bounty_tokens += bt;
    if (bounty_remaining <= 0) {
      bounties_completed++;
      int bonus_bt = 50 + bounties_completed * 15;
      bounty_tokens += bonus_bt;
      gain_xp(SkillType::Bounty, 120 + mon.xp_reward);
      add_log(std::format(
          "BOUNTY CONTRACT COMPLETE! Earned +{} Bounty Tokens! Assigning new target...",
          bonus_bt));
      assign_new_bounty_contract();
    }
  } else if (mon.bounty_req > 1) {
    gain_xp(SkillType::Bounty, mon.xp_reward / 4);
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
    add_log(std::format("Neutralized {}! Siphoned {}.", mon.name,
                        money_string(cr_drop)));
  } else {
    add_log(std::format("Neutralized {}! Siphoned {} and salvaged {}.",
                        mon.name, money_string(cr_drop), loot_str));
  }

  // Respawn monster
  monster_hp = mon.max_hp;
  player_attack_timer_ms = 0;
  monster_attack_timer_ms = 0;
}

void GameState::on_player_defeated() {
  player_deaths++;
  player_hp = max_hp();
  long long lost_cr = std::min(credits, std::max(10LL, credits / 10));
  credits -= lost_cr;
  add_log(std::format(
      "CRITICAL FLATLINE fighting {}! Trauma Team reconstructed you in Neo-Sector for {}.",
      monster_info[active_monster_id].name, money_string(lost_cr)));
  stop_activity();
}

void GameState::assign_new_bounty_contract() {
  std::vector<int> eligible;
  int b_lvl = skill_level(SkillType::Bounty);
  int c_lvl = combat_level();
  for (int i = 0; i < MONSTER_COUNT; ++i) {
    if (monster_info[i].bounty_req <= b_lvl &&
        monster_info[i].combat_level <= c_lvl + 15 &&
        !monster_info[i].is_boss) {
      eligible.push_back(i);
    }
  }
  if (eligible.empty()) eligible.push_back(0);
  bounty_target_id = eligible[rand_int(0, static_cast<int>(eligible.size()) - 1)];
  bounty_remaining = rand_int(6, 15);
  add_log(std::format("New Bounty Contract: Neutralize {}x {} ({}).",
                      bounty_remaining,
                      monster_info[bounty_target_id].name,
                      monster_info[bounty_target_id].zone_name));
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
  for (auto& s : bank) {
    if (s.item_id == item_id) {
      s.qty += qty;
      if (log_drop) {
        add_log(std::format("Stored {}x {} in Cyber-Vault.", qty,
                            item_info[item_id].name));
      }
      return true;
    }
  }
  if (static_cast<int>(bank.size()) >= bank_capacity) {
    add_log(std::format("Cyber-Vault is full ({}/{})! Could not store {}!",
                        bank.size(), bank_capacity, item_info[item_id].name));
    return false;
  }
  bank.push_back(BankSlot{item_id, qty});
  if (log_drop) {
    add_log(std::format("Stored {}x {} in Cyber-Vault.", qty,
                        item_info[item_id].name));
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
  credits += value;
  total_credits_earned += value;
  add_log(std::format("Liquidated {}x {} for {}.", sell_q,
                      item_info[item_id].name, money_string(value)));
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
    credits += gained;
    total_credits_earned += gained;
    add_log(std::format("Liquidated all {} Vault items for {}!", items_sold,
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
  add_log(std::format("Installed {} in {} slot.", info.name,
                      equip_slot_name(info.equip_slot)));
  return true;
}

bool GameState::unequip_slot(EquipSlot slot) {
  int slot_idx = static_cast<int>(slot);
  if (slot_idx < 0 || slot_idx >= EQUIP_SLOT_COUNT) return false;
  int cur = equipped_items[slot_idx];
  if (cur < 0) return false;
  if (!can_store_item(cur)) {
    add_log("Cyber-Vault is full! Cannot unequip item.");
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
    add_log(std::format("Loaded {}x {} into Stim-Injector ({} total).", have,
                        item_info[item_id].name, equipped_food_qty));
    return true;
  }

  // Return old equipped stim to vault if any
  if (equipped_food_item >= 0 && equipped_food_qty > 0) {
    if (!can_store_item(equipped_food_item)) {
      add_log("Cyber-Vault is full! Cannot swap equipped stims.");
      return false;
    }
    add_item(equipped_food_item, equipped_food_qty, false);
  }
  remove_item(item_id, have);
  equipped_food_item = item_id;
  equipped_food_qty = have;
  add_log(std::format("Loaded {}x {} (+{} HP each).", have,
                      item_info[item_id].name, item_info[item_id].heal_amount));
  return true;
}

bool GameState::eat_food() {
  if (equipped_food_item < 0 || equipped_food_qty <= 0) {
    add_log("No stim-pack or ration loaded!");
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
  add_log(std::format("Used {} and restored +{} HP ({}/{} HP).",
                      item_info[equipped_food_item].name, player_hp - before,
                      player_hp, max_hp()));
  if (equipped_food_qty == 0) {
    equipped_food_item = -1;
  }
  return true;
}

void GameState::check_auto_eat() {
  if (auto_stim_tier <= 0) return;
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

bool GameState::buy_cutter_upgrade() {
  if (cutter_tier + 1 >= TOOL_TIER_COUNT) return false;
  const auto& upg = cutter_upgrades[cutter_tier + 1];
  if (skill_level(SkillType::Salvaging) < upg.req_skill_level) {
    add_log(std::format("Requires Salvaging Level {} to buy {}.",
                        upg.req_skill_level, upg.name));
    return false;
  }
  if (credits < upg.cost_credits) {
    add_log(std::format("Not enough Credits for {} (need {}).", upg.name,
                        money_string(upg.cost_credits)));
    return false;
  }
  credits -= upg.cost_credits;
  cutter_tier++;
  add_log(std::format("Purchased {} ({})!", upg.name, upg.description));
  return true;
}

bool GameState::buy_harvester_upgrade() {
  if (harvester_tier + 1 >= TOOL_TIER_COUNT) return false;
  const auto& upg = harvester_upgrades[harvester_tier + 1];
  if (skill_level(SkillType::BioHarvest) < upg.req_skill_level) {
    add_log(std::format("Requires Bio-Harvest Level {} to buy {}.",
                        upg.req_skill_level, upg.name));
    return false;
  }
  if (credits < upg.cost_credits) {
    add_log(std::format("Not enough Credits for {} (need {}).", upg.name,
                        money_string(upg.cost_credits)));
    return false;
  }
  credits -= upg.cost_credits;
  harvester_tier++;
  add_log(std::format("Purchased {} ({})!", upg.name, upg.description));
  return true;
}

bool GameState::buy_drill_upgrade() {
  if (drill_tier + 1 >= TOOL_TIER_COUNT) return false;
  const auto& upg = drill_upgrades[drill_tier + 1];
  if (skill_level(SkillType::DeepMining) < upg.req_skill_level) {
    add_log(std::format("Requires Deep-Mining Level {} to buy {}.",
                        upg.req_skill_level, upg.name));
    return false;
  }
  if (credits < upg.cost_credits) {
    add_log(std::format("Not enough Credits for {} (need {}).", upg.name,
                        money_string(upg.cost_credits)));
    return false;
  }
  credits -= upg.cost_credits;
  drill_tier++;
  add_log(std::format("Purchased {} ({})!", upg.name, upg.description));
  return true;
}

bool GameState::buy_reactor_upgrade() {
  if (reactor_tier + 1 >= TOOL_TIER_COUNT) return false;
  const auto& upg = reactor_upgrades[reactor_tier + 1];
  if (skill_level(SkillType::Overclock) < upg.req_skill_level) {
    add_log(std::format("Requires Overclock Level {} to buy {}.",
                        upg.req_skill_level, upg.name));
    return false;
  }
  if (credits < upg.cost_credits) {
    add_log(std::format("Not enough Credits for {} (need {}).", upg.name,
                        money_string(upg.cost_credits)));
    return false;
  }
  credits -= upg.cost_credits;
  reactor_tier++;
  add_log(std::format("Purchased {} ({})!", upg.name, upg.description));
  return true;
}

bool GameState::buy_auto_stim_upgrade() {
  if (auto_stim_tier + 1 >= AUTO_STIM_TIER_COUNT) return false;
  const auto& upg = auto_stim_upgrades[auto_stim_tier + 1];
  if (credits < upg.cost_credits) {
    add_log(std::format("Not enough Credits for {} (need {}).", upg.name,
                        money_string(upg.cost_credits)));
    return false;
  }
  credits -= upg.cost_credits;
  auto_stim_tier++;
  add_log(std::format("Purchased {} ({})!", upg.name, upg.description));
  return true;
}

bool GameState::buy_bank_slot() {
  int cost = next_bank_slot_cost();
  if (credits < cost) {
    add_log(std::format("Not enough Credits for +4 Vault Slots (need {}).",
                        money_string(cost)));
    return false;
  }
  credits -= cost;
  bank_capacity += 4;
  add_log(std::format("Purchased +4 Vault Slots! Cyber-Vault capacity is now {}.",
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
  if (auto_stim_tier <= 0 || auto_stim_tier >= AUTO_STIM_TIER_COUNT) return 0;
  int pct = auto_stim_upgrades[auto_stim_tier].speed_bonus_pct;
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
  out << credits << " " << bounty_tokens << " " << total_ticks_ms << "\n";
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
  out << cutter_tier << " " << harvester_tier << " " << drill_tier << " "
      << reactor_tier << " " << auto_stim_tier << "\n";
  out << static_cast<int>(active_type) << " " << active_action_id << " "
      << active_monster_id << " " << player_hp << " " << monster_hp << " "
      << static_cast<int>(attack_style) << "\n";
  out << bounty_target_id << " " << bounty_remaining << " "
      << bounties_completed << "\n";
  out << total_items_gathered << " " << total_monsters_killed << " "
      << total_credits_earned << " " << player_deaths << "\n";
  return out.good();
}

bool GameState::load_from_file(const std::string& path) {
  std::ifstream in(path);
  if (!in.is_open()) return false;

  std::string header;
  if (!(in >> header) || header != "ROUTINEVERSE_SAVE_V1") return false;

  in >> credits >> bounty_tokens >> total_ticks_ms;
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
  in >> cutter_tier >> harvester_tier >> drill_tier >> reactor_tier >>
      auto_stim_tier;

  int act_t = 0;
  int style_t = 0;
  in >> act_t >> active_action_id >> active_monster_id >> player_hp >>
      monster_hp >> style_t;
  active_type = static_cast<ActiveActivityType>(std::clamp(act_t, 0, 2));
  attack_style = static_cast<AttackStyle>(std::clamp(style_t, 0, 2));

  in >> bounty_target_id >> bounty_remaining >> bounties_completed;
  in >> total_items_gathered >> total_monsters_killed >>
      total_credits_earned >> player_deaths;

  if (active_type == ActiveActivityType::Skill && active_action_id >= 0 &&
      active_action_id < static_cast<int>(skill_actions.size())) {
    active_target_ms = action_effective_interval_ms(active_action_id);
    status_banner =
        std::format("{} ({})", skill_actions[active_action_id].name,
                    skill_name(skill_actions[active_action_id].skill));
  } else if (active_type == ActiveActivityType::Combat &&
             active_monster_id >= 0 && active_monster_id < MONSTER_COUNT) {
    status_banner =
        std::format("Engaging {}", monster_info[active_monster_id].name);
  } else {
    status_banner = "Standby — Select a Skill or Hostile Target";
  }

  record_history_snapshot();
  add_log("Loaded saved neural state.");
  return true;
}
