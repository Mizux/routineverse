#include "window.h"

#include <QApplication>
#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QFormLayout>
#include <QGraphicsLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QKeySequence>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPainter>
#include <QProgressBar>
#include <QPushButton>
#include <QScrollBar>
#include <QShortcut>
#include <QSpinBox>
#include <QTabWidget>
#include <QTextEdit>
#include <QTimer>
#include <QTreeWidget>
#include <QWidget>
#include <algorithm>
#include <array>
#include <format>
#include <string>

#include "config.h"
#include "routineverse.h"
#include "window-cb.h"

// ============================================================================
// HistoryChartView Implementation
// ============================================================================

HistoryChartView::HistoryChartView(bool compact, QWidget* parent)
    : QChartView(parent), _compact(compact) {
  _setupChart();
}

void HistoryChartView::_setupChart() {
  _chart = new QChart();
  _chart->legend()->hide();
  _chart->setBackgroundBrush(QBrush(Qt::black));
  _chart->setPlotAreaBackgroundBrush(QBrush(Qt::black));
  _chart->setPlotAreaBackgroundVisible(true);
  _chart->setBackgroundRoundness(0);

  if (_compact) {
    _chart->setMargins(QMargins(2, 2, 2, 2));
    if (_chart->layout()) {
      _chart->layout()->setContentsMargins(0, 0, 0, 0);
    }
    QFont titleFont = _chart->titleFont();
    titleFont.setPointSize(8);
    titleFont.setBold(true);
    _chart->setTitleFont(titleFont);
    _chart->setTitleBrush(QBrush(QColor(0, 255, 0)));
  } else {
    _chart->setMargins(QMargins(8, 8, 8, 8));
    QFont titleFont = _chart->titleFont();
    titleFont.setPointSize(11);
    titleFont.setBold(true);
    _chart->setTitleFont(titleFont);
    _chart->setTitleBrush(QBrush(QColor(0, 255, 0)));
  }

  _axis_x = new QValueAxis();
  _axis_x->setRange(1, 20);
  _axis_x->setTickCount(6);
  _axis_x->setLabelFormat("%d");
  _axis_x->setLabelsColor(QColor(180, 180, 180));
  _axis_x->setGridLineColor(QColor(40, 40, 40));
  _axis_x->setLinePenColor(QColor(100, 100, 100));
  if (_compact) {
    _axis_x->setLabelsVisible(false);
    _axis_x->setGridLineVisible(false);
    _axis_x->setLineVisible(false);
  } else {
    _axis_x->setTitleText("Snapshot");
    _axis_x->setTitleBrush(QBrush(QColor(180, 180, 180)));
  }

  _axis_y = new QValueAxis();
  _axis_y->setLabelFormat("%d");
  _axis_y->setLabelsColor(QColor(180, 180, 180));
  _axis_y->setGridLineColor(QColor(40, 40, 40));
  _axis_y->setLinePenColor(QColor(100, 100, 100));
  if (_compact) {
    _axis_y->setLabelsVisible(false);
    _axis_y->setGridLineVisible(false);
    _axis_y->setLineVisible(false);
  }

  _chart->addAxis(_axis_x, Qt::AlignBottom);
  _chart->addAxis(_axis_y, Qt::AlignLeft);

  _series_data = new QLineSeries();
  _series_data->setPen(
      QPen(QColor(0, 229, 255), _compact ? 1.5 : 2.0, Qt::SolidLine));

  _series_points = new QScatterSeries();
  _series_points->setMarkerShape(QScatterSeries::MarkerShapeCircle);
  _series_points->setMarkerSize(_compact ? 3.5 : 5.0);
  _series_points->setColor(QColor(0, 229, 255));
  _series_points->setBorderColor(QColor(0, 229, 255));

  _chart->addSeries(_series_data);
  _chart->addSeries(_series_points);

  for (auto* s :
       std::initializer_list<QAbstractSeries*>{_series_data, _series_points}) {
    s->attachAxis(_axis_x);
    s->attachAxis(_axis_y);
  }

  setChart(_chart);
  setRenderHint(QPainter::Antialiasing);
  setFrameShape(QFrame::NoFrame);
  setContentsMargins(0, 0, 0, 0);
}

QString HistoryChartView::itemName(int item_idx) {
  switch (item_idx) {
    case ITEM_CREDITS:
      return "Credits (Cr)";
    case ITEM_BANK_VALUE:
      return "Vault Value (Cr)";
    case ITEM_TOTAL_LEVEL:
      return "Total Skill Level";
    case ITEM_TOTAL_XP:
      return "Total Skill XP";
    case ITEM_HP:
      return "Player Hitpoints";
    default: {
      int s_idx = item_idx - ITEM_FIRST_SKILL;
      if (s_idx >= 0 && s_idx < SKILL_COUNT) {
        return QString::fromStdString(
            skill_name(static_cast<SkillType>(s_idx)) + " XP");
      }
      return "Credits (Cr)";
    }
  }
}

void HistoryChartView::setItemIndex(int idx) {
  int clamped = std::clamp(idx, 0, TOTAL_ITEMS - 1);
  if (_item_idx != clamped) {
    _item_idx = clamped;
    emit itemChanged(_item_idx);
  }
}

void HistoryChartView::updateChart(const GameState& gameState) {
  _series_data->clear();
  _series_points->clear();

  _chart->setTitle(itemName(_item_idx));

  std::vector<double> values;
  if (_item_idx == ITEM_CREDITS) {
    for (long long v : gameState.credits_history) values.push_back(v);
    values.push_back(gameState.credits);
  } else if (_item_idx == ITEM_BANK_VALUE) {
    for (long long v : gameState.bank_value_history) values.push_back(v);
    values.push_back(gameState.total_bank_value());
  } else if (_item_idx == ITEM_TOTAL_LEVEL) {
    for (int v : gameState.total_level_history) values.push_back(v);
    values.push_back(gameState.total_skill_level());
  } else if (_item_idx == ITEM_TOTAL_XP) {
    for (long long v : gameState.total_xp_history) values.push_back(v);
    values.push_back(gameState.total_skill_xp());
  } else if (_item_idx == ITEM_HP) {
    for (int v : gameState.hp_history) values.push_back(v);
    values.push_back(gameState.player_hp);
  } else {
    int s_idx = std::clamp(_item_idx - ITEM_FIRST_SKILL, 0, SKILL_COUNT - 1);
    for (long long v : gameState.skill_xp_history[s_idx]) values.push_back(v);
    values.push_back(gameState.xp[s_idx]);
  }

  int count = static_cast<int>(values.size());
  int max_x = std::max(10, count);
  _axis_x->setRange(1, max_x);

  double min_val = values.empty() ? 0.0 : values[0];
  double max_val = values.empty() ? 100.0 : values[0];
  for (int i = 0; i < count; ++i) {
    double v = values[i];
    _series_data->append(i + 1, v);
    _series_points->append(i + 1, v);
    if (v < min_val) min_val = v;
    if (v > max_val) max_val = v;
  }

  double span = std::max(10.0, max_val - min_val);
  double pad = std::max(2.0, span * 0.15);
  _axis_y->setRange(std::max(0.0, min_val - pad), max_val + pad);
}

void HistoryChartView::mousePressEvent(QMouseEvent* event) {
  if (_compact && event->button() == Qt::LeftButton) {
    setItemIndex((_item_idx + 1) % TOTAL_ITEMS);
    event->accept();
    return;
  }
  QChartView::mousePressEvent(event);
}

