#include "tui.hpp"

#include <ncurses.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <format>
#include <sstream>
#include <string>
#include <vector>

#include "config.hpp"
#include "routineverse.hpp"

namespace {

enum ColorPair : short {
  CP_DEFAULT = 1,
  CP_BORDER,
  CP_BORDER_ACTIVE,
  CP_TITLE,
  CP_KEY,
  CP_RED,
  CP_GREEN,
  CP_BLUE,
  CP_YELLOW,
  CP_CYAN,
  CP_MAGENTA,
  CP_WHITE,
  CP_SELECTED,
  CP_HEADER_BAR,
  CP_DIM,
  CP_GRAPH_LINE,
  CP_GRAPH_REF,
};

void init_btop_colors() {
  if (!has_colors()) return;
  start_color();
  use_default_colors();

  init_pair(CP_DEFAULT, COLOR_WHITE, -1);
  init_pair(CP_BORDER, COLOR_BLUE, -1);
  init_pair(CP_BORDER_ACTIVE, COLOR_CYAN, -1);
  init_pair(CP_TITLE, COLOR_WHITE, -1);
  init_pair(CP_KEY, COLOR_RED, -1);
  init_pair(CP_RED, COLOR_RED, -1);
  init_pair(CP_GREEN, COLOR_GREEN, -1);
  init_pair(CP_BLUE, COLOR_BLUE, -1);
  init_pair(CP_YELLOW, COLOR_YELLOW, -1);
  init_pair(CP_CYAN, COLOR_CYAN, -1);
  init_pair(CP_MAGENTA, COLOR_MAGENTA, -1);
  init_pair(CP_WHITE, COLOR_WHITE, -1);
  init_pair(CP_SELECTED, COLOR_BLACK, COLOR_CYAN);
  init_pair(CP_HEADER_BAR, COLOR_WHITE, COLOR_BLUE);
  init_pair(CP_DIM, COLOR_BLUE, -1);
  init_pair(CP_GRAPH_LINE, COLOR_CYAN, -1);
  init_pair(CP_GRAPH_REF, COLOR_MAGENTA, -1);
}

void draw_btop_box(int y, int x, int h, int w, const std::string& title,
                   const std::string& right_hint = "", bool active = false,
                   short custom_border_cp = 0) {
  if (h < 2 || w < 4) return;
  short b_cp =
      custom_border_cp ? custom_border_cp : (active ? CP_BORDER_ACTIVE : CP_BORDER);

  attron(COLOR_PAIR(b_cp) | (active ? A_BOLD : A_NORMAL));
  mvaddstr(y, x, "╭");
  for (int i = 1; i < w - 1; ++i) mvaddstr(y, x + i, "─");
  mvaddstr(y, x + w - 1, "╮");

  for (int r = 1; r < h - 1; ++r) {
    mvaddstr(y + r, x, "│");
    for (int c = 1; c < w - 1; ++c) mvaddch(y + r, x + c, ' ');
    mvaddstr(y + r, x + w - 1, "│");
  }

  mvaddstr(y + h - 1, x, "╰");
  for (int i = 1; i < w - 1; ++i) mvaddstr(y + h - 1, x + i, "─");
  mvaddstr(y + h - 1, x + w - 1, "╯");
  attroff(COLOR_PAIR(b_cp) | (active ? A_BOLD : A_NORMAL));

  if (!title.empty() && w > 8) {
    attron(COLOR_PAIR(b_cp) | (active ? A_BOLD : A_NORMAL));
    mvaddstr(y, x + 2, "┤ ");
    attroff(COLOR_PAIR(b_cp) | (active ? A_BOLD : A_NORMAL));

    attron(COLOR_PAIR(active ? CP_CYAN : CP_TITLE) | A_BOLD);
    std::string t = title;
    if (static_cast<int>(t.size()) > w - 8) t = t.substr(0, w - 8);
    addstr(t.c_str());
    attroff(COLOR_PAIR(active ? CP_CYAN : CP_TITLE) | A_BOLD);

    attron(COLOR_PAIR(b_cp) | (active ? A_BOLD : A_NORMAL));
    addstr(" ├");
    attroff(COLOR_PAIR(b_cp) | (active ? A_BOLD : A_NORMAL));
  }

  if (!right_hint.empty() &&
      static_cast<int>(right_hint.size() + title.size() + 12) < w) {
    int rx = x + w - static_cast<int>(right_hint.size()) - 6;
    attron(COLOR_PAIR(b_cp));
    mvaddstr(y, rx, "┤ ");
    attroff(COLOR_PAIR(b_cp));

    bool in_bracket = false;
    for (char ch : right_hint) {
      if (ch == '[') {
        in_bracket = true;
        attron(COLOR_PAIR(CP_DIM));
        addch('[');
        attroff(COLOR_PAIR(CP_DIM));
      } else if (ch == ']') {
        in_bracket = false;
        attron(COLOR_PAIR(CP_DIM));
        addch(']');
        attroff(COLOR_PAIR(CP_DIM));
      } else if (in_bracket) {
        attron(COLOR_PAIR(CP_KEY) | A_BOLD);
        addch(ch);
        attroff(COLOR_PAIR(CP_KEY) | A_BOLD);
      } else {
        attron(COLOR_PAIR(CP_TITLE));
        addch(ch);
        attroff(COLOR_PAIR(CP_TITLE));
      }
    }

    attron(COLOR_PAIR(b_cp));
    addstr(" ├");
    attroff(COLOR_PAIR(b_cp));
  }
}

void draw_progress_bar(int y, int x, int width, double ratio, short color_cp) {
  if (width <= 0) return;
  ratio = std::clamp(ratio, 0.0, 1.0);
  int filled = static_cast<int>(std::round(ratio * width));
  attron(COLOR_PAIR(color_cp) | A_BOLD);
  for (int i = 0; i < filled; ++i) {
    mvaddstr(y, x + i, "█");
  }
  attroff(COLOR_PAIR(color_cp) | A_BOLD);

  attron(COLOR_PAIR(CP_DIM));
  for (int i = filled; i < width; ++i) {
    mvaddstr(y, x + i, "░");
  }
  attroff(COLOR_PAIR(CP_DIM));
}

std::vector<std::string> wrap_text(const std::string& text, int max_width) {
  std::vector<std::string> lines;
  if (max_width <= 4) return lines;
  std::istringstream paragraphs(text);
  std::string para;
  while (std::getline(paragraphs, para, '\n')) {
    if (para.empty()) {
      lines.push_back("");
      continue;
    }
    std::istringstream words(para);
    std::string word;
    std::string current;
    while (words >> word) {
      if (current.empty()) {
        current = word;
      } else if (static_cast<int>(current.size() + 1 + word.size()) <= max_width) {
        current += " " + word;
      } else {
        lines.push_back(current);
        current = word;
      }
    }
    if (!current.empty()) lines.push_back(current);
  }
  return lines;
}

std::string braille_utf8(uint8_t mask) {
  char buf[4];
  buf[0] = static_cast<char>(0xE2);
  buf[1] = static_cast<char>(0xA0 | ((mask >> 6) & 0x03));
  buf[2] = static_cast<char>(0x80 | (mask & 0x3F));
  buf[3] = '\0';
  return std::string(buf);
}

}  // namespace

TuiApp::TuiApp() : _gameState() { clampCursors(); }

TuiApp::~TuiApp() = default;

std::string TuiApp::itemName(int item_idx) {
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
      if (s_idx >= 0 && s_idx < static_cast<int>(all_skills.size())) {
        return skill_name(all_skills[s_idx]) + " XP";
      }
      return "Credits (Cr)";
    }
  }
}

bool TuiApp::isCombatView() const {
  if (_forceCombatView) return true;
  int idx = std::clamp(_skillCursor, 0, static_cast<int>(all_skills.size()) - 1);
  return is_combat_skill(all_skills[idx]);
}

