#ifndef VARIABLE_H
#define VARIABLE_H

#include <QtGui>

class VariableFrame: public QFrame
{
    Q_OBJECT

public:
    VariableFrame();
    QLineEdit *nameEdit;
    QLineEdit *addressEdit;
    QComboBox *formatCombo;
    QComboBox *sizeCombo;
    QLineEdit *firstEdit;
    QLineEdit *lastEdit;
    QSize sizeHint() const;

public slots:
    void defineVariable();
    void cancel();

private:
    QPushButton *okButton;
    QPushButton *cancelButton;

signals:
    void sendVariableDefinition(bool OK,QStringList);
};

#endif
