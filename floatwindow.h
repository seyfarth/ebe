#ifndef FLOATWINDOW_H
#define FLOATWINDOW_H

#include <QFrame>
#include <QString>
#include <QVBoxLayout>
#include <QLabel>


class FloatWindow : public QFrame
{
    Q_OBJECT

public:
    FloatWindow(QWidget *parent=0);

private slots:

private:
    void addRegister ( int n, QString value );
    QVBoxLayout *floatLayout;

    QLabel *floatLabels[16];
};

#endif