void TuiApp::clampCursors() {
  _skillCursor =
      std::clamp(_skillCursor, 0, static_cast<int>(all_skills.size()) - 1);
  if (!isCombatView()) {
    auto acts = actions_for_skill(all_skills[_skillCursor]);
    if (acts.empty()) {
      _actionCursor = 0;
    } else {
      _actionCursor = std::clamp(_actionCursor, 0, static_cast<int>(acts.size()) - 1);
    }
  }
  _monsterCursor = std::clamp(_monsterCursor, 0, MONSTER_COUNT - 1);
  int b_sz = static_cast<int>(_gameState.bank.size());
  if (b_sz <= 0) {
    _bankCursor = 0;
  } else {
    _bankCursor = std::clamp(_bankCursor, 0, b_sz - 1);
  }
}

int TuiApp::run() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  set_escdelay(25);
  timeout(100);  // Real-time 100ms tick for idle RPG progression!
  mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, nullptr);

  init_btop_colors();

  auto last_tick = std::chrono::steady_clock::now();

  while (_running) {
    auto now = std::chrono::steady_clock::now();
    int elapsed_ms = static_cast<int>(
        std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count());
    if (elapsed_ms >= 50) {
      _gameState.tick(std::min(elapsed_ms, 1000));
      last_tick = now;
    }

    clampCursors();
    drawDashboard();

    int ch = getch();
    if (ch == ERR) {
      continue;
    }

    switch (ch) {
      case 'q':
      case 'Q':
        if (showConfirmModal("Disconnect Routineverse",
                             "Are you sure you want to jack out of Routineverse?")) {
          _running = false;
        }
        last_tick = std::chrono::steady_clock::now();
        break;

      case '\t':
        if (_focus == FocusPane::Skills) {
          _focus = FocusPane::Actions;
        } else if (_focus == FocusPane::Actions) {
          _focus = FocusPane::Bank;
        } else {
          _focus = FocusPane::Skills;
        }
        break;

      case KEY_BTAB:
        if (_focus == FocusPane::Skills) {
          _focus = FocusPane::Bank;
        } else if (_focus == FocusPane::Actions) {
          _focus = FocusPane::Skills;
        } else {
          _focus = FocusPane::Actions;
        }
        break;

      case KEY_LEFT:
      case 'h':
        if (_focus == FocusPane::Bank) {
          _focus = FocusPane::Actions;
        } else if (_focus == FocusPane::Actions) {
          _focus = FocusPane::Skills;
        }
        break;

      case KEY_RIGHT:
      case 'l':
        if (_focus == FocusPane::Skills) {
          _focus = FocusPane::Actions;
        } else if (_focus == FocusPane::Actions) {
          _focus = FocusPane::Bank;
        }
        break;

      case KEY_UP:
      case 'k':
        if (_focus == FocusPane::Skills) {
          _skillCursor--;
          _forceCombatView = false;
          _actionCursor = 0;
        } else if (_focus == FocusPane::Actions) {
          if (isCombatView()) {
            _monsterCursor--;
          } else {
            _actionCursor--;
          }
        } else if (_focus == FocusPane::Bank) {
          _bankCursor--;
        }
        clampCursors();
        break;

      case KEY_DOWN:
      case 'j':
        if (_focus == FocusPane::Skills) {
          _skillCursor++;
          _forceCombatView = false;
          _actionCursor = 0;
        } else if (_focus == FocusPane::Actions) {
          if (isCombatView()) {
            _monsterCursor++;
          } else {
            _actionCursor++;
          }
        } else if (_focus == FocusPane::Bank) {
          _bankCursor++;
        }
        clampCursors();
        break;

      case '\n':
      case KEY_ENTER:
      case ' ':
        if (_focus == FocusPane::Skills) {
          _focus = FocusPane::Actions;
        } else if (_focus == FocusPane::Actions) {
          actionStartSelected();
        } else if (_focus == FocusPane::Bank) {
          actionEquipSelected();
        }
        break;

      case 'c':
      case 'C':
        _forceCombatView = !isCombatView();
        if (_forceCombatView && !is_combat_skill(all_skills[_skillCursor])) {
          _skillCursor = 8;
        } else if (!_forceCombatView &&
                   is_combat_skill(all_skills[_skillCursor])) {
          _skillCursor = 0;
        }
        _focus = FocusPane::Actions;
        clampCursors();
        break;

      case 'x':
      case 'X':
        actionStopActivity();
        break;

      case 'f':
      case 'F':
        actionEatFood();
        break;

      case 'e':
      case 'E':
        actionEquipSelected();
        last_tick = std::chrono::steady_clock::now();
        break;

      case 's':
        actionSellSelected();
        last_tick = std::chrono::steady_clock::now();
        break;

      case 'S':
        actionSellAllBank();
        last_tick = std::chrono::steady_clock::now();
        break;

      case 'y':
      case 'Y':
        actionCycleAttackStyle();
        break;

      case 't':
      case 'T':
        actionNewBountyContract();
        break;

      case 'u':
      case 'U':
        showShopDialog();
        last_tick = std::chrono::steady_clock::now();
        break;

      case 'i':
      case 'I':
        showEquipmentDialog();
        last_tick = std::chrono::steady_clock::now();
        break;

      case 'b':
      case 'B':
        showBestiaryDialog();
        last_tick = std::chrono::steady_clock::now();
        break;

      case 'g':
        _chartItemIdx = (_chartItemIdx + 1) % TOTAL_ITEMS;
        break;

      case 'G':
        showHistoryDialog(_chartItemIdx);
        last_tick = std::chrono::steady_clock::now();
        break;

      case '+':
      case '=':
      case '[':
        actionFastForward(60);
        last_tick = std::chrono::steady_clock::now();
        break;

      case ']':
        actionFastForward(600);
        last_tick = std::chrono::steady_clock::now();
        break;

      case KEY_F(5):
      case 'w':
      case 'W':
        actionSaveGame();
        last_tick = std::chrono::steady_clock::now();
        break;

      case KEY_F(9):
      case 'o':
      case 'O':
        actionLoadGame();
        last_tick = std::chrono::steady_clock::now();
        break;

      case 'm':
      case 'M':
        showMilestonesDialog();
        last_tick = std::chrono::steady_clock::now();
        break;

      case 'd':
      case 'D':
        showDocsDialog();
        last_tick = std::chrono::steady_clock::now();
        break;

      case 'a':
      case 'A':
        showAboutDialog();
        last_tick = std::chrono::steady_clock::now();
        break;

      case 'n':
      case 'N':
        actionNewGame();
        last_tick = std::chrono::steady_clock::now();
        break;

      case '?':
      case KEY_F(1):
        showHelpDialog();
        last_tick = std::chrono::steady_clock::now();
        break;

      case KEY_MOUSE: {
        MEVENT ev;
        if (getmouse(&ev) == OK && (ev.bstate & (BUTTON1_CLICKED | BUTTON1_PRESSED |
                                                 BUTTON1_DOUBLE_CLICKED))) {
          int rows, cols;
          getmaxyx(stdscr, rows, cols);
          int left_w = std::max(26, cols * 24 / 100);
          int center_w = std::max(38, cols * 44 / 100);
          int top_h = std::max(14, rows - 8);

          if (ev.y >= 1 && ev.y < 1 + top_h) {
            if (ev.x < left_w) {
              _focus = FocusPane::Skills;
              int idx = ev.y - 3;
              if (idx >= 0 && idx < static_cast<int>(all_skills.size())) {
                _skillCursor = idx;
                _forceCombatView = false;
                _actionCursor = 0;
              }
            } else if (ev.x < left_w + center_w) {
              _focus = FocusPane::Actions;
              if (isCombatView()) {
                int idx = ev.y - 8;
                if (idx >= 0 && idx < MONSTER_COUNT) {
                  _monsterCursor = idx;
                  if (ev.bstate & BUTTON1_DOUBLE_CLICKED) {
                    actionStartSelected();
                  }
                }
              } else {
                auto acts = actions_for_skill(all_skills[_skillCursor]);
                int idx = ev.y - 5;
                if (idx >= 0 && idx < static_cast<int>(acts.size())) {
                  _actionCursor = idx;
                  if (ev.bstate & BUTTON1_DOUBLE_CLICKED) {
                    actionStartSelected();
                  }
                }
              }
            } else {
              _focus = FocusPane::Bank;
            }
          }
          clampCursors();
        }
        break;
      }

      default:
        if (ch >= '1' && ch <= '9') {
          _skillCursor = ch - '1';
          _forceCombatView = false;
          _actionCursor = 0;
          clampCursors();
        } else if (ch == '0') {
          _skillCursor = 9;
          _forceCombatView = false;
          clampCursors();
        }
        break;
    }
  }

  endwin();
  return 0;
}

