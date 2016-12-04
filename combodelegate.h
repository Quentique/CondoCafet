#ifndef COMBODELEGATE_H
#define COMBODELEGATE_H

#include <QStyledItemDelegate>
#include <QComboBox>
#include <QMap>

class ComboDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    ComboDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const Q_DECL_OVERRIDE;

    virtual QString displayText(const QVariant &value, const QLocale &locale) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    static QMap<QString, QString> getColours();
private:
    static QMap<QString, int> my_map;
};
#endif // COMBODELEGATE_H
