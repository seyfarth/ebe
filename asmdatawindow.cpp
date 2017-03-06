#include "asmdatawindow.h"
#include "datawindow.h"
#include "settings.h"
#include "sourceframe.h"
#include "gdb.h"
#include <QDebug>
#include <QScrollBar>
#include <QMenu>
#include <cstdio>
#include <ctype.h>

extern GDB *gdb;
IntHash formatToSpan;
IntHash formatToSize;
QHash<QString,FormatFunction> formatToFunction;

extern QMap<QString,VariableInfo> asmVariables;
extern QVector<StrucInfo> asmStrucs;

extern bool running;

AsmVariable::AsmVariable(QString _name)
    : name(_name)
{
    address = 0;
    format = "hex1";
    expanded = false;
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
    columns = ebe["asmdata/columns"].toInt();
    buildTable();
    variables.clear();
    userDefinedVariables.clear();
    varNames.clear();
    formatToSpan.clear();
    formatToSpan["character"] = 1;
    formatToSpan["dec1"] = 2;
    formatToSpan["dec2"] = 2;
    formatToSpan["dec4"] = 4;
    formatToSpan["dec8"] = 4;
    formatToSpan["udec1"] = 2;
    formatToSpan["udec2"] = 2;
    formatToSpan["udec4"] = 4;
    formatToSpan["udec8"] = 4;
    formatToSpan["hex1"] = 1;
    formatToSpan["hex2"] = 2;
    formatToSpan["hex4"] = 4;
    formatToSpan["hex8"] = 4;
    formatToSpan["bin1"] = 2;
    formatToSpan["bin2"] = 4;
    formatToSpan["bin4"] = 8;
    formatToSpan["bin8"] = 16;
    formatToSpan["fields4"] = 8;
    formatToSpan["fields8"] = 16;
    formatToSpan["binaryfp4"] = 8;
    formatToSpan["binaryfp8"] = 16;
    formatToSpan["bool1"] = 1;
    formatToSpan["bool2"] = 1;
    formatToSpan["bool4"] = 1;
    formatToSpan["bool8"] = 1;
    formatToSpan["float"] = 4;
    formatToSpan["double"] = 4;
    formatToSpan["string"] = 1;
    formatToSpan["string array"] = 1;
    formatToSize["character"] = 1;
    formatToSize["dec1"] = 1;
    formatToSize["dec2"] = 2;
    formatToSize["dec4"] = 4;
    formatToSize["dec8"] = 8;
    formatToSize["udec1"] = 1;
    formatToSize["udec2"] = 2;
    formatToSize["udec4"] = 4;
    formatToSize["udec8"] = 8;
    formatToSize["hex1"] = 1;
    formatToSize["hex2"] = 2;
    formatToSize["hex4"] = 4;
    formatToSize["hex8"] = 8;
    formatToSize["bool1"] = 1;
    formatToSize["bool2"] = 2;
    formatToSize["bool4"] = 4;
    formatToSize["bool8"] = 8;
    formatToSize["bin1"] = 1;
    formatToSize["bin2"] = 2;
    formatToSize["bin4"] = 4;
    formatToSize["bin8"] = 8;
    formatToSize["fields4"] = 4;
    formatToSize["fields8"] = 8;
    formatToSize["binaryfp4"] = 4;
    formatToSize["binaryfp8"] = 8;
    formatToSize["string"] = 8;
    formatToSize["string array"] = 8;
    formatToSize["float"] = 4;
    formatToSize["double"] = 8;
    formatToFunction["string array"] = toString;
    formatToFunction["string"] = toString;
    formatToFunction["std::string"] = toString;
    formatToFunction["character"] = toChar;
    formatToFunction["hex1"] = toHex1;
    formatToFunction["hex2"] = toHex2;
    formatToFunction["hex4"] = toHex4;
    formatToFunction["hex8"] = toHex8;
    formatToFunction["dec1"] = toDec1;
    formatToFunction["dec2"] = toDec2;
    formatToFunction["dec4"] = toDec4;
    formatToFunction["dec8"] = toDec8;
    formatToFunction["udec1"] = toUDec1;
    formatToFunction["udec2"] = toUDec2;
    formatToFunction["udec4"] = toUDec4;
    formatToFunction["udec8"] = toUDec8;
    formatToFunction["float"] = toFloat;
    formatToFunction["double"] = toDouble;
    formatToFunction["fields4"] = toFields4;
    formatToFunction["fields8"] = toFields8;
    formatToFunction["binaryfp4"] = toBinaryFP4;
    formatToFunction["binaryfp8"] = toBinaryFP8;
    formatToFunction["bin1"] = toBin1;
    formatToFunction["bin2"] = toBin2;
    formatToFunction["bin4"] = toBin4;
    formatToFunction["bin8"] = toBin8;
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
    //qDebug() << variables.size() << varNames;
    columns = ebe["asmdata/columns"].toInt();
    rows = 0;

    //qDebug() << "rows" << rows;
    table->setPlankCount(variables.size());
    table->setColumnCount(columns+2);

    for ( int i=0; i < variables.size(); i++ ) {
        table->setCurrentPlank(i);
        if ( table->rowCount() < 1 ) {
            table->setRowCount(1);
        }
        variables[i].item = table->cell(0,0);
        table->setSpan(0,0,1,1);
        table->setText(0,0,QString(" 0x%1").
                       arg(variables[i].address,0,16));
        table->setSpan(0,1,1,1);
        table->setText(0,1,variables[i].name);
    }

    for ( int i=0; i < variables.size(); i++ ) {
    //qDebug() << "request" << variables[i].name << variables[i].address
         //<< variables[i].size;
        emit requestAsmVariable ( i, variables[i].address, variables[i].size );
    }
}

