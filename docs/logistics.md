# Logistics & Production Chains

This document details all resource extraction, refinement, fabrication, and culinary logistics chains in **Routineverse**.

---

## 1. End-to-End Logistics Overview

The non-combat economy is divided into three interconnected industrial branches:
1. **Metallurgy & Cyberware Branch**: `Salvaging` + `Deep-Mining` $\rightarrow$ `Recycling` + `Smithing` $\rightarrow$ `Cyber-Fab` (Weapons, Armor, and Data Crystals).
2. **Hardware & Hacking Branch**: `Salvaging` + `Deep-Mining` + `Recycling` + `Smithing` $\rightarrow$ `Cyber-Fab` (CPUs & RAM) $\rightarrow$ `Hacking` (Attack ICE, Defense/Repair ICE, Firewalls, and Integrity Patches — see [`hacking.md`](./hacking.md)).
3. **Bio-Agri & Culinary Branch**: `Fishing` + `Farming` $\rightarrow$ `Synth-Noodles` $\rightarrow$ `Synth-Cook` (Biota Stims and Cyber-Ramen).

```mermaid
flowchart TD
  subgraph Metallurgy["Metallurgy, Hardware & Hacking Branch"]
    S["Salvaging<br/>(9 Scrap Tiers + Scrap CPU/RAM)"]
    R["Recycling<br/>(9 Raw Materials)"]
    M["Deep-Mining<br/>(10 Ores + Carbon Cell)"]
    SM_ING["Smithing: Smelting<br/>(8 Combat Alloys + 2 Conductors)"]
    SM_EQ["Smithing: Forging<br/>(8 Mono-Blades + 8 Exo-Suits)"]
    CF_EQ["Cyber-Fab: Gear<br/>(8 Visors + 8 Holo-Shields)"]
    CF_CR["Cyber-Fab: Tech Cores<br/>(5 Data Crystals)"]
    CF_HW["Cyber-Fab: Hardware<br/>(4 CPU Tiers + 4 RAM Tiers)"]
    HCK["Hacking<br/>(Attack/Defense ICE, Firewalls & Patches)"]

    S -->|"1x Scrap"| R
    S -->|"Scrap CPU / RAM"| HCK
    R -.->|"25% Bonus Proc:<br/>1x Carbon Cell"| SM_ING
    M -->|"Ores + Carbon Cells"| SM_ING
    M -.->|"8% Bonus Proc"| CF_CR
    SM_ING -->|"2x–5x Alloy Ingot"| SM_EQ
    SM_ING -->|"1x–2x Alloy Ingot"| CF_EQ
    R -->|"1x Raw Material"| CF_EQ
    SM_ING -->|"1x Conductor / Alloy"| CF_CR
    R -->|"1x Raw Material"| CF_CR
    M & R & SM_ING & CF_CR -->|"Silicon / Conductors / Wafers / Lattice"| CF_HW
    CF_HW -->|"CPUs + RAM"| HCK
  end

  subgraph Culinary["Bio-Agri & Culinary Branch"]
    BH["Fishing<br/>(9 Raw Synth-Biota)"]
    FM["Farming<br/>(9 Hydroponic Crops)"]
    HW["Hydro-Wheat<br/>(Lv 1 Crop + 20% Farming Proc)"]
    ND["Synth-Noodles<br/>(1x Hydro-Wheat -> 2x Noodles)"]
    SC_STIM["Synth-Cook: Biota Stims<br/>(9 Healing Rations/Stims)"]
    SC_RAMEN["Synth-Cook: Cyber-Ramen<br/>(9 High-Healing Ramen Bowls)"]

    BH -->|"1x Raw Biota"| SC_STIM
    FM -->|"Cultivate Lv 1<br/>or 20% Bonus Proc"| HW
    HW -->|"Farming Lv 3 or<br/>Synth-Cook Lv 1"| ND
    ND -->|"1x Synth-Noodles"| SC_RAMEN
    FM -->|"1x Crop (Soy..Truffle)"| SC_RAMEN
    BH -->|"1x Raw Cyber-Kraken"| SC_RAMEN
  end
```