void HistoryChartView::contextMenuEvent(QContextMenuEvent* event) {
  QMenu menu(this);
  QAction* zoomAction = menu.addAction("Zoom in...");
  connect(zoomAction, &QAction::triggered, this,
           [this]() { emit zoomRequested(_item_idx); });
  menu.addSeparator();

  for (int i = 0; i < TOTAL_ITEMS; ++i) {
    if (i == ITEM_FIRST_SKILL) menu.addSeparator();
    QAction* act = menu.addAction(itemName(i));
    act->setCheckable(true);
    act->setChecked(i == _item_idx);
    connect(act, &QAction::triggered, this, [this, i]() { setItemIndex(i); });
  }
  menu.exec(event->globalPos());
}

// ============================================================================
// MainWindow Implementation
// ============================================================================

MainWindow::MainWindow(QWidget* parent) : QWidget(parent), _gameState() {
  _setupWidget();
  updateAllUi();
}

MainWindow::MainWindow(GameState game_state, QWidget* parent)
    : QWidget(parent), _gameState(std::move(game_state)) {
  _setupWidget();
  updateAllUi();
}

void MainWindow::_setupWidget() {
  setWindowTitle(QString::fromUtf8(kProgramName.data(), kProgramName.size()));

  _tick_timer = new QTimer(this);
  _tick_timer->setInterval(100);
  connect(_tick_timer, &QTimer::timeout, this, &MainWindow::onTickTimer);
  _tick_timer->start();

  QVBoxLayout* vbox_main = new QVBoxLayout(this);
  vbox_main->setContentsMargins(6, 6, 6, 6);
  vbox_main->setSpacing(6);

  // Top Activity Banner + Progress Bar + Event Log
  QGroupBox* frame_info =
      new QGroupBox("Active Protocol & Cyber-Log", this);
  QVBoxLayout* vbox_info = new QVBoxLayout(frame_info);
  vbox_info->setContentsMargins(6, 6, 6, 6);
  vbox_info->setSpacing(4);

  QHBoxLayout* hbox_banner = new QHBoxLayout();
  _label_active_banner = new QLabel(frame_info);
  QFont bannerFont = _label_active_banner->font();
  bannerFont.setBold(true);
  _label_active_banner->setFont(bannerFont);
  hbox_banner->addWidget(_label_active_banner, 1);

  _progressbar_action = new QProgressBar(frame_info);
  _progressbar_action->setRange(0, 100);
  _progressbar_action->setValue(0);
  _progressbar_action->setFixedWidth(260);
  hbox_banner->addWidget(_progressbar_action);
  vbox_info->addLayout(hbox_banner);

  _textview_information = new QTextEdit(frame_info);
  _textview_information->setReadOnly(true);
  _textview_information->setMinimumHeight(80);
  vbox_info->addWidget(_textview_information);
  vbox_main->addWidget(frame_info);

  // Main 3-column horizontal area
  QHBoxLayout* hbox_down = new QHBoxLayout();
  hbox_down->setSpacing(8);
  vbox_main->addLayout(hbox_down);

  // Left Column: Skills List + Skill Actions / Combat Tabs
  QVBoxLayout* vbox_left = new QVBoxLayout();
  vbox_left->setSpacing(6);
  hbox_down->addLayout(vbox_left, 3);

  QGroupBox* frame_skills = new QGroupBox("Neural Skills (Click to Select)", this);
  QVBoxLayout* vbox_skills = new QVBoxLayout(frame_skills);
  vbox_skills->setContentsMargins(5, 5, 5, 5);
  _treeview_skills = new QTreeWidget(frame_skills);
  _treeview_skills->setRootIsDecorated(false);
  _treeview_skills->setUniformRowHeights(true);
  _treeview_skills->setColumnCount(4);
  _treeview_skills->setHeaderLabels({"Skill", "Level", "XP", "Next Lv"});
  _treeview_skills->setColumnWidth(0, 130);
  _treeview_skills->setColumnWidth(1, 65);
  _treeview_skills->setColumnWidth(2, 100);
  _treeview_skills->setColumnWidth(3, 75);
  _treeview_skills->setMinimumHeight(210);
  connect(_treeview_skills, &QTreeWidget::itemSelectionChanged, this,
          &MainWindow::onSkillSelectionChanged);
  vbox_skills->addWidget(_treeview_skills);
  vbox_left->addWidget(frame_skills);

  _tabs_mode = new QTabWidget(this);
  vbox_left->addWidget(_tabs_mode, 1);

  // Tab 0: Non-Combat Skill Actions
  QWidget* tab_skills = new QWidget(_tabs_mode);
  QVBoxLayout* vbox_tab_skills = new QVBoxLayout(tab_skills);
  vbox_tab_skills->setContentsMargins(4, 4, 4, 4);
  _treeview_actions = new QTreeWidget(tab_skills);
  _treeview_actions->setRootIsDecorated(false);
  _treeview_actions->setUniformRowHeights(true);
  _treeview_actions->setColumnCount(6);
  _treeview_actions->setHeaderLabels(
      {"Lv", "Protocol / Schematic", "Cycle", "XP", "Mastery", "Inputs -> Output"});
  _treeview_actions->setColumnWidth(0, 38);
  _treeview_actions->setColumnWidth(1, 175);
  _treeview_actions->setColumnWidth(2, 55);
  _treeview_actions->setColumnWidth(3, 50);
  _treeview_actions->setColumnWidth(4, 65);
  _treeview_actions->setColumnWidth(5, 180);
  _treeview_actions->setMinimumSize(520, 220);
  connect(_treeview_actions, &QTreeWidget::itemDoubleClicked, this,
          &MainWindow::onActionDoubleClicked);
  vbox_tab_skills->addWidget(_treeview_actions);
  _tabs_mode->addTab(tab_skills, "Tech Protocols && Schematics");

  // Tab 1: Combat & Bounty
  QWidget* tab_combat = new QWidget(_tabs_mode);
  QVBoxLayout* vbox_tab_combat = new QVBoxLayout(tab_combat);
  vbox_tab_combat->setContentsMargins(4, 4, 4, 4);
  vbox_tab_combat->setSpacing(4);

  QHBoxLayout* hbox_combat_top = new QHBoxLayout();
  hbox_combat_top->addWidget(new QLabel("Mode:", tab_combat));
  _combo_attack_style = new QComboBox(tab_combat);
  _combo_attack_style->addItem("Precision (Trains Attack)");
  _combo_attack_style->addItem("Overdrive (Trains Strength)");
  _combo_attack_style->addItem("Evasive (Trains Defence)");
  connect(_combo_attack_style,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &MainWindow::onAttackStyleChanged);
  hbox_combat_top->addWidget(_combo_attack_style);

  _label_bounty_task = new QLabel(tab_combat);
  hbox_combat_top->addWidget(_label_bounty_task, 1);

  _button_bounty_task = new QPushButton("New Bounty Contract", tab_combat);
  connect(_button_bounty_task, &QPushButton::clicked, this,
          &MainWindow::slotNewBountyContract);
  hbox_combat_top->addWidget(_button_bounty_task);
  vbox_tab_combat->addLayout(hbox_combat_top);

  QHBoxLayout* hbox_mon_hp = new QHBoxLayout();
  hbox_mon_hp->addWidget(new QLabel("Hostile Integrity:", tab_combat));
  _progressbar_monster_hp = new QProgressBar(tab_combat);
  _progressbar_monster_hp->setRange(0, 100);
  _progressbar_monster_hp->setValue(100);
  hbox_mon_hp->addWidget(_progressbar_monster_hp, 1);
  vbox_tab_combat->addLayout(hbox_mon_hp);

  _treeview_monsters = new QTreeWidget(tab_combat);
  _treeview_monsters->setRootIsDecorated(false);
  _treeview_monsters->setUniformRowHeights(true);
  _treeview_monsters->setColumnCount(7);
  _treeview_monsters->setHeaderLabels(
      {"Lv", "Hostile Target", "Sector", "HP", "Max Hit", "Bounty", "Kills"});
  _treeview_monsters->setColumnWidth(0, 38);
  _treeview_monsters->setColumnWidth(1, 165);
  _treeview_monsters->setColumnWidth(2, 120);
  _treeview_monsters->setColumnWidth(3, 55);
  _treeview_monsters->setColumnWidth(4, 60);
  _treeview_monsters->setColumnWidth(5, 55);
  _treeview_monsters->setColumnWidth(6, 55);
  connect(_treeview_monsters, &QTreeWidget::itemDoubleClicked, this,
          &MainWindow::onMonsterDoubleClicked);
  vbox_tab_combat->addWidget(_treeview_monsters);
  _tabs_mode->addTab(tab_combat, "Combat && Bounty");

  // Middle Column: Action / Places / Time / Game Controls
  QVBoxLayout* vbox_middle = new QVBoxLayout();
  vbox_middle->setSpacing(5);
  hbox_down->addLayout(vbox_middle);

  QGroupBox* frame_action = new QGroupBox("Commands", this);
  QVBoxLayout* box_action = new QVBoxLayout(frame_action);
  box_action->setContentsMargins(5, 5, 5, 5);
  box_action->setSpacing(3);

  _button_start = new QPushButton("&Execute / Engage", frame_action);
  connect(_button_start, &QPushButton::clicked, this, &MainWindow::slotStart);
  box_action->addWidget(_button_start);

  _button_stop = new QPushButton("S&top Protocol", frame_action);
  connect(_button_stop, &QPushButton::clicked, this, &MainWindow::slotStop);
  box_action->addWidget(_button_stop);

  _button_eat = new QPushButton("&Use Stim / Ration", frame_action);
  connect(_button_eat, &QPushButton::clicked, this, &MainWindow::slotEat);
  box_action->addWidget(_button_eat);

  _button_equip = new QPushButton("E&quip Selected", frame_action);
  connect(_button_equip, &QPushButton::clicked, this, &MainWindow::slotEquip);
  box_action->addWidget(_button_equip);

  _button_sell = new QPushButton("Se&ll Selected", frame_action);
  connect(_button_sell, &QPushButton::clicked, this, &MainWindow::slotSell);
  box_action->addWidget(_button_sell);

  _button_sell_all = new QPushButton("Sell &All Vault", frame_action);
  connect(_button_sell_all, &QPushButton::clicked, this,
          &MainWindow::slotSellAll);
  box_action->addWidget(_button_sell_all);
  vbox_middle->addWidget(frame_action);

  QGroupBox* frame_places = new QGroupBox("Neo-Sector & Info", this);
  QVBoxLayout* box_places = new QVBoxLayout(frame_places);
  box_places->setContentsMargins(5, 5, 5, 5);
  box_places->setSpacing(3);

  _button_shop = new QPushButton("&Cyber-Shop && Tools...", frame_places);
  connect(_button_shop, &QPushButton::clicked, this, &MainWindow::slotShop);
  box_places->addWidget(_button_shop);

  _button_equipment = new QPushButton("Cyberware && &Stats...", frame_places);
  connect(_button_equipment, &QPushButton::clicked, this,
          &MainWindow::slotEquipment);
  box_places->addWidget(_button_equipment);

  _button_bestiary = new QPushButton("&Hostiles && Drops...", frame_places);
  connect(_button_bestiary, &QPushButton::clicked, this,
          &MainWindow::slotBestiary);
  box_places->addWidget(_button_bestiary);

  _button_history = new QPushButton("Chart &History...", frame_places);
  connect(_button_history, &QPushButton::clicked, this,
          &MainWindow::slotHistory);
  box_places->addWidget(_button_history);
  vbox_middle->addWidget(frame_places);

  QGroupBox* frame_time = new QGroupBox("Time & Neural Save", this);
  QVBoxLayout* box_time = new QVBoxLayout(frame_time);
  box_time->setContentsMargins(5, 5, 5, 5);
  box_time->setSpacing(3);

  _button_ff1m = new QPushButton("Fast-Forward +&1m", frame_time);
  connect(_button_ff1m, &QPushButton::clicked, this,
          &MainWindow::slotFastForward1m);
  box_time->addWidget(_button_ff1m);

  _button_ff10m = new QPushButton("Fast-Forward +1&0m", frame_time);
  connect(_button_ff10m, &QPushButton::clicked, this,
          &MainWindow::slotFastForward10m);
  box_time->addWidget(_button_ff10m);

  _button_save = new QPushButton("Sa&ve State", frame_time);
  connect(_button_save, &QPushButton::clicked, this, &MainWindow::slotSave);
  box_time->addWidget(_button_save);

  _button_load = new QPushButton("&Load State", frame_time);
  connect(_button_load, &QPushButton::clicked, this, &MainWindow::slotLoad);
  box_time->addWidget(_button_load);
  vbox_middle->addWidget(frame_time);

  QGroupBox* frame_game = new QGroupBox("System", this);
  QVBoxLayout* box_game = new QVBoxLayout(frame_game);
  box_game->setContentsMargins(5, 5, 5, 5);
  box_game->setSpacing(3);

  _checkbutton_sound = new QCheckBox("Sou&nd", frame_game);
  _checkbutton_sound->setChecked(_gameState.sound_enabled);
  connect(_checkbutton_sound, &QCheckBox::toggled, this, [this](bool checked) {
    _gameState.sound_enabled = checked;
  });
  box_game->addWidget(_checkbutton_sound);

  _button_about = new QPushButton("&About", frame_game);
  connect(_button_about, &QPushButton::clicked, this, &MainWindow::slotAbout);
  box_game->addWidget(_button_about);

  _button_docs = new QPushButton("Docs", frame_game);
  connect(_button_docs, &QPushButton::clicked, this, &MainWindow::slotDocs);
  box_game->addWidget(_button_docs);

  _button_highscores = new QPushButton("Milestones", frame_game);
  connect(_button_highscores, &QPushButton::clicked, this,
          &MainWindow::slotHighscores);
  box_game->addWidget(_button_highscores);

  _button_newgame = new QPushButton("&New Game", frame_game);
  connect(_button_newgame, &QPushButton::clicked, this,
          &MainWindow::slotNewGame);
  box_game->addWidget(_button_newgame);
  vbox_middle->addWidget(frame_game);
  vbox_middle->addStretch();

  // Right Column: Cyber-Vault Inventory + Player Status & History Chart
  QVBoxLayout* vbox_right = new QVBoxLayout();
  vbox_right->setSpacing(5);
  hbox_down->addLayout(vbox_right, 2);

  _group_bank = new QGroupBox("Cyber-Vault Inventory", this);
  QVBoxLayout* vbox_bank = new QVBoxLayout(_group_bank);
  vbox_bank->setContentsMargins(5, 5, 5, 5);
  _treeview_bank = new QTreeWidget(_group_bank);
  _treeview_bank->setRootIsDecorated(false);
  _treeview_bank->setUniformRowHeights(true);
  _treeview_bank->setColumnCount(4);
  _treeview_bank->setHeaderLabels({"Item", "Category", "Qty", "Value / Specs"});
  _treeview_bank->setColumnWidth(0, 145);
  _treeview_bank->setColumnWidth(1, 70);
  _treeview_bank->setColumnWidth(2, 45);
  _treeview_bank->setColumnWidth(3, 110);
  _treeview_bank->setMinimumSize(375, 200);
  connect(_treeview_bank, &QTreeWidget::itemDoubleClicked, this,
          &MainWindow::onBankDoubleClicked);
  vbox_bank->addWidget(_treeview_bank);
  vbox_right->addWidget(_group_bank, 1);

  QGroupBox* frame_status = new QGroupBox("Operative Status", this);
  QVBoxLayout* vbox_status = new QVBoxLayout(frame_status);
  vbox_status->setContentsMargins(5, 5, 5, 5);
  vbox_status->setSpacing(4);

  QGridLayout* grid_info = new QGridLayout();
  grid_info->setHorizontalSpacing(10);
  grid_info->setVerticalSpacing(3);

  grid_info->addWidget(new QLabel("Credits:", frame_status), 0, 0);
  _label_credits = new QLabel(frame_status);
  grid_info->addWidget(_label_credits, 0, 1);

  grid_info->addWidget(new QLabel("Bounty Tokens:", frame_status), 1, 0);
  _label_bounty_tokens = new QLabel(frame_status);
  grid_info->addWidget(_label_bounty_tokens, 1, 1);

  grid_info->addWidget(new QLabel("Combat / Total Lv:", frame_status), 2, 0);
  _label_combat_lvl = new QLabel(frame_status);
  grid_info->addWidget(_label_combat_lvl, 2, 1);

  grid_info->addWidget(new QLabel("Cyber-Tools:", frame_status), 3, 0);
  _label_tools = new QLabel(frame_status);
  grid_info->addWidget(_label_tools, 3, 1);

  grid_info->addWidget(new QLabel("Weapon:", frame_status), 4, 0);
  _label_equipped_weapon = new QLabel(frame_status);
  grid_info->addWidget(_label_equipped_weapon, 4, 1);

  grid_info->addWidget(new QLabel("Armor (DR):", frame_status), 5, 0);
  _label_equipped_armor = new QLabel(frame_status);
  grid_info->addWidget(_label_equipped_armor, 5, 1);

  grid_info->addWidget(new QLabel("Loaded Stim:", frame_status), 6, 0);
  _label_equipped_food = new QLabel(frame_status);
  grid_info->addWidget(_label_equipped_food, 6, 1);

  grid_info->addWidget(new QLabel("Hitpoints:", frame_status), 7, 0);
  _progressbar_hp = new QProgressBar(frame_status);
  _progressbar_hp->setRange(0, 100);
  grid_info->addWidget(_progressbar_hp, 7, 1);

  vbox_status->addLayout(grid_info);

  _drawingarea_status = new HistoryChartView(true, frame_status);
  _drawingarea_status->setMinimumSize(250, 95);
  connect(_drawingarea_status, &HistoryChartView::itemChanged, this,
          [this](int) { _drawingarea_status->updateChart(_gameState); });
  connect(_drawingarea_status, &HistoryChartView::zoomRequested, this,
          &MainWindow::onStatusZoomRequested);
  vbox_status->addWidget(_drawingarea_status);
  vbox_right->addWidget(frame_status);

  _shortcut_quit = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q), this);
  connect(_shortcut_quit, &QShortcut::activated, qApp, &QApplication::quit);
}

