#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>

enum class ItemId : uint16_t {
  None,
  // Scrap & Tech Nodes
  CopperWireScrap,
  PlasteelShards,
  CarbonNanotubes,
  OpticFiberBundle,
  PositronicRelays,
  CryoCellCore,
  PlasmaConduit,
  QuantumNode,
  AiMainframeCore,

  // Recycled Basic / Raw Materials
  CopperFilament,
  PlasteelPolymer,
  CarbonFiberWeave,
  OpticSilicaGlass,
  PositronicWafer,
  CryoCoolantGel,
  PlasmaCoil,
  QuantumLattice,
  NeuralMatrix,

  // Raw Synth-Biota
  RawKrillBiomass,
  RawNeonEel,
  RawSynthCarp,
  RawChromeSalmon,
  RawCyberLobster,
  RawPlasmaRay,
  RawApexShark,
  RawLeviathanCell,
  RawCyberKraken,

  // Hydro-Farmed Crops & Synth-Noodles
  HydroWheat,
  SoyPods,
  NeonScallion,
  GlowNori,
  BioBamboo,
  CyberShiitake,
  PlasmaChili,
  ChronoLotus,
  QuantumTruffle,
  SynthNoodles,

  // Synthesized Stims, Cyber-Ramen & Toxic Slag
  KrillRation,
  NeonEelSkewer,
  SynthCarpPack,
  ChromeSalmonStim,
  CyberLobsterMeal,
  PlasmaRayInfusion,
  ApexSharkBooster,
  LeviathanNanomed,
  KrakenBioElixir,
  ShoyuRamen,
  ScallionRamen,
  NoriRamen,
  BambooRamen,
  ShiitakeRamen,
  PlasmaChiliRamen,
  ChronoLotusRamen,
  TruffleRamen,
  QuantumKrakenRamen,
  SynthProteinBar,
  ToxicSlag,

  // Deep-Mined Ores & Cells
  CopperOre,
  SiliconOre,
  TitaniumOre,
  CarbonCell,
  SilverOre,
  GoldOre,
  CobaltOre,
  TungstenOre,
  NeutroniumOre,
  ChronoOre,
  QuantumOre,

  // Refined Alloys & Conductors
  ScrapAlloy,
  TitaniumAlloy,
  DurasteelAlloy,
  SilverConductor,
  GoldSuperconductor,
  CobaltAlloy,
  TungstenAlloy,
  NeutroniumAlloy,
  ChronoAlloy,
  QuantumAlloy,

  // Data Crystals
  AmberDatachip,
  SapphireCortex,
  RubyLaserCore,
  EmeraldCryptokey,
  QuantumDiamond,

  // Weapons - Mono-Blades
  ScrapBlade,
  TitaniumBlade,
  DurasteelBlade,
  CobaltBlade,
  TungstenBlade,
  NeutroniumBlade,
  ChronoBlade,
  QuantumBlade,

  // Visors
  ScrapVisor,
  TitaniumVisor,
  DurasteelVisor,
  CobaltVisor,
  TungstenVisor,
  NeutroniumVisor,
  ChronoVisor,
  QuantumVisor,

  // Exo-Suits
  ScrapExoSuit,
  TitaniumExoSuit,
  DurasteelExoSuit,
  CobaltExoSuit,
  TungstenExoSuit,
  NeutroniumExoSuit,
  ChronoExoSuit,
  QuantumExoSuit,

  // Holo-Shields
  ScrapShield,
  TitaniumShield,
  DurasteelShield,
  CobaltShield,
  TungstenShield,
  NeutroniumShield,
  ChronoShield,
  QuantumShield,

  // Tools - Salvaging Cutters
  ScrapCutter,
  TitaniumCutter,
  DurasteelCutter,
  CobaltCutter,
  TungstenCutter,
  NeutroniumCutter,
  ChronoCutter,

  // Tools - Bio-Harvesters
  ScrapHarvester,
  TitaniumHarvester,
  DurasteelHarvester,
  CobaltHarvester,
  TungstenHarvester,
  NeutroniumHarvester,
  ChronoHarvester,

  // Tools - Mining Drills
  ScrapDrill,
  TitaniumDrill,
  DurasteelDrill,
  CobaltDrill,
  TungstenDrill,
  NeutroniumDrill,
  ChronoDrill,

  // Tools - Synth-Reactors
  BasicReactor,
  PlasteelReactor,
  NanotubeReactor,
  PositronicReactor,
  PlasmaReactor,
  QuantumReactor,
  MainframeReactor,

  // Cyberware - Auto-Stim Injectors
  AutoStimMk1,
  AutoStimMk2,
  AutoStimMk3,

  // Enemy Salvage Loot
  ServoParts,
  HeavyChassis,
  ApexCyberCore,
  Microchip,
  SynthWeaveHide,
};

inline constexpr size_t ITEM_COUNT = 152;

inline constexpr bool is_valid_item(ItemId id) noexcept {
  return id != ItemId::None && static_cast<size_t>(id) < ITEM_COUNT;
}

inline constexpr std::optional<ItemId> item_id_from_int(int val) noexcept {
  if (val > 0 && static_cast<size_t>(val) < ITEM_COUNT) {
    return static_cast<ItemId>(val);
  }
  return std::nullopt;
}

inline constexpr ItemId item_id_or_none(int val) noexcept {
  if (val >= 0 && static_cast<size_t>(val) < ITEM_COUNT) {
    return static_cast<ItemId>(val);
  }
  return ItemId::None;
}

inline constexpr int item_id_to_int(ItemId id) noexcept {
  return static_cast<int>(id);
}

inline constexpr std::array<ItemId, 5> data_crystal_ids = {
    ItemId::AmberDatachip,
    ItemId::SapphireCortex,
    ItemId::RubyLaserCore,
    ItemId::EmeraldCryptokey,
    ItemId::QuantumDiamond,
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

