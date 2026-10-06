#include <clocale>

#include "tui.hpp"

int main() {
  std::setlocale(LC_ALL, "");
  TuiApp app;
  return app.run();
}
