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

class Settings : public QDialog
{
public:
    Settings();

private:
    QGroupBox *colours;
    QPushButton *ok, *cancel;
    QTableView *coloursT;
    QLabel *colours_explanation;
};

#endif // SETTINGS_H
