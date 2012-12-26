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
    void sendVariableDefinition(bool OK,QStringList);
};

struct VariableDefinition
{
    QString name;
    QString type;
    QVector<int> dimensions;
    bool isSimple;
};

struct ClassDefinition
{
    QString name;
    QVector<VariableDefinition> members;
};

#endif