void MainWindow::setSelectedSkill(SkillType skill) {
  _selected_skill = skill;
  if (static_cast<int>(skill) >= NON_COMBAT_SKILL_COUNT) {
    if (_tabs_mode) _tabs_mode->setCurrentIndex(1);
  } else {
    if (_tabs_mode) _tabs_mode->setCurrentIndex(0);
    _fillTreeviewActions();
  }
}

int MainWindow::selectedActionId() const {
  if (!_treeview_actions) return -1;
  auto* item = _treeview_actions->currentItem();
  if (!item) return -1;
  return item->data(0, Qt::UserRole).toInt();
}

int MainWindow::selectedMonsterId() const {
  if (!_treeview_monsters) return -1;
  auto* item = _treeview_monsters->currentItem();
  if (!item) return -1;
  return item->data(0, Qt::UserRole).toInt();
}

ItemId MainWindow::selectedBankItemId() const {
  if (!_treeview_bank) return ItemId::None;
  auto* item = _treeview_bank->currentItem();
  if (!item) return ItemId::None;
  return static_cast<ItemId>(item->data(0, Qt::UserRole).toInt());
}

void MainWindow::updateLiveProgressOnly() {
  if (_label_active_banner) {
    _label_active_banner->setText(
        QString::fromStdString(_gameState.status_banner));
  }
  if (_progressbar_action) {
    if (_gameState.active_type == ActiveActivityType::Skill) {
      int pct = (_gameState.active_target_ms > 0)
                    ? (_gameState.active_progress_ms * 100) /
                          _gameState.active_target_ms
                    : 0;
      _progressbar_action->setValue(std::clamp(pct, 0, 100));
      _progressbar_action->setFormat(
          QString("%1% (%2s)")
              .arg(pct)
              .arg(_gameState.active_target_ms / 1000.0, 0, 'f', 1));
    } else if (_gameState.active_type == ActiveActivityType::Combat) {
      int plr_int = _gameState.player_attack_interval_ms();
      int pct =
          (plr_int > 0) ? (_gameState.player_attack_timer_ms * 100) / plr_int : 0;
      _progressbar_action->setValue(std::clamp(pct, 0, 100));
      _progressbar_action->setFormat(QString("Weapon Cycle: %1%").arg(pct));
    } else {
      _progressbar_action->setValue(0);
      _progressbar_action->setFormat("Standby");
    }
  }

  if (_progressbar_hp) {
    int mhp = std::max(1, _gameState.max_hp());
    _progressbar_hp->setRange(0, mhp);
    _progressbar_hp->setValue(std::clamp(_gameState.player_hp, 0, mhp));
    _progressbar_hp->setFormat(
        QString("%1 / %2 HP").arg(_gameState.player_hp).arg(mhp));
  }

  if (_progressbar_monster_hp) {
    int mon_id = _gameState.active_monster_id;
    int mhp = (mon_id >= 0 && mon_id < MONSTER_COUNT)
                  ? monster_info[mon_id].max_hp
                  : 100;
    int chp = (_gameState.active_type == ActiveActivityType::Combat)
                  ? _gameState.monster_hp
                  : mhp;
    _progressbar_monster_hp->setRange(0, mhp);
    _progressbar_monster_hp->setValue(std::clamp(chp, 0, mhp));
    _progressbar_monster_hp->setFormat(
        QString("%1: %2 / %3 HP")
            .arg(monster_info[std::clamp(mon_id, 0, MONSTER_COUNT - 1)].name)
            .arg(chp)
            .arg(mhp));
  }
}

