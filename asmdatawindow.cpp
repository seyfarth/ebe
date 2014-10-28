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
#include <ctype.h>

extern GDB *gdb;

AsmVariable::AsmVariable(QString _name)
    : name(_name)
{
    address = 0;
    format = "hex1";
    size = 16;
    rows = 0;
    values.clear();
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
        variables[i].rows = (variables[i].size+15)/16;
        rows += variables[i].rows;
    }

    oldRows = table->rowCount();
    table->setRowCount(rows);
    for ( int r = 0; r < oldRows; r++ ) {
        table->item(r,0)->setText("");
        table->item(r,1)->setText("");
    }

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
    variables[i].values.clear();
    int row = variables[i].row;

    for (int j = 0; j < results.size(); j++ ) {
        t1 = results[j];
        parts = t1.split(":");
        if ( parts.size() > 1 ) {
            t1 = parts[1];
            results[j] = t1;
        }
    }
    for (int j = 0; j < results.size(); j += 2 ) {
        t1 = results[j];
        t1.replace(",","");
        t1.replace("(","");
        t1.replace(")","");
        t1.replace("0x","");
        if ( j+1 < results.size() ) {
            t2 = results[j+1];
            t2.replace(",","");
            t2.replace("(","");
            t2.replace(")","");
            t2.replace("0x","");
            t1 = t1 + t2;
        }
        t1.replace("\t"," ");
        t1.replace("  "," ");
        t1.remove(0,1);
        variables[i].values.append(t1);
        row++;
    }
    redisplay(i);
}

