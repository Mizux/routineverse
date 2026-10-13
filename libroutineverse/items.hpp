#pragma once

#include <cstdint>
#include <span>
#include <string>

// enum, name, category, req_level, price[, bonus(atk, str, def, dr, spd[, recover])]
#define RV_ITEM_ID_LIST(X)                                                             \
  X(None, "Empty", Scrap, 0, 0)                                                        \
  /* Scrap & Tech Nodes */                                                             \
  X(CopperWireScrap, "Copper Wire Scrap", Scrap, 1, 2)                                 \
  X(PlasteelShards, "Plasteel Shards", Scrap, 1, 5)                                    \
  X(CarbonNanotubes, "Carbon Nanotubes", Scrap, 1, 10)                                 \
  X(OpticFiberBundle, "Optic Fiber Bundle", Scrap, 1, 18)                              \
  X(PositronicRelays, "Positronic Relays", Scrap, 1, 30)                               \
  X(CryoCellCore, "Cryo-Cell Core", Scrap, 1, 45)                                      \
  X(PlasmaConduit, "Plasma Conduit", Scrap, 1, 70)                                     \
  X(QuantumNode, "Quantum Node", Scrap, 1, 120)                                        \
  X(AiMainframeCore, "AI Mainframe Core", Scrap, 1, 200)                               \
  /* Recycled Basic / Raw Materials */                                                 \
  X(CopperFilament, "Copper Filament", RawMaterial, 1, 6)                              \
  X(PlasteelPolymer, "Plasteel Polymer", RawMaterial, 1, 14)                           \
  X(CarbonFiberWeave, "Carbon Fiber Weave", RawMaterial, 1, 28)                        \
  X(OpticSilicaGlass, "Optic Silica Glass", RawMaterial, 1, 48)                        \
  X(PositronicWafer, "Positronic Wafer", RawMaterial, 1, 80)                           \
  X(CryoCoolantGel, "Cryo-Coolant Gel", RawMaterial, 1, 120)                           \
  X(PlasmaCoil, "Magnetic Plasma Coil", RawMaterial, 1, 185)                           \
  X(QuantumLattice, "Quantum Lattice", RawMaterial, 1, 310)                            \
  X(NeuralMatrix, "Neural Matrix", RawMaterial, 1, 520)                                \
  /* Raw Synth-Biota */                                                                \
  X(RawKrillBiomass, "Raw Krill Biomass", RawBiota, 1, 3)                              \
  X(RawNeonEel, "Raw Neon Eel", RawBiota, 1, 6)                                        \
  X(RawSynthCarp, "Raw Synth-Carp", RawBiota, 1, 14)                                   \
  X(RawChromeSalmon, "Raw Chrome Salmon", RawBiota, 1, 24)                             \
  X(RawCyberLobster, "Raw Cyber-Lobster", RawBiota, 1, 45)                             \
  X(RawPlasmaRay, "Raw Plasma Ray", RawBiota, 1, 75)                                   \
  X(RawApexShark, "Raw Apex Shark", RawBiota, 1, 140)                                  \
  X(RawLeviathanCell, "Raw Leviathan Cell", RawBiota, 1, 260)                          \
  X(RawCyberKraken, "Raw Cyber-Kraken", RawBiota, 1, 450)                              \
  /* Hydro-Farmed Crops & Synth-Noodles */                                             \
  X(HydroWheat, "Hydro-Wheat", Crop, 1, 4)                                             \
  X(SoyPods, "Synth-Soy Pods", Crop, 1, 10)                                            \
  X(NeonScallion, "Neon Scallion", Crop, 1, 22)                                        \
  X(GlowNori, "Bioluminescent Nori", Crop, 1, 42)                                      \
  X(BioBamboo, "Cyber-Bamboo Shoot", Crop, 1, 75)                                      \
  X(CyberShiitake, "Spore-Tech Shiitake", Crop, 1, 130)                                \
  X(PlasmaChili, "Plasma Ghost-Chili", Crop, 1, 225)                                   \
  X(ChronoLotus, "Chrono-Lotus Root", Crop, 1, 380)                                    \
  X(QuantumTruffle, "Quantum Myco-Truffle", Crop, 1, 650)                              \
  X(SynthNoodles, "Synth-Noodles", Crop, 1, 8)                                         \
  /* Synthesized Stims, Cyber-Ramen & Toxic Slag */                                    \
  X(KrillRation, "Krill Ration", StimFood, 1, 8, (0, 0, 0, 0, 0, 30))                  \
  X(NeonEelSkewer, "Neon Eel Skewer", StimFood, 1, 15, (0, 0, 0, 0, 0, 50))            \
  X(SynthCarpPack, "Synth-Carp Pack", StimFood, 1, 32, (0, 0, 0, 0, 0, 80))            \
  X(ChromeSalmonStim, "Chrome Salmon Stim", StimFood, 1, 55, (0, 0, 0, 0, 0, 110))     \
  X(CyberLobsterMeal, "Cyber-Lobster Meal", StimFood, 1, 100, (0, 0, 0, 0, 0, 160))    \
  X(PlasmaRayInfusion, "Plasma Ray Infusion", StimFood, 1, 165, (0, 0, 0, 0, 0, 220))  \
  X(ApexSharkBooster, "Apex Shark Booster", StimFood, 1, 300, (0, 0, 0, 0, 0, 320))    \
  X(LeviathanNanomed, "Leviathan Nanomed", StimFood, 1, 550, (0, 0, 0, 0, 0, 480))     \
  X(KrakenBioElixir, "Kraken Bio-Elixir", StimFood, 1, 950, (0, 0, 0, 0, 0, 680))      \
  X(ShoyuRamen, "Soy-Shoyu Cyber-Ramen", StimFood, 1, 35, (0, 0, 0, 0, 0, 65))         \
  X(ScallionRamen, "Neon Scallion Ramen", StimFood, 1, 68, (0, 0, 0, 0, 0, 125))       \
  X(NoriRamen, "Glow-Nori Umami Ramen", StimFood, 1, 135, (0, 0, 0, 0, 0, 195))        \
  X(BambooRamen, "Cyber-Bamboo Miso Ramen", StimFood, 1, 230, (0, 0, 0, 0, 0, 280))    \
  X(ShiitakeRamen, "Spore-Shiitake Tonkotsu Ramen", StimFood, 1, 390,                  \
    (0, 0, 0, 0, 0, 390))                                                              \
  X(PlasmaChiliRamen, "Plasma Volcano Ramen", StimFood, 1, 680, (0, 0, 0, 0, 0, 540))  \
  X(ChronoLotusRamen, "Chrono-Lotus Broth Ramen", StimFood, 1, 1150,                   \
    (0, 0, 0, 0, 0, 720))                                                              \
  X(TruffleRamen, "Quantum Truffle Ramen", StimFood, 1, 1850, (0, 0, 0, 0, 0, 860))    \
  X(QuantumKrakenRamen, "Quantum Kraken Special Ramen", StimFood, 1, 2200,             \
    (0, 0, 0, 0, 0, 980))                                                              \
  X(SynthProteinBar, "Synth-Protein Bar", StimFood, 1, 6, (0, 0, 0, 0, 0, 25))         \
  X(ToxicSlag, "Toxic Bio-Slag", ToxicWaste, 1, 1)                                     \
  /* Deep-Mined Ores & Cells */                                                        \
  X(CopperOre, "Copper Ore", RawOre, 1, 4)                                             \
  X(SiliconOre, "Silicon Ore", RawOre, 1, 4)                                           \
  X(TitaniumOre, "Titanium Ore", RawOre, 1, 12)                                        \
  X(CarbonCell, "Carbon Cell", RawOre, 1, 18)                                          \
  X(SilverOre, "Silver Ore", RawOre, 1, 30)                                            \
  X(GoldOre, "Gold Ore", RawOre, 1, 50)                                                \
  X(CobaltOre, "Cobalt Ore", RawOre, 1, 70)                                            \
  X(TungstenOre, "Tungsten Ore", RawOre, 1, 120)                                       \
  X(NeutroniumOre, "Neutronium Ore", RawOre, 1, 220)                                   \
  X(ChronoOre, "Chrono-Crystal Ore", RawOre, 1, 400)                                   \
  X(QuantumOre, "Quantum Singularity Ore", RawOre, 1, 750)                             \
  /* Refined Alloys & Conductors */                                                    \
  X(ScrapAlloy, "Scrap-Alloy Ingot", Alloy, 1, 15)                                     \
  X(TitaniumAlloy, "Titanium Ingot", Alloy, 1, 32)                                     \
  X(DurasteelAlloy, "Durasteel Ingot", Alloy, 1, 65)                                   \
  X(SilverConductor, "Silver Conductor", Alloy, 1, 80)                                 \
  X(GoldSuperconductor, "Gold Superconductor", Alloy, 1, 130)                          \
  X(CobaltAlloy, "Cobalt-Chrome Ingot", Alloy, 1, 175)                                 \
  X(TungstenAlloy, "Tungsten Ingot", Alloy, 1, 310)                                    \
  X(NeutroniumAlloy, "Neutronium Ingot", Alloy, 1, 580)                                \
  X(ChronoAlloy, "Chrono-Alloy Ingot", Alloy, 1, 1100)                                 \
  X(QuantumAlloy, "Quantum-Flux Ingot", Alloy, 1, 2000)                                \
  /* Data Crystals */                                                                  \
  X(AmberDatachip, "Amber Datachip", DataCrystal, 1, 150)                              \
  X(RubyLaserCore, "Ruby Laser Core", DataCrystal, 1, 250)                             \
  X(EmeraldCryptokey, "Emerald Cryptokey", DataCrystal, 1, 450)                        \
  X(SapphireCortex, "Sapphire Cortex", DataCrystal, 1, 750)                            \
  X(QuantumDiamond, "Quantum Diamond", DataCrystal, 1, 1500)                           \
  /* Weapons - Mono-Blades */                                                          \
  X(ScrapBlade, "Scrap Vibro-Knife", Weapon, 1, 45, (10, 12, 0, 0, 0))                 \
  X(TitaniumBlade, "Titanium Mono-Blade", Weapon, 5, 100, (18, 20, 0, 0, 0))           \
  X(DurasteelBlade, "Durasteel Katana", Weapon, 10, 220, (28, 32, 0, 0, 0))            \
  X(CobaltBlade, "Cobalt Laser-Edge", Weapon, 20, 550, (42, 48, 0, 0, 0))              \
  X(TungstenBlade, "Tungsten Mantis-Blade", Weapon, 30, 1100, (60, 66, 0, 0, 0))       \
  X(NeutroniumBlade, "Neutronium Phase-Saber", Weapon, 40, 2400, (84, 92, 0, 0, 0))    \
  X(ChronoBlade, "Chrono-Edge Katana", Weapon, 60, 6000, (120, 130, 0, 0, 0))          \
  X(QuantumBlade, "Quantum Singularity Blade", Weapon, 75, 12000, (165, 180, 0, 0, 0)) \
  /* Visors */                                                                         \
  X(ScrapVisor, "Scrap Optic Visor", Head, 1, 40, (0, 0, 6, 1, 0))                     \
  X(TitaniumVisor, "Titanium HUD Visor", Head, 5, 90, (0, 0, 11, 2, 0))                \
  X(DurasteelVisor, "Durasteel Tac-Helm", Head, 10, 200, (0, 0, 18, 3, 0))             \
  X(CobaltVisor, "Cobalt Neural Visor", Head, 20, 500, (0, 0, 27, 4, 0))               \
  X(TungstenVisor, "Tungsten Cyber-Helm", Head, 30, 950, (0, 0, 38, 5, 0))             \
  X(NeutroniumVisor, "Neutronium Mind-Crown", Head, 40, 2100, (0, 0, 52, 7, 0))        \
  X(ChronoVisor, "Chrono-Sync Visor", Head, 60, 5200, (0, 0, 72, 10, 0))               \
  X(QuantumVisor, "Quantum Tachyon Visor", Head, 75, 10500, (0, 0, 98, 13, 0))         \
  /* Exo-Suits */                                                                      \
  X(ScrapExoSuit, "Scrap Exo-Harness", Armor, 1, 85, (0, 0, 14, 2, 0))                 \
  X(TitaniumExoSuit, "Titanium Flak-Jacket", Armor, 5, 180, (0, 0, 24, 3, 0))          \
  X(DurasteelExoSuit, "Durasteel Exo-Rig", Armor, 10, 400, (0, 0, 36, 5, 0))           \
  X(CobaltExoSuit, "Cobalt Subdermal Rig", Armor, 20, 950, (0, 0, 52, 7, 0))           \
  X(TungstenExoSuit, "Tungsten Power-Armor", Armor, 30, 1900, (0, 0, 74, 9, 0))        \
  X(NeutroniumExoSuit, "Neutronium Nano-Suit", Armor, 40, 4200, (0, 0, 102, 12, 0))    \
  X(ChronoExoSuit, "Chrono-Weave Exo-Suit", Armor, 60, 9800, (0, 0, 140, 16, 0))       \
  X(QuantumExoSuit, "Quantum Phase Exo-Suit", Armor, 75, 19500, (0, 0, 190, 20, 0))    \
  /* Holo-Shields */                                                                   \
  X(ScrapShield, "Scrap Riot Buckler", Shield, 1, 55, (0, 0, 9, 1, 0))                 \
  X(TitaniumShield, "Titanium Deflector", Shield, 5, 120, (0, 0, 16, 2, 0))            \
  X(DurasteelShield, "Durasteel Barrier", Shield, 10, 260, (0, 0, 25, 3, 0))           \
  X(CobaltShield, "Cobalt Holo-Aegis", Shield, 20, 620, (0, 0, 36, 5, 0))              \
  X(TungstenShield, "Tungsten Pulse-Shield", Shield, 30, 1250, (0, 0, 50, 6, 0))       \
  X(NeutroniumShield, "Neutronium Forcefield", Shield, 40, 2800, (0, 0, 68, 8, 0))     \
  X(ChronoShield, "Chrono-Phase Barrier", Shield, 60, 6800, (0, 0, 96, 12, 0))         \
  X(QuantumShield, "Quantum Event-Horizon Shield", Shield, 75, 13500,                  \
    (0, 0, 132, 15, 0))                                                                \
  /* Tools - Salvaging Cutters */                                                      \
  X(ScrapCutter, "Scrap Cutter", Cutter, 1, 10)                                        \
  X(TitaniumCutter, "Titanium Cutter", Cutter, 10, 50, (0, 0, 0, 0, 6))                \
  X(DurasteelCutter, "Durasteel Cutter", Cutter, 25, 180, (0, 0, 0, 0, 12))            \
  X(CobaltCutter, "Cobalt Laser-Cutter", Cutter, 40, 625, (0, 0, 0, 0, 18))            \
  X(TungstenCutter, "Tungsten Plasma-Torch", Cutter, 55, 2000, (0, 0, 0, 0, 24))       \
  X(NeutroniumCutter, "Neutronium Arc-Splicer", Cutter, 70, 6250, (0, 0, 0, 0, 30))    \
  X(ChronoCutter, "Chrono Deconstructor", Cutter, 85, 20000, (0, 0, 0, 0, 38))         \
  /* Tools - Bio-Harvesters */                                                         \
  X(ScrapHarvester, "Scrap Bio-Net", Harvester, 1, 10)                                 \
  X(TitaniumHarvester, "Titanium Bio-Rig", Harvester, 10, 50, (0, 0, 0, 0, 6))         \
  X(DurasteelHarvester, "Durasteel Bio-Sampler", Harvester, 25, 180, (0, 0, 0, 0, 12)) \
  X(CobaltHarvester, "Cobalt Gene-Extractor", Harvester, 40, 625, (0, 0, 0, 0, 18))    \
  X(TungstenHarvester, "Tungsten Drone-Trawler", Harvester, 55, 2000,                  \
    (0, 0, 0, 0, 24))                                                                  \
  X(NeutroniumHarvester, "Neutronium Bio-Harvester", Harvester, 70, 6250,              \
    (0, 0, 0, 0, 30))                                                                  \
  X(ChronoHarvester, "Chrono Stasis-Harvester", Harvester, 85, 20000,                  \
    (0, 0, 0, 0, 38))                                                                  \
  /* Tools - Mining Drills */                                                          \
  X(ScrapDrill, "Scrap Rotary Drill", Drill, 1, 10)                                    \
  X(TitaniumDrill, "Titanium Impact Drill", Drill, 10, 50, (0, 0, 0, 0, 6))            \
  X(DurasteelDrill, "Durasteel Sonic Drill", Drill, 25, 180, (0, 0, 0, 0, 12))         \
  X(CobaltDrill, "Cobalt Laser Bore", Drill, 40, 625, (0, 0, 0, 0, 18))                \
  X(TungstenDrill, "Tungsten Plasma Bore", Drill, 55, 2000, (0, 0, 0, 0, 24))          \
  X(NeutroniumDrill, "Neutronium Quantum Drill", Drill, 70, 6250, (0, 0, 0, 0, 30))    \
  X(ChronoDrill, "Chrono Singularity Bore", Drill, 85, 20000, (0, 0, 0, 0, 38))        \
  /* Tools - Synth-Reactors */                                                         \
  X(BasicReactor, "Basic Micro-Reactor", Reactor, 1, 10)                               \
  X(PlasteelReactor, "Plasteel Thermal Unit", Reactor, 10, 60, (0, 0, 0, 0, 5))        \
  X(NanotubeReactor, "Nanotube Induction Core", Reactor, 25, 225, (0, 0, 0, 0, 10))    \
  X(PositronicReactor, "Positronic Reactor", Reactor, 45, 750, (0, 0, 0, 0, 15))       \
  X(PlasmaReactor, "Plasma Fusion Furnace", Reactor, 60, 2500, (0, 0, 0, 0, 20))       \
  X(QuantumReactor, "Quantum Synth-Core", Reactor, 75, 7500, (0, 0, 0, 0, 26))         \
  X(MainframeReactor, "AI Mainframe Reactor", Reactor, 90, 23750, (0, 0, 0, 0, 34))    \
  /* Cyberware - Auto-Stim Injectors */                                                \
  X(AutoStimMk1, "Auto-Stim — Mk I", AutoStim, 1, 375, (0, 0, 0, 0, 25))               \
  X(AutoStimMk2, "Auto-Stim — Mk II", AutoStim, 1, 3000, (0, 0, 0, 0, 40))             \
  X(AutoStimMk3, "Auto-Stim — Mk III", AutoStim, 1, 12500, (0, 0, 0, 0, 55))           \
  /* Enemy Salvage Loot */                                                             \
  X(ServoParts, "Servo Parts", CyberLoot, 1, 8)                                        \
  X(HeavyChassis, "Heavy Mech Chassis", CyberLoot, 1, 30)                              \
  X(ApexCyberCore, "Apex Cyber-Core", CyberLoot, 1, 180)                               \
  X(Microchip, "Microchip", CyberLoot, 1, 3)                                           \
  X(SynthWeaveHide, "Synth-Weave Hide", CyberLoot, 1, 16)                              \
  /* Hacking Hardware - CPUs & RAMs */                                                 \
  X(ScrapCpu, "Scrap Logic CPU", Hardware, 1, 12)                                      \
  X(PositronicCpu, "Positronic Multi-Core CPU", Hardware, 25, 95)                      \
  X(QuantumCpu, "Quantum Co-Processor", Hardware, 55, 340)                             \
  X(NeuralCpu, "Neural Overmind CPU", Hardware, 80, 920)                               \
  X(ScrapRam, "Scrap DRAM Stick", Hardware, 1, 10)                                     \
  X(OpticRam, "Optic-NAND Storage Bank", Hardware, 25, 85)                             \
  X(CryoRam, "Cryo-Holographic RAM", Hardware, 55, 310)                                \
  X(QuantumRam, "Quantum Qubit Vault", Hardware, 80, 860)                              \
  /* Firewalls & Barriers (str = +Max Integrity, def = Cyber Evasion, dr = ICE DR%) */ \
  X(BasicFirewall, "Packet Filter Firewall", Firewall, 1, 65, (0, 25, 10, 6, 0))       \
  X(ProxyFirewall, "Proxy-Mesh Barrier", Firewall, 20, 260, (0, 60, 24, 12, 0))        \
  X(NeuralFirewall, "Neural Blackwall Barrier", Firewall, 45, 850,                     \
    (0, 120, 45, 18, 0))                                                               \
  X(CryoFirewall, "Cryo-Lattice Aegis", Firewall, 65, 2400, (0, 200, 70, 25, 0))       \
  X(QuantumFirewall, "Quantum Encryption Barrier", Firewall, 80, 6800,                 \
    (0, 320, 105, 32, 0))                                                              \
  X(SingularityFirewall, "Singularity AI Bastion", Firewall, 92, 16500,                \
    (0, 480, 150, 40, 0))                                                              \
  /* Attack ICE Bots (atk = Cyber Accuracy, str = ICE Max Hit bonus) */                \
  X(SpikeIceMk1, "Spike-ICE v1.0", AttackIce, 1, 8, (12, 14, 0, 0, 0))                 \
  X(BreachIceMk2, "Breach-ICE v2.0", AttackIce, 20, 25, (24, 28, 0, 0, 0))             \
  X(ViperIceMk3, "Viper-ICE v3.0", AttackIce, 40, 65, (42, 48, 0, 0, 0))               \
  X(KrakenIceMk4, "Kraken-ICE v4.0", AttackIce, 60, 160, (68, 76, 0, 0, 0))            \
  X(WraithIceMk5, "Wraith-ICE v5.0", AttackIce, 80, 380, (102, 115, 0, 0, 0))          \
  X(SingularityIceMk6, "Overmind-ICE v6.0", AttackIce, 92, 850, (150, 170, 0, 0, 0))   \
  /* Defense & Repair ICE Bots (recover = +INT repair, def = Cyber Evasion, dr = ICE   \
   * DR%) */                                                                           \
  X(WatchdogIceMk1, "Watchdog-ICE v1.0", DefenseIce, 1, 10, (0, 0, 8, 5, 0, 30))       \
  X(MedicIceMk2, "SysMedic-ICE v2.0", DefenseIce, 20, 30, (0, 0, 18, 9, 0, 65))        \
  X(AegisIceMk3, "Aegis-ICE v3.0", DefenseIce, 40, 75, (0, 0, 32, 14, 0, 130))         \
  X(SeraphIceMk4, "Seraph-ICE v4.0", DefenseIce, 60, 185, (0, 0, 50, 19, 0, 240))      \
  X(ArchonIceMk5, "Archon-ICE v5.0", DefenseIce, 80, 420, (0, 0, 75, 25, 0, 400))      \
  X(GuardianIceMk6, "Bastion-ICE v6.0", DefenseIce, 95, 950, (0, 0, 110, 32, 0, 650))  \
  /* Integrity Recovery & Upgrade Patches (recover = +INT restored, spd = +Integrity   \
   * XP) */                                                                            \
  X(ParityPatch, "Parity Checksum Patch", IntegrityPatch, 1, 14, (0, 0, 0, 0, 0, 40))  \
  X(KernelHotfix, "Kernel Hotfix Script", IntegrityPatch, 20, 45,                      \
    (0, 0, 0, 0, 0, 100))                                                              \
  X(SectorDefrag, "Sector Defrag Daemon", IntegrityPatch, 40, 120,                     \
    (0, 0, 0, 0, 0, 220))                                                              \
  X(NeuralRestore, "Neural State Restore", IntegrityPatch, 65, 290,                    \
    (0, 0, 0, 0, 0, 450))                                                              \
  X(QuantumRollback, "Quantum Snapshot Rollback", IntegrityPatch, 85, 720,             \
    (0, 0, 0, 0, 0, 850))                                                              \
  X(FirmwareBoostMk1, "Integrity Firmware Matrix", IntegrityPatch, 15, 150,            \
    (0, 0, 0, 0, 250, 100))

