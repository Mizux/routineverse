#pragma once

#include <cstdint>
#include <span>
#include <string>

#include "items.hpp"

// Skills

// enum, name, short_name, is_combat
#define RV_SKILL_TYPE_LIST(X)                \
  X(Salvaging, "Salvaging", "SLV", false)    \
  X(Fishing, "Fishing", "FSH", false)        \
  X(Farming, "Farming", "FRM", false)        \
  X(Recycling, "Recycling", "REC", false)    \
  X(SynthCook, "Synth-Cook", "SYN", false)   \
  X(DeepMining, "Deep-Mining", "MIN", false) \
  X(Smithing, "Smithing", "SMT", false)      \
  X(CyberFab, "Cyber-Fab", "FAB", false)     \
  X(Hacking, "Hacking", "HCK", false)        \
  X(Attack, "Attack", "ATK", true)           \
  X(Strength, "Strength", "STR", true)       \
  X(Defence, "Defence", "DEF", true)         \
  X(Hitpoints, "Hitpoints", "HP", true)      \
  X(Integrity, "Integrity", "INT", true)     \
  X(Bounty, "Bounty", "BNT", true)

enum class SkillType : uint8_t {
#define X(id, name, short_name, is_combat) id,
  RV_SKILL_TYPE_LIST(X)
#undef X
};
std::string skill_name(SkillType skill);
std::string skill_short_name(SkillType skill);

std::span<const SkillType> all_skills() noexcept;

bool is_combat_skill(SkillType skill) noexcept;

int max_total_skill_level() noexcept;

