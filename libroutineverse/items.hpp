#pragma once

#include <array>
#include <cstdint>
#include <span>
#include <string>

// enum, name, category, price, heal_amount, req_level, atk, str, def, dr, spd
#define RV_ITEM_ID_LIST(X)                                                                           \
  X(None, "Empty", Scrap, 0, 0, 0, 0, 0, 0, 0, 0)                                                    \
  /* Scrap & Tech Nodes */                                                                           \
  X(CopperWireScrap, "Copper Wire Scrap", Scrap, 2, 0, 1, 0, 0, 0, 0, 0)                             \
  X(PlasteelShards, "Plasteel Shards", Scrap, 5, 0, 1, 0, 0, 0, 0, 0)                                \
  X(CarbonNanotubes, "Carbon Nanotubes", Scrap, 10, 0, 1, 0, 0, 0, 0, 0)                             \
  X(OpticFiberBundle, "Optic Fiber Bundle", Scrap, 18, 0, 1, 0, 0, 0, 0, 0)                          \
  X(PositronicRelays, "Positronic Relays", Scrap, 30, 0, 1, 0, 0, 0, 0, 0)                           \
  X(CryoCellCore, "Cryo-Cell Core", Scrap, 45, 0, 1, 0, 0, 0, 0, 0)                                  \
  X(PlasmaConduit, "Plasma Conduit", Scrap, 70, 0, 1, 0, 0, 0, 0, 0)                                 \
  X(QuantumNode, "Quantum Node", Scrap, 120, 0, 1, 0, 0, 0, 0, 0)                                    \
  X(AiMainframeCore, "AI Mainframe Core", Scrap, 200, 0, 1, 0, 0, 0, 0, 0)                           \
  /* Recycled Basic / Raw Materials */                                                               \
  X(CopperFilament, "Copper Filament", RawMaterial, 6, 0, 1, 0, 0, 0, 0, 0)                          \
  X(PlasteelPolymer, "Plasteel Polymer", RawMaterial, 14, 0, 1, 0, 0, 0, 0, 0)                       \
  X(CarbonFiberWeave, "Carbon Fiber Weave", RawMaterial, 28, 0, 1, 0, 0, 0, 0, 0)                    \
  X(OpticSilicaGlass, "Optic Silica Glass", RawMaterial, 48, 0, 1, 0, 0, 0, 0, 0)                    \
  X(PositronicWafer, "Positronic Wafer", RawMaterial, 80, 0, 1, 0, 0, 0, 0, 0)                       \
  X(CryoCoolantGel, "Cryo-Coolant Gel", RawMaterial, 120, 0, 1, 0, 0, 0, 0, 0)                       \
  X(PlasmaCoil, "Magnetic Plasma Coil", RawMaterial, 185, 0, 1, 0, 0, 0, 0, 0)                       \
  X(QuantumLattice, "Quantum Lattice", RawMaterial, 310, 0, 1, 0, 0, 0, 0, 0)                       \
  X(NeuralMatrix, "Neural Matrix", RawMaterial, 520, 0, 1, 0, 0, 0, 0, 0)                            \
  /* Raw Synth-Biota */                                                                              \
  X(RawKrillBiomass, "Raw Krill Biomass", RawBiota, 3, 0, 1, 0, 0, 0, 0, 0)                          \
  X(RawNeonEel, "Raw Neon Eel", RawBiota, 6, 0, 1, 0, 0, 0, 0, 0)                                    \
  X(RawSynthCarp, "Raw Synth-Carp", RawBiota, 14, 0, 1, 0, 0, 0, 0, 0)                               \
  X(RawChromeSalmon, "Raw Chrome Salmon", RawBiota, 24, 0, 1, 0, 0, 0, 0, 0)                         \
  X(RawCyberLobster, "Raw Cyber-Lobster", RawBiota, 45, 0, 1, 0, 0, 0, 0, 0)                         \
  X(RawPlasmaRay, "Raw Plasma Ray", RawBiota, 75, 0, 1, 0, 0, 0, 0, 0)                               \
  X(RawApexShark, "Raw Apex Shark", RawBiota, 140, 0, 1, 0, 0, 0, 0, 0)                              \
  X(RawLeviathanCell, "Raw Leviathan Cell", RawBiota, 260, 0, 1, 0, 0, 0, 0, 0)                      \
  X(RawCyberKraken, "Raw Cyber-Kraken", RawBiota, 450, 0, 1, 0, 0, 0, 0, 0)                          \
  /* Hydro-Farmed Crops & Synth-Noodles */                                                           \
  X(HydroWheat, "Hydro-Wheat", Crop, 4, 0, 1, 0, 0, 0, 0, 0)                                         \
  X(SoyPods, "Synth-Soy Pods", Crop, 10, 0, 1, 0, 0, 0, 0, 0)                                        \
  X(NeonScallion, "Neon Scallion", Crop, 22, 0, 1, 0, 0, 0, 0, 0)                                    \
  X(GlowNori, "Bioluminescent Nori", Crop, 42, 0, 1, 0, 0, 0, 0, 0)                                  \
  X(BioBamboo, "Cyber-Bamboo Shoot", Crop, 75, 0, 1, 0, 0, 0, 0, 0)                                  \
  X(CyberShiitake, "Spore-Tech Shiitake", Crop, 130, 0, 1, 0, 0, 0, 0, 0)                            \
  X(PlasmaChili, "Plasma Ghost-Chili", Crop, 225, 0, 1, 0, 0, 0, 0, 0)                               \
  X(ChronoLotus, "Chrono-Lotus Root", Crop, 380, 0, 1, 0, 0, 0, 0, 0)                                \
  X(QuantumTruffle, "Quantum Myco-Truffle", Crop, 650, 0, 1, 0, 0, 0, 0, 0)                          \
  X(SynthNoodles, "Synth-Noodles", Crop, 8, 0, 1, 0, 0, 0, 0, 0)                                     \
  /* Synthesized Stims, Cyber-Ramen & Toxic Slag */                                                  \
  X(KrillRation, "Krill Ration", StimFood, 8, 30, 1, 0, 0, 0, 0, 0)                                  \
  X(NeonEelSkewer, "Neon Eel Skewer", StimFood, 15, 50, 1, 0, 0, 0, 0, 0)                            \
  X(SynthCarpPack, "Synth-Carp Pack", StimFood, 32, 80, 1, 0, 0, 0, 0, 0)                            \
  X(ChromeSalmonStim, "Chrome Salmon Stim", StimFood, 55, 110, 1, 0, 0, 0, 0, 0)                     \
  X(CyberLobsterMeal, "Cyber-Lobster Meal", StimFood, 100, 160, 1, 0, 0, 0, 0, 0)                    \
  X(PlasmaRayInfusion, "Plasma Ray Infusion", StimFood, 165, 220, 1, 0, 0, 0, 0, 0)                  \
  X(ApexSharkBooster, "Apex Shark Booster", StimFood, 300, 320, 1, 0, 0, 0, 0, 0)                    \
  X(LeviathanNanomed, "Leviathan Nanomed", StimFood, 550, 480, 1, 0, 0, 0, 0, 0)                     \
  X(KrakenBioElixir, "Kraken Bio-Elixir", StimFood, 950, 680, 1, 0, 0, 0, 0, 0)                      \
  X(ShoyuRamen, "Soy-Shoyu Cyber-Ramen", StimFood, 35, 65, 1, 0, 0, 0, 0, 0)                         \
  X(ScallionRamen, "Neon Scallion Ramen", StimFood, 68, 125, 1, 0, 0, 0, 0, 0)                       \
  X(NoriRamen, "Glow-Nori Umami Ramen", StimFood, 135, 195, 1, 0, 0, 0, 0, 0)                        \
  X(BambooRamen, "Cyber-Bamboo Miso Ramen", StimFood, 230, 280, 1, 0, 0, 0, 0, 0)                    \
  X(ShiitakeRamen, "Spore-Shiitake Tonkotsu Ramen", StimFood, 390, 390, 1, 0, 0, 0, 0, 0)            \
  X(PlasmaChiliRamen, "Plasma Volcano Ramen", StimFood, 680, 540, 1, 0, 0, 0, 0, 0)                  \
  X(ChronoLotusRamen, "Chrono-Lotus Broth Ramen", StimFood, 1150, 720, 1, 0, 0, 0, 0, 0)             \
  X(TruffleRamen, "Quantum Truffle Ramen", StimFood, 1850, 860, 1, 0, 0, 0, 0, 0)                    \
  X(QuantumKrakenRamen, "Quantum Kraken Special Ramen", StimFood, 2200, 980, 1, 0, 0, 0, 0, 0)       \
  X(SynthProteinBar, "Synth-Protein Bar", StimFood, 6, 25, 1, 0, 0, 0, 0, 0)                         \
  X(ToxicSlag, "Toxic Bio-Slag", ToxicWaste, 1, 0, 1, 0, 0, 0, 0, 0)                                 \
  /* Deep-Mined Ores & Cells */                                                                      \
  X(CopperOre, "Copper Ore", RawOre, 4, 0, 1, 0, 0, 0, 0, 0)                                         \
  X(SiliconOre, "Silicon Ore", RawOre, 4, 0, 1, 0, 0, 0, 0, 0)                                       \
  X(TitaniumOre, "Titanium Ore", RawOre, 12, 0, 1, 0, 0, 0, 0, 0)                                    \
  X(CarbonCell, "Carbon Cell", RawOre, 18, 0, 1, 0, 0, 0, 0, 0)                                      \
  X(SilverOre, "Silver Ore", RawOre, 30, 0, 1, 0, 0, 0, 0, 0)                                        \
  X(GoldOre, "Gold Ore", RawOre, 50, 0, 1, 0, 0, 0, 0, 0)                                            \
  X(CobaltOre, "Cobalt Ore", RawOre, 70, 0, 1, 0, 0, 0, 0, 0)                                        \
  X(TungstenOre, "Tungsten Ore", RawOre, 120, 0, 1, 0, 0, 0, 0, 0)                                   \
  X(NeutroniumOre, "Neutronium Ore", RawOre, 220, 0, 1, 0, 0, 0, 0, 0)                               \
  X(ChronoOre, "Chrono-Crystal Ore", RawOre, 400, 0, 1, 0, 0, 0, 0, 0)                               \
  X(QuantumOre, "Quantum Singularity Ore", RawOre, 750, 0, 1, 0, 0, 0, 0, 0)                         \
  /* Refined Alloys & Conductors */                                                                  \
  X(ScrapAlloy, "Scrap-Alloy Ingot", Alloy, 15, 0, 1, 0, 0, 0, 0, 0)                                 \
  X(TitaniumAlloy, "Titanium Ingot", Alloy, 32, 0, 1, 0, 0, 0, 0, 0)                                 \
  X(DurasteelAlloy, "Durasteel Ingot", Alloy, 65, 0, 1, 0, 0, 0, 0, 0)                               \
  X(SilverConductor, "Silver Conductor", Alloy, 80, 0, 1, 0, 0, 0, 0, 0)                             \
  X(GoldSuperconductor, "Gold Superconductor", Alloy, 130, 0, 1, 0, 0, 0, 0, 0)                      \
  X(CobaltAlloy, "Cobalt-Chrome Ingot", Alloy, 175, 0, 1, 0, 0, 0, 0, 0)                             \
  X(TungstenAlloy, "Tungsten Ingot", Alloy, 310, 0, 1, 0, 0, 0, 0, 0)                                \
  X(NeutroniumAlloy, "Neutronium Ingot", Alloy, 580, 0, 1, 0, 0, 0, 0, 0)                            \
  X(ChronoAlloy, "Chrono-Alloy Ingot", Alloy, 1100, 0, 1, 0, 0, 0, 0, 0)                             \
  X(QuantumAlloy, "Quantum-Flux Ingot", Alloy, 2000, 0, 1, 0, 0, 0, 0, 0)                            \
  /* Data Crystals */                                                                                \
  X(AmberDatachip, "Amber Datachip", DataCrystal, 150, 0, 1, 0, 0, 0, 0, 0)                          \
  X(RubyLaserCore, "Ruby Laser Core", DataCrystal, 250, 0, 1, 0, 0, 0, 0, 0)                         \
  X(EmeraldCryptokey, "Emerald Cryptokey", DataCrystal, 450, 0, 1, 0, 0, 0, 0, 0)                    \
  X(SapphireCortex, "Sapphire Cortex", DataCrystal, 750, 0, 1, 0, 0, 0, 0, 0)                        \
  X(QuantumDiamond, "Quantum Diamond", DataCrystal, 1500, 0, 1, 0, 0, 0, 0, 0)                       \
  /* Weapons - Mono-Blades */                                                                        \
  X(ScrapBlade, "Scrap Vibro-Knife", Weapon, 45, 0, 1, 10, 12, 0, 0, 0)                              \
  X(TitaniumBlade, "Titanium Mono-Blade", Weapon, 100, 0, 5, 18, 20, 0, 0, 0)                        \
  X(DurasteelBlade, "Durasteel Katana", Weapon, 220, 0, 10, 28, 32, 0, 0, 0)                         \
  X(CobaltBlade, "Cobalt Laser-Edge", Weapon, 550, 0, 20, 42, 48, 0, 0, 0)                           \
  X(TungstenBlade, "Tungsten Mantis-Blade", Weapon, 1100, 0, 30, 60, 66, 0, 0, 0)                    \
  X(NeutroniumBlade, "Neutronium Phase-Saber", Weapon, 2400, 0, 40, 84, 92, 0, 0, 0)                 \
  X(ChronoBlade, "Chrono-Edge Katana", Weapon, 6000, 0, 60, 120, 130, 0, 0, 0)                       \
  X(QuantumBlade, "Quantum Singularity Blade", Weapon, 12000, 0, 75, 165, 180, 0, 0, 0)              \
  /* Visors */                                                                                       \
  X(ScrapVisor, "Scrap Optic Visor", Head, 40, 0, 1, 0, 0, 6, 1, 0)                                  \
  X(TitaniumVisor, "Titanium HUD Visor", Head, 90, 0, 5, 0, 0, 11, 2, 0)                             \
  X(DurasteelVisor, "Durasteel Tac-Helm", Head, 200, 0, 10, 0, 0, 18, 3, 0)                          \
  X(CobaltVisor, "Cobalt Neural Visor", Head, 500, 0, 20, 0, 0, 27, 4, 0)                            \
  X(TungstenVisor, "Tungsten Cyber-Helm", Head, 950, 0, 30, 0, 0, 38, 5, 0)                          \
  X(NeutroniumVisor, "Neutronium Mind-Crown", Head, 2100, 0, 40, 0, 0, 52, 7, 0)                     \
  X(ChronoVisor, "Chrono-Sync Visor", Head, 5200, 0, 60, 0, 0, 72, 10, 0)                            \
  X(QuantumVisor, "Quantum Tachyon Visor", Head, 10500, 0, 75, 0, 0, 98, 13, 0)                      \
  /* Exo-Suits */                                                                                    \
  X(ScrapExoSuit, "Scrap Exo-Harness", Armor, 85, 0, 1, 0, 0, 14, 2, 0)                              \
  X(TitaniumExoSuit, "Titanium Flak-Jacket", Armor, 180, 0, 5, 0, 0, 24, 3, 0)                       \
  X(DurasteelExoSuit, "Durasteel Exo-Rig", Armor, 400, 0, 10, 0, 0, 36, 5, 0)                        \
  X(CobaltExoSuit, "Cobalt Subdermal Rig", Armor, 950, 0, 20, 0, 0, 52, 7, 0)                        \
  X(TungstenExoSuit, "Tungsten Power-Armor", Armor, 1900, 0, 30, 0, 0, 74, 9, 0)                     \
  X(NeutroniumExoSuit, "Neutronium Nano-Suit", Armor, 4200, 0, 40, 0, 0, 102, 12, 0)                 \
  X(ChronoExoSuit, "Chrono-Weave Exo-Suit", Armor, 9800, 0, 60, 0, 0, 140, 16, 0)                    \
  X(QuantumExoSuit, "Quantum Phase Exo-Suit", Armor, 19500, 0, 75, 0, 0, 190, 20, 0)                 \
  /* Holo-Shields */                                                                                 \
  X(ScrapShield, "Scrap Riot Buckler", Shield, 55, 0, 1, 0, 0, 9, 1, 0)                              \
  X(TitaniumShield, "Titanium Deflector", Shield, 120, 0, 5, 0, 0, 16, 2, 0)                         \
  X(DurasteelShield, "Durasteel Barrier", Shield, 260, 0, 10, 0, 0, 25, 3, 0)                        \
  X(CobaltShield, "Cobalt Holo-Aegis", Shield, 620, 0, 20, 0, 0, 36, 5, 0)                           \
  X(TungstenShield, "Tungsten Pulse-Shield", Shield, 1250, 0, 30, 0, 0, 50, 6, 0)                    \
  X(NeutroniumShield, "Neutronium Forcefield", Shield, 2800, 0, 40, 0, 0, 68, 8, 0)                  \
  X(ChronoShield, "Chrono-Phase Barrier", Shield, 6800, 0, 60, 0, 0, 96, 12, 0)                      \
  X(QuantumShield, "Quantum Event-Horizon Shield", Shield, 13500, 0, 75, 0, 0, 132, 15, 0)           \
  /* Tools - Salvaging Cutters */                                                                    \
  X(ScrapCutter, "Scrap Cutter", Cutter, 10, 0, 1, 0, 0, 0, 0, 0)                                    \
  X(TitaniumCutter, "Titanium Cutter", Cutter, 50, 0, 10, 0, 0, 0, 0, 6)                             \
  X(DurasteelCutter, "Durasteel Cutter", Cutter, 180, 0, 25, 0, 0, 0, 0, 12)                         \
  X(CobaltCutter, "Cobalt Laser-Cutter", Cutter, 625, 0, 40, 0, 0, 0, 0, 18)                         \
  X(TungstenCutter, "Tungsten Plasma-Torch", Cutter, 2000, 0, 55, 0, 0, 0, 0, 24)                    \
  X(NeutroniumCutter, "Neutronium Arc-Splicer", Cutter, 6250, 0, 70, 0, 0, 0, 0, 30)                 \
  X(ChronoCutter, "Chrono Deconstructor", Cutter, 20000, 0, 85, 0, 0, 0, 0, 38)                      \
  /* Tools - Bio-Harvesters */                                                                       \
  X(ScrapHarvester, "Scrap Bio-Net", Harvester, 10, 0, 1, 0, 0, 0, 0, 0)                             \
  X(TitaniumHarvester, "Titanium Bio-Rig", Harvester, 50, 0, 10, 0, 0, 0, 0, 6)                      \
  X(DurasteelHarvester, "Durasteel Bio-Sampler", Harvester, 180, 0, 25, 0, 0, 0, 0, 12)              \
  X(CobaltHarvester, "Cobalt Gene-Extractor", Harvester, 625, 0, 40, 0, 0, 0, 0, 18)                 \
  X(TungstenHarvester, "Tungsten Drone-Trawler", Harvester, 2000, 0, 55, 0, 0, 0, 0, 24)             \
  X(NeutroniumHarvester, "Neutronium Bio-Harvester", Harvester, 6250, 0, 70, 0, 0, 0, 0, 30)         \
  X(ChronoHarvester, "Chrono Stasis-Harvester", Harvester, 20000, 0, 85, 0, 0, 0, 0, 38)             \
  /* Tools - Mining Drills */                                                                        \
  X(ScrapDrill, "Scrap Rotary Drill", Drill, 10, 0, 1, 0, 0, 0, 0, 0)                                \
  X(TitaniumDrill, "Titanium Impact Drill", Drill, 50, 0, 10, 0, 0, 0, 0, 6)                         \
  X(DurasteelDrill, "Durasteel Sonic Drill", Drill, 180, 0, 25, 0, 0, 0, 0, 12)                      \
  X(CobaltDrill, "Cobalt Laser Bore", Drill, 625, 0, 40, 0, 0, 0, 0, 18)                             \
  X(TungstenDrill, "Tungsten Plasma Bore", Drill, 2000, 0, 55, 0, 0, 0, 0, 24)                       \
  X(NeutroniumDrill, "Neutronium Quantum Drill", Drill, 6250, 0, 70, 0, 0, 0, 0, 30)                 \
  X(ChronoDrill, "Chrono Singularity Bore", Drill, 20000, 0, 85, 0, 0, 0, 0, 38)                     \
  /* Tools - Synth-Reactors */                                                                       \
  X(BasicReactor, "Basic Micro-Reactor", Reactor, 10, 0, 1, 0, 0, 0, 0, 0)                           \
  X(PlasteelReactor, "Plasteel Thermal Unit", Reactor, 60, 0, 10, 0, 0, 0, 0, 5)                     \
  X(NanotubeReactor, "Nanotube Induction Core", Reactor, 225, 0, 25, 0, 0, 0, 0, 10)                 \
  X(PositronicReactor, "Positronic Reactor", Reactor, 750, 0, 45, 0, 0, 0, 0, 15)                    \
  X(PlasmaReactor, "Plasma Fusion Furnace", Reactor, 2500, 0, 60, 0, 0, 0, 0, 20)                    \
  X(QuantumReactor, "Quantum Synth-Core", Reactor, 7500, 0, 75, 0, 0, 0, 0, 26)                      \
  X(MainframeReactor, "AI Mainframe Reactor", Reactor, 23750, 0, 90, 0, 0, 0, 0, 34)                 \
  /* Cyberware - Auto-Stim Injectors */                                                              \
  X(AutoStimMk1, "Auto-Stim — Mk I", AutoStim, 375, 0, 1, 0, 0, 0, 0, 25)                            \
  X(AutoStimMk2, "Auto-Stim — Mk II", AutoStim, 3000, 0, 1, 0, 0, 0, 0, 40)                          \
  X(AutoStimMk3, "Auto-Stim — Mk III", AutoStim, 12500, 0, 1, 0, 0, 0, 0, 55)                        \
  /* Enemy Salvage Loot */                                                                           \
  X(ServoParts, "Servo Parts", CyberLoot, 8, 0, 1, 0, 0, 0, 0, 0)                                    \
  X(HeavyChassis, "Heavy Mech Chassis", CyberLoot, 30, 0, 1, 0, 0, 0, 0, 0)                          \
  X(ApexCyberCore, "Apex Cyber-Core", CyberLoot, 180, 0, 1, 0, 0, 0, 0, 0)                           \
  X(Microchip, "Microchip", CyberLoot, 3, 0, 1, 0, 0, 0, 0, 0)                                       \
  X(SynthWeaveHide, "Synth-Weave Hide", CyberLoot, 16, 0, 1, 0, 0, 0, 0, 0)

enum class ItemId : uint16_t {
#define X(id, name, cat, price, heal, req_lvl, atk, str, def, dr, spd) id,
  RV_ITEM_ID_LIST(X)
#undef X
};

std::string item_name(ItemId id);
std::span<const ItemId> all_item_ids() noexcept;
bool is_valid_item(ItemId id) noexcept;

inline constexpr std::array<ItemId, 5> data_crystal_ids = {
    ItemId::AmberDatachip,    ItemId::SapphireCortex, ItemId::RubyLaserCore,
    ItemId::EmeraldCryptokey, ItemId::QuantumDiamond,
};

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
