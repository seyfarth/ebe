#include "registerwindow.h"
#include "datawindow.h"
#include "settings.h"
#include <QDebug>
#include <QHeaderView>
#include <QTableWidget>
#include <QMenu>
#include <QVBoxLayout>
#include <QTableWidgetItem>
#include <cstdio>

extern DataWindow *dataWindow;

/**
 * Static matrix of register names matching the pattern in the table
 */
static QString names[5][4] = {
    { "rax", "rsi", "r8",  "r12" },
    { "rbx", "rdi", "r9",  "r13" },
    { "rcx", "rbp", "r10", "r14" },
    { "rdx", "rsp", "r11", "r15" },
    { "rip", "eflags", "", "" }
};

static QString names32[3][4] = {
    { "eax", "ebx", "ecx", "edx" },
    { "esi", "edi", "ebp", "esp" },
    { "eip", "eflags", "", "" }
};

/*
 *  Constructor
 */
RegisterWindow::RegisterWindow(QWidget *parent)
: QFrame(parent)
{
/*
 *  For the mainwindow's saveState/restoreState and
 *  saveGeometry/restoreGeometry to work properly each dock widget and toolbar
 *  needs a unique object name set.
 */
    setObjectName("Register");

/*
 *  Set the frame to be raised with a width 4 bevel.
 */
    setFrameStyle ( QFrame::Panel | QFrame::Raised );
    setLineWidth(4);

/*
 *  Create a table to display the registers
 */
    table = new QTableWidget(this);

/*
 *  We need a layout for the table widget
 */
    QVBoxLayout *layout = new QVBoxLayout;

/*
 *  Leave 10 pixels all around the table
 */
    layout->setContentsMargins(10,10,10,10);

/*
 *  There are 16 general purpose registers in 4 rows.  Each register
 *  has a name and a value making 8 columns.
 *
 *  The fifth row is for rip and eflags
 */

    if ( wordSize == 64 ) {
        table->setRowCount(5);
    } else {
        table->setRowCount(3);
    }
    table->setColumnCount(8);

/*
 *  Fill the table with register names and initial values ("0").
 *  Save the QTableWidgetItem pointers for the register values
 *  in registerMap.
 */
    QTableWidgetItem *name;
    QTableWidgetItem *val;
    table->verticalHeader()->hide(); table->horizontalHeader()->hide();
    if ( wordSize == 64 ) {
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
    } else {
        for ( int r = 0; r < 3; r++ ) {
            for ( int c = 0; c < 4; c++ ) {
                name = new QTableWidgetItem(names32[r][c]+QString(" "));
                name->setTextAlignment(Qt::AlignRight);
                if ( r == 2 && c > 1 ) {
                    val = new QTableWidgetItem("");
                } else {
                    val = new QTableWidgetItem("0");
                }
                registerMap[names32[r][c]] = val;
                table->setItem(r,c*2,name);
                table->setItem(r,c*2+1,val);
            }
        }
    }


/*
 *  Resizing based on size hints which is not too accurate
 */
    table->resizeRowsToContents();
    table->resizeColumnsToContents();

/*
 *  Don't show a grid
 */
    table->setShowGrid(false);

/*
 *  Set a tooltip to display when the cursor is over the table
 */
    table->setToolTip(
        tr("Right click on register names to change formats.\n"
           "Right click on a register's value to define a variable\n"
           "with the address contained in the register."));

/*
 *  Add the table to the layout and set the layout for the frame.
 */
    layout->addWidget(table);
    setLayout(layout);

/*
 *  Initialize namesList with all register names
 */
    if ( wordSize == 64 ) {
        namesList << "rax" << "rbx" << "rcx" << "rdx"
             << "rdi" << "rsi" << "rbp" << "rsp"
             << "r8"  << "r9"  << "r10" << "r11"
             << "r12" << "r13" << "r14" << "r15"
             << "rip" << "eflags";
    } else {
        namesList << "eax" << "ebx" << "ecx" << "edx"
             << "edi" << "esi" << "ebp" << "esp"
             << "eip" << "eflags";
    }

/*
 *  For each register name, create a Register object and save the
 *  pointer in the regs map.
 */
    foreach ( QString name, namesList ) {
        regs[name] = new Register(name);
    }

    connect ( this, SIGNAL(sendVariableDefinition(QStringList)),
              dataWindow, SLOT(receiveVariableDefinition(QStringList)) );
}

/*
 *  Crude size hint.  50 rows is not enough to display much, but it allows
 *  the user to shrink the register window.  300 columns is probably not
 *  enough.
 */
QSize RegisterWindow::sizeHint() const
{
    return QSize(200,10);
}

/*
 *  This function sets the row heights and column widths based on
 *  the contents of the table and the current font size.
 */