void TuiApp::drawDashboard() {
  erase();
  int rows, cols;
  getmaxyx(stdscr, rows, cols);

  if (rows < 22 || cols < 78) {
    draw_btop_box(0, 0, rows, cols, "Routineverse TUI — Terminal Too Small", "", true,
                  CP_RED);
    attron(COLOR_PAIR(CP_YELLOW) | A_BOLD);
    mvprintw(rows / 2 - 1, 4, "Please resize terminal to at least 80x24");
    mvprintw(rows / 2, 4, "Current size: %dx%d", cols, rows);
    attroff(COLOR_PAIR(CP_YELLOW) | A_BOLD);
    refresh();
    return;
  }

  drawTopBar(cols);

  int main_y = 1;
  int log_h = 6;
  int main_h = rows - main_y - log_h - 1;

  int left_w = std::max(26, cols * 24 / 100);
  int center_w = std::max(38, cols * 44 / 100);
  int right_w = cols - left_w - center_w;

  drawSkillsPane(main_y, 0, main_h, left_w);
  drawActionsOrCombatPane(main_y, left_w, main_h, center_w);

  int bank_h = std::max(7, main_h * 42 / 100);
  int status_h = std::max(6, main_h * 30 / 100);
  int graph_h = main_h - bank_h - status_h;

  drawBankPane(main_y, left_w + center_w, bank_h, right_w);
  drawStatusPane(main_y + bank_h, left_w + center_w, status_h, right_w);
  drawGraphPane(main_y + bank_h + status_h, left_w + center_w, graph_h, right_w);

  drawLogPane(main_y + main_h, 0, log_h, cols);
  drawBottomKeyBar(rows - 1, cols);

  refresh();
}

void TuiApp::drawTopBar(int cols) {
  attron(COLOR_PAIR(CP_HEADER_BAR) | A_BOLD);
  for (int c = 0; c < cols; ++c) mvaddch(0, c, ' ');
  mvprintw(0, 1, " Routineverse %s [NEO-SECTOR] ",
           std::string(kProgramVersion).c_str());

  std::string right_stats = std::format(
      "Cr: {} │ BT: {} │ Combat Lv: {} │ Total Lv: {}/{} │ HP: {}/{} ",
      number_string(_gameState.credits), number_string(_gameState.bounty_tokens),
      _gameState.combat_level(), _gameState.total_skill_level(),
      max_total_skill_level(), _gameState.combat.player_hp, _gameState.max_hp());

  int rx = std::max(30, cols - static_cast<int>(right_stats.size()) - 1);
  mvaddstr(0, rx, right_stats.c_str());
  attroff(COLOR_PAIR(CP_HEADER_BAR) | A_BOLD);
}

void TuiApp::drawSkillsPane(int y, int x, int h, int w) {
  bool active = (_focus == FocusPane::Skills);
  draw_btop_box(y, x, h, w, "Skills [1-0]", "[c]Combat", active);

  int inner_w = w - 4;
  int row = y + 1;

  attron(COLOR_PAIR(CP_DIM) | A_BOLD);
  mvprintw(row++, x + 2, "%-12s %5s %5s", "Skill", "Level", "Prog");
  attroff(COLOR_PAIR(CP_DIM) | A_BOLD);

  int skill_count = static_cast<int>(all_skills.size());
  for (int i = 0; i < skill_count && row < y + h - 1; ++i) {
    SkillType sk = all_skills[i];
    if (sk == SkillType::Attack && h >= skill_count + 4 && row < y + h - 2) {
      attron(COLOR_PAIR(CP_DIM));
      mvaddstr(row++, x + 2, "── Combat & Bounty ─");
      attroff(COLOR_PAIR(CP_DIM));
    }
    if (row >= y + h - 1) break;

    int lvl = _gameState.skill_level(sk);
    long long s_xp = _gameState.skill_xp(sk);
    int pct = static_cast<int>(std::round(level_progress_ratio(s_xp) * 100.0));

    bool is_sel = (i == _skillCursor);
    bool is_training = (_gameState.active_type == ActiveActivityType::Skill &&
                        _gameState.active_action_id >= 0 &&
                        skill_actions[_gameState.active_action_id].skill == sk) ||
                       (_gameState.active_type == ActiveActivityType::Combat &&
                        is_combat_skill(sk));

    short cp = is_sel ? (active ? CP_SELECTED : CP_CYAN)
                      : (is_training ? CP_GREEN : CP_DEFAULT);
    attron(COLOR_PAIR(cp) | ((is_sel || is_training) ? A_BOLD : A_NORMAL));
    for (int c = 0; c < inner_w; ++c) mvaddch(row, x + 2 + c, ' ');

    std::string prefix = is_training ? "* " : "  ";
    std::string sname = prefix + skill_name(sk);
    if (static_cast<int>(sname.size()) > 13) sname = sname.substr(0, 13);
    mvprintw(row, x + 2, "%-13s %2d/99 %3d%%", sname.c_str(), lvl, pct);
    attroff(COLOR_PAIR(cp) | ((is_sel || is_training) ? A_BOLD : A_NORMAL));
    row++;
  }
}

