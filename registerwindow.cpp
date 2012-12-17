/*
    RegisterWindow class
    For display of general purpose registers, no public functions
    for set and get registers just yet
*/

#include "registerwindow.h"
#include "settings.h"
#include <QDebug>
#include <QHeaderView>
#include <QTableWidget>
#include <QString>
#include <QVBoxLayout>
#include <QTableWidgetItem>
#include <cstdio>

static QString names[5][4] = {
    { "rax", "rsi", "r8",  "r12" },
    { "rbx", "rdi", "r9",  "r13" },
    { "rcx", "rbp", "r10", "r14" },
    { "rdx", "rsp", "r11", "r15" },
    { "rip", "eflags", "", "" }
};

RegisterWindow::RegisterWindow(QWidget *parent)
: QFrame(parent)
{
    setObjectName("Register");
    setFrameStyle ( QFrame::Panel | QFrame::Raised );
    setLineWidth(4);

    table = new QTableWidget(this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(10,10,10,10);

    table->setRowCount(5);
    table->setColumnCount(8);
    QTableWidgetItem *name;
    QTableWidgetItem *val;
    table->verticalHeader()->hide();
    table->horizontalHeader()->hide();
    for ( int r = 0; r < 5; r++ ) {
        for ( int c = 0; c < 4; c++ ) {
            name = new QTableWidgetItem(names[r][c]+QString(" "));
            name->setTextAlignment(Qt::AlignRight);
            if ( r == 4 && c > 1 ) {
                val = new QTableWidgetItem("");
            } else {
                val = new QTableWidgetItem("0");
            }
            registerMap[names[r][c]] = val;
            table->setItem(r,c*2,name);
            table->setItem(r,c*2+1,val);
        }
    }
    table->resizeRowsToContents();
    table->resizeColumnsToContents();
    table->setShowGrid(false);

    layout->addWidget(table);
    setLayout(layout);

    namesList << "rax" << "rbx" << "rcx" << "rdx"
         << "rdi" << "rsi" << "rbp" << "rsp"
         << "r8"  << "r9"  << "r10" << "r11"
         << "r12" << "r13" << "r14" << "r15"
         << "rip" << "eflags";

    foreach ( QString name, namesList ) {
        regs[name] = new Register(name);
    }
}

QSize RegisterWindow::sizeHint() const
{
    return QSize(300,100);
}

void RegisterWindow::setFontHeightAndWidth ( int height, int width )
{
    int max, length;
    fontHeight = height;
    fontWidth = width;
    for ( int r = 0; r < 5; r++ ) {
        table->setRowHeight(r,height+3);
    }
    for ( int c = 0; c < 8; c++ ) {
        max = 1;
        for ( int r = 0; r < 5; r++ ) {
            length = table->item(r,c)->text().length();
            if ( length > max ) max = length;
        }
        table->setColumnWidth(c,(max+1)*width+3);
    }
}

void RegisterWindow::setRegister ( QString name, QString val )
{
    if ( registerMap.contains(name) ) {
        registerMap[name]->setText(val);
    } else {
        qDebug() << "tried to set register " << name << endl;
    }
}

void RegisterWindow::receiveRegs ( QMap<QString,QString> map )
{

    foreach ( QString key, map.keys() ) {
        setRegister(key,map[key]);
    }
    setFontHeightAndWidth(fontHeight,fontWidth);
}

Register::Register(QString n)
{
    name = n;
    format = "decimal";
    contents = "";
}

void Register::setValue(QString v)
{
    contents = v;
}

void Register::setFormat(QString f)
{
    format = f;
}

QString Register::value()
{
    long dec;
    bool ok;
    if ( name == "rip" || name == "eflags" ) {
        return contents;
    } else if ( format == "decimal" ) {
        dec = contents.toLong(&ok,16);
        return QString("%1").arg(dec);
    } else {
        return contents;
    }
}