enum class ItemId : uint16_t {
#define X(id, name, cat, req_lvl, price, ...) id,
  RV_ITEM_ID_LIST(X)
#undef X
};

std::string item_name(ItemId id);
std::span<const ItemId> all_item_ids() noexcept;
bool is_valid_item(ItemId id) noexcept;

std::span<const ItemId> data_crystal_ids() noexcept;

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
  Hardware,
  Firewall,
  AttackIce,
  DefenseIce,
  IntegrityPatch,
};

std::string item_category_name(ItemCategory cat);
std::span<const ItemId> items_by_category(ItemCategory cat) noexcept;

struct Bonus {
  int attack = 0;            // Accuracy bonus (or ICE Cyber Accuracy)
  int strength = 0;          // Max hit bonus (or ICE Max Hit / Firewall +Max INT)
  int defence = 0;           // Evasion bonus (or Cyber Evasion)
  int damage_reduction = 0;  // Damage reduction % (or ICE Barrier DR%)
  int speed_bonus_pct = 0;   // Tool interval reduction %, Auto-Stim %, or Firmware XP
  int recover_amount =
      0;  // > 0 if usable stim/ration (+HP) or Defense ICE / Integrity Patch (+INT)
};

struct ItemInfo {
  ItemId id;
  const char* name;
  ItemCategory category;
  int req_level;  // Required skill level to equip
  int price;
  Bonus bonus{};
};

const ItemInfo& get_item_info(ItemId id);
std::string item_equip_summary(ItemId id);