---

## 2. Salvaging & Recycling Chain

**Salvaging** extracts raw tech scrap (`ItemCategory::Scrap`) as well as low-tier **CPU/RAM hardware** (`Scrap Logic CPU` at Lv 3 and `Scrap DRAM Stick` at Lv 6 for **Hacking**). **Recycling** refines scrap into `ItemCategory::RawMaterial` components used in **Cyber-Fab** and **Hacking**. Every recycling cycle also grants minor Credits and a **25% chance to recover `1x Carbon Cell`**, which feeds directly into **Smithing**.

```mermaid
flowchart LR
  subgraph Salvaging["Salvaging (Scrap & Hardware)"]
    S0["Lv 1: Copper Wire Scrap"]
    S_CPU["Lv 3: Scrap Logic CPU"]
    S_RAM["Lv 6: Scrap DRAM Stick"]
    S1["Lv 10: Plasteel Shards"]
    S2["Lv 25: Carbon Nanotubes"]
    S3["Lv 35: Optic Fiber Bundle"]
    S4["Lv 45: Positronic Relays"]
    S5["Lv 55: Cryo-Cell Core"]
    S6["Lv 60: Plasma Conduit"]
    S7["Lv 75: Quantum Node"]
    S8["Lv 90: AI Mainframe Core"]
  end

  subgraph Recycling["Recycling (Raw Materials)"]
    R0["Lv 1: Copper Filament"]
    R1["Lv 10: Plasteel Polymer"]
    R2["Lv 25: Carbon Fiber Weave"]
    R3["Lv 35: Optic Silica Glass"]
    R4["Lv 45: Positronic Wafer"]
    R5["Lv 55: Cryo-Coolant Gel"]
    R6["Lv 60: Magnetic Plasma Coil"]
    R7["Lv 75: Quantum Lattice"]
    R8["Lv 90: Neural Matrix"]
  end

  S0 --> R0
  S1 --> R1
  S2 --> R2
  S3 --> R3
  S4 --> R4
  S5 --> R5
  S6 --> R6
  S7 --> R7
  S8 --> R8

  S_CPU & S_RAM -->|"Hacking"| HCK1["Spike-ICE v1.0 / Watchdog-ICE v1.0<br/>Packet Filter Firewall / Parity Patch"]
  R0 -->|"Cyber-Fab"| CF0["Scrap Visor, Shield & Scrap CPU"]
  R1 -->|"Cyber-Fab / Hacking"| CF1["Titanium Visor/Shield & Firmware Matrix"]
  R2 -->|"Cyber-Fab"| CF2["Durasteel Visor/Shield & Optic-NAND RAM"]
  R3 -->|"Cyber-Fab"| CF3["Amber Datachip & Optic-NAND RAM"]
  R4 -->|"Cyber-Fab"| CF4["Positronic CPU, Sapphire Cortex, Cobalt Visor & Cryo RAM"]
  R5 -->|"Cyber-Fab"| CF5["Cobalt Holo-Aegis & Cryo-Holographic RAM"]
  R6 -->|"Cyber-Fab / Hacking"| CF6["Ruby Core, Tungsten Gear & Neural Restore"]
  R7 -->|"Cyber-Fab"| CF7["Quantum CPU, Quantum RAM, Emerald Key & Neutronium/Chrono Gear"]
  R8 -->|"Cyber-Fab / Hacking"| CF8["Neural CPU, Quantum Diamond, Quantum Gear & Quantum Rollback"]
```

---

## 3. Deep-Mining, Smithing & Cyber-Fab Gear & Hardware Chain (8 Tiers)

