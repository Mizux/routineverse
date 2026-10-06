#include "skills.hpp"

#include <algorithm>
#include <cmath>
#include <format>

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

    // Fishing
    {SkillType::Fishing, "Culture Krill Biomass", 1, 3000, 12,
     ItemId::RawKrillBiomass, 1, ItemId::None, 0, ItemId::None, 0},
    {SkillType::Fishing, "Net Neon Eel", 5, 3400, 24, ItemId::RawNeonEel, 1,
     ItemId::None, 0, ItemId::None, 0},
    {SkillType::Fishing, "Harvest Synth-Carp", 20, 4000, 55, ItemId::RawSynthCarp, 1,
     ItemId::None, 0, ItemId::None, 0},
    {SkillType::Fishing, "Extract Chrome Salmon", 35, 4500, 90,
     ItemId::RawChromeSalmon, 1, ItemId::None, 0, ItemId::None, 0},
    {SkillType::Fishing, "Trap Cyber-Lobster", 45, 5200, 135,
     ItemId::RawCyberLobster, 1, ItemId::None, 0, ItemId::None, 0},
    {SkillType::Fishing, "Snare Plasma Ray", 55, 6000, 195, ItemId::RawPlasmaRay, 1,
     ItemId::None, 0, ItemId::None, 0},
    {SkillType::Fishing, "Harpoon Apex Shark", 70, 7200, 310, ItemId::RawApexShark,
     1, ItemId::None, 0, ItemId::None, 0},
    {SkillType::Fishing, "Clone Leviathan Cell", 85, 8500, 480,
     ItemId::RawLeviathanCell, 1, ItemId::None, 0, ItemId::None, 0},
    {SkillType::Fishing, "Dredge Cyber-Kraken", 95, 9500, 650,
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


std::string skill_name(SkillType skill) {
  switch (skill) {
    case SkillType::Salvaging:
      return "Salvaging";
    case SkillType::Fishing:
      return "Fishing";
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
    case SkillType::Fishing:
      return "FSH";
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
        std::format("{}x {}", act.input_qty_1, get_item_info(act.input_item_1).name);
  }
  if (is_valid_item(act.input_item_2)) {
    io_str +=
        std::format(" + {}x {}", act.input_qty_2, get_item_info(act.input_item_2).name);
  }
  if (is_valid_item(act.product_item)) {
    if (!io_str.empty()) io_str += " -> ";
    io_str +=
        std::format("{}x {}", act.product_qty, get_item_info(act.product_item).name);
  } else if (io_str.empty()) {
    io_str = "XP+Cell+Cr";
  }
  return io_str;
}

