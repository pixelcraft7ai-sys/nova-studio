#include <QTemporaryDir>
#include <QtTest>

#include "nova/core/settings.hpp"
#include "nova/platform/app_paths.hpp"
#include "nova/platform/file_service.hpp"
#include "nova/platform/qt_settings_store.hpp"

using namespace nova::platform;
using nova::core::ErrorCode;

class PlatformTest : public QObject {
  Q_OBJECT
 private slots:
  void atomicWriteRoundTrip() {
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    LocalFileService fs;
    const std::string path = (dir.path() + "/ملف-اختبار.bin").toStdString();  // non-ASCII name
    const Bytes data = {0, 1, 2, 255, 128};
    QVERIFY(fs.writeAtomic(path, data).ok());
    QVERIFY(fs.exists(path));
    QCOMPARE(fs.readAll(path).value(), data);

    const Bytes second = {9, 9};
    QVERIFY(fs.writeAtomic(path, second).ok());  // replaces existing
    QCOMPARE(fs.readAll(path).value(), second);

    // No temp files left behind.
    QCOMPARE(QDir(dir.path()).entryList(QDir::Files).size(), 1);
  }
  void readMissingFileReportsNotFound() {
    LocalFileService fs;
    QCOMPARE(fs.readAll("/definitely/not/here.bin").error().code, ErrorCode::NotFound);
  }
  void writeIntoMissingDirectoryFailsCleanly() {
    QTemporaryDir dir;
    LocalFileService fs;
    auto r = fs.writeAtomic((dir.path() + "/nope/x.bin").toStdString(), {1});
    QVERIFY(!r.ok());
    QCOMPARE(r.error().code, ErrorCode::IoError);
  }
  void settingsPersistAcrossInstances() {
    QTemporaryDir dir;
    const std::string d = dir.path().toStdString();
    {
      nova::core::Settings s(std::make_shared<QtSettingsStore>(d));
      QVERIFY(s.setString("ui.language", "en").ok());
    }
    nova::core::Settings again(std::make_shared<QtSettingsStore>(d));
    QCOMPARE(QString::fromStdString(again.getString("ui.language").value()), QStringLiteral("en"));
  }
  void pathsHonourOverride() {
    QTemporaryDir dir;
    auto p = resolveAppPaths(dir.path().toStdString());
    QVERIFY(QString::fromStdString(p.logDir).startsWith(dir.path()));
  }
};

QTEST_MAIN(PlatformTest)
#include "platform_test.moc"