void MainWindow::updateAllUi() {
  updateLiveProgressOnly();

  if (_label_credits) {
    _label_credits->setText(
        QString::fromStdString(money_string(_gameState.credits)));
  }
  if (_label_bounty_tokens) {
    _label_bounty_tokens->setText(QString::fromStdString(
        number_string(_gameState.bounty_tokens) + " BT"));
  }
  if (_label_combat_lvl) {
    _label_combat_lvl->setText(
        QString("Lv %1  (Total: %2 / %3)")
            .arg(_gameState.combat_level())
            .arg(_gameState.total_skill_level())
            .arg(SKILL_COUNT * MAX_SKILL_LEVEL));
  }
  if (_label_tools) {
    _label_tools->setText(
        QString("Cut T%1 | Bio T%2 | Drill T%3 | Core T%4")
            .arg(_gameState.cutter_tier + 1)
            .arg(_gameState.harvester_tier + 1)
            .arg(_gameState.drill_tier + 1)
            .arg(_gameState.reactor_tier + 1));
  }
  if (_label_equipped_weapon) {
    ItemId w_id =
        _gameState.equipped_items[static_cast<int>(EquipSlot::Weapon)];
    _label_equipped_weapon->setText(
        is_valid_item(w_id) ? QString("%1 (Max Hit: %2)")
                                  .arg(get_item_info(w_id).name)
                                  .arg(_gameState.player_max_hit())
                            : QString("Unarmed (Max Hit: %1)")
                                  .arg(_gameState.player_max_hit()));
  }
  if (_label_equipped_armor) {
    _label_equipped_armor->setText(
        QString("DR: %1% | Evasion: %2 | Auto-Stim: Mk %3")
            .arg(_gameState.player_damage_reduction())
            .arg(_gameState.player_evasion())
            .arg(_gameState.auto_stim_tier));
  }
  if (_label_equipped_food) {
    if (is_valid_item(_gameState.equipped_food_item) &&
        _gameState.equipped_food_qty > 0) {
      const auto& fi = get_item_info(_gameState.equipped_food_item);
      _label_equipped_food->setText(QString("%1x %2 (+%3 HP)")
                                        .arg(_gameState.equipped_food_qty)
                                        .arg(fi.name)
                                        .arg(fi.heal_amount));
    } else {
      _label_equipped_food->setText("None");
    }
  }
  if (_label_bounty_task) {
    const auto& mon = monster_info[_gameState.bounty_target_id];
    _label_bounty_task->setText(
        QString("Bounty: %1x %2")
            .arg(_gameState.bounty_remaining)
            .arg(mon.name));
  }
  if (_group_bank) {
    _group_bank->setTitle(
        QString("Cyber-Vault (%1/%2 slots — Value: %3)")
            .arg(_gameState.used_bank_slots())
            .arg(_gameState.bank_capacity)
            .arg(QString::fromStdString(
                money_string(_gameState.total_bank_value()))));
  }

  _fillTreeviewSkills();
  _fillTreeviewActions();
  _fillTreeviewMonsters();
  _fillTreeviewBank();

  if (_drawingarea_status) {
    _drawingarea_status->updateChart(_gameState);
  }

  if (_textview_information) {
    QString log_text;
    int start = std::max(0, static_cast<int>(_gameState.game_log.size()) - 15);
    for (int i = start; i < static_cast<int>(_gameState.game_log.size()); ++i) {
      if (!log_text.isEmpty()) log_text += "\n";
      log_text += QString::fromStdString(_gameState.game_log[i]);
    }
    _textview_information->setPlainText(log_text);
    if (auto* sb = _textview_information->verticalScrollBar()) {
      sb->setValue(sb->maximum());
    }
  }
}

