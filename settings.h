#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtGui>
#include <QTabWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>

class Settings: public QObject
{
    Q_OBJECT

public:
    Settings();
    void clear();
    bool read();
    void write();

private slots:

private:
    void setDefaults();
    QSettings *settings;
    QString fileName;
};

class SettingsDialog: public QDialog
{
    Q_OBJECT

public:
    SettingsDialog();

private slots:
    void save();

private:
    QTabWidget *tab;
    int chipSize;
};

class Spinner;
class ComboBox;

class SettingsFrame: public QFrame
{
    Q_OBJECT

public:
    SettingsFrame (QString);
    void addColorSetter(QString, QString);
    Spinner *addSpinner(QString, QString);
    void addCheckBox(QString, QString);
    void addLineEdit(QString, QString);
    ComboBox *addComboBox(QString, QString);
    QGridLayout *grid;
    QVBoxLayout *layout;
    void addStretch();

};

class ColorButton: public QPushButton
{
    Q_OBJECT

public:
    ColorButton (QString);
    QString var;
    QPixmap *pm;

private slots:
    void setColor();
};

class Spinner: public QSpinBox
{
    Q_OBJECT

public:
    Spinner (QString);
    QString var;

private slots:
    void saveValue(int);
};

class ComboBox: public QComboBox
{
    Q_OBJECT

public:
    ComboBox (QString);
    QString var;
    void setChoices(QStringList);

private slots:
    void saveValue(QString);
};

class LineEdit: public QLineEdit
{
    Q_OBJECT

public:
    LineEdit (QString);
    QString var;

private slots:
    void saveValue(QString);
};

class CheckBox: public QCheckBox
{
    Q_OBJECT

public:
    CheckBox (QString);
    QString var;

private slots:
    void saveValue(int);
};

class SettingsHash: public QVariantHash
{
public:
    QString os;

};

#ifdef SETTINGS_CPP
SettingsHash ebe;
int wordSize;
#else
extern SettingsHash ebe;
extern int wordSize;
#endif

#endif
