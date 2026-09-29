#include "window-cb.h"

#include <QDialog>
#include <QMessageBox>
#include <format>
#include <string>

#include "config.h"
#include "routineverse.h"
#include "window.h"

void window_main_button_start_clicked_cb(MainWindow& window) {
  auto& gs = window.gameState();
  if (window.modeTabs() && window.modeTabs()->currentIndex() == 1) {
    int mon_id = window.selectedMonsterId();
    if (mon_id >= 0) {
      gs.start_combat(mon_id);
      window.updateAllUi();
    }
  } else {
    int act_id = window.selectedActionId();
    if (act_id >= 0) {
      gs.start_skill_action(act_id);
      window.updateAllUi();
    }
  }
}

void window_main_button_stop_clicked_cb(MainWindow& window) {
  window.gameState().stop_activity();
  window.updateAllUi();
}

void window_main_button_eat_clicked_cb(MainWindow& window) {
  window.gameState().eat_food();
  window.updateAllUi();
}

void window_main_button_equip_clicked_cb(MainWindow& window) {
  ItemId item_id = window.selectedBankItemId();
  if (!is_valid_item(item_id)) {
    QMessageBox::information(
        &window, "Equip Item",
        "Please select cyberware, a weapon, or a stim from the Cyber-Vault first.");
  } else {
    const auto& info = get_item_info(item_id);
    if (info.equip_slot == EquipSlot::None && info.heal_amount <= 0) {
      QMessageBox::information(
          &window, "Equip Item",
          QString("%1 cannot be equipped or loaded into the Stim-Injector.")
              .arg(info.name));
      return;
    }
    window.gameState().equip_item(item_id);
    window.updateAllUi();
  }
}

void window_main_button_sell_clicked_cb(MainWindow& window) {
  auto& gs = window.gameState();
  ItemId item_id = window.selectedBankItemId();
  if (!is_valid_item(item_id)) {
    QMessageBox::information(
        &window, "Liquidate Item",
        "Please select an item in the Cyber-Vault to sell.");
    return;
  }
  int have = gs.item_qty(item_id);
  if (have <= 0) return;

  const auto& info = get_item_info(item_id);
  if (have == 1) {
    gs.sell_item(item_id, 1);
    window.updateAllUi();
    return;
  }

  QString msg =
      QString("Liquidating %1 (%2 Cr each)\nYou have %3 in your Cyber-Vault.")
          .arg(info.name)
          .arg(info.price)
          .arg(have);
  WindowInput dlg("Liquidate Vault Item", msg, "Quantity to sell:", 1, have,
                  have, &window);
  if (dlg.exec() == QDialog::Accepted) {
    gs.sell_item(item_id, dlg.value());
    window.updateAllUi();
  }
}

void window_main_button_sell_all_clicked_cb(MainWindow& window) {
  auto& gs = window.gameState();
  if (gs.bank.empty()) return;
  long long total_val = gs.total_bank_value();
  auto ans = QMessageBox::question(
      &window, "Liquidate All Vault Items",
      QString("Liquidate all %1 item stacks in your Cyber-Vault for %2?")
          .arg(gs.bank.size())
          .arg(QString::fromStdString(money_string(total_val))),
      QMessageBox::Yes | QMessageBox::No);
  if (ans == QMessageBox::Yes) {
    gs.sell_all_non_equipped();
    window.updateAllUi();
  }
}

void window_main_button_shop_clicked_cb(MainWindow& window) {
  WindowShop dlg(window.gameState(), &window);
  QObject::connect(&dlg, &WindowShop::stateChanged, &window,
                   &MainWindow::updateAllUi);
  dlg.exec();
  window.updateAllUi();
}

void window_main_button_equipment_clicked_cb(MainWindow& window) {
  WindowEquipment dlg(window.gameState(), &window);
  QObject::connect(&dlg, &WindowEquipment::stateChanged, &window,
                   &MainWindow::updateAllUi);
  dlg.exec();
  window.updateAllUi();
}

void window_main_button_bestiary_clicked_cb(MainWindow& window) {
  WindowBestiary dlg(window.gameState(), &window);
  dlg.exec();
}

void window_main_button_history_clicked_cb(MainWindow& window) {
  int item_idx = window.statusChartView()
                     ? window.statusChartView()->itemIndex()
                     : HistoryChartView::ITEM_CREDITS;
  WindowHistory dlg(window.gameState(), item_idx, &window);
  dlg.exec();
}

void window_main_button_ff1m_clicked_cb(MainWindow& window) {
  window.gameState().add_log("Fast-forwarding 1 minute of neural simulation...");
  window.gameState().fast_forward_seconds(60);
  window.updateAllUi();
}

void window_main_button_ff10m_clicked_cb(MainWindow& window) {
  window.gameState().add_log("Fast-forwarding 10 minutes of neural simulation...");
  window.gameState().fast_forward_seconds(600);
  window.updateAllUi();
}