void MainWindow::_fillTreeviewSkills() {
  if (!_treeview_skills) return;
  _treeview_skills->blockSignals(true);
  _treeview_skills->clear();

  for (int i = 0; i < SKILL_COUNT; ++i) {
    auto sk = static_cast<SkillType>(i);
    int lvl = _gameState.skill_level(sk);
    long long s_xp = _gameState.skill_xp(sk);
    double prog = level_progress_ratio(s_xp) * 100.0;

    auto* item = new QTreeWidgetItem(_treeview_skills);
    QString name_str = QString::fromStdString(skill_name(sk));
    if (_gameState.active_type == ActiveActivityType::Skill &&
        _gameState.active_action_id >= 0 &&
        skill_actions[_gameState.active_action_id].skill == sk) {
      name_str = "▶ " + name_str;
    } else if (_gameState.active_type == ActiveActivityType::Combat &&
               i >= NON_COMBAT_SKILL_COUNT) {
      name_str = "⚔ " + name_str;
    }
    item->setText(0, name_str);
    item->setText(1, QString("%1 / 99").arg(lvl));
    item->setText(2, QString::fromStdString(number_string(s_xp)));
    item->setText(3, QString("%1%").arg(prog, 0, 'f', 0));
    item->setData(0, Qt::UserRole, i);
    item->setTextAlignment(1, Qt::AlignRight | Qt::AlignVCenter);
    item->setTextAlignment(2, Qt::AlignRight | Qt::AlignVCenter);
    item->setTextAlignment(3, Qt::AlignRight | Qt::AlignVCenter);

    if (sk == _selected_skill) {
      _treeview_skills->setCurrentItem(item);
    }
  }
  _treeview_skills->blockSignals(false);
}

void MainWindow::_fillTreeviewActions() {
  if (!_treeview_actions) return;
  int prev_act_id = selectedActionId();
  _treeview_actions->clear();

  SkillType sk = _selected_skill;
  if (static_cast<int>(sk) >= NON_COMBAT_SKILL_COUNT) {
    sk = SkillType::Salvaging;
  }

  auto act_ids = actions_for_skill(sk);
  QTreeWidgetItem* to_select = nullptr;

  for (int id : act_ids) {
    const auto& act = skill_actions[id];
    auto* item = new QTreeWidgetItem(_treeview_actions);
    int eff_ms = _gameState.action_effective_interval_ms(id);
    int m_lvl = _gameState.mastery_level(id);

    QString act_name = QString::fromUtf8(act.name);
    if (_gameState.active_type == ActiveActivityType::Skill &&
        _gameState.active_action_id == id) {
      act_name = "▶ " + act_name;
    }

    QString io_str;
    if (is_valid_item(act.input_item_1) && act.input_item_1 != ItemId::None) {
      io_str += QString("%1x %2")
                    .arg(act.input_qty_1)
                    .arg(get_item_info(act.input_item_1).name);
    }
    if (is_valid_item(act.input_item_2) && act.input_item_2 != ItemId::None) {
      io_str += QString(" + %1x %2")
                    .arg(act.input_qty_2)
                    .arg(get_item_info(act.input_item_2).name);
    }
    if (is_valid_item(act.product_item) && act.product_item != ItemId::None) {
      if (!io_str.isEmpty()) io_str += " -> ";
      io_str += QString("%1x %2")
                    .arg(act.product_qty)
                    .arg(get_item_info(act.product_item).name);
    } else if (io_str.isEmpty()) {
      io_str = "XP + Carbon Cell + Cr";
    }

    item->setText(0, QString::number(act.req_level));
    item->setText(1, act_name);
    item->setText(2, QString("%1s").arg(eff_ms / 1000.0, 0, 'f', 2));
    item->setText(3, QString::number(act.xp));
    item->setText(4, QString("Lv %1").arg(m_lvl));
    item->setText(5, io_str);
    item->setData(0, Qt::UserRole, id);

    if (_gameState.skill_level(sk) < act.req_level) {
      for (int c = 0; c < 6; ++c) {
        item->setForeground(c, QBrush(QColor(140, 140, 140)));
      }
    }

    if (id == prev_act_id || (!to_select && id == _gameState.active_action_id)) {
      to_select = item;
    }
  }

  if (!to_select && _treeview_actions->topLevelItemCount() > 0) {
    to_select = _treeview_actions->topLevelItem(0);
  }
  if (to_select) {
    _treeview_actions->setCurrentItem(to_select);
  }
}

