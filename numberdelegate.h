#ifndef NUMBERDELEGATE_H
#define NUMBERDELEGATE_H


#include <QStyledItemDelegate>

class NumberDelegate : public QStyledItemDelegate
{
Q_OBJECT
public:
    explicit NumberDelegate(QObject *parent = 0);
    virtual QString displayText(const QVariant &value, const QLocale &locale) const;

};
#endif // NUMBERDELEGATE_H