// enum, skill, name, req_level, base_interval_ms, xp, (product_item, product_qty)[,
// (input_1, qty_1)[, (input_2, qty_2)]]
#define RV_SKILL_ACTION_ID_LIST(X)                                                     \
  /* Salvaging */                                                                      \
  X(StripCopperWiring, Salvaging, "Strip Copper Wiring", 1, 3000, 15,                  \
    (CopperWireScrap, 1))                                                              \
  X(ScavengeLogicCpu, Salvaging, "Scavenge Logic CPU", 3, 3200, 20, (ScrapCpu, 1))     \
  X(PullScrapDramStick, Salvaging, "Pull Scrap DRAM Stick", 6, 3300, 24,               \
    (ScrapRam, 1))                                                                     \
  X(SalvagePlasteelHull, Salvaging, "Salvage Plasteel Hull", 10, 3500, 30,             \
    (PlasteelShards, 1))                                                               \
  X(ExtractNanotubes, Salvaging, "Extract Nanotubes", 25, 4000, 55,                    \
    (CarbonNanotubes, 1))                                                              \
  X(SpliceOpticFibers, Salvaging, "Splice Optic Fibers", 35, 4500, 85,                 \
    (OpticFiberBundle, 1))                                                             \
  X(PullPositronicRelay, Salvaging, "Pull Positronic Relay", 45, 5000, 120,            \
    (PositronicRelays, 1))                                                             \
  X(DrainCryoCellRack, Salvaging, "Drain Cryo-Cell Rack", 55, 5500, 165,               \
    (CryoCellCore, 1))                                                                 \
  X(TapPlasmaConduit, Salvaging, "Tap Plasma Conduit", 60, 6000, 220,                  \
    (PlasmaConduit, 1))                                                                \
  X(HackQuantumNode, Salvaging, "Hack Quantum Node", 75, 7500, 340, (QuantumNode, 1))  \
  X(RipAiMainframeCore, Salvaging, "Rip AI Mainframe Core", 90, 9000, 500,             \
    (AiMainframeCore, 1))                                                              \
  /* Fishing */                                                                        \
  X(CultureKrillBiomass, Fishing, "Culture Krill Biomass", 1, 3000, 12,                \
    (RawKrillBiomass, 1))                                                              \
  X(NetNeonEel, Fishing, "Net Neon Eel", 5, 3400, 24, (RawNeonEel, 1))                 \
  X(HarvestSynthCarp, Fishing, "Harvest Synth-Carp", 20, 4000, 55, (RawSynthCarp, 1))  \
  X(ExtractChromeSalmon, Fishing, "Extract Chrome Salmon", 35, 4500, 90,               \
    (RawChromeSalmon, 1))                                                              \
  X(TrapCyberLobster, Fishing, "Trap Cyber-Lobster", 45, 5200, 135,                    \
    (RawCyberLobster, 1))                                                              \
  X(SnarePlasmaRay, Fishing, "Snare Plasma Ray", 55, 6000, 195, (RawPlasmaRay, 1))     \
  X(HarpoonApexShark, Fishing, "Harpoon Apex Shark", 70, 7200, 310, (RawApexShark, 1)) \
  X(CloneLeviathanCell, Fishing, "Clone Leviathan Cell", 85, 8500, 480,                \
    (RawLeviathanCell, 1))                                                             \
  X(DredgeCyberKraken, Fishing, "Dredge Cyber-Kraken", 95, 9500, 650,                  \
    (RawCyberKraken, 1))                                                               \
  /* Farming - Hydroponic Crops & Noodle Milling */                                    \
  X(CultivateHydroWheat, Farming, "Cultivate Hydro-Wheat", 1, 2800, 14,                \
    (HydroWheat, 1))                                                                   \
  X(MillSynthNoodles, Farming, "Mill Synth-Noodles", 3, 2200, 20, (SynthNoodles, 2),   \
    (HydroWheat, 1))                                                                   \
  X(GrowSynthSoyPods, Farming, "Grow Synth-Soy Pods", 10, 3200, 28, (SoyPods, 1))      \
  X(HarvestNeonScallion, Farming, "Harvest Neon Scallion", 22, 3600, 55,               \
    (NeonScallion, 1))                                                                 \
  X(CultureGlowNori, Farming, "Culture Glow-Nori", 35, 4200, 92, (GlowNori, 1))        \
  X(HarvestCyberBamboo, Farming, "Harvest Cyber-Bamboo", 48, 4800, 145,                \
    (BioBamboo, 1))                                                                    \
  X(GrowSporeShiitake, Farming, "Grow Spore-Shiitake", 62, 5500, 215,                  \
    (CyberShiitake, 1))                                                                \
  X(CultivatePlasmaChili, Farming, "Cultivate Plasma Chili", 75, 6500, 330,            \
    (PlasmaChili, 1))                                                                  \
  X(HarvestChronoLotus, Farming, "Harvest Chrono-Lotus", 86, 7600, 490,                \
    (ChronoLotus, 1))                                                                  \
  X(ForageQuantumTruffle, Farming, "Forage Quantum Truffle", 94, 8800, 660,            \
    (QuantumTruffle, 1))                                                               \
  /* Recycling - Transforming Scrap into Basic / Raw Materials */                      \
  X(RecycleCopperScrap, Recycling, "Recycle Copper Scrap", 1, 2200, 22,                \
    (CopperFilament, 1), (CopperWireScrap, 1))                                         \
  X(RecyclePlasteelShards, Recycling, "Recycle Plasteel Shards", 10, 2400, 42,         \
    (PlasteelPolymer, 1), (PlasteelShards, 1))                                         \
  X(RecycleNanotubes, Recycling, "Recycle Nanotubes", 25, 2600, 75,                    \
    (CarbonFiberWeave, 1), (CarbonNanotubes, 1))                                       \
  X(RecycleOpticFibers, Recycling, "Recycle Optic Fibers", 35, 2800, 110,              \
    (OpticSilicaGlass, 1), (OpticFiberBundle, 1))                                      \
  X(RecyclePositronicRelay, Recycling, "Recycle Positronic Relay", 45, 3000, 155,      \
    (PositronicWafer, 1), (PositronicRelays, 1))                                       \
  X(RecycleCryoCellCore, Recycling, "Recycle Cryo-Cell Core", 55, 3200, 210,           \
    (CryoCoolantGel, 1), (CryoCellCore, 1))                                            \
  X(RecyclePlasmaConduit, Recycling, "Recycle Plasma Conduit", 60, 3500, 280,          \
    (PlasmaCoil, 1), (PlasmaConduit, 1))                                               \
  X(RecycleQuantumNode, Recycling, "Recycle Quantum Node", 75, 3800, 410,              \
    (QuantumLattice, 1), (QuantumNode, 1))                                             \
  X(RecycleAiMainframe, Recycling, "Recycle AI Mainframe", 90, 4200, 600,              \
    (NeuralMatrix, 1), (AiMainframeCore, 1))                                           \
  /* Synth-Cook - Noodles, Cyber-Ramen & Biota Stims */                                \
  X(PrepSynthNoodles, SynthCook, "Prep Synth-Noodles", 1, 2200, 20, (SynthNoodles, 2), \
    (HydroWheat, 1))                                                                   \
  X(SynthKrillRation, SynthCook, "Synth Krill Ration (+30 HP)", 1, 2600, 18,           \
    (KrillRation, 1), (RawKrillBiomass, 1))                                            \
  X(SynthNeonEel, SynthCook, "Synth Neon Eel (+50 HP)", 5, 2800, 34,                   \
    (NeonEelSkewer, 1), (RawNeonEel, 1))                                               \
  X(CookShoyuRamen, SynthCook, "Cook Shoyu Ramen (+65 HP)", 10, 2800, 45,              \
    (ShoyuRamen, 1), (SynthNoodles, 1), (SoyPods, 1))                                  \
  X(SynthCarpPack, SynthCook, "Synth Carp Pack (+80 HP)", 20, 3000, 70,                \
    (SynthCarpPack, 1), (RawSynthCarp, 1))                                             \
  X(CookScallionRamen, SynthCook, "Cook Scallion Ramen (+125 HP)", 22, 3000, 85,       \
    (ScallionRamen, 1), (SynthNoodles, 1), (NeonScallion, 1))                          \
  X(SynthSalmonStim, SynthCook, "Synth Salmon Stim (+110 HP)", 35, 3200, 115,          \
    (ChromeSalmonStim, 1), (RawChromeSalmon, 1))                                       \
  X(CookNoriRamen, SynthCook, "Cook Nori Ramen (+195 HP)", 38, 3300, 150,              \
    (NoriRamen, 1), (SynthNoodles, 1), (GlowNori, 1))                                  \
  X(SynthLobsterMeal, SynthCook, "Synth Lobster Meal (+160 HP)", 45, 3400, 175,        \
    (CyberLobsterMeal, 1), (RawCyberLobster, 1))                                       \
  X(CookBambooRamen, SynthCook, "Cook Bamboo Ramen (+280 HP)", 50, 3500, 220,          \
    (BambooRamen, 1), (SynthNoodles, 1), (BioBamboo, 1))                               \
  X(SynthPlasmaRay, SynthCook, "Synth Plasma Ray (+220 HP)", 55, 3600, 240,            \
    (PlasmaRayInfusion, 1), (RawPlasmaRay, 1))                                         \
  X(CookShiitakeRamen, SynthCook, "Cook Shiitake Ramen (+390 HP)", 64, 3800, 310,      \
    (ShiitakeRamen, 1), (SynthNoodles, 1), (CyberShiitake, 1))                         \
  X(SynthSharkBoost, SynthCook, "Synth Shark Boost (+320 HP)", 70, 3800, 360,          \
    (ApexSharkBooster, 1), (RawApexShark, 1))                                          \
  X(CookPlasmaChiliRamen, SynthCook, "Cook Plasma Chili Ramen (+540 HP)", 76, 4100,    \
    440, (PlasmaChiliRamen, 1), (SynthNoodles, 1), (PlasmaChili, 1))                   \
  X(SynthLeviathanMed, SynthCook, "Synth Leviathan Med (+480 HP)", 85, 4200, 540,      \
    (LeviathanNanomed, 1), (RawLeviathanCell, 1))                                      \
  X(CookChronoLotusRamen, SynthCook, "Cook Chrono-Lotus Ramen (+720 HP)", 88, 4400,    \
    620, (ChronoLotusRamen, 1), (SynthNoodles, 1), (ChronoLotus, 1))                   \
  X(CookTruffleRamen, SynthCook, "Cook Truffle Ramen (+860 HP)", 94, 4600, 780,        \
    (TruffleRamen, 1), (SynthNoodles, 1), (QuantumTruffle, 1))                         \
  X(SynthKrakenElixir, SynthCook, "Synth Kraken Elixir (+680 HP)", 95, 4600, 720,      \
    (KrakenBioElixir, 1), (RawCyberKraken, 1))                                         \
  X(CookKrakenRamen, SynthCook, "Cook Kraken Ramen (+980 HP)", 97, 4800, 880,          \
    (QuantumKrakenRamen, 1), (SynthNoodles, 1), (RawCyberKraken, 1))                   \
  /* Deep-Mining */                                                                    \
  X(MineCopperVein, DeepMining, "Mine Copper Vein", 1, 2800, 14, (CopperOre, 1))       \
  X(MineSiliconDeposit, DeepMining, "Mine Silicon Deposit", 1, 2800, 14,               \
    (SiliconOre, 1))                                                                   \
  X(MineTitaniumSeam, DeepMining, "Mine Titanium Seam", 15, 3200, 35,                  \
    (TitaniumOre, 1))                                                                  \
  X(MineCarbonCellBed, DeepMining, "Mine Carbon Cell Bed", 30, 3500, 55,               \
    (CarbonCell, 1))                                                                   \
  X(MineSilverVein, DeepMining, "Mine Silver Vein", 35, 3800, 75, (SilverOre, 1))      \
  X(MineGoldDeposit, DeepMining, "Mine Gold Deposit", 40, 4200, 105, (GoldOre, 1))     \
  X(MineCobaltNode, DeepMining, "Mine Cobalt Node", 50, 4800, 150, (CobaltOre, 1))     \
  X(MineTungstenCore, DeepMining, "Mine Tungsten Core", 70, 5800, 230,                 \
    (TungstenOre, 1))                                                                  \
  X(MineNeutroniumRift, DeepMining, "Mine Neutronium Rift", 80, 7000, 350,             \
    (NeutroniumOre, 1))                                                                \
  X(MineChronoCrystal, DeepMining, "Mine Chrono-Crystal", 88, 7800, 480,               \
    (ChronoOre, 1))                                                                    \
  X(MineQuantumSingularity, DeepMining, "Mine Quantum Singularity", 96, 9500, 700,     \
    (QuantumOre, 1))                                                                   \
  /* Smithing - Smelting Alloy Ingots & Forging Gear */                                \
  X(SmeltScrapAlloyIngot, Smithing, "Smelt Scrap-Alloy Ingot", 1, 2200, 16,            \
    (ScrapAlloy, 1), (CopperOre, 1), (SiliconOre, 1))                                  \
  X(ForgeScrapVibroKnife, Smithing, "Forge Scrap Vibro-Knife", 1, 2500, 35,            \
    (ScrapBlade, 1), (ScrapAlloy, 2))                                                  \
  X(ForgeScrapExoHarness, Smithing, "Forge Scrap Exo-Harness", 5, 2800, 80,            \
    (ScrapExoSuit, 1), (ScrapAlloy, 5))                                                \
  X(SmeltTitaniumIngot, Smithing, "Smelt Titanium Ingot", 15, 2400, 32,                \
    (TitaniumAlloy, 1), (TitaniumOre, 1))                                              \
  X(ForgeTitaniumMonoBlade, Smithing, "Forge Titanium Mono-Blade", 15, 2600, 65,       \
    (TitaniumBlade, 1), (TitaniumAlloy, 2))                                            \
  X(ForgeTitaniumFlakJacket, Smithing, "Forge Titanium Flak-Jacket", 20, 2900, 150,    \
    (TitaniumExoSuit, 1), (TitaniumAlloy, 5))                                          \
  X(SmeltDurasteelIngot, Smithing, "Smelt Durasteel Ingot", 30, 2600, 55,              \
    (DurasteelAlloy, 1), (TitaniumOre, 1), (CarbonCell, 2))                            \
  X(ForgeDurasteelKatana, Smithing, "Forge Durasteel Katana", 30, 2800, 110,           \
    (DurasteelBlade, 1), (DurasteelAlloy, 2))                                          \
  X(ForgeDurasteelExoRig, Smithing, "Forge Durasteel Exo-Rig", 36, 3100, 260,          \
    (DurasteelExoSuit, 1), (DurasteelAlloy, 5))                                        \
  X(SmeltSilverConductor, Smithing, "Smelt Silver Conductor", 35, 2500, 68,            \
    (SilverConductor, 1), (SilverOre, 1))                                              \
  X(SmeltGoldSuperconductor, Smithing, "Smelt Gold Superconductor", 40, 2600, 95,      \
    (GoldSuperconductor, 1), (GoldOre, 1))                                             \
  X(SmeltCobaltIngot, Smithing, "Smelt Cobalt Ingot", 50, 2800, 115, (CobaltAlloy, 1), \
    (CobaltOre, 1), (CarbonCell, 4))                                                   \
  X(ForgeCobaltLaserEdge, Smithing, "Forge Cobalt Laser-Edge", 50, 3000, 220,          \
    (CobaltBlade, 1), (CobaltAlloy, 2))                                                \
  X(ForgeCobaltSubdermalRig, Smithing, "Forge Cobalt Subdermal Rig", 56, 3300, 520,    \
    (CobaltExoSuit, 1), (CobaltAlloy, 5))                                              \
  X(SmeltTungstenIngot, Smithing, "Smelt Tungsten Ingot", 70, 3000, 175,               \
    (TungstenAlloy, 1), (TungstenOre, 1), (CarbonCell, 6))                             \
  X(ForgeTungstenMantisBlade, Smithing, "Forge Tungsten Mantis-Blade", 70, 3200, 340,  \
    (TungstenBlade, 1), (TungstenAlloy, 2))                                            \
  X(ForgeTungstenPowerArmor, Smithing, "Forge Tungsten Power-Armor", 76, 3500, 820,    \
    (TungstenExoSuit, 1), (TungstenAlloy, 5))                                          \
  X(SmeltNeutroniumIngot, Smithing, "Smelt Neutronium Ingot", 80, 3200, 260,           \
    (NeutroniumAlloy, 1), (NeutroniumOre, 1), (CarbonCell, 8))                         \
  X(ForgeNeutroniumSaber, Smithing, "Forge Neutronium Saber", 80, 3400, 520,           \
    (NeutroniumBlade, 1), (NeutroniumAlloy, 2))                                        \
  X(ForgeNeutroniumNanoSuit, Smithing, "Forge Neutronium Nano-Suit", 85, 3700, 1250,   \
    (NeutroniumExoSuit, 1), (NeutroniumAlloy, 5))                                      \
  X(SmeltChronoAlloyIngot, Smithing, "Smelt Chrono-Alloy Ingot", 88, 3500, 380,        \
    (ChronoAlloy, 1), (ChronoOre, 1), (NeutroniumOre, 2))                              \
  X(ForgeChronoEdgeKatana, Smithing, "Forge Chrono-Edge Katana", 89, 3700, 760,        \
    (ChronoBlade, 1), (ChronoAlloy, 2))                                                \
  X(ForgeChronoWeaveExoSuit, Smithing, "Forge Chrono-Weave Exo-Suit", 93, 4000, 1800,  \
    (ChronoExoSuit, 1), (ChronoAlloy, 5))                                              \
  X(SmeltQuantumFluxIngot, Smithing, "Smelt Quantum-Flux Ingot", 95, 3800, 550,        \
    (QuantumAlloy, 1), (QuantumOre, 1), (ChronoOre, 2))                                \
  X(ForgeQuantumBlade, Smithing, "Forge Quantum Blade", 96, 4000, 1150,                \
    (QuantumBlade, 1), (QuantumAlloy, 2))                                              \
  X(ForgeQuantumExoSuit, Smithing, "Forge Quantum Exo-Suit", 99, 4400, 2600,           \
    (QuantumExoSuit, 1), (QuantumAlloy, 5))                                            \
  /* Cyber-Fab - Fabricating Visors, Shields, CPUs, RAMs & Tech Cores */               \
  X(FabScrapOpticVisor, CyberFab, "Fab Scrap Optic Visor", 1, 2500, 40,                \
    (ScrapVisor, 1), (ScrapAlloy, 1), (CopperFilament, 1))                             \
  X(FabScrapRiotBuckler, CyberFab, "Fab Scrap Riot Buckler", 3, 2600, 55,              \
    (ScrapShield, 1), (ScrapAlloy, 2), (CopperFilament, 1))                            \
  X(FabScrapLogicCpu, CyberFab, "Fab Scrap Logic CPU", 8, 2400, 45, (ScrapCpu, 1),     \
    (SiliconOre, 1), (CopperFilament, 1))                                              \
  X(FabScrapDramStick, CyberFab, "Fab Scrap DRAM Stick", 10, 2400, 48, (ScrapRam, 1),  \
    (SiliconOre, 1), (CopperWireScrap, 1))                                             \
  X(FabTitaniumHudVisor, CyberFab, "Fab Titanium HUD Visor", 15, 2600, 75,             \
    (TitaniumVisor, 1), (TitaniumAlloy, 1), (PlasteelPolymer, 1))                      \
  X(FabTitaniumDeflector, CyberFab, "Fab Titanium Deflector", 18, 2700, 105,           \
    (TitaniumShield, 1), (TitaniumAlloy, 2), (PlasteelPolymer, 1))                     \
  X(FabDurasteelTacHelm, CyberFab, "Fab Durasteel Tac-Helm", 30, 2800, 125,            \
    (DurasteelVisor, 1), (DurasteelAlloy, 1), (CarbonFiberWeave, 1))                   \
  X(FabPositronicCpu, CyberFab, "Fab Positronic CPU", 32, 2800, 145,                   \
    (PositronicCpu, 1), (PositronicWafer, 1), (SilverConductor, 1))                    \
  X(FabDurasteelBarrier, CyberFab, "Fab Durasteel Barrier", 34, 2900, 175,             \
    (DurasteelShield, 1), (DurasteelAlloy, 2), (CarbonFiberWeave, 1))                  \
  X(FabOpticNandRam, CyberFab, "Fab Optic-NAND RAM", 36, 2800, 155, (OpticRam, 1),     \
    (OpticSilicaGlass, 1), (CarbonFiberWeave, 1))                                      \
  X(FabAmberDatachip, CyberFab, "Fab Amber Datachip", 38, 2700, 140,                   \
    (AmberDatachip, 1), (SilverConductor, 1), (OpticSilicaGlass, 1))                   \
  X(FabSapphireCortex, CyberFab, "Fab Sapphire Cortex", 45, 2900, 190,                 \
    (SapphireCortex, 1), (GoldSuperconductor, 1), (PositronicWafer, 1))                \
  X(FabCobaltNeuralVisor, CyberFab, "Fab Cobalt Neural Visor", 50, 3000, 240,          \
    (CobaltVisor, 1), (CobaltAlloy, 1), (PositronicWafer, 1))                          \
  X(FabCobaltHoloAegis, CyberFab, "Fab Cobalt Holo-Aegis", 54, 3100, 340,              \
    (CobaltShield, 1), (CobaltAlloy, 2), (CryoCoolantGel, 1))                          \
  X(FabQuantumCoProcessor, CyberFab, "Fab Quantum Co-Processor", 58, 3000, 270,        \
    (QuantumCpu, 1), (QuantumLattice, 1), (GoldSuperconductor, 1))                     \
  X(FabCryoHolographicRam, CyberFab, "Fab Cryo-Holographic RAM", 60, 3000, 280,        \
    (CryoRam, 1), (CryoCoolantGel, 1), (PositronicWafer, 1))                           \
  X(FabRubyLaserCore, CyberFab, "Fab Ruby Laser Core", 62, 3100, 290,                  \
    (RubyLaserCore, 1), (GoldSuperconductor, 1), (PlasmaCoil, 1))                      \
  X(FabTungstenCyberHelm, CyberFab, "Fab Tungsten Cyber-Helm", 70, 3200, 370,          \
    (TungstenVisor, 1), (TungstenAlloy, 1), (PlasmaCoil, 1))                           \
  X(FabTungstenPulseShield, CyberFab, "Fab Tungsten Pulse-Shield", 74, 3300, 530,      \
    (TungstenShield, 1), (TungstenAlloy, 2), (PlasmaCoil, 1))                          \
  X(FabEmeraldCryptokey, CyberFab, "Fab Emerald Cryptokey", 78, 3300, 450,             \
    (EmeraldCryptokey, 1), (NeutroniumAlloy, 1), (QuantumLattice, 1))                  \
  X(FabNeutroniumMindCrown, CyberFab, "Fab Neutronium Mind-Crown", 81, 3400, 560,      \
    (NeutroniumVisor, 1), (NeutroniumAlloy, 1), (QuantumLattice, 1))                   \
  X(FabNeutroniumForcefield, CyberFab, "Fab Neutronium Forcefield", 83, 3500, 800,     \
    (NeutroniumShield, 1), (NeutroniumAlloy, 2), (QuantumLattice, 1))                  \
  X(FabNeuralOvermindCpu, CyberFab, "Fab Neural Overmind CPU", 85, 3600, 680,          \
    (NeuralCpu, 1), (NeuralMatrix, 1), (ChronoAlloy, 1))                               \
  X(FabQuantumQubitRam, CyberFab, "Fab Quantum Qubit RAM", 87, 3600, 710,              \
    (QuantumRam, 1), (QuantumLattice, 1), (SapphireCortex, 1))                         \
  X(FabChronoSyncVisor, CyberFab, "Fab Chrono-Sync Visor", 89, 3700, 820,              \
    (ChronoVisor, 1), (ChronoAlloy, 1), (QuantumLattice, 1))                           \
  X(FabQuantumDiamond, CyberFab, "Fab Quantum Diamond", 90, 3600, 700,                 \
    (QuantumDiamond, 1), (ChronoAlloy, 1), (NeuralMatrix, 1))                          \
  X(FabChronoPhaseBarrier, CyberFab, "Fab Chrono-Phase Barrier", 92, 3800, 1180,       \
    (ChronoShield, 1), (ChronoAlloy, 2), (QuantumLattice, 1))                          \
  X(FabQuantumTachyonVisor, CyberFab, "Fab Quantum Tachyon Visor", 97, 4000, 1200,     \
    (QuantumVisor, 1), (QuantumAlloy, 1), (NeuralMatrix, 1))                           \
  X(FabQuantumShield, CyberFab, "Fab Quantum Shield", 98, 4100, 1750,                  \
    (QuantumShield, 1), (QuantumAlloy, 2), (NeuralMatrix, 1))                          \
  /* Hacking - Coding Attack ICE, Defense/Repair ICE, Firewalls & Integrity Patches */ \
  X(CodeSpikeIceMk1, Hacking, "Code Spike-ICE v1.0", 1, 2400, 28, (SpikeIceMk1, 5),    \
    (ScrapCpu, 1), (ScrapRam, 1))                                                      \
  X(CodeParityPatch, Hacking, "Code Parity Patch (+40 INT)", 3, 2500, 34,              \
    (ParityPatch, 2), (ScrapRam, 1), (CopperFilament, 1))                              \
  X(CodeWatchdogIceMk1, Hacking, "Code Watchdog-ICE v1.0", 6, 2600, 42,                \
    (WatchdogIceMk1, 5), (ScrapCpu, 1), (ScrapRam, 1))                                 \
  X(BuildPacketFirewall, Hacking, "Build Packet Firewall", 10, 3000, 65,               \
    (BasicFirewall, 1), (ScrapCpu, 2), (ScrapRam, 2))                                  \
  X(CompileFirmwareMatrix, Hacking, "Compile Firmware Matrix", 15, 3200, 95,           \
    (FirmwareBoostMk1, 1), (ScrapCpu, 1), (PlasteelPolymer, 1))                        \
  X(CodeBreachIceMk2, Hacking, "Code Breach-ICE v2.0", 20, 2800, 115,                  \
    (BreachIceMk2, 5), (ScrapCpu, 1), (ScrapRam, 2))                                   \
  X(CodeKernelHotfix, Hacking, "Code Kernel Hotfix (+100 INT)", 24, 2900, 135,         \
    (KernelHotfix, 2), (OpticRam, 1), (PlasteelPolymer, 1))                            \
  X(CodeSysMedicIceMk2, Hacking, "Code SysMedic-ICE v2.0", 28, 3000, 160,              \
    (MedicIceMk2, 5), (PositronicCpu, 1), (OpticRam, 1))                               \
  X(BuildProxyMeshBarrier, Hacking, "Build Proxy-Mesh Barrier", 34, 3300, 210,         \
    (ProxyFirewall, 1), (PositronicCpu, 2), (OpticRam, 2))                             \
  X(CodeViperIceMk3, Hacking, "Code Viper-ICE v3.0", 40, 3100, 250, (ViperIceMk3, 5),  \
    (PositronicCpu, 1), (OpticRam, 2))                                                 \
  X(CodeSectorDefrag, Hacking, "Code Sector Defrag (+220 INT)", 45, 3200, 290,         \
    (SectorDefrag, 2), (OpticRam, 1), (PositronicWafer, 1))                            \
  X(CodeAegisIceMk3, Hacking, "Code Aegis-ICE v3.0", 50, 3300, 340, (AegisIceMk3, 5),  \
    (PositronicCpu, 2), (OpticRam, 2))                                                 \
  X(BuildBlackwallBarrier, Hacking, "Build Blackwall Barrier", 55, 3600, 420,          \
    (NeuralFirewall, 1), (PositronicCpu, 2), (SapphireCortex, 1))                      \
  X(CodeKrakenIceMk4, Hacking, "Code Kraken-ICE v4.0", 62, 3400, 480,                  \
    (KrakenIceMk4, 5), (QuantumCpu, 1), (CryoRam, 1))                                  \
  X(CodeNeuralRestore, Hacking, "Code Neural Restore (+450 INT)", 66, 3500, 540,       \
    (NeuralRestore, 2), (CryoRam, 1), (PlasmaCoil, 1))                                 \
  X(CodeSeraphIceMk4, Hacking, "Code Seraph-ICE v4.0", 70, 3600, 620,                  \
    (SeraphIceMk4, 5), (QuantumCpu, 1), (CryoRam, 2))                                  \
  X(BuildCryoLatticeAegis, Hacking, "Build Cryo-Lattice Aegis", 75, 3900, 740,         \
    (CryoFirewall, 1), (QuantumCpu, 2), (CryoRam, 2))                                  \
  X(CodeWraithIceMk5, Hacking, "Code Wraith-ICE v5.0", 82, 3700, 860,                  \
    (WraithIceMk5, 5), (QuantumCpu, 2), (CryoRam, 2))                                  \
  X(CodeArchonIceMk5, Hacking, "Code Archon-ICE v5.0", 86, 3800, 980,                  \
    (ArchonIceMk5, 5), (NeuralCpu, 1), (QuantumRam, 1))                                \
  X(BuildQuantumBarrier, Hacking, "Build Quantum Barrier", 90, 4100, 1180,             \
    (QuantumFirewall, 1), (NeuralCpu, 2), (QuantumRam, 2))                             \
  X(CodeQuantumRollback, Hacking, "Code Quantum Rollback (+850 INT)", 93, 4000, 1320,  \
    (QuantumRollback, 2), (QuantumRam, 1), (NeuralMatrix, 1))                          \
  X(CodeOvermindIceMk6, Hacking, "Code Overmind-ICE v6.0", 95, 4000, 1480,             \
    (SingularityIceMk6, 5), (NeuralCpu, 2), (QuantumRam, 2))                           \
  X(CodeBastionIceMk6, Hacking, "Code Bastion-ICE v6.0", 97, 4100, 1650,               \
    (GuardianIceMk6, 5), (NeuralCpu, 2), (QuantumRam, 2))                              \
  X(BuildSingularityBastion, Hacking, "Build Singularity Bastion", 99, 4500, 2100,     \
    (SingularityFirewall, 1), (NeuralCpu, 3), (QuantumRam, 3))

enum class SkillActionId : uint8_t {
#define X(id, skill, name, req_lvl, ms, xp, prod, ...) id,
  RV_SKILL_ACTION_ID_LIST(X)
#undef X
};
std::string skill_action_name(SkillActionId id);
std::span<const SkillActionId> all_skill_action_ids() noexcept;

struct SkillAction {
  SkillActionId id;
  SkillType skill;
  const char* name;
  int req_level;
  int base_interval_ms;
  int xp;
  ItemId product_item = ItemId::None;
  int product_qty = 0;
  ItemId input_item_1 = ItemId::None;
  int input_qty_1 = 0;
  ItemId input_item_2 = ItemId::None;
  int input_qty_2 = 0;
};

const SkillAction& get_skill_action(SkillActionId id);
std::string action_recipe(const SkillAction& act);
std::span<const SkillActionId> actions_for_skill(SkillType skill) noexcept;

// XP & Level utility
uint64_t xp_for_level(int level);
int level_for_xp(uint64_t xp);
double level_progress_ratio(uint64_t xp);

std::span<const uint64_t> xp_table() noexcept;
