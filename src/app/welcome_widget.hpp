#pragma once

#include <QWidget>

namespace nova::app {

class WelcomeWidget final : public QWidget {
  Q_OBJECT
 public:
  WelcomeWidget(const QString& version, bool localOnly, QWidget* parent = nullptr);
};

}  // namespace nova::app
