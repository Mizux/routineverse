#include "routineverse.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <fstream>
#include <random>

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
const std::array<uint64_t, MAX_SKILL_LEVEL + 1>& xp_table() {
  static const auto table = []() {
    std::array<uint64_t, MAX_SKILL_LEVEL + 1> t{};
    t[0] = 0;
    t[1] = 0;
    double points = 0.0;
    for (size_t lvl = 1; lvl < MAX_SKILL_LEVEL; ++lvl) {
      points += std::floor(lvl + 300.0 * std::pow(2.0, lvl / 7.0));
      t[lvl + 1] = static_cast<uint64_t>(std::floor(points / 4.0));
    }
    return t;
  }();
  return table;
}

}  // namespace

const ItemInfo& get_item_info(ItemId id) {
  static const std::array item_info{std::to_array<ItemInfo>({
      {ItemId::None, "Empty", ItemCategory::Scrap, 0, 0, 0, {0, 0, 0, 0}},
      // Scrap & Tech Nodes
      {ItemId::CopperWireScrap,
       "Copper Wire Scrap",
       ItemCategory::Scrap,
       2,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::PlasteelShards,
       "Plasteel Shards",
       ItemCategory::Scrap,
       5,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::CarbonNanotubes,
       "Carbon Nanotubes",
       ItemCategory::Scrap,
       10,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::OpticFiberBundle,
       "Optic Fiber Bundle",
       ItemCategory::Scrap,
       18,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::PositronicRelays,
       "Positronic Relays",
       ItemCategory::Scrap,
       30,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::CryoCellCore,
       "Cryo-Cell Core",
       ItemCategory::Scrap,
       45,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::PlasmaConduit,
       "Plasma Conduit",
       ItemCategory::Scrap,
       70,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::QuantumNode,
       "Quantum Node",
       ItemCategory::Scrap,
       120,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::AiMainframeCore,
       "AI Mainframe Core",
       ItemCategory::Scrap,
       200,
       0,
       1,
       {0, 0, 0, 0}},

      // Recycled Basic / Raw Materials
      {ItemId::CopperFilament,
       "Copper Filament",
       ItemCategory::RawMaterial,
       6,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::PlasteelPolymer,
       "Plasteel Polymer",
       ItemCategory::RawMaterial,
       14,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::CarbonFiberWeave,
       "Carbon Fiber Weave",
       ItemCategory::RawMaterial,
       28,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::OpticSilicaGlass,
       "Optic Silica Glass",
       ItemCategory::RawMaterial,
       48,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::PositronicWafer,
       "Positronic Wafer",
       ItemCategory::RawMaterial,
       80,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::CryoCoolantGel,
       "Cryo-Coolant Gel",
       ItemCategory::RawMaterial,
       120,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::PlasmaCoil,
       "Magnetic Plasma Coil",
       ItemCategory::RawMaterial,
       185,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::QuantumLattice,
       "Quantum Lattice",
       ItemCategory::RawMaterial,
       310,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::NeuralMatrix,
       "Neural Matrix",
       ItemCategory::RawMaterial,
       520,
       0,
       1,
       {0, 0, 0, 0}},

      // Raw Synth-Biota
      {ItemId::RawKrillBiomass,
       "Raw Krill Biomass",
       ItemCategory::RawBiota,
       3,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::RawNeonEel,
       "Raw Neon Eel",
       ItemCategory::RawBiota,
       6,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::RawSynthCarp,
       "Raw Synth-Carp",
       ItemCategory::RawBiota,
       14,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::RawChromeSalmon,
       "Raw Chrome Salmon",
       ItemCategory::RawBiota,
       24,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::RawCyberLobster,
       "Raw Cyber-Lobster",
       ItemCategory::RawBiota,
       45,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::RawPlasmaRay,
       "Raw Plasma Ray",
       ItemCategory::RawBiota,
       75,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::RawApexShark,
       "Raw Apex Shark",
       ItemCategory::RawBiota,
       140,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::RawLeviathanCell,
       "Raw Leviathan Cell",
       ItemCategory::RawBiota,
       260,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::RawCyberKraken,
       "Raw Cyber-Kraken",
       ItemCategory::RawBiota,
       450,
       0,
       1,
       {0, 0, 0, 0}},

      // Hydro-Farmed Crops & Synth-Noodles
      {ItemId::HydroWheat, "Hydro-Wheat", ItemCategory::Crop, 4, 0, 1, {0, 0, 0, 0}},
      {ItemId::SoyPods, "Synth-Soy Pods", ItemCategory::Crop, 10, 0, 1, {0, 0, 0, 0}},
      {ItemId::NeonScallion,
       "Neon Scallion",
       ItemCategory::Crop,
       22,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::GlowNori,
       "Bioluminescent Nori",
       ItemCategory::Crop,
       42,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::BioBamboo,
       "Cyber-Bamboo Shoot",
       ItemCategory::Crop,
       75,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::CyberShiitake,
       "Spore-Tech Shiitake",
       ItemCategory::Crop,
       130,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::PlasmaChili,
       "Plasma Ghost-Chili",
       ItemCategory::Crop,
       225,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::ChronoLotus,
       "Chrono-Lotus Root",
       ItemCategory::Crop,
       380,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::QuantumTruffle,
       "Quantum Myco-Truffle",
       ItemCategory::Crop,
       650,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::SynthNoodles,
       "Synth-Noodles",
       ItemCategory::Crop,
       8,
       0,
       1,
       {0, 0, 0, 0}},

      // Synthesized Stims, Cyber-Ramen & Toxic Slag
      {ItemId::KrillRation,
       "Krill Ration",
       ItemCategory::StimFood,
       8,
       30,
       1,
       {0, 0, 0, 0}},
      {ItemId::NeonEelSkewer,
       "Neon Eel Skewer",
       ItemCategory::StimFood,
       15,
       50,
       1,
       {0, 0, 0, 0}},
      {ItemId::SynthCarpPack,
       "Synth-Carp Pack",
       ItemCategory::StimFood,
       32,
       80,
       1,
       {0, 0, 0, 0}},
      {ItemId::ChromeSalmonStim,
       "Chrome Salmon Stim",
       ItemCategory::StimFood,
       55,
       110,
       1,
       {0, 0, 0, 0}},
      {ItemId::CyberLobsterMeal,
       "Cyber-Lobster Meal",
       ItemCategory::StimFood,
       100,
       160,
       1,
       {0, 0, 0, 0}},
      {ItemId::PlasmaRayInfusion,
       "Plasma Ray Infusion",
       ItemCategory::StimFood,
       165,
       220,
       1,
       {0, 0, 0, 0}},
      {ItemId::ApexSharkBooster,
       "Apex Shark Booster",
       ItemCategory::StimFood,
       300,
       320,
       1,
       {0, 0, 0, 0}},
      {ItemId::LeviathanNanomed,
       "Leviathan Nanomed",
       ItemCategory::StimFood,
       550,
       480,
       1,
       {0, 0, 0, 0}},
      {ItemId::KrakenBioElixir,
       "Kraken Bio-Elixir",
       ItemCategory::StimFood,
       950,
       680,
       1,
       {0, 0, 0, 0}},
      {ItemId::ShoyuRamen,
       "Soy-Shoyu Cyber-Ramen",
       ItemCategory::StimFood,
       35,
       65,
       1,
       {0, 0, 0, 0}},
      {ItemId::ScallionRamen,
       "Neon Scallion Ramen",
       ItemCategory::StimFood,
       68,
       125,
       1,
       {0, 0, 0, 0}},
      {ItemId::NoriRamen,
       "Glow-Nori Umami Ramen",
       ItemCategory::StimFood,
       135,
       195,
       1,
       {0, 0, 0, 0}},
      {ItemId::BambooRamen,
       "Cyber-Bamboo Miso Ramen",
       ItemCategory::StimFood,
       230,
       280,
       1,
       {0, 0, 0, 0}},
      {ItemId::ShiitakeRamen,
       "Spore-Shiitake Tonkotsu Ramen",
       ItemCategory::StimFood,
       390,
       390,
       1,
       {0, 0, 0, 0}},
      {ItemId::PlasmaChiliRamen,
       "Plasma Volcano Ramen",
       ItemCategory::StimFood,
       680,
       540,
       1,
       {0, 0, 0, 0}},
      {ItemId::ChronoLotusRamen,
       "Chrono-Lotus Broth Ramen",
       ItemCategory::StimFood,
       1150,
       720,
       1,
       {0, 0, 0, 0}},
      {ItemId::TruffleRamen,
       "Quantum Truffle Ramen",
       ItemCategory::StimFood,
       1850,
       860,
       1,
       {0, 0, 0, 0}},
      {ItemId::QuantumKrakenRamen,
       "Quantum Kraken Special Ramen",
       ItemCategory::StimFood,
       2200,
       980,
       1,
       {0, 0, 0, 0}},
      {ItemId::SynthProteinBar,
       "Synth-Protein Bar",
       ItemCategory::StimFood,
       6,
       25,
       1,
       {0, 0, 0, 0}},
      {ItemId::ToxicSlag,
       "Toxic Bio-Slag",
       ItemCategory::ToxicWaste,
       1,
       0,
       1,
       {0, 0, 0, 0}},

      // Deep-Mined Ores & Cells
      {ItemId::CopperOre, "Copper Ore", ItemCategory::RawOre, 4, 0, 1, {0, 0, 0, 0}},
      {ItemId::SiliconOre, "Silicon Ore", ItemCategory::RawOre, 4, 0, 1, {0, 0, 0, 0}},
      {ItemId::TitaniumOre,
       "Titanium Ore",
       ItemCategory::RawOre,
       12,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::CarbonCell, "Carbon Cell", ItemCategory::RawOre, 18, 0, 1, {0, 0, 0, 0}},
      {ItemId::SilverOre, "Silver Ore", ItemCategory::RawOre, 30, 0, 1, {0, 0, 0, 0}},
      {ItemId::GoldOre, "Gold Ore", ItemCategory::RawOre, 50, 0, 1, {0, 0, 0, 0}},
      {ItemId::CobaltOre, "Cobalt Ore", ItemCategory::RawOre, 70, 0, 1, {0, 0, 0, 0}},
      {ItemId::TungstenOre,
       "Tungsten Ore",
       ItemCategory::RawOre,
       120,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::NeutroniumOre,
       "Neutronium Ore",
       ItemCategory::RawOre,
       220,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::ChronoOre,
       "Chrono-Crystal Ore",
       ItemCategory::RawOre,
       400,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::QuantumOre,
       "Quantum Singularity Ore",
       ItemCategory::RawOre,
       750,
       0,
       1,
       {0, 0, 0, 0}},

      // Refined Alloys & Conductors
      {ItemId::ScrapAlloy,
       "Scrap-Alloy Ingot",
       ItemCategory::Alloy,
       15,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::TitaniumAlloy,
       "Titanium Ingot",
       ItemCategory::Alloy,
       32,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::DurasteelAlloy,
       "Durasteel Ingot",
       ItemCategory::Alloy,
       65,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::SilverConductor,
       "Silver Conductor",
       ItemCategory::Alloy,
       80,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::GoldSuperconductor,
       "Gold Superconductor",
       ItemCategory::Alloy,
       130,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::CobaltAlloy,
       "Cobalt-Chrome Ingot",
       ItemCategory::Alloy,
       175,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::TungstenAlloy,
       "Tungsten Ingot",
       ItemCategory::Alloy,
       310,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::NeutroniumAlloy,
       "Neutronium Ingot",
       ItemCategory::Alloy,
       580,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::ChronoAlloy,
       "Chrono-Alloy Ingot",
       ItemCategory::Alloy,
       1100,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::QuantumAlloy,
       "Quantum-Flux Ingot",
       ItemCategory::Alloy,
       2000,
       0,
       1,
       {0, 0, 0, 0}},

      // Data Crystals
      {ItemId::AmberDatachip,
       "Amber Datachip",
       ItemCategory::DataCrystal,
       150,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::SapphireCortex,
       "Sapphire Cortex",
       ItemCategory::DataCrystal,
       250,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::RubyLaserCore,
       "Ruby Laser Core",
       ItemCategory::DataCrystal,
       450,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::EmeraldCryptokey,
       "Emerald Cryptokey",
       ItemCategory::DataCrystal,
       750,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::QuantumDiamond,
       "Quantum Diamond",
       ItemCategory::DataCrystal,
       1500,
       0,
       1,
       {0, 0, 0, 0}},

      // Gears
      // Weapons - Melee
      {ItemId::ScrapBlade,
       "Scrap Vibro-Knife",
       ItemCategory::Weapon,
       45,
       0,
       1,
       {10, 12, 0, 0}},
      {ItemId::TitaniumBlade,
       "Titanium Mono-Blade",
       ItemCategory::Weapon,
       100,
       0,
       5,
       {18, 20, 0, 0}},
      {ItemId::DurasteelBlade,
       "Durasteel Katana",
       ItemCategory::Weapon,
       220,
       0,
       10,
       {28, 32, 0, 0}},
      {ItemId::CobaltBlade,
       "Cobalt Laser-Edge",
       ItemCategory::Weapon,
       550,
       0,
       20,
       {42, 48, 0, 0}},
      {ItemId::TungstenBlade,
       "Tungsten Mantis-Blade",
       ItemCategory::Weapon,
       1100,
       0,
       30,
       {60, 66, 0, 0}},
      {ItemId::NeutroniumBlade,
       "Neutronium Phase-Saber",
       ItemCategory::Weapon,
       2400,
       0,
       40,
       {84, 92, 0, 0}},
      {ItemId::ChronoBlade,
       "Chrono-Edge Katana",
       ItemCategory::Weapon,
       6000,
       0,
       60,
       {120, 130, 0, 0}},
      {ItemId::QuantumBlade,
       "Quantum Singularity Blade",
       ItemCategory::Weapon,
       12000,
       0,
       75,
       {165, 180, 0, 0}},

      // Visors
      {ItemId::ScrapVisor,
       "Scrap Optic Visor",
       ItemCategory::Head,
       40,
       0,
       1,
       {0, 0, 6, 1}},
      {ItemId::TitaniumVisor,
       "Titanium HUD Visor",
       ItemCategory::Head,
       90,
       0,
       5,
       {0, 0, 11, 2}},
      {ItemId::DurasteelVisor,
       "Durasteel Tac-Helm",
       ItemCategory::Head,
       200,
       0,
       10,
       {0, 0, 18, 3}},
      {ItemId::CobaltVisor,
       "Cobalt Neural Visor",
       ItemCategory::Head,
       500,
       0,
       20,
       {0, 0, 27, 4}},
      {ItemId::TungstenVisor,
       "Tungsten Cyber-Helm",
       ItemCategory::Head,
       950,
       0,
       30,
       {0, 0, 38, 5}},
      {ItemId::NeutroniumVisor,
       "Neutronium Mind-Crown",
       ItemCategory::Head,
       2100,
       0,
       40,
       {0, 0, 52, 7}},
      {ItemId::ChronoVisor,
       "Chrono-Sync Visor",
       ItemCategory::Head,
       5200,
       0,
       60,
       {0, 0, 72, 10}},
      {ItemId::QuantumVisor,
       "Quantum Tachyon Visor",
       ItemCategory::Head,
       10500,
       0,
       75,
       {0, 0, 98, 13}},

      // Exo-Suits
      {ItemId::ScrapExoSuit,
       "Scrap Exo-Harness",
       ItemCategory::Armor,
       85,
       0,
       1,
       {0, 0, 14, 2}},
      {ItemId::TitaniumExoSuit,
       "Titanium Flak-Jacket",
       ItemCategory::Armor,
       180,
       0,
       5,
       {0, 0, 24, 3}},
      {ItemId::DurasteelExoSuit,
       "Durasteel Exo-Rig",
       ItemCategory::Armor,
       400,
       0,
       10,
       {0, 0, 36, 5}},
      {ItemId::CobaltExoSuit,
       "Cobalt Subdermal Rig",
       ItemCategory::Armor,
       950,
       0,
       20,
       {0, 0, 52, 7}},
      {ItemId::TungstenExoSuit,
       "Tungsten Power-Armor",
       ItemCategory::Armor,
       1900,
       0,
       30,
       {0, 0, 74, 9}},
      {ItemId::NeutroniumExoSuit,
       "Neutronium Nano-Suit",
       ItemCategory::Armor,
       4200,
       0,
       40,
       {0, 0, 102, 12}},
      {ItemId::ChronoExoSuit,
       "Chrono-Weave Exo-Suit",
       ItemCategory::Armor,
       9800,
       0,
       60,
       {0, 0, 140, 16}},
      {ItemId::QuantumExoSuit,
       "Quantum Phase Exo-Suit",
       ItemCategory::Armor,
       19500,
       0,
       75,
       {0, 0, 190, 20}},

      // Holo-Shields
      {ItemId::ScrapShield,
       "Scrap Riot Buckler",
       ItemCategory::Shield,
       55,
       0,
       1,
       {0, 0, 9, 1}},
      {ItemId::TitaniumShield,
       "Titanium Deflector",
       ItemCategory::Shield,
       120,
       0,
       5,
       {0, 0, 16, 2}},
      {ItemId::DurasteelShield,
       "Durasteel Barrier",
       ItemCategory::Shield,
       260,
       0,
       10,
       {0, 0, 25, 3}},
      {ItemId::CobaltShield,
       "Cobalt Holo-Aegis",
       ItemCategory::Shield,
       620,
       0,
       20,
       {0, 0, 36, 5}},
      {ItemId::TungstenShield,
       "Tungsten Pulse-Shield",
       ItemCategory::Shield,
       1250,
       0,
       30,
       {0, 0, 50, 6}},
      {ItemId::NeutroniumShield,
       "Neutronium Forcefield",
       ItemCategory::Shield,
       2800,
       0,
       40,
       {0, 0, 68, 8}},
      {ItemId::ChronoShield,
       "Chrono-Phase Barrier",
       ItemCategory::Shield,
       6800,
       0,
       60,
       {0, 0, 96, 12}},
      {ItemId::QuantumShield,
       "Quantum Event-Horizon Shield",
       ItemCategory::Shield,
       13500,
       0,
       75,
       {0, 0, 132, 15}},

      // Tools - Salvaging Cutters
      {ItemId::ScrapCutter,
       "Scrap Cutter",
       ItemCategory::Cutter,
       10,
       0,
       1,
       {0, 0, 0, 0, 0}},
      {ItemId::TitaniumCutter,
       "Titanium Cutter",
       ItemCategory::Cutter,
       50,
       0,
       10,
       {0, 0, 0, 0, 6}},
      {ItemId::DurasteelCutter,
       "Durasteel Cutter",
       ItemCategory::Cutter,
       180,
       0,
       25,
       {0, 0, 0, 0, 12}},
      {ItemId::CobaltCutter,
       "Cobalt Laser-Cutter",
       ItemCategory::Cutter,
       625,
       0,
       40,
       {0, 0, 0, 0, 18}},
      {ItemId::TungstenCutter,
       "Tungsten Plasma-Torch",
       ItemCategory::Cutter,
       2000,
       0,
       55,
       {0, 0, 0, 0, 24}},
      {ItemId::NeutroniumCutter,
       "Neutronium Arc-Splicer",
       ItemCategory::Cutter,
       6250,
       0,
       70,
       {0, 0, 0, 0, 30}},
      {ItemId::ChronoCutter,
       "Chrono Deconstructor",
       ItemCategory::Cutter,
       20000,
       0,
       85,
       {0, 0, 0, 0, 38}},

      // Tools - Bio-Harvesters
      {ItemId::ScrapHarvester,
       "Scrap Bio-Net",
       ItemCategory::Harvester,
       10,
       0,
       1,
       {0, 0, 0, 0, 0}},
      {ItemId::TitaniumHarvester,
       "Titanium Bio-Rig",
       ItemCategory::Harvester,
       50,
       0,
       10,
       {0, 0, 0, 0, 6}},
      {ItemId::DurasteelHarvester,
       "Durasteel Bio-Sampler",
       ItemCategory::Harvester,
       180,
       0,
       25,
       {0, 0, 0, 0, 12}},
      {ItemId::CobaltHarvester,
       "Cobalt Gene-Extractor",
       ItemCategory::Harvester,
       625,
       0,
       40,
       {0, 0, 0, 0, 18}},
      {ItemId::TungstenHarvester,
       "Tungsten Drone-Trawler",
       ItemCategory::Harvester,
       2000,
       0,
       55,
       {0, 0, 0, 0, 24}},
      {ItemId::NeutroniumHarvester,
       "Neutronium Bio-Harvester",
       ItemCategory::Harvester,
       6250,
       0,
       70,
       {0, 0, 0, 0, 30}},
      {ItemId::ChronoHarvester,
       "Chrono Stasis-Harvester",
       ItemCategory::Harvester,
       20000,
       0,
       85,
       {0, 0, 0, 0, 38}},

      // Tools - Mining Drills
      {ItemId::ScrapDrill,
       "Scrap Rotary Drill",
       ItemCategory::Drill,
       10,
       0,
       1,
       {0, 0, 0, 0, 0}},
      {ItemId::TitaniumDrill,
       "Titanium Impact Drill",
       ItemCategory::Drill,
       50,
       0,
       10,
       {0, 0, 0, 0, 6}},
      {ItemId::DurasteelDrill,
       "Durasteel Sonic Drill",
       ItemCategory::Drill,
       180,
       0,
       25,
       {0, 0, 0, 0, 12}},
      {ItemId::CobaltDrill,
       "Cobalt Laser Bore",
       ItemCategory::Drill,
       625,
       0,
       40,
       {0, 0, 0, 0, 18}},
      {ItemId::TungstenDrill,
       "Tungsten Plasma Bore",
       ItemCategory::Drill,
       2000,
       0,
       55,
       {0, 0, 0, 0, 24}},
      {ItemId::NeutroniumDrill,
       "Neutronium Quantum Drill",
       ItemCategory::Drill,
       6250,
       0,
       70,
       {0, 0, 0, 0, 30}},
      {ItemId::ChronoDrill,
       "Chrono Singularity Bore",
       ItemCategory::Drill,
       20000,
       0,
       85,
       {0, 0, 0, 0, 38}},

      // Tools - Synth-Reactors
      {ItemId::BasicReactor,
       "Basic Micro-Reactor",
       ItemCategory::Reactor,
       10,
       0,
       1,
       {0, 0, 0, 0, 0}},
      {ItemId::PlasteelReactor,
       "Plasteel Thermal Unit",
       ItemCategory::Reactor,
       60,
       0,
       10,
       {0, 0, 0, 0, 5}},
      {ItemId::NanotubeReactor,
       "Nanotube Induction Core",
       ItemCategory::Reactor,
       225,
       0,
       25,
       {0, 0, 0, 0, 10}},
      {ItemId::PositronicReactor,
       "Positronic Reactor",
       ItemCategory::Reactor,
       750,
       0,
       45,
       {0, 0, 0, 0, 15}},
      {ItemId::PlasmaReactor,
       "Plasma Fusion Furnace",
       ItemCategory::Reactor,
       2500,
       0,
       60,
       {0, 0, 0, 0, 20}},
      {ItemId::QuantumReactor,
       "Quantum Synth-Core",
       ItemCategory::Reactor,
       7500,
       0,
       75,
       {0, 0, 0, 0, 26}},
      {ItemId::MainframeReactor,
       "AI Mainframe Reactor",
       ItemCategory::Reactor,
       23750,
       0,
       90,
       {0, 0, 0, 0, 34}},

      // Cyberware - Auto-Stim Injectors
      {ItemId::AutoStimMk1,
       "Auto-Stim — Mk I",
       ItemCategory::AutoStim,
       375,
       0,
       1,
       {0, 0, 0, 0, 25}},
      {ItemId::AutoStimMk2,
       "Auto-Stim — Mk II",
       ItemCategory::AutoStim,
       3000,
       0,
       1,
       {0, 0, 0, 0, 40}},
      {ItemId::AutoStimMk3,
       "Auto-Stim — Mk III",
       ItemCategory::AutoStim,
       12500,
       0,
       1,
       {0, 0, 0, 0, 55}},

      // Enemy Salvage
      {ItemId::ServoParts,
       "Servo Parts",
       ItemCategory::CyberLoot,
       8,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::HeavyChassis,
       "Heavy Mech Chassis",
       ItemCategory::CyberLoot,
       30,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::ApexCyberCore,
       "Apex Cyber-Core",
       ItemCategory::CyberLoot,
       180,
       0,
       1,
       {0, 0, 0, 0}},
      {ItemId::Microchip, "Microchip", ItemCategory::CyberLoot, 3, 0, 1, {0, 0, 0, 0}},
      {ItemId::SynthWeaveHide,
       "Synth-Weave Hide",
       ItemCategory::CyberLoot,
       16,
       0,
       1,
       {0, 0, 0, 0}},
  })};
  return item_info[static_cast<size_t>(id)];
}

