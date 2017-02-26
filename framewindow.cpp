#include "framewindow.h"
#include "asmdatawindow.h"
#include "registerwindow.h"
#include "floatwindow.h"
#include "settings.h"
#include "gdb.h"
#include <QDebug>
#include <cstdio>

extern FloatWindow *floatWindow;
extern RegisterWindow *registerWindow;
extern HalRegisterWindow *halRegisterWindow;
extern AsmDataWindow *asmDataWindow;
extern QMap<FileLine,FrameData*> frameData;
StringHash *itemNames=0;
StringHash *aliasNames=0;
StringHash *fpaliasNames=0;
extern StringHash halToIntel;
extern GDB *gdb;
IntHash items;
extern IntHash registerItems;
extern IntHash halItems;
extern IntHash floatItems;

FrameData::FrameData(int _currPars, int _locals, int _newPars )
    : currPars(_currPars), locals(_locals), newPars(_newPars)
{
    names = 0;
    aliasNames = 0;
    fpaliasNames = 0;
    unalias = "";
    fpunalias = "";
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
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(4);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(10, 10, 10, 10);
    //layout->setSpacing(5);
    limit = 0;
    rows = 5;
    table = new EZTable(this);
    buildTable();
    connect ( this, SIGNAL(requestStack(int)), gdb, SLOT(requestStack(int)) );
    connect ( gdb, SIGNAL(receiveStack(QStringList)),
              this, SLOT(receiveStack(QStringList)) );
    table->resizeToFitContents();
    scrollArea = new QScrollArea;
    scrollArea->setWidget(table);
    layout->addWidget(scrollArea);
    setLayout(layout);
}