void MainWindow::_fillTreeviewMonsters() {
  if (!_treeview_monsters) return;
  int prev_mon_id = selectedMonsterId();
  _treeview_monsters->clear();

  QTreeWidgetItem* to_select = nullptr;
  for (int i = 0; i < MONSTER_COUNT; ++i) {
    const auto& mon = monster_info[i];
    auto* item = new QTreeWidgetItem(_treeview_monsters);

    QString m_name = QString::fromUtf8(mon.name);
    if (_gameState.active_type == ActiveActivityType::Combat &&
        _gameState.active_monster_id == i) {
      m_name = "⚔ " + m_name;
    } else if (_gameState.bounty_target_id == i) {
      m_name = "★ " + m_name;
    }

    item->setText(0, QString::number(mon.combat_level));
    item->setText(1, m_name);
    item->setText(2, QString::fromUtf8(mon.zone_name));
    item->setText(3, QString::number(mon.max_hp));
    item->setText(4, QString::number(mon.max_hit));
    item->setText(5, QString("Lv %1").arg(mon.bounty_req));
    item->setText(6, QString::number(_gameState.monster_kills[i]));
    item->setData(0, Qt::UserRole, i);

    if (i == prev_mon_id ||
        (!to_select && i == _gameState.active_monster_id)) {
      to_select = item;
    }
  }
  if (!to_select && _treeview_monsters->topLevelItemCount() > 0) {
    to_select = _treeview_monsters->topLevelItem(0);
  }
  if (to_select) {
    _treeview_monsters->setCurrentItem(to_select);
  }
}

void MainWindow::_fillTreeviewBank() {
  if (!_treeview_bank) return;
  ItemId prev_item_id = selectedBankItemId();
  _treeview_bank->clear();

  QTreeWidgetItem* to_select = nullptr;
  for (const auto& slot : _gameState.bank) {
    if (!is_valid_item(slot.item_id) || slot.qty <= 0)
      continue;
    const auto& info = get_item_info(slot.item_id);
    auto* item = new QTreeWidgetItem(_treeview_bank);

    QString extra = QString::fromStdString(
        money_string(static_cast<long long>(slot.qty) * info.price));
    if (info.heal_amount > 0) {
      extra += QString(" (+%1 HP)").arg(info.heal_amount);
    } else if (info.equip_slot == EquipSlot::Weapon) {
      extra += QString(" (+%1 Str)").arg(info.strength_bonus);
    } else if (info.equip_slot != EquipSlot::None) {
      extra += QString(" (%1% DR)").arg(info.damage_reduction);
    }

    item->setText(0, QString::fromUtf8(info.name));
    item->setText(1, QString::fromStdString(item_category_name(info.category)));
    item->setText(2, QString::number(slot.qty));
    item->setText(3, extra);
    item->setData(0, Qt::UserRole, static_cast<int>(slot.item_id));
    item->setTextAlignment(2, Qt::AlignRight | Qt::AlignVCenter);

    if (slot.item_id == prev_item_id) {
      to_select = item;
    }
  }
  if (!to_select && _treeview_bank->topLevelItemCount() > 0) {
    to_select = _treeview_bank->topLevelItem(0);
  }
  if (to_select) {
    _treeview_bank->setCurrentItem(to_select);
  }
}

void MainWindow::onTickTimer() {
  long long prev_xp = _gameState.total_skill_xp();
  long long prev_kills = _gameState.total_monsters_killed;
  std::size_t prev_logs = _gameState.game_log.size();

  _gameState.tick(100);

  if (_gameState.total_skill_xp() != prev_xp ||
      _gameState.total_monsters_killed != prev_kills ||
      _gameState.game_log.size() != prev_logs) {
    updateAllUi();
  } else {
    updateLiveProgressOnly();
  }
}

void MainWindow::onSkillSelectionChanged() {
  if (!_treeview_skills) return;
  auto* item = _treeview_skills->currentItem();
  if (!item) return;
  int sk_idx = item->data(0, Qt::UserRole).toInt();
  setSelectedSkill(static_cast<SkillType>(std::clamp(sk_idx, 0, SKILL_COUNT - 1)));
}

void MainWindow::onActionDoubleClicked() {
  int act_id = selectedActionId();
  if (act_id >= 0) {
    _gameState.start_skill_action(act_id);
    updateAllUi();
  }
}

void MainWindow::onMonsterDoubleClicked() {
  int mon_id = selectedMonsterId();
  if (mon_id >= 0) {
    _gameState.start_combat(mon_id);
    updateAllUi();
  }
}

void MainWindow::onBankDoubleClicked() {
  window_main_button_equip_clicked_cb(*this);
}

void MainWindow::onAttackStyleChanged(int idx) {
  _gameState.attack_style = static_cast<CombatStyle>(std::clamp(idx, 0, 2));
  updateAllUi();
}

void MainWindow::onStatusZoomRequested(int item_idx) {
  WindowHistory dlg(_gameState, item_idx, this);
  dlg.exec();
}

void MainWindow::slotNewBountyContract() {
  _gameState.assign_new_bounty_contract();
  updateAllUi();
}

void MainWindow::slotStart() { window_main_button_start_clicked_cb(*this); }
void MainWindow::slotStop() { window_main_button_stop_clicked_cb(*this); }
void MainWindow::slotEat() { window_main_button_eat_clicked_cb(*this); }
void MainWindow::slotEquip() { window_main_button_equip_clicked_cb(*this); }
void MainWindow::slotSell() { window_main_button_sell_clicked_cb(*this); }
void MainWindow::slotSellAll() {
  window_main_button_sell_all_clicked_cb(*this);
}
void MainWindow::slotShop() { window_main_button_shop_clicked_cb(*this); }
void MainWindow::slotEquipment() {
  window_main_button_equipment_clicked_cb(*this);
}
void MainWindow::slotBestiary() {
  window_main_button_bestiary_clicked_cb(*this);
}
void MainWindow::slotHistory() { window_main_button_history_clicked_cb(*this); }
void MainWindow::slotFastForward1m() {
  window_main_button_ff1m_clicked_cb(*this);
}
void MainWindow::slotFastForward10m() {
  window_main_button_ff10m_clicked_cb(*this);
}
void MainWindow::slotSave() { window_main_button_save_clicked_cb(*this); }
void MainWindow::slotLoad() { window_main_button_load_clicked_cb(*this); }
void MainWindow::slotAbout() { window_main_button_about_clicked_cb(*this); }
void MainWindow::slotDocs() { window_main_button_docs_clicked_cb(*this); }
void MainWindow::slotHighscores() {
  window_main_button_highscores_clicked_cb(*this);
}
void MainWindow::slotNewGame() { window_main_button_newgame_clicked_cb(*this); }

// ============================================================================
// WindowShop Implementation
// ============================================================================

WindowShop::WindowShop(GameState& gameState, QWidget* parent)
    : QDialog(parent), _gameState(gameState) {
  _setupWidget();
  updateShopUi();
}