const std::vector<SkillAction> skill_actions = {
    // Salvaging
    {SkillType::Salvaging, "Strip Copper Wiring", 1, 3000, 15, ItemId::CopperWireScrap,
     1, ItemId::None, 0, ItemId::None, 0},
    {SkillType::Salvaging, "Salvage Plasteel Hull", 10, 3500, 30,
     ItemId::PlasteelShards, 1, ItemId::None, 0, ItemId::None, 0},
    {SkillType::Salvaging, "Extract Nanotubes", 25, 4000, 55, ItemId::CarbonNanotubes,
     1, ItemId::None, 0, ItemId::None, 0},
    {SkillType::Salvaging, "Splice Optic Fibers", 35, 4500, 85,
     ItemId::OpticFiberBundle, 1, ItemId::None, 0, ItemId::None, 0},
    {SkillType::Salvaging, "Pull Positronic Relay", 45, 5000, 120,
     ItemId::PositronicRelays, 1, ItemId::None, 0, ItemId::None, 0},
    {SkillType::Salvaging, "Drain Cryo-Cell Rack", 55, 5500, 165, ItemId::CryoCellCore,
     1, ItemId::None, 0, ItemId::None, 0},
    {SkillType::Salvaging, "Tap Plasma Conduit", 60, 6000, 220, ItemId::PlasmaConduit,
     1, ItemId::None, 0, ItemId::None, 0},
    {SkillType::Salvaging, "Hack Quantum Node", 75, 7500, 340, ItemId::QuantumNode, 1,
     ItemId::None, 0, ItemId::None, 0},
    {SkillType::Salvaging, "Rip AI Mainframe Core", 90, 9000, 500,
     ItemId::AiMainframeCore, 1, ItemId::None, 0, ItemId::None, 0},

    // Bio-Harvest
    {SkillType::BioHarvest, "Culture Krill Biomass", 1, 3000, 12,
     ItemId::RawKrillBiomass, 1, ItemId::None, 0, ItemId::None, 0},
    {SkillType::BioHarvest, "Net Neon Eel", 5, 3400, 24, ItemId::RawNeonEel, 1,
     ItemId::None, 0, ItemId::None, 0},
    {SkillType::BioHarvest, "Harvest Synth-Carp", 20, 4000, 55, ItemId::RawSynthCarp, 1,
     ItemId::None, 0, ItemId::None, 0},
    {SkillType::BioHarvest, "Extract Chrome Salmon", 35, 4500, 90,
     ItemId::RawChromeSalmon, 1, ItemId::None, 0, ItemId::None, 0},
    {SkillType::BioHarvest, "Trap Cyber-Lobster", 45, 5200, 135,
     ItemId::RawCyberLobster, 1, ItemId::None, 0, ItemId::None, 0},
    {SkillType::BioHarvest, "Snare Plasma Ray", 55, 6000, 195, ItemId::RawPlasmaRay, 1,
     ItemId::None, 0, ItemId::None, 0},
    {SkillType::BioHarvest, "Harpoon Apex Shark", 70, 7200, 310, ItemId::RawApexShark,
     1, ItemId::None, 0, ItemId::None, 0},
    {SkillType::BioHarvest, "Clone Leviathan Cell", 85, 8500, 480,
     ItemId::RawLeviathanCell, 1, ItemId::None, 0, ItemId::None, 0},
    {SkillType::BioHarvest, "Dredge Cyber-Kraken", 95, 9500, 650,
     ItemId::RawCyberKraken, 1, ItemId::None, 0, ItemId::None, 0},

    // Farming - Hydroponic Crops & Noodle Milling
    {SkillType::Farming, "Cultivate Hydro-Wheat", 1, 2800, 14, ItemId::HydroWheat, 1,
     ItemId::None, 0, ItemId::None, 0},
    {SkillType::Farming, "Mill Synth-Noodles", 3, 2200, 20, ItemId::SynthNoodles, 2,
     ItemId::HydroWheat, 1, ItemId::None, 0},
    {SkillType::Farming, "Grow Synth-Soy Pods", 10, 3200, 28, ItemId::SoyPods, 1,
     ItemId::None, 0, ItemId::None, 0},
    {SkillType::Farming, "Harvest Neon Scallion", 22, 3600, 55, ItemId::NeonScallion, 1,
     ItemId::None, 0, ItemId::None, 0},
    {SkillType::Farming, "Culture Glow-Nori", 35, 4200, 92, ItemId::GlowNori, 1,
     ItemId::None, 0, ItemId::None, 0},
    {SkillType::Farming, "Harvest Cyber-Bamboo", 48, 4800, 145, ItemId::BioBamboo, 1,
     ItemId::None, 0, ItemId::None, 0},
    {SkillType::Farming, "Grow Spore-Shiitake", 62, 5500, 215, ItemId::CyberShiitake, 1,
     ItemId::None, 0, ItemId::None, 0},
    {SkillType::Farming, "Cultivate Plasma Chili", 75, 6500, 330, ItemId::PlasmaChili,
     1, ItemId::None, 0, ItemId::None, 0},
    {SkillType::Farming, "Harvest Chrono-Lotus", 86, 7600, 490, ItemId::ChronoLotus, 1,
     ItemId::None, 0, ItemId::None, 0},
    {SkillType::Farming, "Forage Quantum Truffle", 94, 8800, 660,
     ItemId::QuantumTruffle, 1, ItemId::None, 0, ItemId::None, 0},

    // Recycling - Transforming Scrap into Basic / Raw Materials
    {SkillType::Recycling, "Recycle Copper Scrap", 1, 2200, 22, ItemId::CopperFilament,
     1, ItemId::CopperWireScrap, 1, ItemId::None, 0},
    {SkillType::Recycling, "Recycle Plasteel Shards", 10, 2400, 42,
     ItemId::PlasteelPolymer, 1, ItemId::PlasteelShards, 1, ItemId::None, 0},
    {SkillType::Recycling, "Recycle Nanotubes", 25, 2600, 75, ItemId::CarbonFiberWeave,
     1, ItemId::CarbonNanotubes, 1, ItemId::None, 0},
    {SkillType::Recycling, "Recycle Optic Fibers", 35, 2800, 110,
     ItemId::OpticSilicaGlass, 1, ItemId::OpticFiberBundle, 1, ItemId::None, 0},
    {SkillType::Recycling, "Recycle Positronic Relay", 45, 3000, 155,
     ItemId::PositronicWafer, 1, ItemId::PositronicRelays, 1, ItemId::None, 0},
    {SkillType::Recycling, "Recycle Cryo-Cell Core", 55, 3200, 210,
     ItemId::CryoCoolantGel, 1, ItemId::CryoCellCore, 1, ItemId::None, 0},
    {SkillType::Recycling, "Recycle Plasma Conduit", 60, 3500, 280, ItemId::PlasmaCoil,
     1, ItemId::PlasmaConduit, 1, ItemId::None, 0},
    {SkillType::Recycling, "Recycle Quantum Node", 75, 3800, 410,
     ItemId::QuantumLattice, 1, ItemId::QuantumNode, 1, ItemId::None, 0},
    {SkillType::Recycling, "Recycle AI Mainframe", 90, 4200, 600, ItemId::NeuralMatrix,
     1, ItemId::AiMainframeCore, 1, ItemId::None, 0},

    // Synth-Cook - Noodles, Cyber-Ramen & Biota Stims
    {SkillType::SynthCook, "Prep Synth-Noodles", 1, 2200, 20, ItemId::SynthNoodles, 2,
     ItemId::HydroWheat, 1, ItemId::None, 0},
    {SkillType::SynthCook, "Synth Krill Ration (+30 HP)", 1, 2600, 18,
     ItemId::KrillRation, 1, ItemId::RawKrillBiomass, 1, ItemId::None, 0},
    {SkillType::SynthCook, "Synth Neon Eel (+50 HP)", 5, 2800, 34,
     ItemId::NeonEelSkewer, 1, ItemId::RawNeonEel, 1, ItemId::None, 0},
    {SkillType::SynthCook, "Cook Shoyu Ramen (+65 HP)", 10, 2800, 45,
     ItemId::ShoyuRamen, 1, ItemId::SynthNoodles, 1, ItemId::SoyPods, 1},
    {SkillType::SynthCook, "Synth Carp Pack (+80 HP)", 20, 3000, 70,
     ItemId::SynthCarpPack, 1, ItemId::RawSynthCarp, 1, ItemId::None, 0},
    {SkillType::SynthCook, "Cook Scallion Ramen (+125 HP)", 22, 3000, 85,
     ItemId::ScallionRamen, 1, ItemId::SynthNoodles, 1, ItemId::NeonScallion, 1},
    {SkillType::SynthCook, "Synth Salmon Stim (+110 HP)", 35, 3200, 115,
     ItemId::ChromeSalmonStim, 1, ItemId::RawChromeSalmon, 1, ItemId::None, 0},
    {SkillType::SynthCook, "Cook Nori Ramen (+195 HP)", 38, 3300, 150,
     ItemId::NoriRamen, 1, ItemId::SynthNoodles, 1, ItemId::GlowNori, 1},
    {SkillType::SynthCook, "Synth Lobster Meal (+160 HP)", 45, 3400, 175,
     ItemId::CyberLobsterMeal, 1, ItemId::RawCyberLobster, 1, ItemId::None, 0},
    {SkillType::SynthCook, "Cook Bamboo Ramen (+280 HP)", 50, 3500, 220,
     ItemId::BambooRamen, 1, ItemId::SynthNoodles, 1, ItemId::BioBamboo, 1},
    {SkillType::SynthCook, "Synth Plasma Ray (+220 HP)", 55, 3600, 240,
     ItemId::PlasmaRayInfusion, 1, ItemId::RawPlasmaRay, 1, ItemId::None, 0},
    {SkillType::SynthCook, "Cook Shiitake Ramen (+390 HP)", 64, 3800, 310,
     ItemId::ShiitakeRamen, 1, ItemId::SynthNoodles, 1, ItemId::CyberShiitake, 1},
    {SkillType::SynthCook, "Synth Shark Boost (+320 HP)", 70, 3800, 360,
     ItemId::ApexSharkBooster, 1, ItemId::RawApexShark, 1, ItemId::None, 0},
    {SkillType::SynthCook, "Cook Plasma Chili Ramen (+540 HP)", 76, 4100, 440,
     ItemId::PlasmaChiliRamen, 1, ItemId::SynthNoodles, 1, ItemId::PlasmaChili, 1},
    {SkillType::SynthCook, "Synth Leviathan Med (+480 HP)", 85, 4200, 540,
     ItemId::LeviathanNanomed, 1, ItemId::RawLeviathanCell, 1, ItemId::None, 0},
    {SkillType::SynthCook, "Cook Chrono-Lotus Ramen (+720 HP)", 88, 4400, 620,
     ItemId::ChronoLotusRamen, 1, ItemId::SynthNoodles, 1, ItemId::ChronoLotus, 1},
    {SkillType::SynthCook, "Cook Truffle Ramen (+860 HP)", 94, 4600, 780,
     ItemId::TruffleRamen, 1, ItemId::SynthNoodles, 1, ItemId::QuantumTruffle, 1},
    {SkillType::SynthCook, "Synth Kraken Elixir (+680 HP)", 95, 4600, 720,
     ItemId::KrakenBioElixir, 1, ItemId::RawCyberKraken, 1, ItemId::None, 0},
    {SkillType::SynthCook, "Cook Kraken Ramen (+980 HP)", 97, 4800, 880,
     ItemId::QuantumKrakenRamen, 1, ItemId::SynthNoodles, 1, ItemId::RawCyberKraken, 1},

    // Deep-Mining
    {SkillType::DeepMining, "Mine Copper Vein", 1, 2800, 14, ItemId::CopperOre, 1,
     ItemId::None, 0, ItemId::None, 0},
    {SkillType::DeepMining, "Mine Silicon Deposit", 1, 2800, 14, ItemId::SiliconOre, 1,
     ItemId::None, 0, ItemId::None, 0},
    {SkillType::DeepMining, "Mine Titanium Seam", 15, 3200, 35, ItemId::TitaniumOre, 1,
     ItemId::None, 0, ItemId::None, 0},
    {SkillType::DeepMining, "Mine Carbon Cell Bed", 30, 3500, 55, ItemId::CarbonCell, 1,
     ItemId::None, 0, ItemId::None, 0},
    {SkillType::DeepMining, "Mine Silver Vein", 35, 3800, 75, ItemId::SilverOre, 1,
     ItemId::None, 0, ItemId::None, 0},
    {SkillType::DeepMining, "Mine Gold Deposit", 40, 4200, 105, ItemId::GoldOre, 1,
     ItemId::None, 0, ItemId::None, 0},
    {SkillType::DeepMining, "Mine Cobalt Node", 50, 4800, 150, ItemId::CobaltOre, 1,
     ItemId::None, 0, ItemId::None, 0},
    {SkillType::DeepMining, "Mine Tungsten Core", 70, 5800, 230, ItemId::TungstenOre, 1,
     ItemId::None, 0, ItemId::None, 0},
    {SkillType::DeepMining, "Mine Neutronium Rift", 80, 7000, 350,
     ItemId::NeutroniumOre, 1, ItemId::None, 0, ItemId::None, 0},
    {SkillType::DeepMining, "Mine Chrono-Crystal", 88, 7800, 480, ItemId::ChronoOre, 1,
     ItemId::None, 0, ItemId::None, 0},
    {SkillType::DeepMining, "Mine Quantum Singularity", 96, 9500, 700,
     ItemId::QuantumOre, 1, ItemId::None, 0, ItemId::None, 0},

    // Smithing - Smelting Alloy Ingots & Forging Gear
    {SkillType::Smithing, "Smelt Scrap-Alloy Ingot", 1, 2200, 16, ItemId::ScrapAlloy, 1,
     ItemId::CopperOre, 1, ItemId::SiliconOre, 1},
    {SkillType::Smithing, "Forge Scrap Vibro-Knife", 1, 2500, 35, ItemId::ScrapBlade, 1,
     ItemId::ScrapAlloy, 2, ItemId::None, 0},
    {SkillType::Smithing, "Forge Scrap Exo-Harness", 5, 2800, 80, ItemId::ScrapExoSuit,
     1, ItemId::ScrapAlloy, 5, ItemId::None, 0},

    {SkillType::Smithing, "Smelt Titanium Ingot", 15, 2400, 32, ItemId::TitaniumAlloy,
     1, ItemId::TitaniumOre, 1, ItemId::None, 0},
    {SkillType::Smithing, "Forge Titanium Mono-Blade", 15, 2600, 65,
     ItemId::TitaniumBlade, 1, ItemId::TitaniumAlloy, 2, ItemId::None, 0},
    {SkillType::Smithing, "Forge Titanium Flak-Jacket", 20, 2900, 150,
     ItemId::TitaniumExoSuit, 1, ItemId::TitaniumAlloy, 5, ItemId::None, 0},

    {SkillType::Smithing, "Smelt Durasteel Ingot", 30, 2600, 55, ItemId::DurasteelAlloy,
     1, ItemId::TitaniumOre, 1, ItemId::CarbonCell, 2},
    {SkillType::Smithing, "Forge Durasteel Katana", 30, 2800, 110,
     ItemId::DurasteelBlade, 1, ItemId::DurasteelAlloy, 2, ItemId::None, 0},
    {SkillType::Smithing, "Forge Durasteel Exo-Rig", 36, 3100, 260,
     ItemId::DurasteelExoSuit, 1, ItemId::DurasteelAlloy, 5, ItemId::None, 0},

    {SkillType::Smithing, "Smelt Silver Conductor", 35, 2500, 68,
     ItemId::SilverConductor, 1, ItemId::SilverOre, 1, ItemId::None, 0},
    {SkillType::Smithing, "Smelt Gold Superconductor", 40, 2600, 95,
     ItemId::GoldSuperconductor, 1, ItemId::GoldOre, 1, ItemId::None, 0},

    {SkillType::Smithing, "Smelt Cobalt Ingot", 50, 2800, 115, ItemId::CobaltAlloy, 1,
     ItemId::CobaltOre, 1, ItemId::CarbonCell, 4},
    {SkillType::Smithing, "Forge Cobalt Laser-Edge", 50, 3000, 220, ItemId::CobaltBlade,
     1, ItemId::CobaltAlloy, 2, ItemId::None, 0},
    {SkillType::Smithing, "Forge Cobalt Subdermal Rig", 56, 3300, 520,
     ItemId::CobaltExoSuit, 1, ItemId::CobaltAlloy, 5, ItemId::None, 0},

    {SkillType::Smithing, "Smelt Tungsten Ingot", 70, 3000, 175, ItemId::TungstenAlloy,
     1, ItemId::TungstenOre, 1, ItemId::CarbonCell, 6},
    {SkillType::Smithing, "Forge Tungsten Mantis-Blade", 70, 3200, 340,
     ItemId::TungstenBlade, 1, ItemId::TungstenAlloy, 2, ItemId::None, 0},
    {SkillType::Smithing, "Forge Tungsten Power-Armor", 76, 3500, 820,
     ItemId::TungstenExoSuit, 1, ItemId::TungstenAlloy, 5, ItemId::None, 0},

    {SkillType::Smithing, "Smelt Neutronium Ingot", 80, 3200, 260,
     ItemId::NeutroniumAlloy, 1, ItemId::NeutroniumOre, 1, ItemId::CarbonCell, 8},
    {SkillType::Smithing, "Forge Neutronium Saber", 80, 3400, 520,
     ItemId::NeutroniumBlade, 1, ItemId::NeutroniumAlloy, 2, ItemId::None, 0},
    {SkillType::Smithing, "Forge Neutronium Nano-Suit", 85, 3700, 1250,
     ItemId::NeutroniumExoSuit, 1, ItemId::NeutroniumAlloy, 5, ItemId::None, 0},

    {SkillType::Smithing, "Smelt Chrono-Alloy Ingot", 88, 3500, 380,
     ItemId::ChronoAlloy, 1, ItemId::ChronoOre, 1, ItemId::NeutroniumOre, 2},
    {SkillType::Smithing, "Forge Chrono-Edge Katana", 89, 3700, 760,
     ItemId::ChronoBlade, 1, ItemId::ChronoAlloy, 2, ItemId::None, 0},
    {SkillType::Smithing, "Forge Chrono-Weave Exo-Suit", 93, 4000, 1800,
     ItemId::ChronoExoSuit, 1, ItemId::ChronoAlloy, 5, ItemId::None, 0},

    {SkillType::Smithing, "Smelt Quantum-Flux Ingot", 95, 3800, 550,
     ItemId::QuantumAlloy, 1, ItemId::QuantumOre, 1, ItemId::ChronoOre, 2},
    {SkillType::Smithing, "Forge Quantum Blade", 96, 4000, 1150, ItemId::QuantumBlade,
     1, ItemId::QuantumAlloy, 2, ItemId::None, 0},
    {SkillType::Smithing, "Forge Quantum Exo-Suit", 99, 4400, 2600,
     ItemId::QuantumExoSuit, 1, ItemId::QuantumAlloy, 5, ItemId::None, 0},

    // Cyber-Fab - Fabricating Visors, Holo-Shields & Tech Cores from Ingots +
    // Recycled Materials
    {SkillType::CyberFab, "Fab Scrap Optic Visor", 1, 2500, 40, ItemId::ScrapVisor, 1,
     ItemId::ScrapAlloy, 1, ItemId::CopperFilament, 1},
    {SkillType::CyberFab, "Fab Scrap Riot Buckler", 3, 2600, 55, ItemId::ScrapShield, 1,
     ItemId::ScrapAlloy, 2, ItemId::CopperFilament, 1},

    {SkillType::CyberFab, "Fab Titanium HUD Visor", 15, 2600, 75, ItemId::TitaniumVisor,
     1, ItemId::TitaniumAlloy, 1, ItemId::PlasteelPolymer, 1},
    {SkillType::CyberFab, "Fab Titanium Deflector", 18, 2700, 105,
     ItemId::TitaniumShield, 1, ItemId::TitaniumAlloy, 2, ItemId::PlasteelPolymer, 1},

    {SkillType::CyberFab, "Fab Durasteel Tac-Helm", 30, 2800, 125,
     ItemId::DurasteelVisor, 1, ItemId::DurasteelAlloy, 1, ItemId::CarbonFiberWeave, 1},
    {SkillType::CyberFab, "Fab Durasteel Barrier", 34, 2900, 175,
     ItemId::DurasteelShield, 1, ItemId::DurasteelAlloy, 2, ItemId::CarbonFiberWeave,
     1},
    {SkillType::CyberFab, "Fab Amber Datachip", 38, 2700, 140, ItemId::AmberDatachip, 1,
     ItemId::SilverConductor, 1, ItemId::OpticSilicaGlass, 1},

    {SkillType::CyberFab, "Fab Sapphire Cortex", 45, 2900, 190, ItemId::SapphireCortex,
     1, ItemId::GoldSuperconductor, 1, ItemId::PositronicWafer, 1},
    {SkillType::CyberFab, "Fab Cobalt Neural Visor", 50, 3000, 240, ItemId::CobaltVisor,
     1, ItemId::CobaltAlloy, 1, ItemId::PositronicWafer, 1},
    {SkillType::CyberFab, "Fab Cobalt Holo-Aegis", 54, 3100, 340, ItemId::CobaltShield,
     1, ItemId::CobaltAlloy, 2, ItemId::CryoCoolantGel, 1},

    {SkillType::CyberFab, "Fab Ruby Laser Core", 62, 3100, 290, ItemId::RubyLaserCore,
     1, ItemId::GoldSuperconductor, 1, ItemId::PlasmaCoil, 1},
    {SkillType::CyberFab, "Fab Tungsten Cyber-Helm", 70, 3200, 370,
     ItemId::TungstenVisor, 1, ItemId::TungstenAlloy, 1, ItemId::PlasmaCoil, 1},
    {SkillType::CyberFab, "Fab Tungsten Pulse-Shield", 74, 3300, 530,
     ItemId::TungstenShield, 1, ItemId::TungstenAlloy, 2, ItemId::PlasmaCoil, 1},

    {SkillType::CyberFab, "Fab Emerald Cryptokey", 78, 3300, 450,
     ItemId::EmeraldCryptokey, 1, ItemId::NeutroniumAlloy, 1, ItemId::QuantumLattice,
     1},
    {SkillType::CyberFab, "Fab Neutronium Mind-Crown", 81, 3400, 560,
     ItemId::NeutroniumVisor, 1, ItemId::NeutroniumAlloy, 1, ItemId::QuantumLattice, 1},
    {SkillType::CyberFab, "Fab Neutronium Forcefield", 83, 3500, 800,
     ItemId::NeutroniumShield, 1, ItemId::NeutroniumAlloy, 2, ItemId::QuantumLattice,
     1},

    {SkillType::CyberFab, "Fab Chrono-Sync Visor", 89, 3700, 820, ItemId::ChronoVisor,
     1, ItemId::ChronoAlloy, 1, ItemId::QuantumLattice, 1},
    {SkillType::CyberFab, "Fab Quantum Diamond", 90, 3600, 700, ItemId::QuantumDiamond,
     1, ItemId::ChronoAlloy, 1, ItemId::NeuralMatrix, 1},
    {SkillType::CyberFab, "Fab Chrono-Phase Barrier", 92, 3800, 1180,
     ItemId::ChronoShield, 1, ItemId::ChronoAlloy, 2, ItemId::QuantumLattice, 1},

    {SkillType::CyberFab, "Fab Quantum Tachyon Visor", 97, 4000, 1200,
     ItemId::QuantumVisor, 1, ItemId::QuantumAlloy, 1, ItemId::NeuralMatrix, 1},
    {SkillType::CyberFab, "Fab Quantum Shield", 98, 4100, 1750, ItemId::QuantumShield,
     1, ItemId::QuantumAlloy, 2, ItemId::NeuralMatrix, 1},
};

