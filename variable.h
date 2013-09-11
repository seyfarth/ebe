#ifndef VARIABLE_H
#define VARIABLE_H

#include <QtGui>
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QGridLayout>
#include <QPushButton>
#include <QSpinBox>

class DefineVariableDialog: public QDialog
{
    Q_OBJECT

public:
    DefineVariableDialog();
    QLineEdit *nameEdit;
    QLineEdit *addressEdit;
    QComboBox *typeCombo;
    QComboBox *formatCombo;
    QCheckBox *arrayCheck;
    QLineEdit *firstEdit;
    QLineEdit *lastEdit;
    QStringList result;
    QSize sizeHint() const;
    QGridLayout *layout;

public slots:
    void defineVariable();
    void checkChanged(int);
    void typeChanged(QString);

private:
    QPushButton *okButton;
    QPushButton *cancelButton;

signals:
};

class ArrayBoundsDialog: public QDialog
{
    Q_OBJECT

public:
    ArrayBoundsDialog();
    QCheckBox *arrayCheck;
    QSpinBox *firstSpin;
    QSpinBox *lastSpin;
    QSize sizeHint() const;
    void setMax(int max);
    void setMin(int min);
    int min;
    int max;

private:
    QPushButton *okButton;
    QPushButton *cancelButton;

public slots:
    void setArrayBounds();
    void checkChanged(int);

signals:
};

struct Limits
{
    int first;
    int last;
};

struct VariableDefinition
{
    QString name;
    QString type;
    QString value;
    QList<Limits> dimensions;
    bool isSimple;
    bool isFortran;
};

struct ClassDefinition
{
    QString name;
    QVector<VariableDefinition> members;
};

#endif