void TuiApp::drawActionsOrCombatPane(int y, int x, int h, int w) {
  bool active = (_focus == FocusPane::Actions);
  int inner_w = w - 4;

  if (!isCombatView()) {
    SkillType sk = all_skills[_skillCursor];
    if (is_combat_skill(sk)) sk = SkillType::Salvaging;
    std::string title = skill_name(sk) + " Protocols";
    draw_btop_box(y, x, h, w, title, "[Enter]Execute [x]Stop", active);

    int row = y + 1;
    // Active task banner + progress bar
    double ratio = 0.0;
    if (_gameState.active_type == ActiveActivityType::Skill &&
        _gameState.active_target_ms > 0) {
      ratio = static_cast<double>(_gameState.active_progress_ms) /
              static_cast<double>(_gameState.active_target_ms);
    }
    attron(COLOR_PAIR(CP_YELLOW) | A_BOLD);
    std::string banner = _gameState.status_banner;
    if (static_cast<int>(banner.size()) > inner_w) {
      banner = banner.substr(0, inner_w);
    }
    mvprintw(row++, x + 2, "%s", banner.c_str());
    attroff(COLOR_PAIR(CP_YELLOW) | A_BOLD);

    const SkillType active_skill = skill_actions[_gameState.active_action_id].skill;
    ColorPair active_cp = CP_GREEN;
    switch(active_skill) {
      case SkillType::Salvaging:
      active_cp = CP_WHITE;
      break;
      case SkillType::Fishing:
      active_cp = CP_BLUE;
      break;
      case SkillType::Farming:
      case SkillType::SynthCook:
      active_cp = CP_GREEN;
      break;
      case SkillType::Recycling:
      case SkillType::Smithing:
      active_cp = CP_RED;
      break;
      case SkillType::DeepMining:
      case SkillType::CyberFab:
      active_cp = CP_YELLOW;
      break;
      default:
      break;
    }
    draw_progress_bar(row++, x + 2, std::max(10, inner_w - 8), ratio, active_cp);
    attron(COLOR_PAIR(CP_CYAN) | A_BOLD);
    mvprintw(row - 1, x + 2 + std::max(10, inner_w - 7), "%3d%%",
             static_cast<int>(ratio * 100.0));
    attroff(COLOR_PAIR(CP_CYAN) | A_BOLD);

    attron(COLOR_PAIR(CP_DIM) | A_BOLD);
    mvprintw(row++, x + 2, "%-3s %-24s %-5s %-4s %-4s %s", "Lv", "Protocol", "Cycle",
             "XP", "Mst", "Schematic / Output");
    attroff(COLOR_PAIR(CP_DIM) | A_BOLD);

    auto act_ids = actions_for_skill(sk);
    int visible_rows = std::max(1, (y + h - 1) - row);
    int start_idx = 0;
    if (_actionCursor >= visible_rows) {
      start_idx = _actionCursor - visible_rows + 1;
    }

    for (int i = start_idx; i < static_cast<int>(act_ids.size()) && row < y + h - 1;
         ++i) {
      int id = act_ids[i];
      const auto& act = skill_actions[id];
      bool is_sel = (i == _actionCursor);
      bool is_running = (_gameState.active_type == ActiveActivityType::Skill &&
                         _gameState.active_action_id == id);
      bool unlocked = (_gameState.skill_level(sk) >= act.req_level);

      short cp = is_sel ? (active ? CP_SELECTED : CP_CYAN)
                        : (is_running ? CP_GREEN : (unlocked ? CP_DEFAULT : CP_DIM));
      attron(COLOR_PAIR(cp) | ((is_sel || is_running) ? A_BOLD : A_NORMAL));
      for (int c = 0; c < inner_w; ++c) mvaddch(row, x + 2 + c, ' ');

      double eff_s = _gameState.action_effective_interval_ms(id) / 1000.0;
      int m_lvl = _gameState.mastery_level(id);
      std::string aname = (is_running ? "* " : "") + std::string(act.name);
      if (static_cast<int>(aname.size()) > 24) aname = aname.substr(0, 24);

      std::string line =
          std::format("{:>2}  {:<24} {:>4.1f}s {:>4} {:>3}  {}", act.req_level, aname,
                      eff_s, act.xp, m_lvl, action_recipe(act));
      if (static_cast<int>(line.size()) > inner_w) {
        line = line.substr(0, inner_w);
      }
      mvaddstr(row, x + 2, line.c_str());
      attroff(COLOR_PAIR(cp) | ((is_sel || is_running) ? A_BOLD : A_NORMAL));
      row++;
    }
  } else {
    draw_btop_box(y, x, h, w, "Combat & Bounty Arena",
                  "[Enter]Engage [y]Mode [t]Bounty", active);

    int row = y + 1;
    // Player vs Hostile live HUD
    double plr_hp_r = static_cast<double>(_gameState.combat.player_hp) /
                      std::max(1, _gameState.max_hp());
    attron(COLOR_PAIR(CP_GREEN) | A_BOLD);
    mvprintw(row, x + 2, "You HP %4d/%-4d ", _gameState.combat.player_hp,
             _gameState.max_hp());
    attroff(COLOR_PAIR(CP_GREEN) | A_BOLD);
    draw_progress_bar(row++, x + 19, std::max(8, inner_w - 19), plr_hp_r,
                      plr_hp_r > 0.35 ? CP_GREEN : CP_RED);

    int mon_id = (_gameState.active_type == ActiveActivityType::Combat)
                     ? _gameState.combat.active_monster_id
                     : _monsterCursor;
    mon_id = std::clamp(mon_id, 0, MONSTER_COUNT - 1);
    const auto& cur_mon = monster_info[mon_id];
    int cur_mhp = (_gameState.active_type == ActiveActivityType::Combat)
                      ? _gameState.combat.monster_hp
                      : cur_mon.max_hp;
    double mon_hp_r = static_cast<double>(cur_mhp) / std::max(1, cur_mon.max_hp);

    attron(COLOR_PAIR(CP_RED) | A_BOLD);
    mvprintw(row, x + 2, "Foe HP %4d/%-4d ", cur_mhp, cur_mon.max_hp);
    attroff(COLOR_PAIR(CP_RED) | A_BOLD);
    draw_progress_bar(row++, x + 19, std::max(8, inner_w - 19), mon_hp_r, CP_RED);

    attron(COLOR_PAIR(CP_YELLOW));
    std::string style_task = std::format(
        "Mode: {} │ Bounty: {}x {}", combat_style_name(_gameState.combat.style),
        _gameState.combat.bounty_remaining,
        monster_info[_gameState.combat.bounty_target_id].name);
    if (static_cast<int>(style_task.size()) > inner_w) {
      style_task = style_task.substr(0, inner_w);
    }
    mvaddstr(row++, x + 2, style_task.c_str());
    attroff(COLOR_PAIR(CP_YELLOW));

    attron(COLOR_PAIR(CP_DIM) | A_BOLD);
    mvprintw(row++, x + 2, "%-3s %-18s %-13s %-5s %-4s %-4s %-5s", "Lv", "Hostile",
             "Sector", "HP", "Max", "Bnt", "Kills");
    attroff(COLOR_PAIR(CP_DIM) | A_BOLD);

    for (int i = 0; i < MONSTER_COUNT && row < y + h - 1; ++i) {
      const auto& mon = monster_info[i];
      bool is_sel = (i == _monsterCursor);
      bool is_fighting = (_gameState.active_type == ActiveActivityType::Combat &&
                          _gameState.combat.active_monster_id == i);
      bool is_task = (i == _gameState.combat.bounty_target_id);

      short cp = is_sel ? (active ? CP_SELECTED : CP_CYAN)
                        : (is_fighting ? CP_RED : (is_task ? CP_YELLOW : CP_DEFAULT));
      attron(COLOR_PAIR(cp) | ((is_sel || is_fighting) ? A_BOLD : A_NORMAL));
      for (int c = 0; c < inner_w; ++c) mvaddch(row, x + 2 + c, ' ');

      std::string mname =
          (is_fighting ? "* " : (is_task ? "! " : "")) + std::string(mon.name);
      if (static_cast<int>(mname.size()) > 18) mname = mname.substr(0, 18);
      std::string zname = mon.zone_name;
      if (static_cast<int>(zname.size()) > 13) zname = zname.substr(0, 13);

      std::string line =
          std::format("{:>3} {:<18} {:<13} {:>5} {:>4} {:>4} {:>5}", mon.combat_level,
                      mname, zname, mon.max_hp, mon.max_hit, mon.bounty_req,
                      _gameState.stats.monster_kills[i]);
      if (static_cast<int>(line.size()) > inner_w) {
        line = line.substr(0, inner_w);
      }
      mvaddstr(row, x + 2, line.c_str());
      attroff(COLOR_PAIR(cp) | ((is_sel || is_fighting) ? A_BOLD : A_NORMAL));
      row++;
    }
  }
}