const std::array<MonsterInfo, MONSTER_COUNT> monster_info = {{
    {"Stray Servo-Drone",
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
     {{{ItemId::Microchip, 90, 2, 6},
       {ItemId::ServoParts, 100, 1, 1},
       {ItemId::KrillRation, 25, 1, 2},
       {ItemId::ScrapCutter, 12, 1, 1}}}},
    {"Bio-Vat Hound",
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
     {{{ItemId::SynthWeaveHide, 85, 1, 2},
       {ItemId::SynthProteinBar, 70, 1, 2},
       {ItemId::ServoParts, 100, 1, 1},
       {ItemId::TitaniumHarvester, 10, 1, 1}}}},
    {"Street Scavenger",
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
     {{{ItemId::PlasteelShards, 50, 2, 5},
       {ItemId::SynthCarpPack, 40, 1, 2},
       {ItemId::ServoParts, 100, 1, 1},
       {ItemId::TitaniumCutter, 10, 1, 1}}}},
    {"Chrome Gang Punk",
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
     {{{ItemId::ScrapBlade, 15, 1, 1},
       {ItemId::TitaniumOre, 45, 2, 4},
       {ItemId::ServoParts, 100, 1, 1},
       {ItemId::TitaniumDrill, 10, 1, 1}}}},
    {"Riot Enforcer Bot",
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
     {{{ItemId::HeavyChassis, 100, 1, 2},
       {ItemId::DurasteelBlade, 12, 1, 1},
       {ItemId::CarbonCell, 40, 3, 6},
       {ItemId::DurasteelCutter, 10, 1, 1}}}},
    {"Chem-Mutant Brute",
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
     {{{ItemId::HeavyChassis, 100, 1, 2},
       {ItemId::CobaltOre, 45, 2, 5},
       {ItemId::CyberLobsterMeal, 35, 2, 4},
       {ItemId::DurasteelHarvester, 10, 1, 1}}}},
    {"Cryo-Sec Mech",
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
     {{{ItemId::HeavyChassis, 100, 2, 3},
       {ItemId::CobaltExoSuit, 10, 1, 1},
       {ItemId::SapphireCortex, 25, 1, 2},
       {ItemId::CobaltDrill, 8, 1, 1}}}},
    {"Corp Shadow-Op",
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
     {{{ItemId::TungstenBlade, 12, 1, 1},
       {ItemId::TungstenOre, 45, 2, 5},
       {ItemId::PlasmaRayInfusion, 40, 2, 4},
       {ItemId::PositronicReactor, 8, 1, 1}}}},
    {"Cobalt Cyber-Ninja",
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
     {{{ItemId::TungstenExoSuit, 10, 1, 1},
       {ItemId::RubyLaserCore, 30, 1, 2},
       {ItemId::ApexSharkBooster, 35, 2, 4},
       {ItemId::TungstenCutter, 8, 1, 1}}}},
    {"Neutronium Cyborg",
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
     {{{ItemId::NeutroniumBlade, 10, 1, 1},
       {ItemId::NeutroniumExoSuit, 8, 1, 1},
       {ItemId::EmeraldCryptokey, 30, 1, 2},
       {ItemId::NeutroniumDrill, 7, 1, 1}}}},
    {"Apex Cyber-Wyrm",
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
     {{{ItemId::ApexCyberCore, 100, 1, 2},
       {ItemId::ChronoBlade, 15, 1, 1},
       {ItemId::QuantumOre, 30, 1, 3},
       {ItemId::ChronoHarvester, 6, 1, 1}}}},
    {"NEXUS-9, Rogue Overmind",
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
     {{{ItemId::QuantumBlade, 15, 1, 1},
       {ItemId::QuantumExoSuit, 12, 1, 1},
       {ItemId::KrakenBioElixir, 60, 4, 8},
       {ItemId::MainframeReactor, 5, 1, 1}}}},
}};

