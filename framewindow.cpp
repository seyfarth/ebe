#include "framewindow.h"
#include "datawindow.h"
#include "settings.h"
#include "gdb.h"
#include <QDebug>
#include <QHeaderView>
#include <QTableWidget>
#include <QMenu>
#include <QTableWidgetItem>
#include <cstdio>

extern DataWindow *dataWindow;
extern QMap<FileLine,FrameLimit*> frameLimits;
QMap<int,FrameItem*> frameItems;
extern GDB *gdb;

FrameLimit::FrameLimit(int _currPars, int _locals, int _newPars )
    : currPars(_currPars), locals(_locals), newPars(_newPars)
{
}

/*
 *  Constructor
 */
FrameWindow::FrameWindow(QWidget *parent)
    : QFrame (parent)
{
    /*
     *  For the mainwindow's saveState/restoreState and
     *  saveGeometry/restoreGeometry to work properly each dock widget and toolbar
     *  needs a unique object name set.
     */
    setObjectName("Stack Frame");
    limit = 0;
    rows = 0;
    buildTable();
    connect ( this, SIGNAL(requestStack(int)), gdb, SLOT(requestStack(int)) );
    connect ( gdb, SIGNAL(receiveStack(QStringList)),
              this, SLOT(receiveStack(QStringList)) );
}

void FrameWindow::rebuildTable()
{
    QTableWidgetItem *name;
    FrameItem *item;
#ifdef Q_WS_WIN
    int local5Row;
    rows = 6;
    if ( limit->currPars > 4 ) {
        rows += limit->currPars - 4;
        if ( rows & 1 ) rows++;
    }
    returnRow = rows - 2;
    local1Row = rows - 3;
    local5Row = rows+1;

    if ( limit->locals > 4 ) {
        rows += limit->locals - 4;
    }
    if ( limit->newPars > 4 ) {
        rows += limit->newPars;
    } else {
        rows += 4;
    }
    if ( rows & 1 ) rows++;
    
    int oldRows = table->rowCount();
    for ( int r=0; r < oldRows; r++ ) {
        table->item(r,0)->setText("");
        table->item(r,2)->setText("");
    }

    table->setRowCount(rows);
    for (int r = oldRows; r < rows; r++) {
        frameItems[r] = new FrameItem();
        name = new QTableWidgetItem(QString(""));
        table->setItem(r,0,name);
        item = new FrameItem;
        item->setValue(0);
        table->setItem(r,1,item);
        name = new QTableWidgetItem(QString(""));
        table->setItem(r,2,name);
    }
    if ( limit->currPars > 4 ) {
        table->item(0,0)->setText("");
        for ( int i=0; i < limit->currPars-4; i++ ) {
            table->item(returnRow-i-5,0)->
                   setText(QString("currPar%1").arg(i+5));
            table->item(returnRow-i-5,2)->
                   setText(QString("rbp+%1").arg((i+6)*8));
        }
    }

    table->item(returnRow+1,2)->setText("rbp");
    table->item(returnRow,0)->setText("retAddr");
    table->item(returnRow,2)->setText("");
    table->item(returnRow+1,0)->setText("prevRbp");

    int num=limit->locals;
    if ( num > 4 ) num = 4;

    for ( int i = 0; i <  num; i++ ) {
        table->item(local1Row-i,0)->setText(QString("local%1").arg(i+1));
        table->item(local1Row-i,2)->setText(QString("rbp+%1").arg((i+2)*8));
    }
    for ( int i = 4; i <  limit->locals; i++ ) {
        table->item(local5Row+i-5,0)->setText(QString("local%1").arg(i+1));
        table->item(local5Row+i-5,2)->setText(QString("rbp-%1").arg((i-3)*8));
    }

    if ( limit->locals > 4 && rows > local5Row+limit->locals-4) {
        table->item(local5Row+limit->locals-4,0)->setText("");
    }
    for ( int i = 5; i <= limit->newPars; i++ ) {
        table->item(rows-i,0)->setText(QString("newPar%1").arg(i));
        table->item(rows-i,2)->setText(QString("rsp+%1").arg((i-1)*8));
    }
    table->item(rows-1,2)->setText("rsp");
#else
    rows = 2;
    if ( limit->currPars > 6 ) {
        rows += limit->currPars - 6;
        if ( rows & 1 ) rows++;
    }
    returnRow = rows - 2;
    rows += limit->locals;
    local1Row = returnRow+2;
    if ( limit->newPars > 6 ) rows += limit->newPars - 6;
    if ( rows & 1 ) rows++;
    int oldRows = table->rowCount();
    for ( int r=0; r < oldRows; r++ ) {
        table->item(r,0)->setText("");
        table->item(r,2)->setText("");
    }
    
    table->setRowCount(rows);
    for (int r = oldRows; r < rows; r++) {
        frameItems[r] = new FrameItem();
        name = new QTableWidgetItem(QString(""));
        table->setItem(r,0,name);
        item = new FrameItem;
        item->setValue(0);
        table->setItem(r,1,item);
        name = new QTableWidgetItem(QString(""));
        table->setItem(r,2,name);
    }
    if ( limit->currPars > 6 ) {
        table->item(0,0)->setText("");
        for ( int i=0; i < limit->currPars-6; i++ ) {
            table->item(returnRow-i-1,0)->
                   setText(QString("currPar%1").arg(i+7));
            table->item(returnRow-i-1,2)->
                   setText(QString("rbp+%1").arg((i+2)*8));
        }
    }

    table->item(returnRow+1,2)->setText("rbp");
    table->item(returnRow,0)->setText("retAddr");
    table->item(returnRow,2)->setText("");
    table->item(returnRow+1,0)->setText("prevRbp");
    for ( int i = 0; i < limit->locals; i++ ) {
        table->item(local1Row+i,0)->setText(QString("local%1").arg(i+1));
        table->item(local1Row+i,2)->setText(QString("rbp-%1").arg((i+1)*8));
    }
    for ( int i = 7; i <= limit->newPars; i++ ) {
        table->item(rows-i+6,0)->setText(QString("newPar%1").arg(i));
        table->item(rows-i+6,2)->setText(QString("rsp+%1").arg((i-7)*8));
    }
    if ( limit->locals == 0 && limit->newPars <= 6 ) {
        table->item(rows-1,2)->setText("rbp, rsp");
    } else if ( (limit->locals & 1) == 0 && limit->newPars <= 6 ){
        table->item(rows-1,2)->
                   setText(QString("rbp-%1, rsp").arg(limit->locals*8));
    } else {
        table->item(rows-1,2)->setText("rsp");
    }
#endif

    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}