void RegisterWindow::setFontHeightAndWidth ( int height, int width )
{
    int max, length;
    fontHeight = height;
    fontWidth = width;
    int rows = table->rowCount();
    for ( int r = 0; r < rows; r++ ) {
        table->setRowHeight(r,height+3);
    }
    for ( int c = 0; c < 8; c++ ) {
        max = 1;
        for ( int r = 0; r < rows; r++ ) {
            length = table->item(r,c)->text().length();
            if ( length > max ) max = length;
        }
        table->setColumnWidth(c,(max+1)*width+3);
    }
}

/*
 *  Set the content of a Register based on its name
 */
void RegisterWindow::setRegister ( QString name, QString val )
{
    if ( registerMap.contains(name) ) {
        registerMap[name]->setText(val);
    } else {
        qDebug() << "tried to set register " << name << endl;
    }
}

/*
 *  Slot triggered by the gdb class sending a map of register
 *  values.
 */
void RegisterWindow::receiveRegs ( StringHash map )
{

    foreach ( QString key, map.keys() ) {
        regs[key]->setValue(map[key]);
        setRegister(key,regs[key]->value());
    }
    setFontHeightAndWidth(fontHeight,fontWidth);
}

/*
 *  Virtual function from the QFrame class over-ridden to provide
 *  popup menus for the register window.
 */
void RegisterWindow::contextMenuEvent(QContextMenuEvent * /* event */)
{
    int column = table->currentColumn();
    QMenu menu(tr("Register menu"));
    if ( column % 2 == 0 ) {
/*
 *      If the column is an even number, this is a register name
 *      and the menu needs to allow changing formats.
 */
        menu.addAction(tr("Decimal format"), this, SLOT(setDecimal()) );
        menu.addAction(tr("Hexadecimal format"), this, SLOT(setHex()) );
        menu.addSeparator();
        menu.addAction(tr("Decimal format - all"),
                       this, SLOT(setDecimalAll()) );
        menu.addAction(tr("Hexadecimal format - all"),
                       this, SLOT(setHexAll()) );
    } else {
/*
 *      If the column is odd, then this is a register value which
 *      can be used to create a variable with the address specified
 *      by the register.  This is only important for assembly.
 */
        menu.addAction(tr("Define a variable with this address"),
                       this, SLOT(defineVariableByAddress()) );
    }
    menu.exec(QCursor::pos());
}

void RegisterWindow::defineVariableByAddress()
{
    QString address = table->currentItem()->text();
    DefineVariableDialog *dialog = new DefineVariableDialog;
    dialog->addressEdit->setText(address);
    if ( dialog->exec() ) {
        dialog->result.append("");
        dialog->result.append("");
        emit sendVariableDefinition(dialog->result);
    }
    delete dialog;
}

/*
 *  Change the current table item's format to decimal.
 */
void RegisterWindow::setDecimal()
{
    QString reg = table->currentItem()->text();
    //qDebug() << "reg" << reg;
    if ( reg.at(reg.length()-1) == ' ' ) reg.chop(1);
    if ( regs.contains(reg) ) {
        regs[reg]->setFormat("decimal");
        setRegister(reg,regs[reg]->value());
    }
}

/*
 *  Change the current table item's format to hexadecimal.
 */
void RegisterWindow::setHex()
{
    //qDebug() << table->currentItem();
    QString reg = table->currentItem()->text();
    //qDebug() << reg;
    if ( reg.at(reg.length()-1) == ' ' ) reg.chop(1);
    if ( regs.contains(reg) ) {
        regs[reg]->setFormat("hexadecimal");
        setRegister(reg,regs[reg]->value());
    }
}

/*
 *  Change all the register's formats to decimal.
 */
void RegisterWindow::setDecimalAll()
{
    foreach ( QString reg, namesList ) {
        regs[reg]->setFormat("decimal");
        setRegister(reg,regs[reg]->value());
    }
}

/*
 *  Change all the register's formats to hexadecimal.
 */
void RegisterWindow::setHexAll()
{
    foreach ( QString reg, namesList ) {
        regs[reg]->setFormat("hexadecimal");
        setRegister(reg,regs[reg]->value());
    }
}

/*
 *  Constructor
 *  
 *  The parameter is the register's name.
 */
Register::Register(QString n)
{
    name = n;
    format = "hexadecimal";
    contents = "";
}

/*
 *  Set the value of this Register
 */
void Register::setValue(QString v)
{
    contents = v;
}

/*
 *  Set the format of this Register
 */
void Register::setFormat(QString f)
{
    format = f;
}

/*
 *  Get the value of this Register as it should look in the table
 */
QString Register::value()
{
    long dec;
    bool ok;
    //qDebug() << name << format << contents;
    if ( name == "rip" || name == "eflags" || name == "eip" ) {
/*
 *      rip and eflags should be just like gdb prints them
 */
        return contents;
    } else if ( format == "decimal" ) {
        dec = contents.toLong(&ok,16);
        return QString("%1").arg(dec);
    } else {
        return contents;
    }
}
