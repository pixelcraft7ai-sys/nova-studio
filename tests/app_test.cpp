#include <QDockWidget>
#include <QMenu>
#include <QStatusBar>
#include <QTemporaryDir>
#include <QtTest>

#include "app_services.hpp"
#include "main_window.hpp"
#include "welcome_widget.hpp"

using namespace nova::app;

class AppTest : public QObject {
  Q_OBJECT
 private slots:
  void servicesAreComplete() {
    QTemporaryDir dir;
    AppOptions o;
    o.configDir = dir.path().toStdString();
    o.headless = true;
    auto s = createDefaultServices(o);
    QVERIFY(s.isComplete());
    QVERIFY(s.renderBackends->create("null").ok());
    QVERIFY(s.formats->ids().empty());  // nothing is advertised before phase 09
  }
  void windowBuildsAndCloses() {
    QTemporaryDir dir;
    AppOptions o;
    o.configDir = dir.path().toStdString();
    o.headless = true;
    MainWindow w(createDefaultServices(o));
    w.show();
    QVERIFY(QTest::qWaitForWindowExposed(&w));
    QVERIFY(w.centralWidget() != nullptr);
    QVERIFY(qobject_cast<WelcomeWidget*>(w.centralWidget()) != nullptr);
    QVERIFY(w.fileMenu() != nullptr);
    QVERIFY(w.sidePanel() != nullptr);
    QVERIFY(w.close());
  }
  void showsLocalOnlyByDefault() {
    QTemporaryDir dir;
    AppOptions o;
    o.configDir = dir.path().toStdString();
    o.headless = true;
    MainWindow w(createDefaultServices(o));
    QCOMPARE(w.statusBar()->currentMessage(), QStringLiteral("محلي فقط"));
  }
  void errorsAreExplainedNotCrashing() {
    QTemporaryDir dir;
    AppOptions o;
    o.configDir = dir.path().toStdString();
    auto services = createDefaultServices(o);
    auto recorder = std::make_shared<RecordingErrorPresenter>();
    services.errors = recorder;
    MainWindow w(services);
    w.reportError({nova::core::ErrorCode::NotFound, "missing"}, QStringLiteral("فتح ملف"));
    QCOMPARE(recorder->entries.size(), std::size_t{1});
    QVERIFY(!userMessageFor(nova::core::ErrorCode::Corrupt).isEmpty());
    QVERIFY(userMessageFor(nova::core::ErrorCode::Unknown) !=
            userMessageFor(nova::core::ErrorCode::NotFound));
  }
};

QTEST_MAIN(AppTest)
#include "app_test.moc"
