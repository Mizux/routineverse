#pragma once

#include <QComboBox>
#include <QContextMenuEvent>
#include <QDialog>
#include <QGroupBox>
#include <QKeySequence>
#include <QLabel>
#include <QMenu>
#include <QMouseEvent>
#include <QProgressBar>
#include <QPushButton>
#include <QShortcut>
#include <QSpinBox>
#include <QString>
#include <QTabWidget>
#include <QTextEdit>
#include <QTimer>
#include <QTreeWidget>
#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <optional>
#include <vector>

#include "routineverse.hpp"

class HistoryChartView : public QChartView {
  Q_OBJECT

 public:
  static int totalItems() { return GameState::History::total_items(); }

  explicit HistoryChartView(bool compact = true, QWidget* parent = nullptr);
  virtual ~HistoryChartView() = default;

  HistoryChartView(const HistoryChartView&) = delete;
  HistoryChartView& operator=(const HistoryChartView&) = delete;

  int itemIndex() const { return _item_idx; }
  void setItemIndex(int idx);

  void updateChart(const GameState& gameState);
  static QString itemName(int item_idx);

 signals:
  void itemChanged(int item_idx);
  void zoomRequested(int item_idx);

 protected:
  void mousePressEvent(QMouseEvent* event) override;
  void contextMenuEvent(QContextMenuEvent* event) override;

 private:
  void _setupChart();

  bool _compact = true;
  int _item_idx = GameState::History::ITEM_CREDITS;

  QChart* _chart = nullptr;
  QValueAxis* _axis_x = nullptr;
  QValueAxis* _axis_y = nullptr;
  QLineSeries* _series_data = nullptr;
  QScatterSeries* _series_points = nullptr;
};

class MainWindow : public QWidget {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = nullptr);
  explicit MainWindow(GameState game_state, QWidget* parent = nullptr);
  virtual ~MainWindow() = default;

  MainWindow(const MainWindow&) = delete;
  MainWindow& operator=(const MainWindow&) = delete;

  GameState& gameState() { return _gameState; }
  const GameState& gameState() const { return _gameState; }

  void updateAllUi();
  void updateLiveProgressOnly();

  SkillType selectedSkill() const { return _selected_skill; }
  void setSelectedSkill(SkillType skill);

  int selectedActionId() const;
  std::optional<MonsterId> selectedMonsterId() const;
  ItemId selectedBankItemId() const;

 public slots:
  void slotStart();
  void slotStop();
  void slotEat();
  void slotEquip();
  void slotSell();
  void slotSellAll();
  void slotShop();
  void slotEquipment();
  void slotBestiary();
  void slotHistory();
  void slotFastForward1m();
  void slotFastForward10m();
  void slotSave();
  void slotLoad();
  void slotAbout();
  void slotDocs();
  void slotHighscores();
  void slotNewGame();
  void slotNewBountyContract();

  void onTickTimer();
  void onSkillSelectionChanged();
  void onActionDoubleClicked();
  void onMonsterDoubleClicked();
  void onBankDoubleClicked();
  void onAttackStyleChanged(int idx);
  void onStatusZoomRequested(int item_idx);

 private:
  void _setupWidget();
  void _fillTreeviewSkills();
  void _fillTreeviewActions();
  void _fillTreeviewMonsters();
  void _fillTreeviewBank();

  GameState _gameState;
  SkillType _selected_skill = SkillType::Salvaging;

  QTimer* _tick_timer = nullptr;
  QTextEdit* _textview_information = nullptr;
  QLabel* _label_active_banner = nullptr;
  QProgressBar* _progressbar_action = nullptr;

  QTreeWidget* _treeview_skills = nullptr;
  QTabWidget* _tabs_mode = nullptr;
  QTreeWidget* _treeview_actions = nullptr;
  QTreeWidget* _treeview_monsters = nullptr;
  QTreeWidget* _treeview_bank = nullptr;

  QComboBox* _combo_attack_style = nullptr;
  QLabel* _label_bounty_task = nullptr;
  QPushButton* _button_bounty_task = nullptr;
  QProgressBar* _progressbar_player_atk = nullptr;
  QProgressBar* _progressbar_monster_hp = nullptr;
  QProgressBar* _progressbar_monster_integrity = nullptr;
  QProgressBar* _progressbar_monster_atk = nullptr;

  QPushButton* _button_start = nullptr;
  QPushButton* _button_stop = nullptr;
  QPushButton* _button_eat = nullptr;
  QPushButton* _button_equip = nullptr;
  QPushButton* _button_sell = nullptr;
  QPushButton* _button_sell_all = nullptr;
  QPushButton* _button_shop = nullptr;
  QPushButton* _button_equipment = nullptr;
  QPushButton* _button_bestiary = nullptr;
  QPushButton* _button_history = nullptr;
  QPushButton* _button_ff1m = nullptr;
  QPushButton* _button_ff10m = nullptr;
  QPushButton* _button_save = nullptr;
  QPushButton* _button_load = nullptr;
  QPushButton* _button_about = nullptr;
  QPushButton* _button_docs = nullptr;
  QPushButton* _button_highscores = nullptr;
  QPushButton* _button_newgame = nullptr;

  QGroupBox* _group_bank = nullptr;
  QLabel* _label_credits = nullptr;
  QLabel* _label_bounty_tokens = nullptr;
  QLabel* _label_combat_lvl = nullptr;
  QLabel* _label_total_lvl = nullptr;
  QLabel* _label_tools = nullptr;
  QLabel* _label_equipped_weapon = nullptr;
  QLabel* _label_equipped_armor = nullptr;
  QLabel* _label_equipped_ice = nullptr;
  QLabel* _label_equipped_food = nullptr;
  QProgressBar* _progressbar_hp = nullptr;
  QProgressBar* _progressbar_integrity = nullptr;
  HistoryChartView* _drawingarea_status = nullptr;
  QShortcut* _shortcut_quit = nullptr;
};

