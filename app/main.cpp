#include <QApplication>

#include "config.hpp"
#include "window.hpp"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  app.setOrganizationName(QString::fromUtf8(kProgramAuthorName));
  app.setApplicationName(QString::fromUtf8(kProgramName));

  MainWindow main;
  main.show();
  return app.exec();
}