void AsmDataWindow::redisplay ( int v )
{
    AsmVariable var = variables[v];
    QString s;
    int values[16];
    QStringList parts;
    bool ok;
    AllTypes all;

    for ( int i = 0; i < var.rows; i++ ) {
        if ( var.values.size()-1 < i ) break;
        s = var.values[i];
        parts = s.split(" ");
        for ( int j = 0; j < parts.size(); j++ ) {
            values[j] = parts[j].toInt(&ok,16);
        }
        if ( var.format == "hex2" ) {
            s = "";
            for ( int j = 0; j < parts.size()-1; j += 2 ) {
                s += parts[j+1];
                s += parts[j];
                s += " ";
            }
        } else if ( var.format == "hex4" ) {
            s = "";
            for ( int j = 0; j < parts.size()-3; j += 4 ) {
                s += parts[j+3];
                s += parts[j+2];
                s += parts[j+1];
                s += parts[j];
                s += " ";
            }
        } else if ( var.format == "hex8" ) {
            s = "";
            for ( int j = 0; j < parts.size()-7; j += 8 ) {
                s += parts[j+7];
                s += parts[j+6];
                s += parts[j+5];
                s += parts[j+4];
                s += parts[j+3];
                s += parts[j+2];
                s += parts[j+1];
                s += parts[j];
                s += " ";
            }
        } else if ( var.format == "dec1" ) {
            s = "";
            for ( int j = 0; j < parts.size(); j++ ) {
                s += QString("%1 ").arg(values[j]);
            }
        } else if ( var.format == "dec2" ) {
            s = "";
            for ( int j = 0; j < parts.size()-1; j += 2 ) {
                all.u8 = 0;
                all.b[0] = values[j];
                all.b[1] = values[j+1];
                s += QString("%1 ").arg(all.i2);
            }
        } else if ( var.format == "dec4" ) {
            s = "";
            for ( int j = 0; j < parts.size()-3; j += 4 ) {
                all.u8 = 0;
                all.b[0] = values[j];
                all.b[1] = values[j+1];
                all.b[2] = values[j+2];
                all.b[3] = values[j+3];
                s += QString("%1 ").arg(all.i4);
            }
        } else if ( var.format == "dec8" ) {
            s = "";
            for ( int j = 0; j < parts.size()-7; j += 8 ) {
                all.u8 = 0;
                all.b[0] = values[j];
                all.b[1] = values[j+1];
                all.b[2] = values[j+2];
                all.b[3] = values[j+3];
                all.b[4] = values[j+4];
                all.b[5] = values[j+5];
                all.b[6] = values[j+6];
                all.b[7] = values[j+7];
                s += QString("%1 ").arg(all.i8);
            }
        } else if ( var.format == "float" ) {
            s = "";
            for ( int j = 0; j < parts.size()-3; j += 4 ) {
                all.u8 = 0;
                all.b[0] = values[j];
                all.b[1] = values[j+1];
                all.b[2] = values[j+2];
                all.b[3] = values[j+3];
                s += QString("%1 ").arg(all.f4);
            }
        } else if ( var.format == "double" ) {
            s = "";
            for ( int j = 0; j < parts.size()-7; j += 8 ) {
                all.u8 = 0;
                all.b[0] = values[j];
                all.b[1] = values[j+1];
                all.b[2] = values[j+2];
                all.b[3] = values[j+3];
                all.b[4] = values[j+4];
                all.b[5] = values[j+5];
                all.b[6] = values[j+6];
                all.b[7] = values[j+7];
                s += QString("%1 ").arg(all.f8);
            }
        } else if ( var.format == "char" ) {
            s = "";
            char t[3];
            t[1] = ' ';
            t[2] = 0;
            for ( int j = 0; j < parts.size(); j++ ) {
                if ( isprint(values[j]) ) {
                    t[0] = (char) values[j];
                    s += t;
                } else if ( values[j] == '\t' ) {
                    s += "\\t ";
                } else if ( values[j] == '\n' ) {
                    s += "\\n ";
                } else if ( values[j] == '\r' ) {
                    s += "\\r ";
                } else {
                    s += parts[j] + " ";
                }
            }
        }
        table->item(var.row+i,2)->setText(s);
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
    QMenu *sub;
    sub = menu.addMenu(tr("Decimal format"));
        sub->addAction("1 byte", this, SLOT(setDecimal1()));
        sub->addAction("2 bytes", this, SLOT(setDecimal2()));
        sub->addAction("4 bytes", this, SLOT(setDecimal4()));
        sub->addAction("8 bytes", this, SLOT(setDecimal8()));
    sub = menu.addMenu(tr("Hexadecimal format"));
        sub->addAction("1 byte", this, SLOT(setHex1()));
        sub->addAction("2 bytes", this, SLOT(setHex2()));
        sub->addAction("4 bytes", this, SLOT(setHex4()));
        sub->addAction("8 bytes", this, SLOT(setHex8()));
    menu.addAction(tr("Double format"), this, SLOT(setDouble()));
    menu.addAction(tr("Float format"), this, SLOT(setFloat()));
    menu.addAction(tr("Char format"), this, SLOT(setChar()));
    menu.addAction(tr("Define a variable with this address"), this,
        SLOT(defineVariableByAddress()));
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

/*
 *  Change the current table item's format to decimal.
 */
void AsmDataWindow::setDecimal1()
{
    setFormat("dec1");
}

void AsmDataWindow::setDecimal2()
{
    setFormat("dec2");
}

void AsmDataWindow::setDecimal4()
{
    setFormat("dec4");
}

void AsmDataWindow::setDecimal8()
{
    setFormat("dec8");
}

/*
 *  Change the current table item's format to hexadecimal.
 */
void AsmDataWindow::setHex1()
{
    setFormat("hex1");
}

void AsmDataWindow::setHex2()
{
    setFormat("hex2");
}

void AsmDataWindow::setHex4()
{
    setFormat("hex4");
}

void AsmDataWindow::setHex8()
{
    setFormat("hex8");
}

void AsmDataWindow::setFormat(QString format)
{
    int row = table->currentRow();
    int i;
    for ( i = 0; i < variables.size(); i++ ) {
        if ( row >= variables[i].row &&
             row <= variables[i].row + variables[i].rows -1 ) {
            break;
        }
    }
    if ( i == variables.size() ) {
        qDebug() << "Could not identify variable from row" << row;
        return;
    }
    variables[i].format = format;
    redisplay(i);
}

void AsmDataWindow::setChar()
{
    setFormat("char");
}

void AsmDataWindow::setDouble()
{
    setFormat("double");
}

void AsmDataWindow::setFloat()
{
    setFormat("float");
}
