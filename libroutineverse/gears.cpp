#include "gears.hpp"

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
    {1, "Titanium Bio-Rig", "-6% Fishing/Farming Interval", 10, 200, 6,
     ItemId::TitaniumHarvester},
    {2, "Durasteel Bio-Sampler", "-12% Fishing/Farming Interval", 25, 750, 12,
     ItemId::DurasteelHarvester},
    {3, "Cobalt Gene-Extractor", "-18% Fishing/Farming Interval", 40, 2500, 18,
     ItemId::CobaltHarvester},
    {4, "Tungsten Drone-Trawler", "-24% Fishing/Farming Interval", 55, 8000, 24,
     ItemId::TungstenHarvester},
    {5, "Neutronium Bio-Harvester", "-30% Fishing/Farming Interval", 70, 25000, 30,
     ItemId::NeutroniumHarvester},
    {6, "Chrono Stasis-Harvester", "-38% Fishing/Farming Interval", 85, 80000, 38,
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

