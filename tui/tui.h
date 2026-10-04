#pragma once

#include <string>

#include "routineverse.h"

class TuiApp {
 public:
  static constexpr int ITEM_CREDITS = 0;
  static constexpr int ITEM_BANK_VALUE = 1;
  static constexpr int ITEM_TOTAL_LEVEL = 2;
  static constexpr int ITEM_TOTAL_XP = 3;
  static constexpr int ITEM_HP = 4;
  static constexpr int ITEM_FIRST_SKILL = 5;
  static constexpr int TOTAL_ITEMS = 5 + SKILL_COUNT;

  TuiApp();
  ~TuiApp();

  TuiApp(const TuiApp&) = delete;
  TuiApp& operator=(const TuiApp&) = delete;

  int run();

 private:
  enum class FocusPane { Skills, Actions, Bank };

  // Rendering helpers
  void drawDashboard();
  void drawTopBar(int cols);
  void drawSkillsPane(int y, int x, int h, int w);
  void drawActionsOrCombatPane(int y, int x, int h, int w);
  void drawBankPane(int y, int x, int h, int w);
  void drawStatusPane(int y, int x, int h, int w);
  void drawGraphPane(int y, int x, int h, int w);
  void drawLogPane(int y, int x, int h, int w);
  void drawBottomKeyBar(int y, int cols);

  // Braille chart helper
  void renderBrailleChart(int y, int x, int h, int w, int item_idx,
                          bool show_axes);

  // Actions & Modals (1:1 parity with Qt6 MainWindow & Dialogs)
  void actionStartSelected();
  void actionStopActivity();
  void actionEatFood();
  void actionEquipSelected();
  void actionSellSelected();
  void actionSellAllBank();
  void actionCycleAttackStyle();
  void actionNewBountyContract();
  void actionFastForward(int seconds);
  void actionSaveGame();
  void actionLoadGame();
  void actionNewGame();

  void showShopDialog();
  void showEquipmentDialog();
  void showBestiaryDialog();
  void showHistoryDialog(int initial_item);
  void showAboutDialog();
  void showDocsDialog();
  void showMilestonesDialog();
  void showHelpDialog();

  // Generic UI Dialog primitives
  void showMessageModal(const std::string& title, const std::string& message,
                        int border_color = 1);
  bool showConfirmModal(const std::string& title, const std::string& message);
  int showInputSpinModal(const std::string& title, const std::string& message,
                         const std::string& question, int min_val, int max_val,
                         int initial_val, int unit_price = 0);

  void clampCursors();
  bool isCombatView() const;
  static std::string itemName(int item_idx);

  GameState _gameState;
  FocusPane _focus = FocusPane::Skills;
  int _skillCursor = 0;
  int _actionCursor = 0;
  int _monsterCursor = 0;
  int _bankCursor = 0;
  int _chartItemIdx = ITEM_CREDITS;
  bool _forceCombatView = false;
  bool _running = true;
};
