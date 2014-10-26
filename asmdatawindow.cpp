#include "asmdatawindow.h"
#include "datawindow.h"
#include "settings.h"
#include "gdb.h"
#include <QDebug>
#include <QHeaderView>
#include <QTableWidget>
#include <QMenu>
#include <QTableWidgetItem>
#include <cstdio>

extern GDB *gdb;

AsmVariable::AsmVariable(QString _name)
    : name(_name)
{
    address = 0;
    type = "unsigned char";
    size = 16;
}

/*
 *  Constructor
 */
AsmDataWindow::AsmDataWindow(QWidget *parent)
    : QFrame (parent)
{
    /*
     *  For the mainwindow's saveState/restoreState and
     *  saveGeometry/restoreGeometry to work properly each dock widget and toolbar
     *  needs a unique object name set.
     */
    setObjectName("Assembly Data");
    rows = 0;
    buildTable();
    variables.clear();
    varNames.clear();
    qRegisterMetaType<uLong>("uLong");
    connect ( this, SIGNAL(requestAsmVariable(int,uLong,int)),
              gdb, SLOT(requestAsmVariable(int,uLong,int)) );
    connect ( gdb, SIGNAL(sendAsmVariable(int,QStringList)),
              this, SLOT(receiveAsmVariable(int,QStringList)) );
}

void AsmDataWindow::rebuildTable()
{
    QTableWidgetItem *item;
    int oldRows;

    rows = 0;
    for ( int i=0; i < variables.size(); i++ ) {
        variables[i].row = rows;
        rows += (variables[i].size+15)/16;
    }
    //qDebug() << rows;

    oldRows = table->rowCount();
    table->setRowCount(rows);
    for ( int r = oldRows; r < rows; r++ ) {
        for (int c = 0; c < 3; c++) {
            item = new QTableWidgetItem(QString("   "));
            table->setItem(r,c,item);
        }
    } 

    if ( rows != oldRows ) {
        for ( int i=0; i < variables.size(); i++ ) {
            int r = variables[i].row;
            item = table->item(r,0);
            item->setText(QString("0x%1 ").arg(variables[i].address,0,16));
            item = table->item(r,1);
            item->setText(variables[i].name+" ");
        }
    }
    table->resizeColumnsToContents();
    table->resizeRowsToContents();

    for ( int i=0; i < variables.size(); i++ ) {
        emit requestAsmVariable ( i, variables[i].address, variables[i].size );
    }
}

void AsmDataWindow::receiveAsmVariable ( int i, QStringList results )
{
    QString t1, t2;
    QStringList parts;
    //qDebug() << results;
    int row = variables[i].row;
    for (int i = 0; i < results.size(); i += 2 ) {
        t1 = results[i];
        parts = t1.split(":");
        if ( parts.size() > 1 ) t1 = parts[1];
        t1.replace(",","");
        t1.replace("(","");
        t1.replace(")","");
        t1.replace("0x","");
        if ( i+1 < results.size() ) {
            t2 = results[i+1];
            parts = t2.split(":");
            if ( parts.size() > 1 ) t2 = parts[1];
            t2.replace(",","");
            t2.replace("(","");
            t2.replace(")","");
            t2.replace("0x","");
            t1 = t1 + t2;
        }
        t1.replace("\t"," ");
        t1.replace("  "," ");
        table->item(row,2)->setText(t1);
        row++;
    }
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}

void AsmDataWindow::buildTable()
{
    /*
     *  Set the frame to be raised with a width 4 bevel.
     */
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(4);

    /*
     *  Create a table to display the registers
     */
    table = new QTableWidget(this);

    /*
     *  We need a layout for the table widget
     */
    layout = new QVBoxLayout;
    layout->setSpacing(5);
    
    /*
     *  Leave 10 pixels all around the table
     */
    layout->setContentsMargins(10, 10, 10, 10);

    table->setRowCount(rows);
    table->setColumnCount(3);

    QTableWidgetItem *item;
    table->verticalHeader()->hide();
    table->horizontalHeader()->hide();
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < 3; c++) {
            item = new QTableWidgetItem(QString("   "));
            table->setItem(r,c,item);
        }
    }

    /*
     *  Resizing based on size hints which is not too accurate
     */
    table->resizeColumnsToContents();
    table->resizeRowsToContents();

    /*
     *  Don't show a grid
     */
    table->setShowGrid(false);

    /*
     *  Set a tooltip to display when the cursor is over the table
     */
    table->setToolTip(tr("Right click on quad-word names to change formats.\n"
        "Right click on a quad-word's value to define a variable\n"
        "with the address contained in the quad-word."));

    /*
     *  Add the table to the layout and set the layout for the frame.
     */
    layout->addWidget(table);
    setLayout(layout);
}

/*
 *  Crude size hint.  50 rows is not enough to display much, but it allows
 *  the user to shrink the register window.  300 columns is probably not
 *  enough.
 */