void TuiApp::drawBankPane(int y, int x, int h, int w) {
  bool active = (_focus == FocusPane::Bank);
  std::string title = std::format("Vault ({}/{})", _gameState.used_bank_slots(),
                                  _gameState.bank.capacity);
  draw_btop_box(y, x, h, w, title, "[e]Equip [s]Sell", active);

  int inner_w = w - 4;
  int row = y + 1;

  attron(COLOR_PAIR(CP_DIM) | A_BOLD);
  mvprintw(row++, x + 2, "%-16s %5s %s", "Item", "Qty", "Value / Specs");
  attroff(COLOR_PAIR(CP_DIM) | A_BOLD);

  if (_gameState.bank.empty()) {
    attron(COLOR_PAIR(CP_DIM));
    mvaddstr(row, x + 2, "(Cyber-Vault is empty)");
    attroff(COLOR_PAIR(CP_DIM));
    return;
  }

  int visible_rows = std::max(1, h - 3);
  int start_idx = 0;
  if (_bankCursor >= visible_rows) {
    start_idx = _bankCursor - visible_rows + 1;
  }

  for (int i = start_idx;
       i < static_cast<int>(_gameState.bank.size()) && row < y + h - 1; ++i) {
    const auto& slot = _gameState.bank[i];
    const auto& info = get_item_info(slot.item_id);
    bool is_sel = (i == _bankCursor);

    short cp = is_sel ? (active ? CP_SELECTED : CP_CYAN) : CP_DEFAULT;
    attron(COLOR_PAIR(cp) | (is_sel ? A_BOLD : A_NORMAL));
    for (int c = 0; c < inner_w; ++c) mvaddch(row, x + 2 + c, ' ');

    std::string iname = info.name;
    if (static_cast<int>(iname.size()) > 16) iname = iname.substr(0, 16);

    std::string extra = money_string(static_cast<long long>(slot.qty) * info.price);
    if (info.heal_amount > 0) {
      extra += std::format(" (+{}HP)", info.heal_amount);
    } else if (equip_slot(info.category) == EquipSlot::Weapon) {
      extra += std::format(" (+{}Str)", info.bonus.strength);
    } else if (info.bonus.speed_bonus_pct > 0) {
      extra += std::format(" (-{}%Spd)", info.bonus.speed_bonus_pct);
    } else if (info.bonus.damage_reduction > 0) {
      extra += std::format(" ({}%DR)", info.bonus.damage_reduction);
    }

    std::string line = std::format("{:<16} {:>5} {}", iname, slot.qty, extra);
    if (static_cast<int>(line.size()) > inner_w) {
      line = line.substr(0, inner_w);
    }
    mvaddstr(row, x + 2, line.c_str());
    attroff(COLOR_PAIR(cp) | (is_sel ? A_BOLD : A_NORMAL));
    row++;
  }
}

void TuiApp::drawStatusPane(int y, int x, int h, int w) {
  draw_btop_box(y, x, h, w, "Cyberware & Tools", "[u]Shop [i]Gear");
  int inner_w = w - 4;
  int row = y + 1;

  ItemId w_id = _gameState.equipment.at(EquipSlot::Weapon);
  std::string w_str = is_valid_item(w_id) ? get_item_info(w_id).name : "Unarmed";

  w_id = _gameState.equipment.at(EquipSlot::Shield);
  std::string w_shield = is_valid_item(w_id) ? get_item_info(w_id).name : "Unequiped";

  w_id = _gameState.equipment.at(EquipSlot::Head);
  std::string w_visor = is_valid_item(w_id) ? get_item_info(w_id).name : "Unequiped";

  w_id = _gameState.equipment.at(EquipSlot::Armor);
  std::string w_armor = is_valid_item(w_id) ? get_item_info(w_id).name : "Unequiped";

  std::string food_str =
      (is_valid_item(_gameState.equipment.food_item) && _gameState.equipment.food_qty > 0)
          ? std::format("{}x {} (+{}HP)", _gameState.equipment.food_qty,
                        get_item_info(_gameState.equipment.food_item).name,
                        get_item_info(_gameState.equipment.food_item).heal_amount)
          : "None";

  auto print_line = [&](short cp, const std::string& s) {
    if (row >= y + h - 1) return;
    attron(COLOR_PAIR(cp));
    std::string clipped =
        static_cast<int>(s.size()) > inner_w ? s.substr(0, inner_w) : s;
    mvaddstr(row++, x + 2, clipped.c_str());
    attroff(COLOR_PAIR(cp));
  };

  print_line(CP_RED,
             std::format("Weapon: {} (MaxHit {})", w_str, _gameState.player_max_hit()));
  print_line(CP_MAGENTA, std::format("Shield: {}", w_shield));
  print_line(CP_CYAN, std::format("Visor: {}", w_visor));
  print_line(CP_BORDER, std::format("Armor: {}", w_armor));
  print_line(CP_GREEN, std::format("Stim [f]: {}", food_str));
  print_line(
      CP_YELLOW,
      std::format("Acc: {} │ Eva: {} │ DR: {}% │ AutoStim: Mk{}",
                  _gameState.player_accuracy(), _gameState.player_evasion(),
                  _gameState.player_damage_reduction(), _gameState.auto_stim_tier()));
  print_line(CP_DEFAULT,
             std::format("Tools: Cut T{} Bio T{} Drl T{} Core T{}",
                         _gameState.cutter_tier() + 1, _gameState.harvester_tier() + 1,
                         _gameState.drill_tier() + 1, _gameState.reactor_tier() + 1));
}

void TuiApp::drawGraphPane(int y, int x, int h, int w) {
  std::string title = "Chart: " + itemName(_chartItemIdx);
  draw_btop_box(y, x, h, w, title, "[g]Metric [G]Zoom");
  if (h >= 4 && w >= 10) {
    renderBrailleChart(y + 1, x + 2, h - 2, w - 4, _chartItemIdx, false);
  }
}

void TuiApp::renderBrailleChart(int y, int x, int h, int w, int item_idx,
                                bool show_axes) {
  if (h <= 0 || w <= 2) return;

  std::vector<double> values;
  if (item_idx == ITEM_CREDITS) {
    for (long long v : _gameState.history.credits) values.push_back(v);
    values.push_back(_gameState.credits);
  } else if (item_idx == ITEM_BANK_VALUE) {
    for (long long v : _gameState.history.bank_value) values.push_back(v);
    values.push_back(_gameState.total_bank_value());
  } else if (item_idx == ITEM_TOTAL_LEVEL) {
    for (int v : _gameState.history.total_level) values.push_back(v);
    values.push_back(_gameState.total_skill_level());
  } else if (item_idx == ITEM_TOTAL_XP) {
    for (long long v : _gameState.history.total_xp) values.push_back(v);
    values.push_back(_gameState.total_skill_xp());
  } else if (item_idx == ITEM_HP) {
    for (int v : _gameState.history.hp) values.push_back(v);
    values.push_back(_gameState.combat.player_hp);
  } else {
    int s_idx = std::clamp(item_idx - ITEM_FIRST_SKILL, 0,
                           static_cast<int>(all_skills.size()) - 1);
    SkillType sk = all_skills[s_idx];
    auto it = _gameState.history.skill_xp.find(sk);
    if (it != _gameState.history.skill_xp.end()) {
      for (long long v : it->second) values.push_back(v);
    }
    values.push_back(_gameState.skill_xp(sk));
  }

  if (values.empty()) values.push_back(0.0);

  double min_v = values[0];
  double max_v = values[0];
  for (double v : values) {
    if (v < min_v) min_v = v;
    if (v > max_v) max_v = v;
  }
  if (max_v <= min_v) max_v = min_v + 10.0;

  int plot_x = x;
  int plot_w = w;
  if (show_axes && w > 16) {
    plot_x = x + 10;
    plot_w = w - 10;
    attron(COLOR_PAIR(CP_DIM));
    mvprintw(y, x, "%9.0f", max_v);
    mvprintw(y + h - 1, x, "%9.0f", min_v);
    attroff(COLOR_PAIR(CP_DIM));
  }

  int px_w = plot_w * 2;
  int px_h = h * 4;
  std::vector<uint8_t> grid(plot_w * h, 0);

  auto set_dot = [&](int gx, int gy) {
    if (gx < 0 || gx >= px_w || gy < 0 || gy >= px_h) return;
    int cell_x = gx / 2;
    int cell_y = gy / 4;
    int sub_x = gx % 2;
    int sub_y = gy % 4;
    static const uint8_t bit_map[4][2] = {
        {0x01, 0x08}, {0x02, 0x10}, {0x04, 0x20}, {0x40, 0x80}};
    grid[cell_y * plot_w + cell_x] |= bit_map[sub_y][sub_x];
  };

  int n = static_cast<int>(values.size());
  for (int gx = 0; gx < px_w; ++gx) {
    double t = (px_w > 1) ? static_cast<double>(gx) / (px_w - 1) : 0.0;
    double idx_f = t * std::max(0, n - 1);
    int i0 = static_cast<int>(std::floor(idx_f));
    int i1 = std::min(n - 1, i0 + 1);
    double frac = idx_f - i0;
    double val = values[i0] * (1.0 - frac) + values[i1] * frac;
    double norm = (val - min_v) / (max_v - min_v);
    int gy = px_h - 1 -
             static_cast<int>(std::round(std::clamp(norm, 0.0, 1.0) * (px_h - 1)));
    set_dot(gx, gy);
  }

  attron(COLOR_PAIR(CP_GRAPH_LINE) | A_BOLD);
  for (int r = 0; r < h; ++r) {
    for (int c = 0; c < plot_w; ++c) {
      uint8_t mask = grid[r * plot_w + c];
      if (mask != 0) {
        mvaddstr(y + r, plot_x + c, braille_utf8(mask).c_str());
      }
    }
  }
  attroff(COLOR_PAIR(CP_GRAPH_LINE) | A_BOLD);
}