const std::array<ShopUpgradeInfo, TOOL_TIER_COUNT> cutter_upgrades = {{
    {0, "Scrap Cutter", "Starter Salvaging Cutter", 1, 0, 0, ItemId::ScrapCutter},
    {1, "Titanium Cutter", "-6% Salvaging Interval", 10, 200, 6,
     ItemId::TitaniumCutter},
    {2, "Durasteel Cutter", "-12% Salvaging Interval", 25, 750, 12,
     ItemId::DurasteelCutter},
    {3, "Cobalt Laser-Cutter", "-18% Salvaging Interval", 40, 2500, 18,
     ItemId::CobaltCutter},
    {4, "Tungsten Plasma-Torch", "-24% Salvaging Interval", 55, 8000, 24,
     ItemId::TungstenCutter},
    {5, "Neutronium Arc-Splicer", "-30% Salvaging Interval", 70, 25000, 30,
     ItemId::NeutroniumCutter},
    {6, "Chrono Deconstructor", "-38% Salvaging Interval", 85, 80000, 38,
     ItemId::ChronoCutter},
}};

const std::array<ShopUpgradeInfo, TOOL_TIER_COUNT> harvester_upgrades = {{
    {0, "Scrap Bio-Net", "Starter Bio-Harvester", 1, 0, 0, ItemId::ScrapHarvester},
    {1, "Titanium Bio-Rig", "-6% Bio-Harvest/Farming Interval", 10, 200, 6,
     ItemId::TitaniumHarvester},
    {2, "Durasteel Bio-Sampler", "-12% Bio-Harvest/Farming Interval", 25, 750, 12,
     ItemId::DurasteelHarvester},
    {3, "Cobalt Gene-Extractor", "-18% Bio-Harvest/Farming Interval", 40, 2500, 18,
     ItemId::CobaltHarvester},
    {4, "Tungsten Drone-Trawler", "-24% Bio-Harvest/Farming Interval", 55, 8000, 24,
     ItemId::TungstenHarvester},
    {5, "Neutronium Bio-Harvester", "-30% Bio-Harvest/Farming Interval", 70, 25000, 30,
     ItemId::NeutroniumHarvester},
    {6, "Chrono Stasis-Harvester", "-38% Bio-Harvest/Farming Interval", 85, 80000, 38,
     ItemId::ChronoHarvester},
}};