void AsmDataWindow::receiveAsmVariable ( int i, QStringList results )
{
    QString t1, t2;
    QStringList parts;
    bool ok;
    int k=0;

    table->setCurrentPlank(i);
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
        if ( n > variables[i].size ) n = variables[i].size;
        for ( int r=0; r < n; r++ ) {
            variables[i].values->u1(k) = parts[r].toInt(&ok,16);
            k++;
        }
    }
    redisplay(i,EZ::Highlight);
}

void AsmDataWindow::redisplay ( int v, EZ::Color highlight )
{
    QString s;
    QString t;
    QStringList parts;
    int size;
    int left;
    int max=0;
    int k = 0;
    int count;
    QString format = variables[v].format;
    int num;
    int span;

    FormatFunction f=formatToFunction[format];

    table->setCurrentPlank(v);
    if ( format == "string" || format == "string array" ) {
        span = columns;
        num = variables[v].stringValues.length();
        if ( num < 1 ) num = 1;
        rows = num;
        left = rows;
        size = 1;
        count = 1;
    } else {
        span = formatToSpan[format];
        num = variables[v].size/formatToSize[format];
        left = variables[v].size;
        if ( num < 1 ) num = 1;
        size = formatToSize[format];
        count = columns/span;
        rows = (num + count - 1) / count;
    }
    if ( variables[v].expanded ) rows = 1;
    //qDebug() << "format:" << format << "  span:" << span
             //<< "  num:" << num << "  size:" << size
             //<< "  left:" << left << "  count;" << count;

    table->setRowCount(rows);

    //qDebug() << "redisplay" << v << rows;
    for ( int r = 0; r < rows; r++ ) {
        //qDebug() << "working on row" << r;

        if ( r > 0 ) {
            table->setSpan(r,0,1,1);
            table->setText(r,0,"");
            table->setSpan(r,1,1,1);
            table->setText(r,1,"");
        }

        max = count;
        if ( max > left/size ) max = left/size;
        for ( int j=0; j < max; j++ ) {
            s = (*f)(variables[v].values,variables[v].stringValues,k,j);
            table->setSpan(r,j*span+2,1,span);
            table->setText(r,j*span+2,s,highlight);
            k++;
        }
        for ( int j = max; j < count; j++ ) {
            table->setSpan(r,j*span+2,1,span);
            table->setText(r,j*span+2,"");
        }
        left -= max*size;
    }
    if ( v == table->planks - 1 ) {
        saveScroll();
        table->resizeToFitContents();
        restoreScroll();
    }
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
    table = new EZTable(this);

    /*
     *  We need a layout for the table widget
     */
    layout = new QVBoxLayout;
    layout->setSpacing(5);
    
    /*
     *  Leave 10 pixels all around the table
     */
    layout->setContentsMargins(10, 10, 10, 10);

    table->setPlankCount(0);
    table->setColumnCount(columns+2);
    fontWidth = ebe["font_size"].toInt() * 0.8;
    table->setColumnWidth(0,8*fontWidth);
    table->setColumnWidth(1,15*fontWidth);
    //for ( int c = 2; c < 34; c++ ) {
        //table->setColumnWidth(c,5*fontWidth);
    //}


    /*
     *  Set a tooltip to display when the cursor is over the table
     */
    table->setToolTip(tr("Right click on variable names to change formats."));

    /*
     *  Add the table to the layout and set the layout for the frame.
     */
    scrollArea = new QScrollArea;
    scrollArea->setWidget(table);
    layout->addWidget(scrollArea);
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

void AsmDataWindow::saveScroll()
{
    QScrollBar *b = scrollArea->horizontalScrollBar();
    xScroll = b->value();
    b = scrollArea->verticalScrollBar();
    yScroll = b->value();
    //qDebug() << "Scroll" << xScroll << yScroll;
}

void AsmDataWindow::restoreScroll()
{
    //qDebug() << "Setting scroll" << xScroll << yScroll;
    QScrollBar *b = scrollArea->horizontalScrollBar();
    b->setValue(0);
    b->setValue(xScroll);
    b = scrollArea->verticalScrollBar();
    b->setValue(0);
    b->setValue(yScroll);
}


/*
 *  This function sets the row heights and column widths based on
 *  the contents of the table and the current font size.
 */
void AsmDataWindow::setFontHeightAndWidth(int height, int width)
{
    saveScroll();
    fontHeight = height;
    fontWidth = width;
    table->setFontHeightAndWidth(height,width);
    table->resizeToFitContents();
    restoreScroll();
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
    QAction *action;
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
    sub = menu.addMenu(tr("Binary format"));
        sub->addAction("1 byte", this, SLOT(setBin1()));
        sub->addAction("2 bytes", this, SLOT(setBin2()));
        sub->addAction("4 bytes", this, SLOT(setBin4()));
        sub->addAction("8 bytes", this, SLOT(setBin8()));
    sub = menu.addMenu(tr("Double format"));
        sub->addAction(tr("Double"), this, SLOT(setDouble()));
        sub->addAction(tr("Binary fp"), this, SLOT(setBinaryFP8()));
        sub->addAction(tr("Double fields"), this, SLOT(setFields8()));
    sub = menu.addMenu(tr("Float format"));
        sub->addAction(tr("Float"), this, SLOT(setFloat()));
        sub->addAction(tr("Binary fp"), this, SLOT(setBinaryFP4()));
        sub->addAction(tr("Float fields"), this, SLOT(setFields4()));
    menu.addAction(tr("Char format"), this, SLOT(setChar()));

    int p = table->latestPlank->plankNumber;
    //qDebug() << "selected" << p << variables[p].size;
    if ( variables[p].size >= (int)sizeof(char *) &&
         asmStrucs.size() > 0 ) {
        sub = menu.addMenu(tr("Expand struc *"));
        for ( int i=0; i < asmStrucs.size(); i++ ) {
            action = sub->addAction(asmStrucs[i].name, this,
                     SLOT(expandStruc()));
            action->setData(i);
        }
    }
    
    if ( asmStrucs.size() > 0 ) {
        sub = menu.addMenu(tr("Struc"));
        for ( int i=0; i < asmStrucs.size(); i++ ) {
            action = sub->addAction(asmStrucs[i].name, this, SLOT(setStruc()));
            action->setData(i);
        }
    }
    menu.addAction(tr("Define a variable with this address"), this,
        SLOT(defineVariableByAddress()));
    menu.addAction(tr("Delete variable"), this, SLOT(deleteVariable()));
    menu.exec(QCursor::pos());
}

void AsmDataWindow::defineVariableByAddress()
{
    int p = table->latestPlank->plankNumber;
    //qDebug() << "defineVariableByAddress" << p;
    QStringList parts;

    if ( !running ) return;

    int n = variables[p].values->size/sizeof(int *);
    for ( int i = 0; i < n; i++ ) {
        parts.append(QString("%1").arg(variables[p].values->u8(i),0,16));
    }
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
    int row = 0; // table->currentRow();

    for ( int i=0; i < variables.size(); i++ ) {
        if ( variables[i].item == table->cell(row,0) ) {
            table->setRowCount(table->rowCount()-variables[i].rows);
            variables.remove(i);
            break;
        }
    }
    rebuildTable();
}

void AsmDataWindow::setStruc()
{
    QAction *action = (QAction *)sender();
    int i = action->data().toInt();
    StrucInfo s=asmStrucs[i];
    QVector<AsmVariable> vars(variables);
    AsmVariable v;
    int p = table->latestPlank->plankNumber;

    //qDebug() << "setStruc" << i << p << s.name;
    variables.clear();
    for ( int j=0; j <= p; j++ ) variables.append(vars[j]);
    variables[p].expanded = true;
    for ( int j=0; j < s.variables.size(); j++ ) {
        //qDebug() << s.variables[j].name;
        v.name = s.variables[j].name;
        v.size = s.variables[j].size;
        v.format = s.variables[j].format;
        v.address = variables[p].address + s.variables[j].loc;
        v.values = new AllTypesArray(v.size);
        variables.append(v);
    }
    for ( int j=p+1; j < vars.size(); j++ ) variables.append(vars[j]);
    rebuildTable();
}

void AsmDataWindow::expandStruc()
{
    QAction *action = (QAction *)sender();
    int i = action->data().toInt();
    StrucInfo s=asmStrucs[i];
    AsmVariable v;
    int p = table->latestPlank->plankNumber;


    if ( !running ) return;

    //qDebug() << "expandStruc" << i << p << s.name;
    variables[p].expanded = true;
    for ( int j=0; j < s.variables.size(); j++ ) {
        v.name = s.variables[j].name;
        v.size = s.variables[j].size;
        v.format = s.variables[j].format;
        v.address = variables[p].values->u8(0) + s.variables[j].loc;
        v.values = new AllTypesArray(v.size);
        variables.append(v);
        //qDebug() << "added" << v.name << v.size << v.format
                 //<< QString("%1").arg(v.address,0,16);
    }
    rebuildTable();
}

void AsmDataWindow::collapseStruc()
{
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

void AsmDataWindow::setBin1()
{
    setFormat("bin1");
}

void AsmDataWindow::setBin2()
{
    setFormat("bin2");
}

void AsmDataWindow::setBin4()
{
    setFormat("bin4");
}

void AsmDataWindow::setBin8()
{
    setFormat("bin8");
}

void AsmDataWindow::setFields4()
{
    setFormat("fields4");
}

void AsmDataWindow::setFields8()
{
    setFormat("fields8");
}

void AsmDataWindow::setBinaryFP4()
{
    setFormat("binaryfp4");
}

void AsmDataWindow::setBinaryFP8()
{
    setFormat("binaryfp8");
}

void AsmDataWindow::setFormat(QString format)
{
    //qDebug() << "setFormat" << table->latestPlank;
    int v;
    if ( table->latestPlank == 0 ) return;
    saveScroll();
    //qDebug() << "setFormat" << variables[table->latestPlank].name << format;
    for ( v = 0; v < table->table.size(); v++ ) {
        if ( table->table[v] == table->latestPlank ) {
            variables[v].format = format;
            redisplay(v);
            table->resizeToFitContents();
            break;
        }
    }
    restoreScroll();
}

void AsmDataWindow::setChar()
{
    setFormat("character");
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
            << "double" << "float" << "character";
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
