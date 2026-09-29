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
  int item_id = window.selectedBankItemId();
  if (item_id < 0) {
    QMessageBox::information(&window, "Equip Item",
                             "Please select an item or food from the Bank first.");
    return;
  }
  const auto& info = item_info[item_id];
  if (info.equip_slot == EquipSlot::None && info.heal_amount <= 0) {
    QMessageBox::information(
        &window, "Equip Item",
        QString("%1 cannot be equipped or eaten.").arg(info.name));
    return;
  }
  window.gameState().equip_item(item_id);
  window.updateAllUi();
}

void window_main_button_sell_clicked_cb(MainWindow& window) {
  auto& gs = window.gameState();
  int item_id = window.selectedBankItemId();
  if (item_id < 0) {
    QMessageBox::information(&window, "Sell Item",
                             "Please select an item in the Bank to sell.");
    return;
  }
  int have = gs.item_qty(item_id);
  if (have <= 0) return;

  const auto& info = item_info[item_id];
  if (have == 1) {
    gs.sell_item(item_id, 1);
    window.updateAllUi();
    return;
  }

  QString msg = QString("Selling %1 (%2 GP each)\nYou have %3 in your Bank.")
                    .arg(info.name)
                    .arg(info.price)
                    .arg(have);
  WindowInput dlg("Sell Bank Item", msg, "Quantity to sell:", 1, have, have,
                  &window);
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
      &window, "Sell All Bank Items",
      QString("Sell all %1 item stacks in your Bank for %2?")
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
                     : HistoryChartView::ITEM_GP;
  WindowHistory dlg(window.gameState(), item_idx, &window);
  dlg.exec();
}

void window_main_button_ff1m_clicked_cb(MainWindow& window) {
  window.gameState().add_log("Fast-forwarding 1 minute of activity...");
  window.gameState().fast_forward_seconds(60);
  window.updateAllUi();
}

void window_main_button_ff10m_clicked_cb(MainWindow& window) {
  window.gameState().add_log("Fast-forwarding 10 minutes of activity...");
  window.gameState().fast_forward_seconds(600);
  window.updateAllUi();
}

void window_main_button_save_clicked_cb(MainWindow& window) {
  std::string path = GameState::default_save_path();
  if (window.gameState().save_to_file(path)) {
    window.gameState().add_log(std::format("Game saved to {}.", path));
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
    QMessageBox::information(&window, "Load Game",
                             QString("No save file found at %1")
                                 .arg(QString::fromStdString(path)));
  }
}

void window_main_button_about_clicked_cb(MainWindow& window) {
  std::string info = std::format(
      "{}\n{}\n\nInspired by Idle RPG\nAuthor: "
      "{}\nVersion: {}",
      kProgramName, kProgramDescription, kProgramAuthorName, kProgramVersion);
  QMessageBox::about(&window, "About Routineverse", QString::fromStdString(info));
}

void window_main_button_docs_clicked_cb(MainWindow& window) {
  QMessageBox::information(
      &window, "Routineverse Guide & Documentation",
      QString::fromUtf8(
          "Welcome to Routineverse (Simplified Idle RPG)!\n\n"
          "• Gathering Skills:\n"
          "  - Woodcutting: Chop trees to gather Logs.\n"
          "  - Fishing: Catch raw fish and occasional sunken treasure.\n"
          "  - Mining: Mine ores and find rare sparkling Gems.\n\n"
          "• Artisan Skills:\n"
          "  - Firemaking: Burn logs for XP, Coal procs, and a global XP bonus.\n"
          "  - Cooking: Cook raw fish into healing food for Combat.\n"
          "  - Smithing: Smelt ores into bars and forge Bronze through Dragon "
          "Scimitars, Helmets, Shields, and Platebodies.\n\n"
          "• Combat & Slayer:\n"
          "  - Equip forged weapons, armor, and cooked food from your Bank.\n"
          "  - Choose your Attack Style (Accurate = Attack, Aggressive = "
          "Strength, Defensive = Defence).\n"
          "  - Defeat Slayer Task monsters to earn Slayer XP and Slayer Coins.\n"
          "  - Unlock Auto-Eat in the Shop to automatically heal during "
          "combat!\n\n"
          "• Time & Offline Progress:\n"
          "  - Use +1m / +10m Fast-Forward buttons to simulate idle bursts at "
          "any time."));
}

void window_main_button_highscores_clicked_cb(MainWindow& window) {
  const auto& gs = window.gameState();
  long long minutes = gs.total_ticks_ms / 60000;
  long long seconds = (gs.total_ticks_ms / 1000) % 60;
  std::string text = std::format(
      "Character Summary & Milestones:\n\n"
      "Combat Level: {}   |   Total Skill Level: {} / {}\n"
      "Total Skill XP: {}\n"
      "Current Gold: {}   |   Total Gold Earned: {}\n"
      "Bank Value: {} ({} / {} slots)\n"
      "Slayer Coins: {}   |   Slayer Tasks Completed: {}\n"
      "Items Gathered/Crafted: {}\n"
      "Monsters Defeated: {}   |   Deaths: {}\n"
      "Malcs (Volcanic Boss) Kills: {}\n"
      "Simulated Playtime: {}m {}s",
      gs.combat_level(), gs.total_skill_level(),
      SKILL_COUNT * MAX_SKILL_LEVEL, number_string(gs.total_skill_xp()),
      money_string(gs.gp), money_string(gs.total_gp_earned),
      money_string(gs.total_bank_value()), gs.used_bank_slots(),
      gs.bank_capacity, number_string(gs.slayer_coins),
      gs.slayer_tasks_completed, number_string(gs.total_items_gathered),
      number_string(gs.total_monsters_killed), gs.player_deaths,
      gs.monster_kills[MONSTER_COUNT - 1], minutes, seconds);
  QMessageBox::information(&window, "Statistics & Milestones",
                           QString::fromStdString(text));
}

void window_main_button_newgame_clicked_cb(MainWindow& window) {
  auto ans = QMessageBox::question(
      &window, "New Game",
      "Are you sure you want to reset your character and start a New Game?",
      QMessageBox::Yes | QMessageBox::No);
  if (ans == QMessageBox::Yes) {
    window.gameState().new_game();
    window.updateAllUi();
  }
}