void WindowShop::_setupWidget() {
  setWindowTitle("Cyber-Shop & Tool Upgrades");
  setMinimumWidth(580);

  QVBoxLayout* vbox = new QVBoxLayout(this);
  _label_credits = new QLabel(this);
  QFont f = _label_credits->font();
  f.setBold(true);
  _label_credits->setFont(f);
  vbox->addWidget(_label_credits);

  QGridLayout* grid = new QGridLayout();
  grid->setHorizontalSpacing(12);
  grid->setVerticalSpacing(8);

  _label_cutter = new QLabel(this);
  _btn_cutter = new QPushButton("Upgrade Cutter", this);
  connect(_btn_cutter, &QPushButton::clicked, this, &WindowShop::onBuyCutter);
  grid->addWidget(new QLabel("<b>Salvaging Cutter:</b>", this), 0, 0);
  grid->addWidget(_label_cutter, 0, 1);
  grid->addWidget(_btn_cutter, 0, 2);

  _label_harvester = new QLabel(this);
  _btn_harvester = new QPushButton("Upgrade Harvester", this);
  connect(_btn_harvester, &QPushButton::clicked, this,
          &WindowShop::onBuyHarvester);
  grid->addWidget(new QLabel("<b>Bio-Harvester:</b>", this), 1, 0);
  grid->addWidget(_label_harvester, 1, 1);
  grid->addWidget(_btn_harvester, 1, 2);

  _label_drill = new QLabel(this);
  _btn_drill = new QPushButton("Upgrade Drill", this);
  connect(_btn_drill, &QPushButton::clicked, this, &WindowShop::onBuyDrill);
  grid->addWidget(new QLabel("<b>Mining Drill:</b>", this), 2, 0);
  grid->addWidget(_label_drill, 2, 1);
  grid->addWidget(_btn_drill, 2, 2);

  _label_reactor = new QLabel(this);
  _btn_reactor = new QPushButton("Upgrade Reactor", this);
  connect(_btn_reactor, &QPushButton::clicked, this, &WindowShop::onBuyReactor);
  grid->addWidget(new QLabel("<b>Synth-Reactor:</b>", this), 3, 0);
  grid->addWidget(_label_reactor, 3, 1);
  grid->addWidget(_btn_reactor, 3, 2);

  _label_autostim = new QLabel(this);
  _btn_autostim = new QPushButton("Upgrade Auto-Stim", this);
  connect(_btn_autostim, &QPushButton::clicked, this,
          &WindowShop::onBuyAutoStim);
  grid->addWidget(new QLabel("<b>Auto-Stim:</b>", this), 4, 0);
  grid->addWidget(_label_autostim, 4, 1);
  grid->addWidget(_btn_autostim, 4, 2);

  _label_bank = new QLabel(this);
  _btn_bank = new QPushButton("Buy +4 Slots", this);
  connect(_btn_bank, &QPushButton::clicked, this, &WindowShop::onBuyBankSlot);
  grid->addWidget(new QLabel("<b>Vault Space:</b>", this), 5, 0);
  grid->addWidget(_label_bank, 5, 1);
  grid->addWidget(_btn_bank, 5, 2);

  vbox->addLayout(grid);

  QPushButton* btn_close = new QPushButton("Done", this);
  connect(btn_close, &QPushButton::clicked, this, &QDialog::accept);
  vbox->addWidget(btn_close);
}

void WindowShop::updateShopUi() {
  _label_credits->setText(
      QString("Available Credits: %1")
          .arg(QString::fromStdString(money_string(_gameState.credits))));

  auto format_tool = [](const auto& arr, int tier) {
    const auto& cur = arr[tier];
    if (tier + 1 >= static_cast<int>(arr.size())) {
      return QString("%1 (MAX TIER)").arg(cur.name);
    }
    const auto& nxt = arr[tier + 1];
    return QString("%1 -> Next: %2 (Lv %3, %4 Cr)")
        .arg(cur.name)
        .arg(nxt.name)
        .arg(nxt.req_skill_level)
        .arg(nxt.cost_credits);
  };

  _label_cutter->setText(format_tool(cutter_upgrades, _gameState.cutter_tier));
  _btn_cutter->setEnabled(_gameState.cutter_tier + 1 < TOOL_TIER_COUNT);

  _label_harvester->setText(
      format_tool(harvester_upgrades, _gameState.harvester_tier));
  _btn_harvester->setEnabled(_gameState.harvester_tier + 1 < TOOL_TIER_COUNT);

  _label_drill->setText(format_tool(drill_upgrades, _gameState.drill_tier));
  _btn_drill->setEnabled(_gameState.drill_tier + 1 < TOOL_TIER_COUNT);

  _label_reactor->setText(
      format_tool(reactor_upgrades, _gameState.reactor_tier));
  _btn_reactor->setEnabled(_gameState.reactor_tier + 1 < TOOL_TIER_COUNT);

  _label_autostim->setText(
      format_tool(auto_stim_upgrades, _gameState.auto_stim_tier));
  _btn_autostim->setEnabled(_gameState.auto_stim_tier + 1 <
                            AUTO_STIM_TIER_COUNT);

  _label_bank->setText(
      QString("%1 Slots -> +4 Slots for %2")
          .arg(_gameState.bank_capacity)
          .arg(QString::fromStdString(
               money_string(_gameState.next_bank_slot_cost()))));
}

void WindowShop::onBuyCutter() {
  _gameState.buy_cutter_upgrade();
  updateShopUi();
  emit stateChanged();
}

void WindowShop::onBuyHarvester() {
  _gameState.buy_harvester_upgrade();
  updateShopUi();
  emit stateChanged();
}

void WindowShop::onBuyDrill() {
  _gameState.buy_drill_upgrade();
  updateShopUi();
  emit stateChanged();
}

void WindowShop::onBuyReactor() {
  _gameState.buy_reactor_upgrade();
  updateShopUi();
  emit stateChanged();
}

void WindowShop::onBuyAutoStim() {
  _gameState.buy_auto_stim_upgrade();
  updateShopUi();
  emit stateChanged();
}

void WindowShop::onBuyBankSlot() {
  _gameState.buy_bank_slot();
  updateShopUi();
  emit stateChanged();
}

// ============================================================================
// WindowEquipment Implementation
// ============================================================================

WindowEquipment::WindowEquipment(GameState& gameState, QWidget* parent)
    : QDialog(parent), _gameState(gameState) {
  _setupWidget();
  updateEquipmentUi();
}

void WindowEquipment::_setupWidget() {
  setWindowTitle("Cyberware Loadout & Combat Stats");
  setMinimumWidth(460);

  QVBoxLayout* vbox = new QVBoxLayout(this);
  QGroupBox* grp_gear = new QGroupBox("Installed Cyberware & Gear", this);
  QGridLayout* grid = new QGridLayout(grp_gear);

  for (int i = 0; i < EQUIP_SLOT_COUNT; ++i) {
    auto slot = static_cast<EquipSlot>(i);
    grid->addWidget(
        new QLabel(
            QString("<b>%1:</b>")
                .arg(QString::fromStdString(equip_slot_name(slot))),
            grp_gear),
        i, 0);
    _slot_labels[i] = new QLabel(grp_gear);
    grid->addWidget(_slot_labels[i], i, 1);
    _slot_buttons[i] = new QPushButton("Unequip", grp_gear);
    connect(_slot_buttons[i], &QPushButton::clicked, this,
            [this, i]() { onUnequipSlot(i); });
    grid->addWidget(_slot_buttons[i], i, 2);
  }

  grid->addWidget(new QLabel("<b>Stim-Injector:</b>", grp_gear),
                  EQUIP_SLOT_COUNT, 0);
  _label_food = new QLabel(grp_gear);
  grid->addWidget(_label_food, EQUIP_SLOT_COUNT, 1, 1, 2);
  vbox->addWidget(grp_gear);

  QGroupBox* grp_stats = new QGroupBox("Effective Combat Telemetry", this);
  QVBoxLayout* vbox_stats = new QVBoxLayout(grp_stats);
  _label_stats = new QLabel(grp_stats);
  vbox_stats->addWidget(_label_stats);
  vbox->addWidget(grp_stats);

  QPushButton* btn_close = new QPushButton("Close", this);
  connect(btn_close, &QPushButton::clicked, this, &QDialog::accept);
  vbox->addWidget(btn_close);
}