const std::array<ShopUpgradeInfo, TOOL_TIER_COUNT> drill_upgrades = {{
    {0, "Scrap Rotary Drill", "Starter Mining Drill", 1, 0, 0, ItemId::ScrapDrill},
    {1, "Titanium Impact Drill", "-6% Deep-Mining Interval", 10, 200, 6,
     ItemId::TitaniumDrill},
    {2, "Durasteel Sonic Drill", "-12% Deep-Mining Interval", 25, 750, 12,
     ItemId::DurasteelDrill},
    {3, "Cobalt Laser Bore", "-18% Deep-Mining Interval", 40, 2500, 18,
     ItemId::CobaltDrill},
    {4, "Tungsten Plasma Bore", "-24% Deep-Mining Interval", 55, 8000, 24,
     ItemId::TungstenDrill},
    {5, "Neutronium Quantum Drill", "-30% Deep-Mining Interval", 70, 25000, 30,
     ItemId::NeutroniumDrill},
    {6, "Chrono Singularity Bore", "-38% Deep-Mining Interval", 85, 80000, 38,
     ItemId::ChronoDrill},
}};

const std::array<ShopUpgradeInfo, TOOL_TIER_COUNT> reactor_upgrades = {{
    {0, "Basic Micro-Reactor", "Starter Synth-Reactor", 1, 0, 0, ItemId::BasicReactor},
    {1, "Plasteel Thermal Unit", "-5% Recycling/Synth-Cook Interval & +5% XP", 10, 250,
     5, ItemId::PlasteelReactor},
    {2, "Nanotube Induction Core", "-10% Recycling/Synth-Cook Interval & +10% XP", 25,
     900, 10, ItemId::NanotubeReactor},
    {3, "Positronic Reactor", "-15% Recycling/Synth-Cook Interval & +15% XP", 45, 3000,
     15, ItemId::PositronicReactor},
    {4, "Plasma Fusion Furnace", "-20% Recycling/Synth-Cook Interval & +20% XP", 60,
     10000, 20, ItemId::PlasmaReactor},
    {5, "Quantum Synth-Core", "-26% Recycling/Synth-Cook Interval & +26% XP", 75, 30000,
     26, ItemId::QuantumReactor},
    {6, "AI Mainframe Reactor", "-34% Recycling/Synth-Cook Interval & +34% XP", 90,
     95000, 34, ItemId::MainframeReactor},
}};

