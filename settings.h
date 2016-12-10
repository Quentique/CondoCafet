#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QGroupBox>
#include <QCheckBox>
#include <QTableView>
#include <QSettings>
#include <QLabel>
#include <QPoint>
#include <QAction>

class Settings : public QDialog
{
    Q_OBJECT
public:
    Settings();
    void fullInformation();

public slots:
    void customMenu(const QPoint &pos);
    void createRow();
    void deleteRow();
    void expandRow(QModelIndex index);
    void resizeRow(QModelIndex, QModelIndex index);
    void writeInformation();

private:
    QSettings *settings;
    QGroupBox *colours;
    QPushButton *ok, *cancel;
    QTableView *coloursT;
    QLabel *colours_explanation;
    QAction *delete_r, *create;
};

#endif // SETTINGS_H
