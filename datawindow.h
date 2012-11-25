#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include <QFrame>
#include <QString>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
class QKeyEvent;
QT_END_NAMESPACE

class DataWindow : public QFrame
{
    Q_OBJECT

public:
    DataWindow(QWidget *parent=0);

private slots:

private:
    void addVariable ( QString name, QString value );
    QVBoxLayout *dataLayout;
};

#endif