const std::array<ShopUpgradeInfo, AUTO_STIM_TIER_COUNT> auto_stim_upgrades = {{
    {0, "No Auto-Stim", "Manual stim-pack injection only", 1, 0, 0, ItemId::None},
    {1, "Auto-Stim — Mk I", "Auto-injects equipped stim below 25% HP", 1, 1500, 25,
     ItemId::AutoStimMk1},
    {2, "Auto-Stim — Mk II", "Auto-injects equipped stim below 40% HP", 1, 12000, 40,
     ItemId::AutoStimMk2},
    {3, "Auto-Stim — Mk III", "Auto-injects equipped stim below 55% HP", 1, 50000, 55,
     ItemId::AutoStimMk3},
}};

std::string skill_name(SkillType skill) {
  switch (skill) {
    case SkillType::Salvaging:
      return "Salvaging";
    case SkillType::BioHarvest:
      return "Bio-Harvest";
    case SkillType::Farming:
      return "Farming";
    case SkillType::Recycling:
      return "Recycling";
    case SkillType::SynthCook:
      return "Synth-Cook";
    case SkillType::DeepMining:
      return "Deep-Mining";
    case SkillType::Smithing:
      return "Smithing";
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
    case SkillType::Farming:
      return "FRM";
    case SkillType::Recycling:
      return "REC";
    case SkillType::SynthCook:
      return "SYN";
    case SkillType::DeepMining:
      return "MIN";
    case SkillType::Smithing:
      return "SMT";
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
    case ItemCategory::RawMaterial:
      return "Raw Material";
    case ItemCategory::RawBiota:
      return "Raw Biota";
    case ItemCategory::Crop:
      return "Crop/Ingr";
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
    case ItemCategory::Head:
      return "Head";
    case ItemCategory::Armor:
      return "Armor";
    case ItemCategory::Shield:
      return "Shield";
    case ItemCategory::Cutter:
      return "Cutter";
    case ItemCategory::Harvester:
      return "Harvester";
    case ItemCategory::Drill:
      return "Drill";
    case ItemCategory::Reactor:
      return "Reactor";
    case ItemCategory::AutoStim:
      return "Auto-Stim";
    case ItemCategory::CyberLoot:
      return "Salvage";
  }
  return "Item";
}

