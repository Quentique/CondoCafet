#include "numberdelegate.h"

NumberDelegate::NumberDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
}

QString NumberDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    QString formattedNum = locale.toString(value.toDouble(), 'f', 2);
    formattedNum.append(" €");
    return formattedNum;
}
