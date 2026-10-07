#pragma once

#include <array>
#include <cstdint>
#include <span>
#include <string>

#define RV_ITEM_ID_LIST(X) \
  X(None) \
  /* Scrap & Tech Nodes */ \
  X(CopperWireScrap) \
  X(PlasteelShards) \
  X(CarbonNanotubes) \
  X(OpticFiberBundle) \
  X(PositronicRelays) \
  X(CryoCellCore) \
  X(PlasmaConduit) \
  X(QuantumNode) \
  X(AiMainframeCore) \
  /* Recycled Basic / Raw Materials */ \
  X(CopperFilament) \
  X(PlasteelPolymer) \
  X(CarbonFiberWeave) \
  X(OpticSilicaGlass) \
  X(PositronicWafer) \
  X(CryoCoolantGel) \
  X(PlasmaCoil) \
  X(QuantumLattice) \
  X(NeuralMatrix) \
  /* Raw Synth-Biota */ \
  X(RawKrillBiomass) \
  X(RawNeonEel) \
  X(RawSynthCarp) \
  X(RawChromeSalmon) \
  X(RawCyberLobster) \
  X(RawPlasmaRay) \
  X(RawApexShark) \
  X(RawLeviathanCell) \
  X(RawCyberKraken) \
  /* Hydro-Farmed Crops & Synth-Noodles */ \
  X(HydroWheat) \
  X(SoyPods) \
  X(NeonScallion) \
  X(GlowNori) \
  X(BioBamboo) \
  X(CyberShiitake) \
  X(PlasmaChili) \
  X(ChronoLotus) \
  X(QuantumTruffle) \
  X(SynthNoodles) \
  /* Synthesized Stims, Cyber-Ramen & Toxic Slag */ \
  X(KrillRation) \
  X(NeonEelSkewer) \
  X(SynthCarpPack) \
  X(ChromeSalmonStim) \
  X(CyberLobsterMeal) \
  X(PlasmaRayInfusion) \
  X(ApexSharkBooster) \
  X(LeviathanNanomed) \
  X(KrakenBioElixir) \
  X(ShoyuRamen) \
  X(ScallionRamen) \
  X(NoriRamen) \
  X(BambooRamen) \
  X(ShiitakeRamen) \
  X(PlasmaChiliRamen) \
  X(ChronoLotusRamen) \
  X(TruffleRamen) \
  X(QuantumKrakenRamen) \
  X(SynthProteinBar) \
  X(ToxicSlag) \
  /* Deep-Mined Ores & Cells */ \
  X(CopperOre) \
  X(SiliconOre) \
  X(TitaniumOre) \
  X(CarbonCell) \
  X(SilverOre) \
  X(GoldOre) \
  X(CobaltOre) \
  X(TungstenOre) \
  X(NeutroniumOre) \
  X(ChronoOre) \
  X(QuantumOre) \
  /* Refined Alloys & Conductors */ \
  X(ScrapAlloy) \
  X(TitaniumAlloy) \
  X(DurasteelAlloy) \
  X(SilverConductor) \
  X(GoldSuperconductor) \
  X(CobaltAlloy) \
  X(TungstenAlloy) \
  X(NeutroniumAlloy) \
  X(ChronoAlloy) \
  X(QuantumAlloy) \
  /* Data Crystals */ \
  X(AmberDatachip) \
  X(SapphireCortex) \
  X(RubyLaserCore) \
  X(EmeraldCryptokey) \
  X(QuantumDiamond) \
  /* Weapons - Mono-Blades */ \
  X(ScrapBlade) \
  X(TitaniumBlade) \
  X(DurasteelBlade) \
  X(CobaltBlade) \
  X(TungstenBlade) \
  X(NeutroniumBlade) \
  X(ChronoBlade) \
  X(QuantumBlade) \
  /* Visors */ \
  X(ScrapVisor) \
  X(TitaniumVisor) \
  X(DurasteelVisor) \
  X(CobaltVisor) \
  X(TungstenVisor) \
  X(NeutroniumVisor) \
  X(ChronoVisor) \
  X(QuantumVisor) \
  /* Exo-Suits */ \
  X(ScrapExoSuit) \
  X(TitaniumExoSuit) \
  X(DurasteelExoSuit) \
  X(CobaltExoSuit) \
  X(TungstenExoSuit) \
  X(NeutroniumExoSuit) \
  X(ChronoExoSuit) \
  X(QuantumExoSuit) \
  /* Holo-Shields */ \
  X(ScrapShield) \
  X(TitaniumShield) \
  X(DurasteelShield) \
  X(CobaltShield) \
  X(TungstenShield) \
  X(NeutroniumShield) \
  X(ChronoShield) \
  X(QuantumShield) \
  /* Tools - Salvaging Cutters */ \
  X(ScrapCutter) \
  X(TitaniumCutter) \
  X(DurasteelCutter) \
  X(CobaltCutter) \
  X(TungstenCutter) \
  X(NeutroniumCutter) \
  X(ChronoCutter) \
  /* Tools - Bio-Harvesters */ \
  X(ScrapHarvester) \
  X(TitaniumHarvester) \
  X(DurasteelHarvester) \
  X(CobaltHarvester) \
  X(TungstenHarvester) \
  X(NeutroniumHarvester) \
  X(ChronoHarvester) \
  /* Tools - Mining Drills */ \
  X(ScrapDrill) \
  X(TitaniumDrill) \
  X(DurasteelDrill) \
  X(CobaltDrill) \
  X(TungstenDrill) \
  X(NeutroniumDrill) \
  X(ChronoDrill) \
  /* Tools - Synth-Reactors */ \
  X(BasicReactor) \
  X(PlasteelReactor) \
  X(NanotubeReactor) \
  X(PositronicReactor) \
  X(PlasmaReactor) \
  X(QuantumReactor) \
  X(MainframeReactor) \
  /* Cyberware - Auto-Stim Injectors */ \
  X(AutoStimMk1) \
  X(AutoStimMk2) \
  X(AutoStimMk3) \
  /* Enemy Salvage Loot */ \
  X(ServoParts) \
  X(HeavyChassis) \
  X(ApexCyberCore) \
  X(Microchip) \
  X(SynthWeaveHide)

enum class ItemId : uint16_t {
#define X(id) id,
  RV_ITEM_ID_LIST(X)
#undef X
};

std::string item_name(ItemId id);
std::span<const ItemId> all_item_ids() noexcept;
bool is_valid_item(ItemId id) noexcept;

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

