#pragma once

#include <array>
#include <cstdint>
#include <span>
#include <string>

#include "items.hpp"

#define RV_ZONE_ID_LIST(X)                 \
  X(NeonSlums, "Neon Slums")               \
  X(BackAlleySector, "Back-Alley Sector")  \
  X(IndustrialSector, "Industrial Sector") \
  X(MegacorpPlaza, "Megacorp Plaza")       \
  X(OrbitalSpire, "Orbital Spire")         \
  X(MetaverseGrid, "Metaverse Grid")       \
  X(MainframeCore, "Mainframe Core [BOSS]")

enum class ZoneId : uint8_t {
#define X(id, name) id,
  RV_ZONE_ID_LIST(X)
#undef X
};
std::string zone_name(ZoneId zone);
std::span<const ZoneId> all_zone_ids() noexcept;

// enum, name, zone, lvl, hp, intg, atk_ms, hit, ice_hit, ice_pool, acc, eva, xp,
// cr_min, cr_max, bounty_req, is_boss, drop1, drop2, drop3, drop4
#define RV_MONSTER_ID_LIST(X)                                                          \
  /* StrayServoDrone: Neon Slums — unconnected (0 INT) */                              \
  X(StrayServoDrone, "Stray Servo-Drone", NeonSlums, 1, 30, 0, 2600, 6, 0, 0, 15, 10,  \
    20, 3, 10, 1, false, (Microchip, 90, 2, 6), (ScrapCpu, 45, 1, 2),                  \
    (ScrapRam, 45, 1, 2), (ScrapCutter, 12, 1, 1))                                     \
  /* BioVatHound: Neon Slums — organic, unconnected (0 INT) */                         \
  X(BioVatHound, "Bio-Vat Hound", NeonSlums, 4, 65, 0, 2800, 12, 0, 0, 25, 18, 42, 8,  \
    22, 1, false, (SynthWeaveHide, 85, 1, 2), (SynthProteinBar, 70, 1, 2),             \
    (ServoParts, 100, 1, 1), (TitaniumHarvester, 10, 1, 1))                            \
  /* SlumDataTerminal: Metaverse Grid — pure Terminal (0 HP, 60 INT) */                \
  X(SlumDataTerminal, "Slum Data-Terminal", MetaverseGrid, 5, 0, 60, 2400, 0, 9, 3,    \
    28, 20, 52, 12, 30, 1, false, (ScrapCpu, 85, 1, 3), (ScrapRam, 85, 1, 3),          \
    (SpikeIceMk1, 60, 3, 6), (ParityPatch, 40, 1, 2))                                  \
  /* StreetScavenger: Neon Slums — unconnected (0 INT) */                              \
  X(StreetScavenger, "Street Scavenger", NeonSlums, 9, 110, 0, 2600, 20, 0, 0, 40, 32, \
    72, 18, 45, 1, false, (PlasteelShards, 50, 2, 5), (SynthCarpPack, 40, 1, 2),       \
    (ServoParts, 100, 1, 1), (TitaniumCutter, 10, 1, 1))                               \
  /* ChromeGangPunk: Back-Alley Sector — connected cyber-punk (160 HP, 90 INT) */      \
  X(ChromeGangPunk, "Chrome Gang Punk", BackAlleySector, 14, 160, 90, 2500, 28, 10, 2, \
    55, 45, 105, 28, 70, 1, false, (ScrapBlade, 15, 1, 1), (TitaniumOre, 45, 2, 4),    \
    (SpikeIceMk1, 50, 2, 5), (TitaniumDrill, 10, 1, 1))                                \
  /* RiotEnforcerBot: Industrial Sector — networked bot (280 HP, 180 INT) */           \
  X(RiotEnforcerBot, "Riot Enforcer Bot", IndustrialSector, 24, 280, 180, 3000, 45,    \
    18, 3, 85, 72, 185, 55, 130, 10, false, (HeavyChassis, 100, 1, 2),                 \
    (DurasteelBlade, 12, 1, 1), (PositronicCpu, 35, 1, 2),                             \
    (DurasteelCutter, 10, 1, 1))                                                       \
  /* CorpSubroutineAi: Metaverse Grid — pure AI (0 HP, 260 INT) */                     \
  X(CorpSubroutineAi, "Corp Subroutine AI", MetaverseGrid, 28, 0, 260, 2500, 0, 32, 5, \
    100, 85, 230, 75, 175, 15, false, (PositronicCpu, 75, 1, 2),                       \
    (OpticRam, 75, 1, 2), (BreachIceMk2, 55, 4, 8), (KernelHotfix, 40, 1, 2))          \
  /* ChemMutantBrute: Industrial Sector — organic mutant, unconnected (450 HP) */      \
  X(ChemMutantBrute, "Chem-Mutant Brute", IndustrialSector, 36, 450, 0, 3000, 68, 0,   \
    0, 120, 105, 295, 95, 220, 20, false, (HeavyChassis, 100, 1, 2),                   \
    (CobaltOre, 45, 2, 5), (CyberLobsterMeal, 35, 2, 4),                               \
    (DurasteelHarvester, 10, 1, 1))                                                    \
  /* CryoSecMech: Industrial Sector — connected mech (650 HP, 420 INT) */              \
  X(CryoSecMech, "Cryo-Sec Mech", IndustrialSector, 48, 650, 420, 2900, 92, 42, 4,     \
    165, 145, 430, 150, 340, 30, false, (HeavyChassis, 100, 2, 3),                     \
    (CobaltExoSuit, 10, 1, 1), (SapphireCortex, 25, 1, 2), (CobaltDrill, 8, 1, 1))     \
  /* BlackwallDaemon: Metaverse Grid — pure AI Daemon (0 HP, 750 INT) */               \
  X(BlackwallDaemon, "Blackwall Sentinel AI", MetaverseGrid, 58, 0, 750, 2500, 0, 78,  \
    7, 210, 190, 560, 220, 500, 38, false, (QuantumCpu, 70, 1, 2),                     \
    (CryoRam, 70, 1, 2), (KrakenIceMk4, 50, 4, 8), (NeuralFirewall, 12, 1, 1))         \
  /* CorpShadowOp: Megacorp Plaza — connected operative (880 HP, 600 INT) */           \
  X(CorpShadowOp, "Corp Shadow-Op", MegacorpPlaza, 60, 880, 600, 2700, 125, 60, 5,     \
    215, 195, 580, 230, 520, 40, false, (TungstenBlade, 12, 1, 1),                     \
    (TungstenOre, 45, 2, 5), (PlasmaRayInfusion, 40, 2, 4),                            \
    (PositronicReactor, 8, 1, 1))                                                      \
  /* CobaltCyberNinja: Megacorp Plaza — connected cyber-ninja (1150 HP, 800 INT) */    \
  X(CobaltCyberNinja, "Cobalt Cyber-Ninja", MegacorpPlaza, 74, 1150, 800, 2600, 160,   \
    80, 6, 270, 250, 780, 350, 780, 50, false, (TungstenExoSuit, 10, 1, 1),            \
    (RubyLaserCore, 30, 1, 2), (ApexSharkBooster, 35, 2, 4),                           \
    (TungstenCutter, 8, 1, 1))                                                         \
  /* NeutroniumCyborg: Megacorp Plaza — connected cyborg (1500 HP, 1100 INT) */        \
  X(NeutroniumCyborg, "Neutronium Cyborg", MegacorpPlaza, 88, 1500, 1100, 2500, 205,   \
    110, 7, 340, 320, 1050, 550, 1200, 65, false, (NeutroniumBlade, 10, 1, 1),         \
    (NeutroniumExoSuit, 8, 1, 1), (EmeraldCryptokey, 30, 1, 2),                        \
    (NeutroniumDrill, 7, 1, 1))                                                        \
  /* ArchonNetAi: Metaverse Grid — pure Overmind AI (0 HP, 1900 INT) */                \
  X(ArchonNetAi, "ARCHON-0, Metaverse AI", MetaverseGrid, 95, 0, 1900, 2400, 0, 165,   \
    10, 390, 370, 1350, 750, 1700, 70, false, (NeuralCpu, 75, 1, 3),                   \
    (QuantumRam, 75, 1, 3), (SingularityIceMk6, 50, 5, 10),                            \
    (QuantumFirewall, 10, 1, 1))                                                       \
  /* ApexCyberWyrm: Orbital Spire — connected wyrm (2150 HP, 1600 INT) */              \
  X(ApexCyberWyrm, "Apex Cyber-Wyrm", OrbitalSpire, 110, 2150, 1600, 2600, 270, 150,   \
    8, 430, 410, 1500, 900, 2000, 75, false, (ApexCyberCore, 100, 1, 2),               \
    (ChronoBlade, 15, 1, 1), (QuantumOre, 30, 1, 3), (ChronoHarvester, 6, 1, 1))       \
  /* Nexus9: Mainframe Core [BOSS] — hybrid Overmind (3500 HP, 3000 INT) */            \
  X(Nexus9, "NEXUS-9, Rogue Overmind", MainframeCore, 150, 3500, 3000, 2400, 360, 220, \
    12, 550, 520, 3000, 2500, 5500, 85, true, (QuantumBlade, 15, 1, 1),                \
    (QuantumExoSuit, 12, 1, 1), (SingularityFirewall, 10, 1, 1),                       \
    (MainframeReactor, 5, 1, 1))

