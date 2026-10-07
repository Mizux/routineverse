#include "monsters.hpp"

#include <array>
#include <cstddef>
#include <format>

std::string zone_name(ZoneId zone) {
  switch (zone) {
#define X(id, name) \
  case ZoneId::id:  \
    return name;
    RV_ZONE_ID_LIST(X)
#undef X
  }
  return "Unknown";
}

std::span<const ZoneId> all_zone_ids() noexcept {
  static constexpr std::array ids{std::to_array<ZoneId>({
#define X(id, name) ZoneId::id,
      RV_ZONE_ID_LIST(X)
#undef X
  })};
  return ids;
}

std::string monster_name(MonsterId monster) {
  switch (monster) {
#define X(id, name)   \
  case MonsterId::id: \
    return name;
    RV_MONSTER_ID_LIST(X)
#undef X
  }
  return "Unknown";
}

std::span<const MonsterId> all_monster_ids() noexcept {
  static constexpr std::array ids{std::to_array<MonsterId>({
#define X(id, name) MonsterId::id,
      RV_MONSTER_ID_LIST(X)
#undef X
  })};
  return ids;
}

const MonsterInfo& get_monster_info(MonsterId id) {
  static const std::array monster_info{std::to_array<MonsterInfo>({
      {MonsterId::StrayServoDrone,
       ZoneId::NeonSlums,
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
      {MonsterId::BioVatHound,
       ZoneId::NeonSlums,
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
      {MonsterId::StreetScavenger,
       ZoneId::NeonSlums,
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
      {MonsterId::ChromeGangPunk,
       ZoneId::BackAlleySector,
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
      {MonsterId::RiotEnforcerBot,
       ZoneId::IndustrialSector,
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
      {MonsterId::ChemMutantBrute,
       ZoneId::IndustrialSector,
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
      {MonsterId::CryoSecMech,
       ZoneId::IndustrialSector,
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
      {MonsterId::CorpShadowOp,
       ZoneId::MegacorpPlaza,
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
      {MonsterId::CobaltCyberNinja,
       ZoneId::MegacorpPlaza,
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
      {MonsterId::NeutroniumCyborg,
       ZoneId::MegacorpPlaza,
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
      {MonsterId::ApexCyberWyrm,
       ZoneId::OrbitalSpire,
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
      {MonsterId::Nexus9,
       ZoneId::MainframeCore,
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
  })};
  size_t idx = static_cast<size_t>(id);
  if (idx < monster_info.size()) {
    return monster_info[idx];
  }
  return monster_info[0];
}

std::string monster_summary(MonsterId id) {
  const auto& mon = get_monster_info(id);
  return std::format("{} (Lv {}, {} HP, MaxHit {}, {})", monster_name(id),
                     mon.combat_level, mon.max_hp, mon.max_hit, zone_name(mon.zone));
}