**Deep-Mining** extracts ores (`ItemCategory::RawOre`), which **Smithing** smelts into `ItemCategory::Alloy` ingots.
- **Smithing** forges offensive **Mono-Blades** (`2x Ingot`) and heavy **Exo-Suits** (`5x Ingot`).
- **Cyber-Fab** combines **Alloy Ingots** and **Ores** with **Recycled Raw Materials** to fabricate:
  - **Visors** (`1x Ingot + 1x Raw Material`) and **Holo-Shields** (`2x Ingot + 1x Raw Material`)
  - **Data Crystals** (`1x Conductor/Ingot + 1x Raw Material`)
  - **CPUs & RAM Hardware** (`Scrap Logic CPU`, `Scrap DRAM Stick`, `Positronic Multi-Core CPU`, `Optic-NAND Storage Bank`, `Quantum Co-Processor`, `Cryo-Holographic RAM`, `Neural Overmind CPU`, `Quantum Qubit Vault`) for **Hacking** (see [`hacking.md`](./hacking.md#2-hardware-components-itemcategoryhardware)).

### Tier 1–4 Metallurgy & Cyber-Fab Graph

```mermaid
flowchart LR
  subgraph Ores1["Deep-Mining (Tiers 1-4)"]
    O_CU["Lv 1: Copper Ore"]
    O_SI["Lv 1: Silicon Ore"]
    O_TI["Lv 15: Titanium Ore"]
    O_CC["Lv 30: Carbon Cell"]
    O_AG["Lv 35: Silver Ore"]
    O_AU["Lv 40: Gold Ore"]
    O_CO["Lv 50: Cobalt Ore"]
  end

  subgraph Ingots1["Smithing: Smelting"]
    A_SC["Lv 1: Scrap-Alloy Ingot"]
    A_TI["Lv 15: Titanium Ingot"]
    A_DU["Lv 30: Durasteel Ingot"]
    A_AG["Lv 35: Silver Conductor"]
    A_AU["Lv 40: Gold Superconductor"]
    A_CO["Lv 50: Cobalt-Chrome Ingot"]
  end

  O_CU & O_SI --> A_SC
  O_SI -->|"Cyber-Fab + Copper"| HW_T1["Scrap Logic CPU (Lv 8)<br/>Scrap DRAM Stick (Lv 10)"]
  O_TI --> A_TI
  O_TI -->|"1x"| A_DU
  O_CC -->|"2x"| A_DU
  O_AG --> A_AG
  O_AU --> A_AU
  O_CO -->|"1x"| A_CO
  O_CC -->|"4x"| A_CO

  A_SC -->|"Smithing"| G_SC["Scrap Blade (2x)<br/>Scrap Exo-Harness (5x)"]
  A_SC -->|"Cyber-Fab + Copper Filament"| F_SC["Scrap Optic Visor<br/>Scrap Riot Buckler"]

  A_TI -->|"Smithing"| G_TI["Titanium Mono-Blade (2x)<br/>Titanium Flak-Jacket (5x)"]
  A_TI -->|"Cyber-Fab + Plasteel Polymer"| F_TI["Titanium HUD Visor<br/>Titanium Deflector"]

  A_DU -->|"Smithing"| G_DU["Durasteel Katana (2x)<br/>Durasteel Exo-Rig (5x)"]
  A_DU -->|"Cyber-Fab + Carbon Fiber Weave"| F_DU["Durasteel Tac-Helm<br/>Durasteel Barrier"]

  A_AG -->|"Cyber-Fab + Optic Silica Glass"| C_AMB["Amber Datachip"]
  A_AG -->|"Cyber-Fab + Positronic Wafer"| HW_CPU2["Positronic Multi-Core CPU (Lv 32)"]
  A_AU -->|"Cyber-Fab + Positronic Wafer"| C_SAP["Sapphire Cortex"]
  A_AU -->|"Cyber-Fab + Quantum Lattice"| HW_CPU3["Quantum Co-Processor (Lv 58)"]
  A_AU -->|"Cyber-Fab + Plasma Coil"| C_RUB["Ruby Laser Core"]

  A_CO -->|"Smithing"| G_CO["Cobalt Laser-Edge (2x)<br/>Cobalt Subdermal Rig (5x)"]
  A_CO -->|"Cyber-Fab + Wafer / Cryo-Gel"| F_CO["Cobalt Neural Visor<br/>Cobalt Holo-Aegis"]
```

### Tier 5–8 Metallurgy & Cyber-Fab Graph (Endgame & Quantum Tier)

```mermaid
flowchart LR
  subgraph Ores2["Deep-Mining (Tiers 5-8)"]
    O_CC2["Lv 30: Carbon Cell"]
    O_TU["Lv 70: Tungsten Ore"]
    O_NE["Lv 80: Neutronium Ore"]
    O_CH["Lv 88: Chrono-Crystal Ore"]
    O_QU["Lv 96: Quantum Singularity Ore"]
  end

  subgraph Ingots2["Smithing: Smelting"]
    A_TU["Lv 70: Tungsten Ingot"]
    A_NE["Lv 80: Neutronium Ingot"]
    A_CH["Lv 88: Chrono-Alloy Ingot"]
    A_QU["Lv 95: Quantum-Flux Ingot"]
  end

  O_TU -->|"1x"| A_TU
  O_CC2 -->|"6x"| A_TU

  O_NE -->|"1x"| A_NE
  O_CC2 -->|"8x"| A_NE

  O_CH -->|"1x"| A_CH
  O_NE -->|"2x"| A_CH

  O_QU -->|"1x"| A_QU
  O_CH -->|"2x"| A_QU

  A_TU -->|"Smithing"| G_TU["Tungsten Mantis-Blade (2x)<br/>Tungsten Power-Armor (5x)"]
  A_TU -->|"Cyber-Fab + Plasma Coil"| F_TU["Tungsten Cyber-Helm<br/>Tungsten Pulse-Shield"]

  A_NE -->|"Smithing"| G_NE["Neutronium Phase-Saber (2x)<br/>Neutronium Nano-Suit (5x)"]
  A_NE -->|"Cyber-Fab + Quantum Lattice"| F_NE["Emerald Cryptokey<br/>Neutronium Crown & Forcefield"]

  A_CH -->|"Smithing"| G_CH["Chrono-Edge Katana (2x)<br/>Chrono-Weave Exo-Suit (5x)"]
  A_CH -->|"Cyber-Fab + Lattice / Matrix"| F_CH["Neural Overmind CPU (Lv 85)<br/>Quantum Diamond, Chrono Visor & Barrier"]

  A_QU -->|"Smithing"| G_QU["Quantum Singularity Blade (2x)<br/>Quantum Phase Exo-Suit (5x)"]
  A_QU -->|"Cyber-Fab + Neural Matrix"| F_QU["Quantum Tachyon Visor<br/>Quantum Event-Horizon Shield"]
```

---

## 4. Farming, Synth-Noodles & Cyber-Ramen Culinary Chain

The culinary system offers two parallel paths in **Synth-Cook**:
1. **Single-Input Biota Stims**: Directly synthesize aquatic `RawBiota` from **Fishing** into combat rations (`+30 HP` to `+680 HP`).
2. **Multi-Ingredient Cyber-Ramen**: Cultivate `Hydro-Wheat` in **Farming** (also obtained via a **20% bonus proc** on any hydroponic crop harvest), mill it into `2x Synth-Noodles`, and combine `1x Synth-Noodles` with hydroponic crops (or `Raw Cyber-Kraken`) to cook high-healing **Cyber-Ramen** bowls (`+65 HP` to `+980 HP`).

```mermaid
flowchart LR
  subgraph Hydroponics["Farming (Hydroponic Crops)"]
    F_WH["Lv 1: Hydro-Wheat<br/>(+20% proc on all crops)"]
    F_SOY["Lv 10: Synth-Soy Pods"]
    F_SC["Lv 22: Neon Scallion"]
    F_NR["Lv 35: Bioluminescent Nori"]
    F_BM["Lv 48: Cyber-Bamboo Shoot"]
    F_SH["Lv 62: Spore-Tech Shiitake"]
    F_CH["Lv 75: Plasma Ghost-Chili"]
    F_LO["Lv 86: Chrono-Lotus Root"]
    F_TR["Lv 94: Quantum Myco-Truffle"]
  end

  subgraph Aquaculture["Fishing (Raw Biota)"]
    B_ALL["Lv 1–85: Krill .. Leviathan"]
    B_KR["Lv 95: Raw Cyber-Kraken"]
  end

  subgraph NoodlePrep["Noodle Milling (100% Success)"]
    ND["Synth-Noodles (x2)<br/>Farming Lv 3 or Synth-Cook Lv 1"]
  end

  subgraph RamenCook["Synth-Cook: Cyber-Ramen & Stims"]
    STIM["Biota Stims (+30 to +680 HP)"]
    R1["Lv 10: Soy-Shoyu Cyber-Ramen (+65 HP)"]
    R2["Lv 22: Neon Scallion Ramen (+125 HP)"]
    R3["Lv 38: Glow-Nori Umami Ramen (+195 HP)"]
    R4["Lv 50: Cyber-Bamboo Miso Ramen (+280 HP)"]
    R5["Lv 64: Spore-Shiitake Tonkotsu Ramen (+390 HP)"]
    R6["Lv 76: Plasma Volcano Ramen (+540 HP)"]
    R7["Lv 88: Chrono-Lotus Broth Ramen (+720 HP)"]
    R8["Lv 94: Quantum Truffle Ramen (+860 HP)"]
    R9["Lv 97: Quantum Kraken Special Ramen (+980 HP)"]
  end

  F_WH -->|"1x Hydro-Wheat"| ND
  B_ALL --> STIM
  B_KR -->|"1x Raw Kraken"| STIM

  ND & F_SOY --> R1
  ND & F_SC --> R2
  ND & F_NR --> R3
  ND & F_BM --> R4
  ND & F_SH --> R5
  ND & F_CH --> R6
  ND & F_LO --> R7
  ND & F_TR --> R8
  ND & B_KR --> R9
```

---

## 5. Complete Recipe Reference Tables

### Farming Protocols (`SkillType::Farming`)

| Lv | Protocol | Base Cycle | XP | Inputs | Output | Value |
|---:|----------|:----------:|---:|--------|--------|------:|
| 1 | Cultivate Hydro-Wheat | 2.8s | 14 | — | `1x Hydro-Wheat` | 4 Cr |
| 3 | Mill Synth-Noodles | 2.2s | 20 | `1x Hydro-Wheat` | `2x Synth-Noodles` | 8 Cr ea |
| 10 | Grow Synth-Soy Pods | 3.2s | 28 | — | `1x Synth-Soy Pods` | 10 Cr |
| 22 | Harvest Neon Scallion | 3.6s | 55 | — | `1x Neon Scallion` | 22 Cr |
| 35 | Culture Glow-Nori | 4.2s | 92 | — | `1x Bioluminescent Nori` | 42 Cr |
| 48 | Harvest Cyber-Bamboo | 4.8s | 145 | — | `1x Cyber-Bamboo Shoot` | 75 Cr |
| 62 | Grow Spore-Shiitake | 5.5s | 215 | — | `1x Spore-Tech Shiitake` | 130 Cr |
| 75 | Cultivate Plasma Chili | 6.5s | 330 | — | `1x Plasma Ghost-Chili` | 225 Cr |
| 86 | Harvest Chrono-Lotus | 7.6s | 490 | — | `1x Chrono-Lotus Root` | 380 Cr |
| 94 | Forage Quantum Truffle | 8.8s | 660 | — | `1x Quantum Myco-Truffle` | 650 Cr |

### Synth-Cook Protocols (`SkillType::SynthCook`)

| Lv | Protocol | Base Cycle | XP | Input 1 | Input 2 | Output | Heal | Value |
|---:|----------|:----------:|---:|---------|---------|--------|-----:|------:|
| 1 | Prep Synth-Noodles | 2.2s | 20 | `1x Hydro-Wheat` | — | `2x Synth-Noodles` | — | 8 Cr |
| 1 | Synth Krill Ration | 2.6s | 18 | `1x Raw Krill Biomass` | — | `1x Krill Ration` | +30 HP | 8 Cr |
| 5 | Synth Neon Eel | 2.8s | 34 | `1x Raw Neon Eel` | — | `1x Neon Eel Skewer` | +50 HP | 15 Cr |
| 10 | Cook Shoyu Ramen | 2.8s | 45 | `1x Synth-Noodles` | `1x Synth-Soy Pods` | `1x Soy-Shoyu Cyber-Ramen` | +65 HP | 35 Cr |
| 20 | Synth Carp Pack | 3.0s | 70 | `1x Raw Synth-Carp` | — | `1x Synth-Carp Pack` | +80 HP | 32 Cr |
| 22 | Cook Scallion Ramen | 3.0s | 85 | `1x Synth-Noodles` | `1x Neon Scallion` | `1x Neon Scallion Ramen` | +125 HP | 68 Cr |
| 35 | Synth Salmon Stim | 3.2s | 115 | `1x Raw Chrome Salmon` | — | `1x Chrome Salmon Stim` | +110 HP | 55 Cr |
| 38 | Cook Nori Ramen | 3.3s | 150 | `1x Synth-Noodles` | `1x Bioluminescent Nori` | `1x Glow-Nori Umami Ramen` | +195 HP | 135 Cr |
| 45 | Synth Lobster Meal | 3.4s | 175 | `1x Raw Cyber-Lobster` | — | `1x Cyber-Lobster Meal` | +160 HP | 100 Cr |
| 50 | Cook Bamboo Ramen | 3.5s | 220 | `1x Synth-Noodles` | `1x Cyber-Bamboo Shoot` | `1x Cyber-Bamboo Miso Ramen` | +280 HP | 230 Cr |
| 55 | Synth Plasma Ray | 3.6s | 240 | `1x Raw Plasma Ray` | — | `1x Plasma Ray Infusion` | +220 HP | 165 Cr |
| 64 | Cook Shiitake Ramen | 3.8s | 310 | `1x Synth-Noodles` | `1x Spore-Tech Shiitake` | `1x Spore-Shiitake Tonkotsu Ramen` | +390 HP | 390 Cr |
| 70 | Synth Shark Boost | 3.8s | 360 | `1x Raw Apex Shark` | — | `1x Apex Shark Booster` | +320 HP | 300 Cr |
| 76 | Cook Plasma Chili Ramen | 4.1s | 440 | `1x Synth-Noodles` | `1x Plasma Ghost-Chili` | `1x Plasma Volcano Ramen` | +540 HP | 680 Cr |
| 85 | Synth Leviathan Med | 4.2s | 540 | `1x Raw Leviathan Cell` | — | `1x Leviathan Nanomed` | +480 HP | 550 Cr |
| 88 | Cook Chrono-Lotus Ramen | 4.4s | 620 | `1x Synth-Noodles` | `1x Chrono-Lotus Root` | `1x Chrono-Lotus Broth Ramen` | +720 HP | 1,150 Cr |
| 94 | Cook Truffle Ramen | 4.6s | 780 | `1x Synth-Noodles` | `1x Quantum Myco-Truffle` | `1x Quantum Truffle Ramen` | +860 HP | 1,850 Cr |
| 95 | Synth Kraken Elixir | 4.6s | 720 | `1x Raw Cyber-Kraken` | — | `1x Kraken Bio-Elixir` | +680 HP | 950 Cr |
| 97 | Cook Kraken Ramen | 4.8s | 880 | `1x Synth-Noodles` | `1x Raw Cyber-Kraken` | `1x Quantum Kraken Special Ramen` | +980 HP | 2,200 Cr |
