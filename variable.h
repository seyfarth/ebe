#ifndef VARIABLE_H
#define VARIABLE_H

#include <QtGui>

class DefineVariableDialog: public QDialog
{
    Q_OBJECT

public:
    DefineVariableDialog();
    QLineEdit *nameEdit;
    QLineEdit *addressEdit;
    QComboBox *formatCombo;
    QComboBox *sizeCombo;
    QLineEdit *firstEdit;
    QLineEdit *lastEdit;
    QStringList result;
    QSize sizeHint() const;

public slots:
    void defineVariable();

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
