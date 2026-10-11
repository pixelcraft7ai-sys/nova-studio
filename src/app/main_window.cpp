#include "main_window.hpp"

#include <QAction>
#include <QDockWidget>
#include <QKeySequence>
#include <QLabel>
#include <QMenuBar>
#include <QStatusBar>

#include "nova/core/version.hpp"
#include "welcome_widget.hpp"

namespace nova::app {

MainWindow::MainWindow(AppServices services, QWidget* parent)
    : QMainWindow(parent), services_(std::move(services)) {
  setObjectName(QStringLiteral("mainWindow"));
  setWindowTitle(QStringLiteral("NOVA Studio"));
  resize(1100, 700);

  const bool network = services_.settings->getBool("privacy.network_enabled").ok() &&
                       services_.settings->getBool("privacy.network_enabled").value();

  setCentralWidget(new WelcomeWidget(QString::fromUtf8(core::kVersion), !network, this));
  buildMenus();
  buildSidePanel();
  statusBar()->showMessage(network ? QStringLiteral("الشبكة مفعّلة") : QStringLiteral("محلي فقط"));

  services_.logger->info("app", "main window created");
}

void MainWindow::buildMenus() {
  fileMenu_ = menuBar()->addMenu(QStringLiteral("&ملف"));
  fileMenu_->setObjectName(QStringLiteral("fileMenu"));
  auto* quit = fileMenu_->addAction(QStringLiteral("خروج"));
  quit->setObjectName(QStringLiteral("quitAction"));
  quit->setShortcut(QKeySequence::Quit);
  connect(quit, &QAction::triggered, this, &QWidget::close);
}

void MainWindow::buildSidePanel() {
  sidePanel_ = new QDockWidget(QStringLiteral("اللوحة الجانبية"), this);
  sidePanel_->setObjectName(QStringLiteral("sidePanel"));
  auto* empty = new QLabel(QStringLiteral("لا توجد مستندات مفتوحة."), sidePanel_);
  empty->setAlignment(Qt::AlignCenter);
  empty->setWordWrap(true);
  sidePanel_->setWidget(empty);
  addDockWidget(Qt::RightDockWidgetArea, sidePanel_);
}

void MainWindow::reportError(const core::Error& error, const QString& context) {
  services_.logger->error("app", std::string(core::toString(error.code)) + ": " + error.message);
  services_.errors->present(error, context);
}

}  // namespace nova::app