void TuiApp::drawLogPane(int y, int x, int h, int w) {
  draw_btop_box(y, x, h, w, "Cyber-Log & Telemetry", "[+]/[]]Fast-Forward +1m/+10m");
  int inner_w = w - 4;
  int max_lines = std::max(1, h - 2);
  int total = static_cast<int>(_gameState.game_log.size());
  int start = std::max(0, total - max_lines);

  for (int i = 0; i < max_lines && (start + i) < total; ++i) {
    std::string entry = _gameState.game_log[start + i];
    if (static_cast<int>(entry.size()) > inner_w) {
      entry = entry.substr(0, inner_w);
    }
    short cp = (i + start == total - 1) ? CP_YELLOW : CP_DEFAULT;
    attron(COLOR_PAIR(cp));
    mvaddstr(y + 1 + i, x + 2, entry.c_str());
    attroff(COLOR_PAIR(cp));
  }
}

void TuiApp::drawBottomKeyBar(int y, int cols) {
  attron(COLOR_PAIR(CP_HEADER_BAR));
  for (int c = 0; c < cols; ++c) mvaddch(y, c, ' ');
  std::string bar =
      " [Tab]Pane [Enter]Run/Engage [x]Stop [f]Stim [e]Equip [s/S]Sell [u]Shop "
      "[i]Gear [b]Hostiles [+]/[]]FF [w/o]Save/Load [?]Help [q]Quit";
  if (static_cast<int>(bar.size()) > cols) bar = bar.substr(0, cols);
  mvaddstr(y, 0, bar.c_str());
  attroff(COLOR_PAIR(CP_HEADER_BAR));
}

// ============================================================================
// Actions & Modals
// ============================================================================

void TuiApp::actionStartSelected() {
  if (isCombatView()) {
    _gameState.start_combat(_monsterCursor);
  } else {
    auto acts = actions_for_skill(all_skills[_skillCursor]);
    if (_actionCursor >= 0 && _actionCursor < static_cast<int>(acts.size())) {
      _gameState.start_skill_action(acts[_actionCursor]);
    }
  }
}

void TuiApp::actionStopActivity() { _gameState.stop_activity(); }

void TuiApp::actionEatFood() { _gameState.eat_food(); }

void TuiApp::actionEquipSelected() {
  if (_gameState.bank.empty()) return;
  clampCursors();
  ItemId item_id = _gameState.bank[_bankCursor].item_id;
  _gameState.equip_item(item_id);
  clampCursors();
}

void TuiApp::actionSellSelected() {
  if (_gameState.bank.empty()) return;
  clampCursors();
  ItemId item_id = _gameState.bank[_bankCursor].item_id;
  int have = _gameState.item_qty(item_id);
  if (have <= 0) return;

  if (have == 1) {
    _gameState.sell_item(item_id, 1);
    clampCursors();
    return;
  }

  const auto& info = get_item_info(item_id);
  int qty = showInputSpinModal(
      "Liquidate Vault Item",
      std::format("Liquidating {} ({} Cr each)\nYou have {} in your Cyber-Vault.",
                  info.name, info.price, have),
      "Quantity to sell:", 1, have, have, info.price);
  if (qty > 0) {
    _gameState.sell_item(item_id, qty);
    clampCursors();
  }
}

void TuiApp::actionSellAllBank() {
  if (_gameState.bank.empty()) return;
  if (showConfirmModal(
          "Liquidate All Vault Items",
          std::format("Liquidate all {} item stacks in Cyber-Vault for {}?",
                      _gameState.bank.size(),
                      money_string(_gameState.total_bank_value())))) {
    _gameState.sell_all_non_equipped();
    clampCursors();
  }
}

void TuiApp::actionCycleAttackStyle() {
  _gameState.combat.style = next_combat_style(_gameState.combat.style);
  _gameState.add_log(std::format("Switched combat mode to {}.",
                                 combat_style_name(_gameState.combat.style)));
}

void TuiApp::actionNewBountyContract() { _gameState.assign_new_bounty_contract(); }

void TuiApp::actionFastForward(int seconds) {
  _gameState.add_log(
      std::format("Fast-forwarding {}m of neural simulation...", seconds / 60));
  _gameState.fast_forward_seconds(seconds);
  clampCursors();
}

void TuiApp::actionSaveGame() {
  std::string path = GameState::default_save_path();
  if (_gameState.save_to_file(path)) {
    _gameState.add_log(std::format("Neural state saved to {}.", path));
  }
}

void TuiApp::actionLoadGame() {
  std::string path = GameState::default_save_path();
  _gameState.load_from_file(path);
  clampCursors();
}

void TuiApp::actionNewGame() {
  if (showConfirmModal("New Operative",
                       "Wipe your neural profile and start a fresh New Game?")) {
    _gameState.new_game();
    clampCursors();
  }
}

void TuiApp::showShopDialog() {
  int cursor = 0;
  timeout(-1);
  while (true) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    int w = std::min(74, cols - 4);
    int h = 15;
    int y = (rows - h) / 2;
    int x = (cols - w) / 2;

    draw_btop_box(y, x, h, w, "Cyber-Shop & Tool Upgrades", "[Enter]Buy [Esc]Close",
                  true, CP_YELLOW);

    attron(COLOR_PAIR(CP_GREEN) | A_BOLD);
    mvprintw(y + 1, x + 3, "Available Credits: %s",
             money_string(_gameState.credits).c_str());
    attroff(COLOR_PAIR(CP_GREEN) | A_BOLD);

    auto fmt_upg = [](const char* label, const auto& arr, int tier) {
      if (tier + 1 >= static_cast<int>(arr.size())) {
        return std::format("{:<14}: {} (MAX TIER)", label, arr[tier].name);
      }
      const auto& nxt = arr[tier + 1];
      return std::format("{:<14}: {} -> {} (Lv {}, {} Cr)", label, arr[tier].name,
                         nxt.name, nxt.req_skill_level, nxt.cost_credits);
    };

    std::array<std::string, 6> items = {
        fmt_upg("Salvage Cutter", cutter_upgrades, _gameState.cutter_tier()),
        fmt_upg("Bio-Harvester", harvester_upgrades, _gameState.harvester_tier()),
        fmt_upg("Mining Drill", drill_upgrades, _gameState.drill_tier()),
        fmt_upg("Synth-Reactor", reactor_upgrades, _gameState.reactor_tier()),
        fmt_upg("Auto-Stim", auto_stim_upgrades, _gameState.auto_stim_tier()),
        std::format("{:<14}: {} Slots -> +4 Slots ({})", "Vault Space",
                    _gameState.bank.capacity,
                    money_string(_gameState.next_bank_slot_cost())),
    };

    for (int i = 0; i < 6; ++i) {
      bool sel = (i == cursor);
      attron(COLOR_PAIR(sel ? CP_SELECTED : CP_DEFAULT) | (sel ? A_BOLD : A_NORMAL));
      for (int c = 0; c < w - 6; ++c) mvaddch(y + 3 + i, x + 3 + c, ' ');
      std::string s = items[i];
      if (static_cast<int>(s.size()) > w - 6) s = s.substr(0, w - 6);
      mvaddstr(y + 3 + i, x + 3, s.c_str());
      attroff(COLOR_PAIR(sel ? CP_SELECTED : CP_DEFAULT) | (sel ? A_BOLD : A_NORMAL));
    }

    refresh();
    int ch = getch();
    if (ch == 27 || ch == 'q' || ch == 'u') break;
    if (ch == KEY_UP || ch == 'k') cursor = (cursor + 5) % 6;
    if (ch == KEY_DOWN || ch == 'j') cursor = (cursor + 1) % 6;
    if (ch == '\n' || ch == KEY_ENTER || ch == ' ') {
      if (cursor == 0) _gameState.buy_cutter_upgrade();
      if (cursor == 1) _gameState.buy_harvester_upgrade();
      if (cursor == 2) _gameState.buy_drill_upgrade();
      if (cursor == 3) _gameState.buy_reactor_upgrade();
      if (cursor == 4) _gameState.buy_auto_stim_upgrade();
      if (cursor == 5) _gameState.buy_bank_slot();
    }
  }
  timeout(100);
}