std::string equip_slot_name(EquipSlot slot) {
  switch (slot) {
    case EquipSlot::Weapon:
      return "Weapon";
    case EquipSlot::Head:
      return "Head";
    case EquipSlot::Armor:
      return "Armor";
    case EquipSlot::Shield:
      return "Shield";
    case EquipSlot::Cutter:
      return "Cutter";
    case EquipSlot::Harvester:
      return "Harvester";
    case EquipSlot::Drill:
      return "Drill";
    case EquipSlot::Reactor:
      return "Reactor";
    case EquipSlot::AutoStim:
      return "Auto-Stim";
    case EquipSlot::None:
      return "None";
  }
  return "None";
}

std::string item_equip_summary(ItemId id) {
  if (!is_valid_item(id)) return "Empty";
  const auto& info = get_item_info(id);
  switch (info.category) {
    case ItemCategory::Cutter:
    case ItemCategory::Harvester:
    case ItemCategory::Drill:
      return std::format("{} (-{}% Cycle)", info.name, info.bonus.speed_bonus_pct);
    case ItemCategory::Reactor:
      return std::format("{} (-{}% Cycle, +{}% XP)", info.name,
                         info.bonus.speed_bonus_pct, info.bonus.speed_bonus_pct);
    case ItemCategory::AutoStim:
      return std::format("{} (Auto-Stim <= {}% HP)", info.name,
                         info.bonus.speed_bonus_pct);
    default:
      return std::format("{} (+{}Atk, +{}Str, +{}Def, {}%DR)", info.name,
                         info.bonus.attack, info.bonus.strength, info.bonus.defence,
                         info.bonus.damage_reduction);
  }
}

std::string combat_style_name(CombatStyle style) {
  switch (style) {
    case CombatStyle::Accurate:
      return "Precision (Accuracy)";
    case CombatStyle::Aggressive:
      return "Overdrive (Strength)";
    case CombatStyle::Defensive:
      return "Evasive (Defence)";
  }
  return "Unknown";
}

std::string money_string(uint64_t value) { return number_string(value) + " Cr"; }

std::string number_string(uint64_t value) {
  std::string raw = std::to_string(value);
  std::string out;
  int count = 0;
  for (auto it = raw.rbegin(); it != raw.rend(); ++it) {
    if (count > 0 && count % 3 == 0) out.push_back(',');
    out.push_back(*it);
    ++count;
  }
  std::reverse(out.begin(), out.end());
  return out;
}

uint64_t xp_for_level(int level) {
  const auto& tbl = xp_table();
  int clamped = std::clamp(level, 1, (int)MAX_SKILL_LEVEL);
  return tbl[clamped];
}

int level_for_xp(uint64_t xp) {
  const auto& tbl = xp_table();
  for (int lvl = MAX_SKILL_LEVEL; lvl >= 1; --lvl) {
    if (xp >= tbl[lvl]) return lvl;
  }
  return 1;
}

double level_progress_ratio(uint64_t xp) {
  int lvl = level_for_xp(xp);
  if (lvl >= MAX_SKILL_LEVEL) return 1.0;
  uint64_t cur_base = xp_for_level(lvl);
  uint64_t next_base = xp_for_level(lvl + 1);
  if (next_base <= cur_base) return 1.0;
  return std::clamp(
      static_cast<double>(xp - cur_base) / static_cast<double>(next_base - cur_base),
      0.0, 1.0);
}

std::vector<int> actions_for_skill(SkillType skill) {
  std::vector<int> res;
  for (int i = 0; i < static_cast<int>(skill_actions.size()); ++i) {
    if (skill_actions[i].skill == skill) res.push_back(i);
  }
  return res;
}