void FrameWindow::rebuildTable()
{
    QString s;
    items.clear();
#ifdef Q_OS_WIN32
    int local5Row;
    rows = 6;
    if ( limit->currPars > 4 ) {
        rows += limit->currPars - 4;
        if ( rows & 1 ) rows++;
    }
    returnRow = rows - 2;
    local1Row = returnRow - 4;
    local5Row = returnRow + 2;
    //qDebug() << "returnRow" << returnRow << ";   local1Row" << local1Row << ";   local5Row" << local5Row;

    if ( limit->locals > 4 ) {
        rows += limit->locals - 4;
    }
    if ( limit->newPars > 4 ) {
        rows += limit->newPars;
    } else {
        rows += 4;
    }
    if ( rows & 1 ) rows++;
    
    table->setRowCount(rows);
    for ( int r=0; r < rows; r++ ) {
        table->setText(r,0,"");
        table->setText(r,1,"");
        table->setText(r,2,"");
    }

    //qDebug() << "curpars";
//
//  Set names and addresses for stacked parameters
//
    if ( limit->currPars > 4 ) {
        for ( int i=0; i < limit->currPars-4; i++ ) {
            s = QString("currPar%1").arg(i+5);
            items[s] = (returnRow-i-5)*10;
            if ( limit->names->contains(s) ) s = limit->names->value(s);
            table->setText(returnRow-i-5,0," "+s);
            table->setText(returnRow-i-5,2,QString("rbp+%1").arg((i+6)*8));
        }
    }

    //qDebug() << "retaddr";
//
//  Set names and addresses for saved rbp and return address
//
    table->setText(returnRow+1,0," prevRbp");
    table->setText(returnRow+1,2,"rbp");
    table->setText(returnRow,0," retAddr");
    table->setText(returnRow,2,"");

    int num=limit->locals;
    if ( num > 4 ) num = 4;

    //qDebug() << "locals 1-4";
//
//  Set names and addresses for locals less than 5
//
    for ( int i = 0; i <  num; i++ ) {
        s = QString("local%1").arg(i+1);
        items[s] = (local1Row-i)*10;
        if ( limit->names->contains(s) ) s = limit->names->value(s);
        table->setText(local1Row+i,0," "+s);
        table->setText(local1Row+i,2,QString("rbp+%1").arg((3-i+2)*8));
    }

    //qDebug() << "locals > 4";
//
//  Set names and addresses for locals greater than 4
//
    for ( int i = 4; i <  limit->locals; i++ ) {
        s = QString("local%1").arg(i+1);
        items[s] = (local5Row+i-5)*10;
        if ( limit->names->contains(s) ) s = limit->names->value(s);
        table->setText(local5Row+i-4,0," "+s);
        table->setText(local5Row+i-4,2,QString("rbp-%1").arg((i-3)*8));
    }

    //qDebug() << "locals" << limit->locals << ";   rows" << rows;
    if ( limit->locals > 4 && rows > local5Row+limit->locals-4) {
        //qDebug() << "empty row" << "locals" << limit->locals << ";   rows" << rows;
        table->setText(local5Row+limit->locals-4,0,"");
        table->setText(local5Row+limit->locals-4,2,"");
    }
    //qDebug() << "newpars";
    for ( int i = 5; i <= limit->newPars; i++ ) {
        s = QString("newPar%1").arg(i);
        items[s] = (rows-i)*10;
        if ( limit->names->contains(s) ) s = limit->names->value(s);
        table->setText(rows-i,0," "+s);
        table->setText(rows-i,2,QString("rsp+%1").arg((i-1)*8));
    }
    for ( int i = 1; i <= 4; i++ ) {
        s = QString("shadow%1").arg(i);
        table->setText(rows-i,0," "+s);
        table->setText(rows-i,2,QString("rsp+%1").arg((i-1)*8));
    }
    table->setText(rows-1,2,"rsp");
#else
    if ( wordSize == 64 ) {
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
        //qDebug() << "rows:" << rows << "   oldrows:" << oldRows;
        //for ( int r=0; r < oldRows; r++ ) {
            //table->item(r,0)->setText("");
            //table->item(r,2)->setText("");
        //}
        
        table->setRowCount(rows);
        for (int r = oldRows; r < rows; r++) {
            name = new QTableWidgetItem(QString(""));
            table->setItem(r,0,name);
            item = new EZCell(table);
            item->setValue(0);
            table->setItem(r,1,item);
            name = new QTableWidgetItem(QString(""));
            table->setItem(r,2,name);
        }
        if ( limit->currPars > 6 ) {
            table->item(0,0)->setText("");
            for ( int i=0; i < limit->currPars-6; i++ ) {
                s = QString("currPar%1").arg(i+7);
                items[s] = (returnRow-i-1)*10;
                if ( limit->names->contains(s) ) s = limit->names->value(s);
                table->setText(returnRow-i-1,0,s);
                table->setText(returnRow-i-1,2,QString("rbp+%1").arg((i+2)*8));
            }
        }

        table->setText(returnRow+1,0,"prevRbp");
        table->setText(returnRow+1,2,"rbp");
        table->setText(returnRow,0,"retAddr");
        table->setText(returnRow,2,"");
        StringHash d;
        d = *(limit->names);
        for ( int i = 0; i < limit->locals; i++ ) {
            s = QString("local%1").arg(i+1);
            items[s] = (local1Row+i)*10;
            //qDebug() << s << d;
            if ( limit->names->contains(s) ) s = limit->names->value(s);
            table->setText(local1Row+i,0,s);
            table->setText(local1Row+i,2,QString("rbp-%1").arg((i+1)*8));
        }
        for ( int i = 7; i <= limit->newPars; i++ ) {
            s = QString("newPar%1").arg(i);
            items[s] = (rows-i+6)*10;
            if ( limit->names->contains(s) ) s = limit->names->value(s);
            table->setText(rows-i+6,0,s);
            table->setText(rows-i+6,2,QString("rsp+%1").arg((i-7)*8));
        }
        if ( limit->locals == 0 && limit->newPars <= 6 ) {
            table->setText(rows-1,2,"rbp, rsp");
        } else if ( (limit->locals & 1) == 0 && limit->newPars <= 6 ){
            table->setText(
                rows-1,2,QString("rbp-%1, rsp").arg(limit->locals*8));
        } else {
            table->setText(rows-1,2,"rsp");
        }
    } else {
        rows = 2;
        rows += limit->currPars;
        returnRow = rows - 2;
        rows += limit->locals;
        local1Row = returnRow+2;
        rows += limit->newPars;
        int oldRows = table->rowCount();
        //qDebug() << "rows:" << rows << "   oldrows:" << oldRows;
        //for ( int r=0; r < oldRows; r++ ) {
            //table->setText(r,0,"");
            //table->setText(r,2,"");
        //}
        
        table->setRowCount(rows);
        for (int r = oldRows; r < rows; r++) {
            name = new QTableWidgetItem(QString(""));
            table->setItem(r,0,name);
            item = new EZCell(table);
            item->setValue(0);
            table->setItem(r,1,item);
            name = new QTableWidgetItem(QString(""));
            table->setItem(r,2,name);
        }
        //qDebug() << "reset names";
        table->item(0,0)->setText("");
        for ( int i=0; i < limit->currPars; i++ ) {
            s = QString("currPar%1").arg(i+1);
            items[s] = (returnRow-i-1)*10;
            if ( limit->names->contains(s) ) s = limit->names->value(s);
            table->setText(returnRow-i-1,0,s);
            table->setText(returnRow-i-1,2,QString("ebp+%1").arg((i+2)*4));
        }

        //qDebug() << "labeled currpars";
        table->setText(returnRow+1,2,"ebp");
        table->setText(returnRow,0,"retAddr");
        table->setText(returnRow,2,"");
        table->setText(returnRow+1,0,"prevEbp");
        StringHash d;
        d = *(limit->names);
        for ( int i = 0; i < limit->locals; i++ ) {
            s = QString("local%1").arg(i+1);
            items[s] = (local1Row+i)*10;
            //qDebug() << s << d;
            if ( limit->names->contains(s) ) s = limit->names->value(s);
            if ( local1Row+i < rows ) {
                table->setText(local1Row+i,0,s);
                table->setText(local1Row+i,2,QString("ebp-%1").arg((i+1)*4));
            }
        }
        //qDebug() << "labeled locals";
        for ( int i = 0; i < limit->newPars; i++ ) {
            s = QString("newPar%1").arg(i+1);
            items[s] = (rows-i-1)*10;
            if ( limit->names->contains(s) ) s = limit->names->value(s);
            table->setText(rows-i-1,0,s);
            if ( i > 0 ) {
                table->setText(rows-i-1,2,QString("esp+%1").arg((i)*4));
            }
        }
        //qDebug() << "labeled newpars";
        if ( limit->locals == 0 && limit->newPars == 0 ) {
            table->setText(rows-1,2,"ebp, esp");
        } else if ( (limit->locals > 0) == 0 && limit->newPars == 0 ){
            table->setText(
                rows-1,2,QString("ebp-%1, esp").arg(limit->locals*4));
        } else {
            table->setText(rows-1,2,"esp");
        }
        //qDebug() << "labeled last row";
    }
#endif

    table->resizeToFitContents();
}

