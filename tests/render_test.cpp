#include <QtTest>

#include "nova/render/render_backend.hpp"

using namespace nova::render;

class RenderTest : public QObject {
  Q_OBJECT
 private slots:
  void nullBackendLifecycle() {
    NullRenderBackend b;
    QVERIFY(!b.isInitialized());
    QVERIFY(b.initialize().ok());
    QVERIFY(b.isInitialized());
    QVERIFY(!b.capabilities().gpuAccelerated);
    b.shutdown();
    QVERIFY(!b.isInitialized());
  }
  void registryCreatesAndRejects() {
    RenderBackendRegistry r;
    QVERIFY(r.registerBackend("null", [] { return std::make_unique<NullRenderBackend>(); }).ok());
    QVERIFY(!r.registerBackend("null", [] { return std::make_unique<NullRenderBackend>(); }).ok());
    QVERIFY(!r.registerBackend("", [] { return std::make_unique<NullRenderBackend>(); }).ok());
    auto made = r.create("null");
    QVERIFY(made.ok());
    QCOMPARE(QString::fromStdString(made.take()->name()), QStringLiteral("null"));
    QCOMPARE(r.create("vulkan").error().code, nova::core::ErrorCode::NotFound);
  }
};

QTEST_APPLESS_MAIN(RenderTest)
#include "render_test.moc"