std::string action_recipe(const SkillAction& act) {
  std::string io_str;
  if (is_valid_item(act.input_item_1)) {
    io_str +=
        std::format("{}x{}", act.input_qty_1, get_item_info(act.input_item_1).name);
  }
  if (is_valid_item(act.input_item_2)) {
    io_str +=
        std::format("+{}x{}", act.input_qty_2, get_item_info(act.input_item_2).name);
  }
  if (is_valid_item(act.product_item)) {
    if (!io_str.empty()) io_str += " -> ";
    io_str +=
        std::format("{}x{}", act.product_qty, get_item_info(act.product_item).name);
  } else if (io_str.empty()) {
    io_str = "XP+Cell+Cr";
  }
  return io_str;
}

// ============================================================================
// GameState Implementation
// ============================================================================

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

namespace {
template <size_t N>
int find_upgrade_tier(const std::array<ShopUpgradeInfo, N>& upgrades, ItemId id) {
  if (!is_valid_item(id)) return 0;
  for (size_t i = 0; i < N; ++i) {
    if (upgrades[i].item_id == id) return static_cast<int>(upgrades[i].tier);
  }
  return 0;
}
}  // namespace

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
    case SkillType::BioHarvest:
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
                        mon.bounty_req, mon.name));
    return false;
  }
  active_type = ActiveActivityType::Combat;
  combat.active_monster_id = monster_id;
  combat.monster_hp = mon.max_hp;
  combat.player_attack_timer_ms = 0;
  combat.monster_attack_timer_ms = 0;
  status_banner = std::format("Engaging {} (Lv {}) in {}", mon.name, mon.combat_level,
                              mon.zone_name);
  add_log(std::format("Engaged hostile {} ({} HP) in {}.", mon.name, mon.max_hp,
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
  // Reactor tier grants a global XP bonus
  uint64_t bonus = (amount * reactor_tier() * 2) / 100;
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
          static_cast<ItemId>(static_cast<int>(ItemId::AmberDatachip) + rand_int(0, 4));
      if (add_item(gem_id, 1, false)) {
        add_log(std::format("While deep-mining, you extracted a rare {}!",
                            get_item_info(gem_id).name));
      }
    }
  } else if (act.skill == SkillType::BioHarvest) {
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
        std::format("Neutralized {}! Siphoned {}.", mon.name, money_string(cr_drop)));
  } else {
    add_log(std::format("Neutralized {}! Siphoned {} and salvaged {}.", mon.name,
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
                  monster_info[combat.active_monster_id].name, money_string(lost_cr)));
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
                      monster_info[combat.bounty_target_id].name,
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
      if (std::max(skill_level(SkillType::BioHarvest),
                   skill_level(SkillType::Farming)) < info.req_level) {
        add_log(std::format("Requires Bio-Harvest or Farming Level {} to equip {}.",
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
    return true;
  }

  // Return old equipped stim to vault if any
  if (is_valid_item(equipment.food_item) && equipment.food_qty > 0) {
    if (!can_store_item(equipment.food_item)) {
      add_log("Cyber-Vault is full! Cannot swap equipped stims.");
      return false;
    }
    add_item(equipment.food_item, equipment.food_qty, false);
  }
  remove_item(item_id, have);
  equipment.food_item = item_id;
  equipment.food_qty = have;
  add_log(
      std::format("Loaded {}x {} (+{} HP each).", have, info.name, info.heal_amount));
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
  while (combat.player_hp > 0 && combat.player_hp <= threshold &&
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
  if (std::max(skill_level(SkillType::BioHarvest), skill_level(SkillType::Farming)) <
      upg.req_skill_level) {
    add_log(std::format("Requires Bio-Harvest or Farming Level {} to buy {}.",
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
  if (skill_level(SkillType::Recycling) < upg.req_skill_level) {
    add_log(std::format("Requires Recycling Level {} to buy {}.", upg.req_skill_level,
                        upg.name));
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
  if (!home || std::string_view(home).empty()) {
    return "routineverse.save";
  }
  std::filesystem::path dir = std::filesystem::path(home) / ".config" / "Mizux";
  std::error_code ec;
  std::filesystem::create_directories(dir, ec);
  return (dir / "routineverse.save").string();
}

bool GameState::save_to_file(const std::string& path) const {
  std::ofstream out(path);
  if (!out.is_open()) return false;

  out << "ROUTINEVERSE_SAVE_V2\n";
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
    out << static_cast<int>(s.item_id) << " " << s.qty << "\n";
  }
  for (size_t i = 0; i < EQUIP_SLOT_COUNT; ++i) {
    auto slot = static_cast<EquipSlot>(i);
    out << static_cast<int>(equipment.at(slot)) << " ";
  }
  out << static_cast<int>(equipment.food_item) << " " << equipment.food_qty << "\n";
  out << static_cast<int>(active_type) << " " << active_action_id << " "
      << combat.active_monster_id << " " << combat.player_hp << " " << combat.monster_hp
      << " " << static_cast<int>(combat.style) << "\n";
  out << combat.bounty_target_id << " " << combat.bounty_remaining << " "
      << combat.bounties_completed << "\n";
  out << stats.total_items_gathered << " " << stats.total_monsters_killed << " "
      << stats.total_credits_earned << " " << stats.player_deaths << "\n";
  return out.good();
}

bool GameState::load_from_file(const std::string& path) {
  std::ifstream in(path);
  if (!in.is_open()) return false;

  std::string header;
  if (!(in >> header) ||
      (header != "ROUTINEVERSE_SAVE_V1" && header != "ROUTINEVERSE_SAVE_V2")) {
    return false;
  }

  in >> credits >> bounty_tokens >> total_ticks_ms;
  for (SkillType sk : all_skills) in >> skills.xp[sk];

  size_t m_sz = 0;
  in >> m_sz;
  skills.action_mastery_xp.assign(skill_actions.size(), 0);
  for (size_t i = 0; i < m_sz; ++i) {
    uint64_t val = 0;
    in >> val;
    if (i < skills.action_mastery_xp.size()) skills.action_mastery_xp[i] = val;
  }

  size_t b_sz = 0;
  in >> bank.capacity >> b_sz;
  bank.clear();
  for (size_t i = 0; i < b_sz; ++i) {
    int raw_id = -1;
    int qty = 0;
    in >> raw_id >> qty;
    auto id = static_cast<ItemId>(raw_id);
    if (is_valid_item(id) && qty > 0) {
      bank.items.push_back(Bank::Slot{id, qty});
    }
  }

  if (header == "ROUTINEVERSE_SAVE_V2") {
    for (size_t i = 0; i < EQUIP_SLOT_COUNT; ++i) {
      auto slot = static_cast<EquipSlot>(i);
      int raw_id = -1;
      in >> raw_id;
      auto id = static_cast<ItemId>(raw_id);
      equipment[slot] = is_valid_item(id) ? id : ItemId::None;
    }

    int raw_food_id = -1;
    in >> raw_food_id >> equipment.food_qty;
    auto food_id = static_cast<ItemId>(raw_food_id);
    equipment.food_item = is_valid_item(food_id) ? food_id : ItemId::None;
  } else {
    for (const auto& slot :
         {EquipSlot::Weapon, EquipSlot::Head, EquipSlot::Armor, EquipSlot::Shield}) {
      int raw_id = -1;
      in >> raw_id;
      auto id = static_cast<ItemId>(raw_id);
      equipment[slot] = is_valid_item(id) ? id : ItemId::None;
    }

    int raw_food_id = -1;
    in >> raw_food_id >> equipment.food_qty;
    auto food_id = static_cast<ItemId>(raw_food_id);
    equipment.food_item = is_valid_item(food_id) ? food_id : ItemId::None;

    int c_t = 0, h_t = 0, d_t = 0, r_t = 0, a_t = 0;
    in >> c_t >> h_t >> d_t >> r_t >> a_t;
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
  in >> act_t >> active_action_id >> combat.active_monster_id >> combat.player_hp >>
      combat.monster_hp >> style_t;
  active_type = static_cast<ActiveActivityType>(std::clamp(act_t, 0, 2));
  combat.style = static_cast<CombatStyle>(std::clamp(style_t, 0, 2));

  in >> combat.bounty_target_id >> combat.bounty_remaining >> combat.bounties_completed;
  in >> stats.total_items_gathered >> stats.total_monsters_killed >>
      stats.total_credits_earned >> stats.player_deaths;

  if (active_type == ActiveActivityType::Skill && active_action_id >= 0 &&
      active_action_id < static_cast<int>(skill_actions.size())) {
    active_target_ms = action_effective_interval_ms(active_action_id);
    status_banner = std::format("{} ({})", skill_actions[active_action_id].name,
                                skill_name(skill_actions[active_action_id].skill));
  } else if (active_type == ActiveActivityType::Combat &&
             combat.active_monster_id >= 0 &&
             combat.active_monster_id < MONSTER_COUNT) {
    status_banner =
        std::format("Engaging {}", monster_info[combat.active_monster_id].name);
  } else {
    status_banner = "Standby — Select a Skill or Hostile Target";
  }

  record_history_snapshot();
  add_log("Loaded saved neural state.");
  return true;
}