QSize AsmDataWindow::sizeHint() const
{
    return QSize(200, 200);
}


/*
 *  This function sets the row heights and column widths based on
 *  the contents of the table and the current font size.
 */
void AsmDataWindow::setFontHeightAndWidth(int height, int width)
{
    fontHeight = height;
    fontWidth = width;
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}

/*
 *  Slot triggered by the gdb class sending a map of register
 *  values.
 */

/*
 *  Virtual function from the QFrame class over-ridden to provide
 *  popup menus for the register window.
 */
void AsmDataWindow::contextMenuEvent(QContextMenuEvent * /* event */)
{
    int column = table->currentColumn();
    QMenu menu(tr("Assembly data menu"));
    if (column % 2 == 0) {
        /*
         *      If the column is an even number, this is a register name
         *      and the menu needs to allow changing formats.
         */
        menu.addAction(tr("Decimal format"), this, SLOT(setDecimal()));
        menu.addAction(tr("Hexadecimal format"), this, SLOT(setHex()));
        menu.addAction(tr("Double format"), this, SLOT(setDouble()));
        menu.addAction(tr("Float format"), this, SLOT(setFloat()));
        menu.addSeparator();
        menu.addAction(tr("Decimal format - all"), this, SLOT(setDecimalAll()));
        menu.addAction(tr("Hexadecimal format - all"), this, SLOT(setHexAll()));
        menu.addAction(tr("Double format - all"), this, SLOT(setDoubleAll()));
        menu.addAction(tr("Float format - all"), this, SLOT(setFloatAll()));
    } else {
        /*
         *      If the column is odd, then this is a register value which
         *      can be used to create a variable with the address specified
         *      by the register.  This is only important for assembly.
         */
        menu.addAction(tr("Define a variable with this address"), this,
            SLOT(defineVariableByAddress()));
    }
    menu.exec(QCursor::pos());
}

//void AsmDataWindow::defineVariableByAddress()
//{
//    QString address = table->currentItem()->text();
//    DefineVariableDialog *dialog = new DefineVariableDialog;
//    dialog->addressEdit->setText(address);
//    if (dialog->exec()) {
//        dialog->result.append("");
//        dialog->result.append("");
//        //emit sendVariableDefinition(dialog->result);
//    }
//    delete dialog;
//}

///*
// *  Change the current table item's format to decimal.
// */
//void AsmDataWindow::setDecimal()
//{
//    int row = table->currentRow();
//    FrameItem *item = (FrameItem *)table->item(row,1);
//    item->setFormat("decimal");
//    item->setText(item->value());
//    table->resizeColumnsToContents();
//}
//
///*
// *  Change the current table item's format to hexadecimal.
// */
//void AsmDataWindow::setHex()
//{
//    int row = table->currentRow();
//    FrameItem *item = (FrameItem *)table->item(row,1);
//    item->setFormat("hexdecimal");
//    item->setText(item->value());
//    table->resizeColumnsToContents();
//}
//
///*
// *  Change all the register's formats to decimal.
// */
//void AsmDataWindow::setDecimalAll()
//{
//    FrameItem *item;
//    for ( int i = 0; i < rows; i++ ) {
//        item = (FrameItem *)table->item(i,1);
//        item->setFormat("decimal");
//        item->setText(item->value());
//    }
//    table->resizeColumnsToContents();
//}
//
///*
// *  Change all the register's formats to hexadecimal.
// */
//void AsmDataWindow::setHexAll()
//{
//    FrameItem *item;
//    for ( int i = 0; i < rows; i++ ) {
//        item = (FrameItem *)table->item(i,1);
//        item->setFormat("hexadecimal");
//        item->setText(item->value());
//    }
//    table->resizeColumnsToContents();
//}
//
//void AsmDataWindow::setDouble()
//{
//    int row = table->currentRow();
//    FrameItem *item = (FrameItem *)table->item(row,1);
//    item->setFormat("double");
//    item->setText(item->value());
//    table->resizeColumnsToContents();
//}
//
//void AsmDataWindow::setFloat()
//{
//    int row = table->currentRow();
//    FrameItem *item = (FrameItem *)table->item(row,1);
//    item->setFormat("float");
//    item->setText(item->value());
//    table->resizeColumnsToContents();
//}
//
//void AsmDataWindow::setDoubleAll()
//{
//    FrameItem *item;
//    for ( int i = 0; i < rows; i++ ) {
//        item = (FrameItem *)table->item(i,1);
//        item->setFormat("double");
//        item->setText(item->value());
//    }
//    table->resizeColumnsToContents();
//}
//
//void AsmDataWindow::setFloatAll()
//{
//    FrameItem *item;
//    for ( int i = 0; i < rows; i++ ) {
//        item = (FrameItem *)table->item(i,1);
//        item->setFormat("float");
//        item->setText(item->value());
//    }
//    table->resizeColumnsToContents();
//}
