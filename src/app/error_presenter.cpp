#include "error_presenter.hpp"

#include <QMessageBox>

namespace nova::app {

QString userMessageFor(core::ErrorCode code) {
  switch (code) {
    case core::ErrorCode::NotFound:
      return QStringLiteral("لم يتم العثور على الملف المطلوب.");
    case core::ErrorCode::PermissionDenied:
      return QStringLiteral("لا تملك صلاحية الوصول إلى هذا الملف.");
    case core::ErrorCode::IoError:
      return QStringLiteral("تعذّرت قراءة الملف أو كتابته. تحقّق من المساحة المتاحة والصلاحيات.");
    case core::ErrorCode::Corrupt:
      return QStringLiteral("الملف تالف أو غير مكتمل.");
    case core::ErrorCode::Unsupported:
      return QStringLiteral("هذه الميزة أو الصيغة غير مدعومة بعد.");
    case core::ErrorCode::InvalidArgument:
      return QStringLiteral("الطلب غير صالح.");
    case core::ErrorCode::Cancelled:
      return QStringLiteral("أُلغيت العملية.");
    case core::ErrorCode::Unknown:
      break;
  }
  return QStringLiteral("حدث خطأ غير متوقع.");
}

void MessageBoxErrorPresenter::present(const core::Error& error, const QString& context) {
  QMessageBox box(QMessageBox::Warning, QStringLiteral("NOVA Studio"), userMessageFor(error.code));
  if (!context.isEmpty()) box.setInformativeText(context);
  box.setDetailedText(QString::fromStdString(std::string(core::toString(error.code)) + ": " + error.message));
  box.exec();
}

}  // namespace nova::app