class WindowShop : public QDialog {
  Q_OBJECT

 public:
  explicit WindowShop(GameState& gameState, QWidget* parent = nullptr);
  virtual ~WindowShop() = default;

  WindowShop(const WindowShop&) = delete;
  WindowShop& operator=(const WindowShop&) = delete;

  void updateShopUi();

 signals:
  void stateChanged();

 private slots:
  void onBuyCutter();
  void onBuyHarvester();
  void onBuyDrill();
  void onBuyReactor();
  void onBuyAutoStim();
  void onBuyBankSlot();

 private:
  void _setupWidget();

  GameState& _gameState;
  QLabel* _label_credits = nullptr;
  QLabel* _label_cutter = nullptr;
  QLabel* _label_harvester = nullptr;
  QLabel* _label_drill = nullptr;
  QLabel* _label_reactor = nullptr;
  QLabel* _label_autostim = nullptr;
  QLabel* _label_bank = nullptr;

  QPushButton* _btn_cutter = nullptr;
  QPushButton* _btn_harvester = nullptr;
  QPushButton* _btn_drill = nullptr;
  QPushButton* _btn_reactor = nullptr;
  QPushButton* _btn_autostim = nullptr;
  QPushButton* _btn_bank = nullptr;
};

class WindowEquipment : public QDialog {
  Q_OBJECT

 public:
  explicit WindowEquipment(GameState& gameState, QWidget* parent = nullptr);
  virtual ~WindowEquipment() = default;

  WindowEquipment(const WindowEquipment&) = delete;
  WindowEquipment& operator=(const WindowEquipment&) = delete;

  void updateEquipmentUi();

 signals:
  void stateChanged();

 private slots:
  void onUnequipSlot(EquipSlot slot);
  void onUnequipAttackIce();
  void onUnequipDefenseIce();

 private:
  void _setupWidget();

  GameState& _gameState;
  std::vector<QLabel*> _slot_labels;
  std::vector<QPushButton*> _slot_buttons;
  QLabel* _label_attack_ice = nullptr;
  QPushButton* _btn_attack_ice = nullptr;
  QLabel* _label_defense_ice = nullptr;
  QPushButton* _btn_defense_ice = nullptr;
  QLabel* _label_food = nullptr;
  QLabel* _label_stats = nullptr;
};

class WindowBestiary : public QDialog {
  Q_OBJECT

 public:
  explicit WindowBestiary(const GameState& gameState, QWidget* parent = nullptr);
  virtual ~WindowBestiary() = default;

  WindowBestiary(const WindowBestiary&) = delete;
  WindowBestiary& operator=(const WindowBestiary&) = delete;

 private:
  void _setupWidget();

  const GameState& _gameState;
};

class WindowHistory : public QDialog {
  Q_OBJECT

 public:
  explicit WindowHistory(const GameState& gameState, int initial_item = 0,
                         QWidget* parent = nullptr);
  virtual ~WindowHistory() = default;

  WindowHistory(const WindowHistory&) = delete;
  WindowHistory& operator=(const WindowHistory&) = delete;

 public slots:
  void onItemChanged(int index);

 private:
  void _setupWidget(int initial_item);
  void _refreshChart();

  const GameState& _gameState;
  QComboBox* _combo_item = nullptr;
  HistoryChartView* _chart_view = nullptr;
  QPushButton* _button_close = nullptr;
};

class WindowInput : public QDialog {
  Q_OBJECT

 public:
  WindowInput(const QString& title, const QString& message, const QString& question,
              int min_val, int max_val, int initial_val, QWidget* parent = nullptr);
  virtual ~WindowInput() = default;

  WindowInput(const WindowInput&) = delete;
  WindowInput& operator=(const WindowInput&) = delete;

  int value() const;

 private:
  QSpinBox* _spinbox = nullptr;
};
