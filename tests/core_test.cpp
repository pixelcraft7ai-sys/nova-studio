#include <QtTest>
#include <memory>

#include "nova/core/logger.hpp"
#include "nova/core/result.hpp"
#include "nova/core/settings.hpp"

using namespace nova::core;

class CoreTest : public QObject {
  Q_OBJECT
 private slots:
  void resultHoldsValueOrError() {
    Result<int> good = 7;
    QVERIFY(good.ok());
    QCOMPARE(good.value(), 7);
    Result<int> bad = Error{ErrorCode::NotFound, "nope"};
    QVERIFY(!bad.ok());
    QCOMPARE(bad.error().code, ErrorCode::NotFound);
    Result<void> okVoid;
    QVERIFY(okVoid.ok());
    Result<void> badVoid = Error{ErrorCode::IoError, "x"};
    QVERIFY(!badVoid);
  }

  void loggerFiltersByLevel() {
    Logger logger(LogLevel::Warning);
    auto sink = std::make_shared<MemorySink>();
    logger.addSink(sink);
    logger.info("t", "dropped");
    logger.warning("t", "kept");
    logger.error("t", "kept too");
    QCOMPARE(sink->records().size(), std::size_t{2});
    logger.setLevel(LogLevel::Off);
    logger.error("t", "dropped");
    QCOMPARE(sink->records().size(), std::size_t{2});
  }

  void loggerSanitisesControlCharacters() {
    Logger logger;
    auto sink = std::make_shared<MemorySink>();
    logger.addSink(sink);
    logger.info("cat", "line1\nFAKE 2020 error [x] forged\r\tend");
    const auto rec = sink->records().at(0);
    QVERIFY(rec.message.find('\n') == std::string::npos);
    QVERIFY(rec.message.find('\r') == std::string::npos);
    QVERIFY(formatRecord(rec).find('\n') == std::string::npos);
  }

  void loggerTruncatesWithoutBreakingUtf8() {
    // "ع" is 2 bytes in UTF-8; an odd cut point would split it.
    std::string text;
    for (int i = 0; i < 2000; ++i) text += "\xD8\xB9";
    const std::string out = Logger::sanitize(text);
    QVERIFY(out.size() < text.size());
    const std::string tail = "...[truncated]";
    QVERIFY(out.size() > tail.size());
    const std::string body = out.substr(0, out.size() - tail.size());
    QCOMPARE(body.size() % 2, std::size_t{0});
    for (std::size_t i = 0; i < body.size(); i += 2) {
      QCOMPARE(static_cast<unsigned char>(body[i]), static_cast<unsigned char>(0xD8));
      QCOMPARE(static_cast<unsigned char>(body[i + 1]), static_cast<unsigned char>(0xB9));
    }
  }

  void parseLogLevel_data() {
    QTest::addColumn<QString>("text");
    QTest::addColumn<bool>("valid");
    QTest::newRow("info") << "info" << true;
    QTest::newRow("upper") << "WARNING" << true;
    QTest::newRow("bad") << "loud" << false;
  }
  void parseLogLevel() {
    QFETCH(QString, text);
    QFETCH(bool, valid);
    QCOMPARE(nova::core::parseLogLevel(text.toStdString()).has_value(), valid);
  }

  void settingsDefaultsAreLocalOnly() {
    Settings s(std::make_shared<MemorySettingsStore>());
    QVERIFY(!s.getBool("privacy.network_enabled").value());
    QVERIFY(!s.getBool("privacy.cloud_ai").value());
    QVERIFY(!s.getBool("privacy.crash_reports").value());
    QCOMPARE(QString::fromStdString(s.getString("ui.language").value()), QStringLiteral("ar"));
  }

  void settingsValidateKeysAndTypes() {
    Settings s(std::make_shared<MemorySettingsStore>());
    QCOMPARE(s.getBool("no.such.key").error().code, ErrorCode::NotFound);
    QCOMPARE(s.setBool("ui.language", true).error().code, ErrorCode::InvalidArgument);
    QVERIFY(s.setBool("privacy.crash_reports", true).ok());
    QVERIFY(s.getBool("privacy.crash_reports").value());
    QVERIFY(s.reset("privacy.crash_reports").ok());
    QVERIFY(!s.getBool("privacy.crash_reports").value());
  }

  void settingsDetectCorruptStoredValue() {
    auto store = std::make_shared<MemorySettingsStore>();
    store->write("privacy.network_enabled", "maybe");
    Settings s(store);
    QCOMPARE(s.getBool("privacy.network_enabled").error().code, ErrorCode::Corrupt);
  }
};

QTEST_APPLESS_MAIN(CoreTest)
#include "core_test.moc"
