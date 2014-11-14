#include "asmdatawindow.h"
#include "datawindow.h"
#include "settings.h"
#include "gdb.h"
#include <QLabel>
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
    userDefinedVariables.clear();
    varNames.clear();
    formatToRowCount.clear();
    formatToRowCount["char"] = 20;
    formatToRowCount["dec1"] = 10;
    formatToRowCount["dec2"] = 10;
    formatToRowCount["dec4"] = 5;
    formatToRowCount["dec8"] = 5;
    formatToRowCount["hex1"] = 20;
    formatToRowCount["hex2"] = 10;
    formatToRowCount["hex4"] = 5;
    formatToRowCount["hex8"] = 5;
    formatToRowCount["float"] = 5;
    formatToRowCount["double"] = 5;
    formatToSize["char"] = 1;
    formatToSize["dec1"] = 1;
    formatToSize["dec2"] = 2;
    formatToSize["dec4"] = 4;
    formatToSize["dec8"] = 8;
    formatToSize["hex1"] = 1;
    formatToSize["hex2"] = 2;
    formatToSize["hex4"] = 4;
    formatToSize["hex8"] = 8;
    formatToSize["float"] = 4;
    formatToSize["double"] = 8;
    qRegisterMetaType<uLong>("uLong");
    connect ( this, SIGNAL(requestAsmVariable(int,uLong,int)),
              gdb, SLOT(requestAsmVariable(int,uLong,int)) );
    connect ( gdb, SIGNAL(sendAsmVariable(int,QStringList)),
              this, SLOT(receiveAsmVariable(int,QStringList)) );
}

void AsmDataWindow::clear()
{
    variables.clear();
    userDefinedVariables.clear();
}

void AsmDataWindow::rebuildTable()
{
    QTableWidgetItem *item;
    int oldRows;
    int count;
    int num;

    //qDebug() << variables.size() << varNames;
    rows = 0;
    for ( int i=0; i < variables.size(); i++ ) {
        variables[i].row = rows;
        count = formatToRowCount[variables[i].format];
        num = variables[i].size/formatToSize[variables[i].format];
        if ( num < 1 ) num = 1;
        variables[i].rows = (num+count-1)/count;
        rows += variables[i].rows;
    }

    oldRows = table->rowCount();
    //qDebug() << "rows" << rows << ";   oldrows" << oldRows;
    table->setRowCount(rows);

    if ( oldRows > rows ) oldRows = rows;

    for ( int r = 0; r < oldRows; r++ ) {
        table->item(r,0)->setText("");
        table->item(r,1)->setText("");
    }

    //qDebug() << "Cleared old names and addresses";
    for ( int r = oldRows; r < rows; r++ ) {
        for (int c = 0; c < 3; c++) {
            item = new QTableWidgetItem(QString("   "));
            table->setItem(r,c,item);
        }
    } 

    //qDebug() << "Cleared new rows";

    for ( int i=0; i < variables.size(); i++ ) {
        //qDebug() << "var" << i << variables[i].name;
        int r = variables[i].row;
        item = table->item(r,0);
        variables[i].item = item;
        item->setText(QString("0x%1 ").arg(variables[i].address,0,16));
        item = table->item(r,1);
        item->setText(variables[i].name+" ");
    }
    table->resizeColumnsToContents();
    table->resizeRowsToContents();

    for ( int i=0; i < variables.size(); i++ ) {
        emit requestAsmVariable ( i, variables[i].address, variables[i].size );
    }
    //qDebug() << "requested data";
}

void AsmDataWindow::receiveAsmVariable ( int i, QStringList results )
{
    QString t1, t2;
    QStringList parts;
    bool ok;
    int k=0;

    //qDebug() << "rav" << i << results;
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
        parts = t1.split(" ");
        int n = parts.length();
        for ( int r=0; r < n; r++ ) {
            variables[i].values->u1(k) = parts[r].toInt(&ok,16);
            k++;
        }
    }
    redisplay(i);
}