void TuiApp::showEquipmentDialog() {
  int cursor = 0;
  timeout(-1);
  while (true) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    int w = std::min(68, cols - 4);
    int h = 21;
    int y = (rows - h) / 2;
    int x = (cols - w) / 2;

    draw_btop_box(y, x, h, w, "Cyberware Loadout & Combat Stats",
                  "[Enter]Unequip [Esc]Close", true, CP_CYAN);

    for (size_t idx = 0; idx < all_equip_slots.size(); ++idx) {
      EquipSlot slot = all_equip_slots[idx];
      ItemId id = _gameState.equipment.at(slot);
      std::string desc = is_valid_item(id) ? item_equip_summary(id) : "Empty";
      bool sel = (static_cast<int>(idx) == cursor);
      attron(COLOR_PAIR(sel ? CP_SELECTED : CP_DEFAULT) | (sel ? A_BOLD : A_NORMAL));
      for (int c = 0; c < w - 6; ++c) mvaddch(y + 2 + static_cast<int>(idx), x + 3 + c, ' ');
      mvprintw(y + 2 + static_cast<int>(idx), x + 3, "%-11s: %s", equip_slot_name(slot).c_str(),
               desc.c_str());
      attroff(COLOR_PAIR(sel ? CP_SELECTED : CP_DEFAULT) | (sel ? A_BOLD : A_NORMAL));
    }

    int stats_y = y + 3 + static_cast<int>(EQUIP_SLOT_COUNT);
    attron(COLOR_PAIR(CP_YELLOW));
    mvprintw(stats_y, x + 3, "Combat Level: %d   │   HP: %d / %d",
             _gameState.combat_level(), _gameState.combat.player_hp,
             _gameState.max_hp());
    mvprintw(stats_y + 1, x + 3, "Combat Mode: %s",
             combat_style_name(_gameState.combat.style).c_str());
    mvprintw(stats_y + 2, x + 3, "Max Hit: %d   │   Accuracy: %d   │   Evasion: %d",
             _gameState.player_max_hit(), _gameState.player_accuracy(),
             _gameState.player_evasion());
    mvprintw(stats_y + 3, x + 3,
             "Damage Reduction: %d%%   │   Auto-Stim Threshold: %d HP",
             _gameState.player_damage_reduction(), _gameState.auto_eat_threshold_hp());
    attroff(COLOR_PAIR(CP_YELLOW));

    refresh();
    int ch = getch();
    if (ch == 27 || ch == 'q' || ch == 'i') break;
    int slot_count = static_cast<int>(all_equip_slots.size());
    if (ch == KEY_UP || ch == 'k') cursor = (cursor + slot_count - 1) % slot_count;
    if (ch == KEY_DOWN || ch == 'j') cursor = (cursor + 1) % slot_count;
    if (ch == '\n' || ch == KEY_ENTER || ch == ' ') {
      _gameState.unequip_slot(equip_slot_or_none(cursor));
    }
  }
  timeout(100);
}

void TuiApp::showBestiaryDialog() {
  std::ostringstream oss;
  for (int i = 0; i < MONSTER_COUNT; ++i) {
    const auto& mon = monster_info[i];
    oss << std::format("[Lv {:>3}] {} ({}) — {} HP, MaxHit {}, Kills: {}\n",
                       mon.combat_level, mon.name, mon.zone_name, mon.max_hp,
                       mon.max_hit, _gameState.stats.monster_kills[i]);
    oss << std::format("   Salvage: {}-{} Cr", mon.credits_min, mon.credits_max);
    for (const auto& d : mon.drops) {
      if (is_valid_item(d.item_id)) {
        oss << std::format(", {} ({}%)", get_item_info(d.item_id).name, d.chance_pct);
      }
    }
    oss << "\n";
  }
  showMessageModal("Hostile Database & Salvage Tables", oss.str(), CP_CYAN);
}

void TuiApp::showHistoryDialog(int initial_item) {
  int item_idx = std::clamp(initial_item, 0, TOTAL_ITEMS - 1);
  timeout(-1);
  while (true) {
    erase();
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    std::string title = std::format("Telemetry History — {} ({}/{})",
                                    itemName(item_idx), item_idx + 1, TOTAL_ITEMS);
    draw_btop_box(0, 0, rows, cols, title, "[Left/Right]Metric [Esc]Close", true,
                  CP_CYAN);
    renderBrailleChart(2, 2, rows - 4, cols - 4, item_idx, true);
    refresh();

    int ch = getch();
    if (ch == 27 || ch == 'q' || ch == 'G' || ch == '\n') break;
    if (ch == KEY_LEFT || ch == 'h') {
      item_idx = (item_idx + TOTAL_ITEMS - 1) % TOTAL_ITEMS;
    } else if (ch == KEY_RIGHT || ch == 'l' || ch == 'g') {
      item_idx = (item_idx + 1) % TOTAL_ITEMS;
    }
  }
  _chartItemIdx = item_idx;
  timeout(100);
}

void TuiApp::showAboutDialog() {
  std::string msg = std::format(
      "{}\n{}\n\nCyberpunk Idle RPG\nAuthor: "
      "{}\nVersion: {}",
      kProgramName, kProgramDescription, kProgramAuthorName, kProgramVersion);
  showMessageModal("About Routineverse", msg, CP_CYAN);
}

void TuiApp::showDocsDialog() {
  showMessageModal(
      "Routineverse Cyber-Guide",
      "Welcome to Routineverse (Cyberpunk Idle RPG)!\n\n"
      "• Extraction Protocols: Train Salvaging, Fishing, Farming, and "
      "Deep-Mining to gather scrap, synth-biota, hydroponic crops, ores, and "
      "rare Data Crystals.\n"
      "• Processing & Fabrication: Train Recycling, Synth-Cook, Smithing, and "
      "Cyber-Fab to recycle scrap into raw materials, mill Synth-Noodles & cook "
      "Cyber-Ramen / healing stims, smelt alloy ingots & forge blades/exo-suits, "
      "and fabricate visors, holo-shields & data crystals.\n"
      "• Combat & Bounty: Equip weapons, cyber-armor, and stims from your Vault. "
      "Neutralize hostiles and complete Bounty contracts for Bounty Tokens!\n"
      "• Cyber-Shop Upgrades: Press [u] to upgrade tools, unlock Auto-Stim, and "
      "expand Cyber-Vault capacity.",
      CP_GREEN);
}