void WindowEquipment::updateEquipmentUi() {
  for (int i = 0; i < EQUIP_SLOT_COUNT; ++i) {
    ItemId id = _gameState.equipped_items[i];
    if (is_valid_item(id)) {
      const auto& info = get_item_info(id);
      _slot_labels[i]->setText(
          QString("%1 (+%2 Atk, +%3 Str, +%4 Def, %5% DR)")
              .arg(info.name)
              .arg(info.attack_bonus)
              .arg(info.strength_bonus)
              .arg(info.defence_bonus)
              .arg(info.damage_reduction));
      _slot_buttons[i]->setEnabled(true);
    } else {
      _slot_labels[i]->setText("Empty");
      _slot_buttons[i]->setEnabled(false);
    }
  }

  if (is_valid_item(_gameState.equipped_food_item) &&
      _gameState.equipped_food_qty > 0) {
    const auto& fi = get_item_info(_gameState.equipped_food_item);
    _label_food->setText(QString("%1x %2 (Restores +%3 HP)")
                             .arg(_gameState.equipped_food_qty)
                             .arg(fi.name)
                             .arg(fi.heal_amount));
  } else {
    _label_food->setText("None");
  }

  std::string st = std::format(
      "Combat Level: {}\n"
      "Hitpoints: {} / {} HP\n"
      "Combat Mode: {}\n"
      "Weapon Cycle: {:.2f}s\n"
      "Max Hit: {}\n"
      "Accuracy Rating: {}\n"
      "Evasion Rating: {}\n"
      "Damage Reduction: {}%\n"
      "Auto-Stim Threshold: {} HP",
      _gameState.combat_level(), _gameState.player_hp, _gameState.max_hp(),
      attack_style_name(_gameState.attack_style),
      _gameState.player_attack_interval_ms() / 1000.0,
      _gameState.player_max_hit(), _gameState.player_accuracy(),
      _gameState.player_evasion(), _gameState.player_damage_reduction(),
      _gameState.auto_eat_threshold_hp());
  _label_stats->setText(QString::fromStdString(st));
}

void WindowEquipment::onUnequipSlot(int slot_idx) {
  _gameState.unequip_slot(static_cast<EquipSlot>(slot_idx));
  updateEquipmentUi();
  emit stateChanged();
}

// ============================================================================
// WindowBestiary Implementation
// ============================================================================

WindowBestiary::WindowBestiary(const GameState& gameState, QWidget* parent)
    : QDialog(parent), _gameState(gameState) {
  _setupWidget();
}

void WindowBestiary::_setupWidget() {
  setWindowTitle("Hostile Database & Salvage Tables");
  resize(780, 420);

  QVBoxLayout* vbox = new QVBoxLayout(this);
  QTreeWidget* tree = new QTreeWidget(this);
  tree->setRootIsDecorated(false);
  tree->setUniformRowHeights(true);
  tree->setColumnCount(7);
  tree->setHeaderLabels(
      {"Lv", "Hostile Target", "Sector", "HP / MaxHit", "Hit% vs You", "Kills", "Credits & Salvage Table"});
  tree->setColumnWidth(0, 40);
  tree->setColumnWidth(1, 175);
  tree->setColumnWidth(2, 125);
  tree->setColumnWidth(3, 85);
  tree->setColumnWidth(4, 90);
  tree->setColumnWidth(5, 55);

  for (int i = 0; i < MONSTER_COUNT; ++i) {
    const auto& mon = monster_info[i];
    auto* item = new QTreeWidgetItem(tree);

    QString drops_str =
        QString("%1-%2 Cr").arg(mon.credits_min).arg(mon.credits_max);
    for (const auto& d : mon.drops) {
      if (is_valid_item(d.item_id)) {
        drops_str += QString(", %1 (%2%)")
                         .arg(get_item_info(d.item_id).name)
                         .arg(d.chance_pct);
      }
    }

    item->setText(0, QString::number(mon.combat_level));
    item->setText(1, QString::fromUtf8(mon.name));
    item->setText(2, QString::fromUtf8(mon.zone_name));
    item->setText(3, QString("%1 HP / %2").arg(mon.max_hp).arg(mon.max_hit));
    item->setText(4, QString("You %1% / Foe %2%")
                         .arg(_gameState.player_hit_chance_pct(i))
                         .arg(_gameState.monster_hit_chance_pct(i)));
    item->setText(5, QString::number(_gameState.monster_kills[i]));
    item->setText(6, drops_str);
  }

  vbox->addWidget(tree);
  QPushButton* btn_close = new QPushButton("Close", this);
  connect(btn_close, &QPushButton::clicked, this, &QDialog::accept);
  vbox->addWidget(btn_close);
}

// ============================================================================
// WindowHistory Implementation
// ============================================================================

WindowHistory::WindowHistory(const GameState& gameState, int initial_item,
                             QWidget* parent)
    : QDialog(parent), _gameState(gameState) {
  _setupWidget(initial_item);
  _refreshChart();
}

void WindowHistory::_setupWidget(int initial_item) {
  setWindowTitle("Routineverse Telemetry History");
  resize(680, 440);

  QVBoxLayout* vbox = new QVBoxLayout(this);
  QHBoxLayout* hbox_top = new QHBoxLayout();
  hbox_top->addWidget(new QLabel("Metric:", this));

  _combo_item = new QComboBox(this);
  for (int i = 0; i < HistoryChartView::TOTAL_ITEMS; ++i) {
    _combo_item->addItem(HistoryChartView::itemName(i));
  }
  _combo_item->setCurrentIndex(
      std::clamp(initial_item, 0, HistoryChartView::TOTAL_ITEMS - 1));
  connect(_combo_item, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, &WindowHistory::onItemChanged);
  hbox_top->addWidget(_combo_item, 1);
  vbox->addLayout(hbox_top);

  _chart_view = new HistoryChartView(false, this);
  _chart_view->setItemIndex(_combo_item->currentIndex());
  vbox->addWidget(_chart_view, 1);

  _button_close = new QPushButton("Close", this);
  connect(_button_close, &QPushButton::clicked, this, &QDialog::accept);
  vbox->addWidget(_button_close);
}

void WindowHistory::onItemChanged(int index) {
  _chart_view->setItemIndex(index);
  _refreshChart();
}

void WindowHistory::_refreshChart() { _chart_view->updateChart(_gameState); }

// ============================================================================
// WindowInput Implementation
// ============================================================================

WindowInput::WindowInput(const QString& title, const QString& message,
                         const QString& question, int min_val, int max_val,
                         int initial_val, QWidget* parent)
    : QDialog(parent) {
  setWindowTitle(title);
  QVBoxLayout* vbox = new QVBoxLayout(this);
  vbox->addWidget(new QLabel(message, this));

  QHBoxLayout* hbox = new QHBoxLayout();
  hbox->addWidget(new QLabel(question, this));
  _spinbox = new QSpinBox(this);
  _spinbox->setRange(min_val, max_val);
  _spinbox->setValue(initial_val);
  hbox->addWidget(_spinbox);
  vbox->addLayout(hbox);

  QHBoxLayout* btns = new QHBoxLayout();
  QPushButton* ok = new QPushButton("OK", this);
  QPushButton* cancel = new QPushButton("Cancel", this);
  connect(ok, &QPushButton::clicked, this, &QDialog::accept);
  connect(cancel, &QPushButton::clicked, this, &QDialog::reject);
  btns->addWidget(ok);
  btns->addWidget(cancel);
  vbox->addLayout(btns);
}

int WindowInput::value() const { return _spinbox ? _spinbox->value() : 0; }