void AsmDataWindow::redisplay ( int v )
{
    QString s;
    QString t;
    QStringList parts;
    int size = variables[v].size;
    int row = variables[v].item->row();
    int rows = variables[v].rows;
    int left = size;
    int max;
    int k = 0;
    int count;
    QString format = variables[v].format;
    AllTypesArray *values = variables[v].values;
    int num;
    int newRows;

    count = formatToRowCount[format];
    num = size/formatToSize[format];
    if ( num < 1 ) num = 1;
    newRows = (num + count - 1) / count;

    if ( newRows > rows ) {
        for ( int r = rows; r < newRows; r++ ) {
            table->insertRow(row+1);
            QTableWidgetItem *item;
            for (int c = 0; c < 3; c++) {
                item = new QTableWidgetItem(QString(""));
                table->setItem(row+1,c,item);
            }
        }
        rows = variables[v].rows = newRows;
    } else if ( newRows < rows ) {
        for ( int r = newRows; r < rows; r++ ) {
            table->removeRow(row+1);
        }
        rows = variables[v].rows = newRows;
    }

    for ( int r = row; r < row+rows; r++ ) {
        if ( format == "char") {
            s = "";
            max = count;
            if ( max > left ) max = left;
            for ( int j=0; j < max; j++ ) {
                if ( isprint(values->c1(k)) ) {
                    s += QString(QChar(values->c1(k))) + " ";
                } else if ( values->c1(k) == '\t' ) {
                    s += "\\t ";
                } else if ( values->c1(k) == '\n' ) {
                    s += "\\n ";
                } else if ( values->c1(k) == '\r' ) {
                    s += "\\r ";
                } else {
                    s += QString("%1 ").arg(values->u1(k),2,16,QChar('0'));
                }
                k++;
            }
            left -= max;
        } else if ( format == "hex1") {
            s = "0x";
            max = count;
            if ( max > left ) max = left;
            for ( int j=0; j < max; j++ ) {
                s += QString("%1 ").arg(values->u1(k),2,16,QChar('0'));
                k++;
            }
            left -= max;
        } else if ( format == "hex2") {
            s = "";
            max = count;
            if ( max > left/2 ) max = left/2;
            for ( int j=0; j < max; j++ ) {
                s += QString("0x%1 ").arg(values->u2(k),0,16);
                k++;
            }
            left -= max;
        } else if ( format == "hex4") {
            s = "";
            max = count;
            if ( max > left/4 ) max = left/4;
            for ( int j=0; j < max; j++ ) {
                s += QString("0x%1 ").arg(values->u4(k),0,16);
                k++;
            }
            left -= max;
        } else if ( format == "hex8") {
            s = "";
            max = count;
            if ( max > left/8 ) max = left/8;
            for ( int j=0; j < max; j++ ) {
                s += QString("0x%1 ").arg(values->u8(k),0,16);
                k++;
            }
            left -= max;
        } else if ( format == "dec1") {
            s = "";
            max = count;
            if ( max > left ) max = left;
            for ( int j=0; j < max; j++ ) {
                s += QString("%1 ").arg(values->i1(k));
                k++;
            }
            left -= max;
        } else if ( format == "dec2") {
            s = "";
            max = count;
            if ( max > left/2 ) max = left/2;
            for ( int j=0; j < max; j++ ) {
                s += QString("%1 ").arg(values->i2(k));
                k++;
            }
            left -= max;
        } else if ( format == "dec4") {
            s = "";
            max = count;
            if ( max > left/4 ) max = left/4;
            for ( int j=0; j < max; j++ ) {
                s += QString("%1 ").arg(values->i4(k));
                k++;
            }
            left -= max;
        } else if ( format == "dec8") {
            s = "";
            max = count;
            if ( max > left/8 ) max = left/8;
            for ( int j=0; j < max; j++ ) {
                s += QString("%1 ").arg(values->i8(k));
                k++;
            }
            left -= max;
        } else if ( format == "double") {
            s = "";
            max = count;
            if ( max > left/8 ) max = left/8;
            for ( int j=0; j < max; j++ ) {
                s += QString("%1 ").arg(values->f8(k));
                k++;
            }
            left -= max;
        } else if ( format == "float") {
            s = "";
            max = count;
            if ( max > left/4 ) max = left/4;
            for ( int j=0; j < max; j++ ) {
                s += QString("%1 ").arg(values->f4(k));
                k++;
            }
            left -= max;
        }
        table->item(r,2)->setText(s);
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
    table->setToolTip(tr("Right click on variable names to change formats."));

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
    menu.addAction(tr("Delete variable"), this, SLOT(deleteVariable()));
    menu.exec(QCursor::pos());
}

void AsmDataWindow::defineVariableByAddress()
{
    int row = table->currentRow();
    QStringList parts = table->item(row,2)->text().split(" ");
    DefineAsmVariableDialog *dialog = new DefineAsmVariableDialog;
    dialog->addressCombo->addItems(parts);
    if (dialog->exec()) {
        //qDebug() << "define var" << dialog->name << dialog->address
                 //<< dialog->size << dialog->format;
        AsmVariable var(dialog->name);
        var.size = dialog->size;
        var.address = dialog->address.toULongLong(0,0);
        var.format = dialog->format;
        var.values = new AllTypesArray(var.size);
        userDefinedVariables.append(var);
        variables.append(var);
        rebuildTable();
    }
    delete dialog;
}

void AsmDataWindow::deleteVariable()
{
    int row = table->currentRow();

    for ( int i=0; i < variables.size(); i++ ) {
        if ( variables[i].item == table->item(row,0) ) {
            for ( int r = row; r < row+variables[i].rows; r++ ) {
                table->removeRow(row);
            }
            variables.remove(i);
            break;
        }
    }
}

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
    for ( i = 0; i < variables.size(); i++ ) {
        variables[i].row = variables[i].item->row();
    }
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

DefineAsmVariableDialog::DefineAsmVariableDialog()
    : QDialog()
{
    setObjectName("Define ASM Variable");
    setWindowTitle(tr("Define ASM Variable"));
    setModal(true);
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);

    move(QCursor::pos());

    layout = new QGridLayout;
    layout->setSpacing(5);
    layout->setContentsMargins(10, 10, 10, 10);

    layout->addWidget(new QLabel(tr("Name")), 0, 0);
    nameEdit = new QLineEdit;
    layout->addWidget(nameEdit, 0, 1);

    layout->addWidget(new QLabel(tr("Address")), 1, 0);
    addressCombo = new QComboBox;
    layout->addWidget(addressCombo, 1, 1);
    addressCombo->setEditable(true);

    layout->addWidget(new QLabel(tr("Size")), 2, 0);
    sizeSpin = new QSpinBox;
    layout->addWidget(sizeSpin, 2, 1);
    sizeSpin->setValue(8);
    sizeSpin->setRange(1,80000);

    layout->addWidget(new QLabel(tr("Format")), 3, 0);
    formatCombo = new QComboBox;
    layout->addWidget(formatCombo, 3, 1);

    QStringList formats;
    formats << "dec1" << "dec2" << "dec4" << "dec8"
            << "hex1" << "hex2" << "hex4" << "hex8"
            << "double" << "float" << "char";
    formatCombo->addItems(formats);

    okButton = new QPushButton("OK");
    cancelButton = new QPushButton(tr("Cancel"));
    connect ( okButton, SIGNAL(clicked()), this, SLOT(defineVariable()));
    connect ( cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    
    layout->addWidget(okButton, 4, 0);
    layout->addWidget(cancelButton, 4, 1);

    setLayout(layout);
    formatCombo->setCurrentIndex(4);
}

QSize DefineAsmVariableDialog::sizeHint() const
{
    return QSize(200, 300);
}

void DefineAsmVariableDialog::defineVariable()
{
    name = nameEdit->text();
    address = addressCombo->currentText();
    size = sizeSpin->value();
    format = formatCombo->currentText();
    accept();
}