void FrameWindow::receiveStack(QStringList results)
{
    uLong x;
    QStringList parts;
    EZCell *item;
    bool ok;
    int row = rows-1;
    //qDebug() << "rs";
    //qDebug() << "rs" << limit->unalias << items.keys();
    //qDebug() << "rs names" << *(limit->names);
    if ( wordSize == 64 &&
         limit->unalias != "" && halToIntel.count(limit->unalias) > 0 ) {
        StringHash::iterator it = limit->aliasNames->begin();
        while ( it != limit->aliasNames->end() ) {
            //qDebug() << it.key() << it.value() << limit->unalias;
            if ( it.value() == limit->unalias ) {
                QString name=it.key();
                int irow = halItems[name];
                if ( irow/10 < halRegisterWindow->table->rowCount() ) {
                    halRegisterWindow->table->setText(irow/10,irow%10,name+" ");
                }
                if ( !(name == "rip" || name == "eflags" ||
                       name == "rsp" || name == "rbp") ) {
                    limit->names->erase(it);
                }
                break;
            }
            it++;
        }
        limit->unalias = "";
    } 
    if ( limit->unalias != "" ) {
        StringHash::iterator it = limit->aliasNames->begin();
        while ( it != limit->aliasNames->end() ) {
            //qDebug() << it.key() << it.value() << limit->unalias;
            if ( it.value() == limit->unalias ) {
                QString name=it.key();
                int irow = registerItems[name];
                if ( irow/10 < registerWindow->table->rowCount() ) {
                    registerWindow->table->setText(irow/10,irow%10,name+" ");
                }
                limit->names->erase(it);
                break;
            }
            it++;
        }
        limit->unalias = "";
    }
    if ( limit->fpunalias != "" ) {
        StringHash::iterator it = limit->fpaliasNames->begin();
        while ( it != limit->fpaliasNames->end() ) {
            //qDebug() << it.key() << it.value() << limit->fpunalias;
            if ( it.value() == limit->fpunalias ) {
                QString name=it.key();
                int irow = floatItems[name];
                if ( irow/10 < floatWindow->table->rowCount() ) {
                    floatWindow->table->setText(irow/10,irow%10,name+" ");
                }
                limit->names->erase(it);
                break;
            }
            it++;
        }
        limit->unalias = "";
    }
    //qDebug() << "rs names" << *(limit->names);
    foreach (QString name, limit->names->keys()) {
        //qDebug() << "rs" << name;
        if ( items.contains(name) ) {
            int irow = items[name];
            //qDebug() << "rs" << name << irow/10 << irow%10;
            if ( irow/10 < rows ) {
                table->setText(irow/10,irow%10,limit->names->value(name)+" ");
            }
        }
    }
    foreach (QString name, limit->aliasNames->keys()) {
        //qDebug() << "rs" << name;
        if ( wordSize == 64 && halItems.contains(name) ) {
            //qDebug() << "rs" << name;
            int irow = halItems[name];
            if ( irow/10 < halRegisterWindow->table->rowCount() ) {
                halRegisterWindow->table-> setText(
                      irow/10,irow%10,limit->aliasNames->value(name)+" ");
            }
        }
        if ( registerItems.contains(name) ) {
            int irow = registerItems[name];
            if ( irow/10 < registerWindow->table->rowCount() ) {
                registerWindow->table-> setText(
                      irow/10,irow%10,limit->aliasNames->value(name)+" ");
            }
        }
    }
    foreach (QString name, limit->fpaliasNames->keys()) {
        //qDebug() << "rs" << name;
        if ( items.contains(name) ) {
            //qDebug() << "rs" << name << limit->fpaliasNames->value(name);
            int irow = items[name];
            if ( irow/10 < floatWindow->table->rowCount() ) {
                floatWindow->table->setText(
                    irow/10,irow%10,limit->fpaliasNames->value(name)+" ");
            }
        }
    }

    //qDebug() << "res" << results;
    for ( int i=0; i < results.length(); i++ ) {
        parts = results[i].split(QRegExp("\\s+"));
        //qDebug() << parts;
        for ( int j = 1; j < parts.length(); j++ ) {
            x = parts[j].toULongLong(&ok,16);
            item = table->cell(row,1);
            item->setValue(x);
            item->setText(item->value(),EZ::Highlight);
            row--;
        }
    }
    table->resizeToFitContents();
}

