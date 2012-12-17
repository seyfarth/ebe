#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

/*
    RegisterWindow class
    For display of general purpose registers, no public functions
    for set and get registers just yet
*/

#include <QMap>
#include <QFrame>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStringList>
#include <QString>
#include <QMap>
#include <QSize>

class Register
{
    public:
    Register(QString name);
    QString value();
    void setValue(QString x);
    void setFormat(QString f);

    private:
    QString contents;
    QString format;
    QString name;
};

class RegisterWindow : public QFrame
{
    Q_OBJECT

public:
    RegisterWindow(QWidget *parent = 0);
    void setFontHeightAndWidth ( int height, int width );
    void setRegister ( QString name, QString value );

private:
    QMap<QString,Register *> regs;
    QMap<QString,QTableWidgetItem *> registerMap;
    QTableWidget *table;
    QSize sizeHint() const;
    QStringList namesList;
    int fontWidth;
    int fontHeight;
    void contextMenuEvent(QContextMenuEvent *event);

public slots:
    void receiveRegs(QMap<QString,QString>);
    void setDecimal();
    void setHex();
    void setDecimalAll();
    void setHexAll();

};

#endif

