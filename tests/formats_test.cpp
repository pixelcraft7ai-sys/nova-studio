#include <QtTest>

#include "nova/formats/format_handler.hpp"

using namespace nova::formats;

namespace {
class FakePng final : public IFormatHandler {
 public:
  std::string id() const override { return "fake-png"; }
  std::string displayName() const override { return "Fake"; }
  std::vector<std::string> extensions() const override { return {"png"}; }
  bool canImport() const override { return false; }
  bool canExport() const override { return false; }
  nova::core::Result<std::unique_ptr<nova::document::Document>> import(const Bytes&) override {
    return nova::core::Error{nova::core::ErrorCode::Unsupported, "test"};
  }
  nova::core::Result<Bytes> exportDocument(const nova::document::Document&) override {
    return nova::core::Error{nova::core::ErrorCode::Unsupported, "test"};
  }
};
}  // namespace

class FormatsTest : public QObject {
  Q_OBJECT
 private slots:
  void startsEmpty() {
    FormatRegistry r;
    QVERIFY(r.ids().empty());
    QVERIFY(r.findByExtension("png") == nullptr);
  }
  void findsByExtensionCaseInsensitive() {
    FormatRegistry r;
    QVERIFY(r.registerHandler(std::make_shared<FakePng>()).ok());
    QVERIFY(r.findByExtension("PNG") != nullptr);
    QVERIFY(!r.registerHandler(std::make_shared<FakePng>()).ok());
    QVERIFY(!r.registerHandler(nullptr).ok());
  }
};

QTEST_APPLESS_MAIN(FormatsTest)
#include "formats_test.moc"
