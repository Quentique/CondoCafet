#include "combodelegate.h"
#include <QComboBox>

ComboDelegate::ComboDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
}

QWidget* ComboDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *combo = new QComboBox(parent);
    combo->setFrame(false);
    combo->setStyleSheet("QComboBox { border: none; opacity: 0; }");
    combo->addItem("Red");
    combo->addItem("Orange", "#FFF000");

    return combo;
}

void ComboDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.data(Qt::EditRole).toString();
    QComboBox *combo = static_cast<QComboBox*>(editor);

    combo->setCurrentText(value);
}

void ComboDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *combo = static_cast<QComboBox*>(editor);
    model->setData(index, combo->currentText(), Qt::EditRole);
}
void ComboDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
