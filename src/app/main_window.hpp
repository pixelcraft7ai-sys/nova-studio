#pragma once

#include <QMainWindow>

#include "app_services.hpp"

class QDockWidget;
class QMenu;

namespace nova::app {

// Shell only: owns no document data and performs no image processing
// (ADR 0005). It talks to the rest of the program through AppServices.
class MainWindow final : public QMainWindow {
  Q_OBJECT
 public:
  explicit MainWindow(AppServices services, QWidget* parent = nullptr);

  // Extension points for later phases.
  [[nodiscard]] QMenu* fileMenu() const { return fileMenu_; }
  [[nodiscard]] QDockWidget* sidePanel() const { return sidePanel_; }

  void reportError(const core::Error& error, const QString& context = {});

 private:
  void buildMenus();
  void buildSidePanel();

  AppServices services_;
  QMenu* fileMenu_{nullptr};
  QDockWidget* sidePanel_{nullptr};
};

}  // namespace nova::app
