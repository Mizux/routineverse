Github-CI:<br>
[![Build Status][amd64_linux_status]][amd64_linux_link]
[![Build Status][amd64_macos_status]][amd64_macos_link]
[![Build Status][arm64_macos_status]][arm64_macos_link]
[![Build Status][amd64_windows_status]][amd64_windows_link]<br>

[amd64_linux_status]: ./../../actions/workflows/amd64_linux_cmake.yml/badge.svg
[amd64_linux_link]: ./../../actions/workflows/amd64_linux_cmake.yml
[amd64_macos_status]: ./../../actions/workflows/amd64_macos_cmake.yml/badge.svg
[amd64_macos_link]: ./../../actions/workflows/amd64_macos_cmake.yml
[arm64_macos_status]: ./../../actions/workflows/arm64_macos_cmake.yml/badge.svg
[arm64_macos_link]: ./../../actions/workflows/arm64_macos_cmake.yml
[amd64_windows_status]: ./../../actions/workflows/amd64_windows_cmake.yml/badge.svg
[amd64_windows_link]: ./../../actions/workflows/amd64_windows_cmake.yml

# Routineverse

A simplified C++20 idle cyberpunk RPG.  
It is featuring both a **Qt6 GUI** and a **btop-inspired ncurses TUI**.

## Description

## Project Structure

- `libroutineverse/`: Shared C++20 Idle simulation engine (`GameState`), skills, recipes, items, combat & Slayer engine, shop upgrades, save/load, and progression history.
- `app/`: Qt6 Widgets + QtCharts graphical application with interactive skill/combat views, progress bars, equipment & bank manager, shop dialog, monster bestiary, and history charts.
- `tui/`: Terminal User Interface built with wide-character `ncursesw`, featuring btop-style boxes, live action progress bars, Braille progression charts, and full keyboard/mouse navigation.

## Prerequisites

- **C++ Compiler** supporting C++20 (GCC 11+, Clang 13+, or MSVC)
- **CMake** (version 3.24 or higher)
- **Qt 6** development libraries (`qt6-base-dev`, `qt6-charts-dev`)
- **NCurses** development libraries (`libncurses-dev`)

On Debian/Ubuntu-based distributions:
```bash
sudo apt update
sudo apt install build-essential cmake qt6-base-dev qt6-charts-dev libncurses-dev
```

## Building and Running

### 1. Configure

```bash
cmake -S . -B build
```

### 2. Compile

```bash
cmake --build build --config Release
```

### 3. Run

Qt6 GUI application:
```bash
./build/bin/routineverse
```

NCurses TUI application:
```bash
./build/bin/routineverse-tui
```
