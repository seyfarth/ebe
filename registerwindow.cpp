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

static char *names[5][4] = {
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
    setMidLineWidth(1);
    setContentsMargins(12,12,12,12);

    table = new QTableWidget(this);
    QVBoxLayout *layout = new QVBoxLayout;

    table->setRowCount(5);
    table->setColumnCount(8);
    QTableWidgetItem *name;
    QTableWidgetItem *value;
    table->verticalHeader()->hide();
    table->horizontalHeader()->hide();
    for ( int r = 0; r < 5; r++ ) {
        for ( int c = 0; c < 4; c++ ) {
            name = new QTableWidgetItem(names[r][c]+QString(" "));
            name->setTextAlignment(Qt::AlignRight);
            if ( r == 4 && c > 1 ) {
                value = new QTableWidgetItem("");
            } else {
                value = new QTableWidgetItem("0");
            }
            registerMap[names[r][c]] = value;
            table->setItem(r,c*2,name);
            table->setItem(r,c*2+1,value);
        }
    }
    table->resizeRowsToContents();
    table->resizeColumnsToContents();
    table->setShowGrid(false);

    layout->addWidget(table);
    setLayout(layout);
}

void RegisterWindow::setFontHeightAndWidth ( int height, int width )
{
    int max, length;
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

void RegisterWindow::setRegister ( QString name, QString value )
{
    if ( registerMap.contains(name) ) {
        registerMap[name]->setText(value);
    } else {
        qDebug() << "tried to set register " << name << endl;
    }
}