void window_main_button_save_clicked_cb(MainWindow& window) {
  std::string path = GameState::default_save_path();
  if (window.gameState().save_to_file(path)) {
    window.gameState().add_log(std::format("Neural state saved to {}.", path));
    window.updateAllUi();
  } else {
    QMessageBox::warning(&window, "Save Error",
                         QString("Failed to save game to %1")
                             .arg(QString::fromStdString(path)));
  }
}

void window_main_button_load_clicked_cb(MainWindow& window) {
  std::string path = GameState::default_save_path();
  if (window.gameState().load_from_file(path)) {
    window.updateAllUi();
  } else {
    QMessageBox::information(&window, "Load State",
                             QString("No save file found at %1")
                                 .arg(QString::fromStdString(path)));
  }
}

void window_main_button_about_clicked_cb(MainWindow& window) {
  std::string info = std::format(
      "{}\n{}\n\nCyberpunk Idle RPG\nAuthor: "
      "{}\nVersion: {}",
      kProgramName, kProgramDescription, kProgramAuthorName, kProgramVersion);
  QMessageBox::about(&window, "About Routineverse", QString::fromStdString(info));
}

void window_main_button_docs_clicked_cb(MainWindow& window) {
  QMessageBox::information(
      &window, "Routineverse Cyber-Guide & Documentation",
      QString::fromUtf8(
          "Welcome to Routineverse (Cyberpunk Idle RPG)!\n\n"
          "• Extraction Protocols:\n"
          "  - Salvaging: Strip wiring, plasteel, nanotubes, and AI mainframe cores.\n"
          "  - Bio-Harvest: Culture synth-biota and recover submerged Corp data-caches.\n"
          "  - Farming: Cultivate hydroponic crops (Hydro-Wheat, Soy, Scallions, Nori, "
          "Bamboo, Shiitake, Plasma Chili, Chrono-Lotus, Quantum Truffle) & mill Synth-Noodles.\n"
          "  - Deep-Mining: Extract industrial ores and rare Data Crystals.\n\n"
          "• Processing, Smithing & Cyber-Fab:\n"
          "  - Recycling: Process tech scrap into Raw Materials (with Carbon Cell procs).\n"
          "  - Synth-Cook: Prep Synth-Noodles, cook high-healing Cyber-Ramen bowls, and "
          "synthesize raw biota into combat stims.\n"
          "  - Smithing: Smelt ores into Alloy Ingots and forge Mono-Blades & Exo-Suits.\n"
          "  - Cyber-Fab: Combine Alloy Ingots with Recycled Raw Materials to fabricate "
          "Visors, Holo-Shields, and high-tier Data Crystals.\n\n"
          "• Combat & Bounty Hunting:\n"
          "  - Equip fabricated weapons, cyber-armor, and stims from your Cyber-Vault.\n"
          "  - Choose your Combat Mode (Precision = Attack, Overdrive = "
          "Strength, Evasive = Defence).\n"
          "  - Neutralize Bounty Contract targets to earn Bounty XP and Bounty Tokens.\n"
          "  - Unlock the Auto-Stim Injector in the Cyber-Shop to automatically heal during "
          "combat!\n\n"
          "• Time & Offline Simulation:\n"
          "  - Use +1m / +10m Fast-Forward buttons to simulate idle bursts at "
          "any time."));
}

void window_main_button_highscores_clicked_cb(MainWindow& window) {
  const auto& gs = window.gameState();
  long long minutes = gs.total_ticks_ms / 60000;
  long long seconds = (gs.total_ticks_ms / 1000) % 60;
  std::string text = std::format(
      "Operative Summary & Milestones:\n\n"
      "Combat Level: {}   |   Total Skill Level: {} / {}\n"
      "Total Skill XP: {}\n"
      "Current Credits: {}   |   Total Credits Earned: {}\n"
      "Cyber-Vault Value: {} ({} / {} slots)\n"
      "Bounty Tokens: {}   |   Bounty Contracts Completed: {}\n"
      "Items Salvaged/Fabricated: {}\n"
      "Hostiles Neutralized: {}   |   Flatlines: {}\n"
      "NEXUS-9 (Mainframe Boss) Kills: {}\n"
      "Simulated Uptime: {}m {}s",
      gs.combat_level(), gs.total_skill_level(),
      SKILL_COUNT * MAX_SKILL_LEVEL, number_string(gs.total_skill_xp()),
      money_string(gs.credits), money_string(gs.total_credits_earned),
      money_string(gs.total_bank_value()), gs.used_bank_slots(),
      gs.bank_capacity, number_string(gs.bounty_tokens),
      gs.bounties_completed, number_string(gs.total_items_gathered),
      number_string(gs.total_monsters_killed), gs.player_deaths,
      gs.monster_kills[MONSTER_COUNT - 1], minutes, seconds);
  QMessageBox::information(&window, "Telemetry & Milestones",
                           QString::fromStdString(text));
}

void window_main_button_newgame_clicked_cb(MainWindow& window) {
  auto ans = QMessageBox::question(
      &window, "New Operative",
      "Are you sure you want to wipe your neural profile and start a New Game?",
      QMessageBox::Yes | QMessageBox::No);
  if (ans == QMessageBox::Yes) {
    window.gameState().new_game();
    window.updateAllUi();
  }
}