void TuiApp::showMilestonesDialog() {
  long long minutes = _gameState.total_ticks_ms / 60000;
  long long seconds = (_gameState.total_ticks_ms / 1000) % 60;
  std::string text = std::format(
      "Combat Level: {}   |   Total Skill Level: {} / {}\n"
      "Total Skill XP: {}\n"
      "Current Credits: {}   |   Total Credits Earned: {}\n"
      "Cyber-Vault Value: {} ({} / {} slots)\n"
      "Bounty Tokens: {}   |   Bounty Contracts Completed: {}\n"
      "Items Salvaged/Fabricated: {}\n"
      "Hostiles Neutralized: {}   |   Flatlines: {}\n"
      "NEXUS-9 (Mainframe Boss) Kills: {}\n"
      "Simulated Uptime: {}m {}s",
      _gameState.combat_level(), _gameState.total_skill_level(),
      max_total_skill_level(), number_string(_gameState.total_skill_xp()),
      money_string(_gameState.credits),
      money_string(_gameState.stats.total_credits_earned),
      money_string(_gameState.total_bank_value()), _gameState.used_bank_slots(),
      _gameState.bank.capacity, number_string(_gameState.bounty_tokens),
      _gameState.combat.bounties_completed,
      number_string(_gameState.stats.total_items_gathered),
      number_string(_gameState.stats.total_monsters_killed),
      _gameState.stats.player_deaths, _gameState.stats.monster_kills[MONSTER_COUNT - 1],
      minutes, seconds);
  showMessageModal("Operative Telemetry & Milestones", text, CP_YELLOW);
}

void TuiApp::showHelpDialog() {
  showMessageModal(
      "Keyboard & Mouse Shortcuts",
      "[Tab / Shift+Tab] : Cycle focus pane (Skills / Protocols / Vault)\n"
      "[Up / Down / j/k] : Navigate list in active pane\n"
      "[Enter / Space]   : Execute Protocol / Engage Hostile / Equip Item\n"
      "[c]               : Toggle between Tech Protocols and Combat Arena\n"
      "[x]               : Stop current protocol\n"
      "[f]               : Inject loaded Stim / Ration (+HP)\n"
      "[e]               : Equip selected Vault item or stim\n"
      "[s / S]           : Sell selected Vault item / Sell All Vault items\n"
      "[y]               : Cycle Combat Mode (Precision/Overdrive/Evasive)\n"
      "[t]               : Request new Bounty Contract\n"
      "[u]               : Open Cyber-Shop & Tool Upgrades\n"
      "[i]               : Open Cyberware Loadout & Combat Stats\n"
      "[b]               : Open Hostile Database & Drops\n"
      "[g / G]           : Cycle Chart Metric / Open Fullscreen Chart\n"
      "[+ / ]]           : Fast-Forward +1m / +10m\n"
      "[w / o]           : Save State / Load State\n"
      "[q]               : Disconnect from Routineverse",
      CP_CYAN);
}

void TuiApp::showMessageModal(const std::string& title, const std::string& message,
                              int border_color) {
  timeout(-1);
  int rows, cols;
  getmaxyx(stdscr, rows, cols);
  int w = std::min(74, cols - 4);
  auto lines = wrap_text(message, w - 6);
  int h = std::min(rows - 2, static_cast<int>(lines.size()) + 5);
  int y = (rows - h) / 2;
  int x = (cols - w) / 2;

  int scroll = 0;
  int visible = std::max(1, h - 4);

  while (true) {
    draw_btop_box(y, x, h, w, title, "[Enter/Esc]Close", true, border_color);
    for (int i = 0; i < visible && (scroll + i) < static_cast<int>(lines.size()); ++i) {
      attron(COLOR_PAIR(CP_DEFAULT));
      mvaddstr(y + 2 + i, x + 3, lines[scroll + i].c_str());
      attroff(COLOR_PAIR(CP_DEFAULT));
    }
    refresh();
    int ch = getch();
    if (ch == '\n' || ch == KEY_ENTER || ch == 27 || ch == ' ' || ch == 'q') {
      break;
    }
    if ((ch == KEY_UP || ch == 'k') && scroll > 0) scroll--;
    if ((ch == KEY_DOWN || ch == 'j') &&
        scroll + visible < static_cast<int>(lines.size())) {
      scroll++;
    }
  }
  timeout(100);
}

bool TuiApp::showConfirmModal(const std::string& title, const std::string& message) {
  timeout(-1);
  int rows, cols;
  getmaxyx(stdscr, rows, cols);
  int w = std::min(62, cols - 4);
  auto lines = wrap_text(message, w - 6);
  int h = static_cast<int>(lines.size()) + 6;
  int y = (rows - h) / 2;
  int x = (cols - w) / 2;

  bool yes = true;
  while (true) {
    draw_btop_box(y, x, h, w, title, "[Y/N]", true, CP_YELLOW);
    for (size_t i = 0; i < lines.size(); ++i) {
      mvaddstr(y + 2 + static_cast<int>(i), x + 3, lines[i].c_str());
    }

    int btn_y = y + h - 2;
    attron(COLOR_PAIR(yes ? CP_SELECTED : CP_DEFAULT) | A_BOLD);
    mvaddstr(btn_y, x + w / 2 - 10, " [ Yes ] ");
    attroff(COLOR_PAIR(yes ? CP_SELECTED : CP_DEFAULT) | A_BOLD);

    attron(COLOR_PAIR(!yes ? CP_SELECTED : CP_DEFAULT) | A_BOLD);
    mvaddstr(btn_y, x + w / 2 + 2, " [ No ] ");
    attroff(COLOR_PAIR(!yes ? CP_SELECTED : CP_DEFAULT) | A_BOLD);

    refresh();
    int ch = getch();
    if (ch == 'y' || ch == 'Y') {
      timeout(100);
      return true;
    }
    if (ch == 'n' || ch == 'N' || ch == 27) {
      timeout(100);
      return false;
    }
    if (ch == KEY_LEFT || ch == KEY_RIGHT || ch == '\t' || ch == 'h' || ch == 'l') {
      yes = !yes;
    }
    if (ch == '\n' || ch == KEY_ENTER || ch == ' ') {
      timeout(100);
      return yes;
    }
  }
}

int TuiApp::showInputSpinModal(const std::string& title, const std::string& message,
                               const std::string& question, int min_val, int max_val,
                               int initial_val, int unit_price) {
  timeout(-1);
  int val = std::clamp(initial_val, min_val, max_val);
  int rows, cols;
  getmaxyx(stdscr, rows, cols);
  int w = std::min(64, cols - 4);
  auto lines = wrap_text(message, w - 6);
  int h = static_cast<int>(lines.size()) + 8;
  int y = (rows - h) / 2;
  int x = (cols - w) / 2;

  while (true) {
    draw_btop_box(y, x, h, w, title, "[Left/Right]Adjust [Enter]OK [Esc]Cancel", true,
                  CP_CYAN);
    for (size_t i = 0; i < lines.size(); ++i) {
      mvaddstr(y + 2 + static_cast<int>(i), x + 3, lines[i].c_str());
    }

    int qy = y + 3 + static_cast<int>(lines.size());
    attron(COLOR_PAIR(CP_YELLOW) | A_BOLD);
    mvprintw(qy, x + 3, "%s  < %d / %d >", question.c_str(), val, max_val);
    if (unit_price > 0) {
      mvprintw(qy + 1, x + 3, "Total Value: %s",
               money_string(static_cast<long long>(val) * unit_price).c_str());
    }
    attroff(COLOR_PAIR(CP_YELLOW) | A_BOLD);

    refresh();
    int ch = getch();
    if (ch == 27 || ch == 'q') {
      timeout(100);
      return -1;
    }
    if (ch == '\n' || ch == KEY_ENTER) {
      timeout(100);
      return val;
    }
    if (ch == KEY_LEFT || ch == 'h' || ch == '-') {
      val = std::max(min_val, val - 1);
    } else if (ch == KEY_RIGHT || ch == 'l' || ch == '+') {
      val = std::min(max_val, val + 1);
    } else if (ch == KEY_DOWN || ch == 'j') {
      val = std::max(min_val, val - 10);
    } else if (ch == KEY_UP || ch == 'k') {
      val = std::min(max_val, val + 10);
    } else if (ch == KEY_HOME) {
      val = min_val;
    } else if (ch == KEY_END) {
      val = max_val;
    }
  }
}