enum class MonsterId : uint8_t {
#define X(id, name, zone, lvl, hp, intg, atk_ms, hit, ice_hit, ice_pool, acc, eva, xp, \
          cr_min, cr_max, bounty, boss, d1, d2, d3, d4)                                \
  id,
  RV_MONSTER_ID_LIST(X)
#undef X
};
std::string monster_name(MonsterId monster);
std::span<const MonsterId> all_monster_ids() noexcept;

struct MonsterDrop {
  ItemId item_id = ItemId::None;
  int chance_pct = 0;  // 1..100
  int min_qty = 0;
  int max_qty = 0;
};

struct MonsterInfo {
  MonsterId id;
  const char* name;
  ZoneId zone;
  int combat_level;
  int max_hp;         // 0 for pure Metaverse Terminal / AI targets
  int max_integrity;  // 0 for unconnected targets (e.g. Neon Slums)
  int attack_interval_ms;
  int max_hit;        // Physical damage to player HP
  int ice_max_hit;    // Cyber/ICE damage to player Integrity
  int ice_pool;       // Number of ICE bots in opponent's pool
  int accuracy;
  int evasion;
  int xp_reward;
  uint64_t credits_min;
  uint64_t credits_max;
  int bounty_req;
  bool is_boss;
  std::array<MonsterDrop, 4> drops;
};
const MonsterInfo& get_monster_info(MonsterId id);
std::string monster_summary(MonsterId id);