void FrameWindow::receiveStack(QStringList results)
{
    uLong x;
    QStringList parts;
    FrameItem *item;
    bool ok;
    int row = rows-1;
    for ( int i=0; i < results.length(); i++ ) {
        parts = results[i].split(QRegExp("\\s+"));
        x = parts[1].toULongLong(&ok,16);
        item = (FrameItem *)table->item(row,1);
        item->setValue(x);
        item->setText(item->value());
        item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        x = parts[2].toULongLong(&ok,16);
        item = (FrameItem *)table->item(row-1,1);
        item->setValue(x);
        item->setText(item->value());
        item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        row -= 2;
        if ( row < 0 ) break;
    }
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}

void FrameWindow::buildTable()
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

    QTableWidgetItem *name;
    table->verticalHeader()->hide();
    table->horizontalHeader()->hide();
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < 3; c++) {
            name = new QTableWidgetItem(QString("   "));
            table->setItem(r,c,name);
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
    //table->setShowGrid(false);

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
QSize FrameWindow::sizeHint() const
{
    return QSize(200, 200);
}

void FrameWindow::receiveFrame(StringHash)
{
}

/*
 *  This function sets the row heights and column widths based on
 *  the contents of the table and the current font size.
 */
void FrameWindow::setFontHeightAndWidth(int height, int width)
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
void FrameWindow::contextMenuEvent(QContextMenuEvent * /* event */)
{
    int column = table->currentColumn();
    QMenu menu(tr("Frame menu"));
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

void FrameWindow::defineVariableByAddress()
{
    QString address = table->currentItem()->text();
    DefineVariableDialog *dialog = new DefineVariableDialog;
    dialog->addressEdit->setText(address);
    if (dialog->exec()) {
        dialog->result.append("");
        dialog->result.append("");
        //emit sendVariableDefinition(dialog->result);
    }
    delete dialog;
}

/*
 *  Change the current table item's format to decimal.
 */
void FrameWindow::setDecimal()
{
    int row = table->currentRow();
    FrameItem *item = (FrameItem *)table->item(row,1);
    item->setFormat("decimal");
    item->setText(item->value());
    table->resizeColumnsToContents();
}

/*
 *  Change the current table item's format to hexadecimal.
 */
void FrameWindow::setHex()
{
    int row = table->currentRow();
    FrameItem *item = (FrameItem *)table->item(row,1);
    item->setFormat("hexdecimal");
    item->setText(item->value());
    table->resizeColumnsToContents();
}

/*
 *  Change all the register's formats to decimal.
 */
void FrameWindow::setDecimalAll()
{
    FrameItem *item;
    for ( int i = 0; i < rows; i++ ) {
        item = (FrameItem *)table->item(i,1);
        item->setFormat("decimal");
        item->setText(item->value());
    }
    table->resizeColumnsToContents();
}

/*
 *  Change all the register's formats to hexadecimal.
 */
void FrameWindow::setHexAll()
{
    FrameItem *item;
    for ( int i = 0; i < rows; i++ ) {
        item = (FrameItem *)table->item(i,1);
        item->setFormat("hexadecimal");
        item->setText(item->value());
    }
    table->resizeColumnsToContents();
}

void FrameWindow::setDouble()
{
    int row = table->currentRow();
    FrameItem *item = (FrameItem *)table->item(row,1);
    item->setFormat("double");
    item->setText(item->value());
    table->resizeColumnsToContents();
}

void FrameWindow::setFloat()
{
    int row = table->currentRow();
    FrameItem *item = (FrameItem *)table->item(row,1);
    item->setFormat("float");
    item->setText(item->value());
    table->resizeColumnsToContents();
}

void FrameWindow::setDoubleAll()
{
    FrameItem *item;
    for ( int i = 0; i < rows; i++ ) {
        item = (FrameItem *)table->item(i,1);
        item->setFormat("double");
        item->setText(item->value());
    }
    table->resizeColumnsToContents();
}

void FrameWindow::setFloatAll()
{
    FrameItem *item;
    for ( int i = 0; i < rows; i++ ) {
        item = (FrameItem *)table->item(i,1);
        item->setFormat("float");
        item->setText(item->value());
    }
    table->resizeColumnsToContents();
}

void FrameWindow::nextLine ( QString file, int line )
{
    FileLine fl;
    fl.file = file;
    fl.line = line;
    FrameLimit *lim = frameLimits[fl];

    if ( lim ) {
        if ( lim  != limit ) {
            limit = lim;
            rebuildTable();
        }
        emit requestStack(rows);
    } else {
        limit = 0;
        rows = 0;
        table->setRowCount(0);
    }
}

/*
 *  Constructor
 *  
 *  The parameter is the register's name.
 */
FrameItem::FrameItem()
    : QTableWidgetItem("")
{
    format = "hexadecimal";
    _value.u8 = 0;
}

/*
 *  Set the value of this FrameItem
 */
void FrameItem::setValue(uLong v)
{
    _value.u8 = v;
}

/*
 *  Set the format of this FrameItem
 */
void FrameItem::setFormat(QString f)
{
    format = f;
}

/*
 *  Get the value of this FrameItem as it should look in the table
 */
QString FrameItem::value()
{
    if (format == "decimal") {
        return QString::number(_value.i8,10);
    } else if (format == "double" ) {
        return QString::number(_value.f8);
    } else if ( format == "float" ) {
        return QString::number(_value.f4);
    } else {
        return QString("0x") + QString::number(_value.u8,16);
    }
}
