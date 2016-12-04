#include "cmath"
#include "numberdelegate.h"
#include <QDoubleSpinBox>

NumberDelegate::NumberDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
}

QString NumberDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    QString formattedNum = locale.toString(value.toDouble(), 'f', 2);
    formattedNum.append(" €");
    return formattedNum;
}

QWidget *NumberDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex &/* index */) const
{
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setDecimals(2);
    editor->setSingleStep(0.05);
    editor->setFrame(false);
    editor->setMinimum(0);
    editor->setMaximum(100);

    return editor;
}

void NumberDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    double value = index.data(Qt::EditRole).toDouble();

    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue(value);
}

void NumberDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    double value = round(spinBox->value()*20)/20;

    model->setData(index, value, Qt::EditRole);
}
void NumberDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
