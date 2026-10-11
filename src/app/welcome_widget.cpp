#include "welcome_widget.hpp"

#include <QLabel>
#include <QVBoxLayout>

namespace nova::app {

WelcomeWidget::WelcomeWidget(const QString& version, bool localOnly, QWidget* parent) : QWidget(parent) {
  setObjectName(QStringLiteral("welcomeWidget"));
  auto* layout = new QVBoxLayout(this);
  layout->setAlignment(Qt::AlignCenter);

  auto* title = new QLabel(QStringLiteral("NOVA Studio"), this);
  QFont f = title->font();
  f.setPointSize(f.pointSize() + 14);
  f.setBold(true);
  title->setFont(f);
  title->setAlignment(Qt::AlignCenter);

  auto* subtitle = new QLabel(QStringLiteral("نوفا ستوديو — محطة إبداعية عربية أولًا"), this);
  subtitle->setAlignment(Qt::AlignCenter);

  auto* ver = new QLabel(QStringLiteral("الإصدار %1 (نسخة تأسيسية)").arg(version), this);
  ver->setAlignment(Qt::AlignCenter);

  auto* note = new QLabel(
      QStringLiteral("هذه نسخة تأسيسية للبنية الهندسية؛ أدوات التحرير تُضاف في المراحل التالية."), this);
  note->setAlignment(Qt::AlignCenter);
  note->setWordWrap(true);

  auto* privacy = new QLabel(localOnly ? QStringLiteral("الوضع المحلي: لا يُرسَل أي ملف إلى الشبكة.")
                                       : QStringLiteral("تنبيه: الوصول إلى الشبكة مفعّل في الإعدادات."),
                             this);
  privacy->setObjectName(QStringLiteral("privacyNote"));
  privacy->setAlignment(Qt::AlignCenter);

  for (auto* w : {title, subtitle, ver, note, privacy}) layout->addWidget(w);
}

}  // namespace nova::app
