#include "combodelegate.h"
#include "color_wheel.hpp"
#include <QDebug>
#include <QSettings>
#include <QCoreApplication>

ComboDelegate::ComboDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
}

QWidget* ComboDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *combo = new QComboBox(parent);
    combo->setFrame(false);
    combo->setStyleSheet("QComboBox { border: none; opacity: 0; }");
    QMap<QString, QVariant> map = getColours();
    for (QMap<QString, QVariant>::iterator it = map.begin() ; it!=map.end() ; it++)
    {
        combo->addItem(it.key(), it.value().toString());
    }
    return combo;
}

void ComboDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.data(Qt::EditRole).toString();
    QComboBox *combo = static_cast<QComboBox*>(editor);

    combo->setCurrentIndex(combo->findData(value));
}

void ComboDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *combo = static_cast<QComboBox*>(editor);
    model->setData(index, combo->currentData(), Qt::EditRole);
}
void ComboDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

QString ComboDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    return getColours().key(value);
}

QMap<QString, QVariant> ComboDelegate::getColours()
{
    QSettings *settings = new QSettings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
    QMap<QString, QVariant> map = settings->value("colours").toMap();

    return map;
}
