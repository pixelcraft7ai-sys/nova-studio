#include "nova/document/document.hpp"

#include <QtTest>

using nova::document::Document;

class DocumentTest : public QObject {
  Q_OBJECT
 private slots:
  void tracksRevisionAndModified() {
    Document d("a");
    QVERIFY(!d.isModified());
    d.setName("b");
    QCOMPARE(d.revision(), std::uint64_t{1});
    QVERIFY(d.isModified());
    d.markSaved();
    QVERIFY(!d.isModified());
    d.setName("b");  // no change -> no revision bump
    QCOMPARE(d.revision(), std::uint64_t{1});
  }
  void idsAreUnique() {
    Document a("x"), b("x");
    QCOMPARE(a.id().size(), std::size_t{32});
    QVERIFY(a.id() != b.id());
  }
};

QTEST_APPLESS_MAIN(DocumentTest)
#include "document_test.moc"