void FrameWindow::buildTable()
{
    table->setRowCount(rows);
    table->setColumnCount(3);

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < 3; c++) {
            table->setText(r,c,"");
        }
    }

    /*
     *  Set a tooltip to display when the cursor is over the table
     */
    table->setToolTip(tr("Right click on quad-word names to change formats.\n"
        "Right click on a quad-word's value to define a variable\n"
        "with the address contained in the quad-word."));
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
    //qDebug() << "setFont" << height << width;
    table->resizeToFitContents();
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
    int column = table->latestColumn;
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
    if ( table->rowCount() == 0 ) return;
    QString address = table->latestCell->text;
    DefineAsmVariableDialog *dialog = new DefineAsmVariableDialog;
    dialog->addressCombo->addItem(address);
    if (dialog->exec()) {
        AsmVariable var(dialog->name);
        var.size = dialog->size;
        var.address = dialog->address.toULongLong(0,0);
        var.format = dialog->format;
        var.values = new AllTypesArray(var.size);
        asmDataWindow->userDefinedVariables.append(var);
        asmDataWindow->variables.append(var);
        asmDataWindow->rebuildTable();
    }
    delete dialog;
}

/*
 *  Change the current table item's format to decimal.
 */
void FrameWindow::setDecimal()
{
    int row = table->latestRow;
    EZCell *item = table->cell(row,1);
    item->setFormat("decimal");
    item->setText(item->value());
    table->resizeToFitContents();
}

/*
 *  Change the current table item's format to hexadecimal.
 */
void FrameWindow::setHex()
{
    int row = table->latestRow;
    EZCell *item = table->cell(row,1);
    item->setFormat("hexdecimal");
    item->setText(item->value());
    table->resizeToFitContents();
}

/*
 *  Change all the register's formats to decimal.
 */
void FrameWindow::setDecimalAll()
{
    EZCell *item;
    for ( int i = 0; i < rows; i++ ) {
        item = table->cell(i,1);
        item->setFormat("decimal");
        item->setText(item->value());
    }
    table->resizeToFitContents();
}

/*
 *  Change all the register's formats to hexadecimal.
 */
void FrameWindow::setHexAll()
{
    EZCell *item;
    for ( int i = 0; i < rows; i++ ) {
        item = table->cell(i,1);
        item->setFormat("hexadecimal");
        item->setText(item->value());
    }
    table->resizeToFitContents();
}

void FrameWindow::setDouble()
{
    int row = table->latestRow;
    EZCell *item = table->cell(row,1);
    item->setFormat("double");
    item->setText(item->value());
    table->resizeToFitContents();
}

void FrameWindow::setFloat()
{
    int row = table->latestRow;
    EZCell *item = table->cell(row,1);
    item->setFormat("float");
    item->setText(item->value());
    table->resizeToFitContents();
}

void FrameWindow::setDoubleAll()
{
    EZCell *item;
    for ( int i = 0; i < rows; i++ ) {
        item = table->cell(i,1);
        item->setFormat("double");
        item->setText(item->value());
    }
    table->resizeToFitContents();
}

void FrameWindow::setFloatAll()
{
    EZCell *item;
    for ( int i = 0; i < rows; i++ ) {
        item = table->cell(i,1);
        item->setFormat("float");
        item->setText(item->value());
    }
    table->resizeToFitContents();
}

void FrameWindow::nextLine ( QString file, int line )
{
    FileLine fl;
    fl.file = file;
    fl.line = line;
    FrameData *lim = frameData[fl];

    if ( lim && wordSize == 32 ) {
        limit = lim;
        uLong esp, ebp;
        esp = registerWindow->regs["esp"]->toULong();
        ebp = registerWindow->regs["ebp"]->toULong();
        if ( ebp < esp ) return;
        limit->newPars = (ebp-esp)/4-limit->locals;
        rebuildTable();        
        emit requestStack(rows);
    } else {
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
}
