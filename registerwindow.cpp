#include "registerwindow.h"
#include "asmdatawindow.h"
#include "settings.h"
#include <QDebug>
#include <QHeaderView>
#include <QTableWidget>
#include <QMenu>
#include <QVBoxLayout>
#include <QTableWidgetItem>
#include <cstdio>

extern AsmDataWindow *asmDataWindow;

IntHash registerItems;
IntHash halItems;
extern EZCell *latestCell;

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

static QString names_2[9][2] = {
    { "rax", "r8" },
    { "rbx", "r9" },
    { "rcx", "r10" },
    { "rdx", "r11" },
    { "rsi", "r12" },
    { "rdi", "r13" },
    { "rbp", "r14" },
    { "rsp", "r15" },
    { "rip", "eflags" }
};

#ifdef Q_WS_WIN
static QString halNames[6][4] = {
    { "acc",  "par1", "sav1",  "sav5"},
    { "scr1", "par2", "sav2",  "sav6"},
    { "scr2", "par3", "sav3",  "sav7"},
    { "rbp", "par4",  "sav4",  ""},
    { "rsp", "rip",   "eflags", ""}
};
static QString halNames_2[9][2] = {
    { "acc",  "sav1" },
    { "par1", "sav2" },
    { "par2", "sav3" },
    { "par3", "sav4" },
    { "par4", "sav5" },
    { "scr1", "sav6" },
    { "scr2", "sav7" },
    { "rbp",  "rsp"  },
    { "rip", "eflags" }
};
#else
static QString halNames[5][4] = {
    { "acc",  "par1", "par4", "sav1" },
    { "scr1", "par2", "par5", "sav2" },
    { "scr2", "par3", "par6", "sav3" },
    { "rbp",  "rsp",  "",     "sav4" },
    { "rip",  "eflags", "",   "sav5" }
};
static QString halNames_2[9][2] = {
    { "acc",  "scr1"      },
    { "par1", "scr2" },
    { "par2", "sav1" },
    { "par3", "sav2" },
    { "par4", "sav3" },
    { "par5", "sav4" },
    { "par6", "sav5" },
    { "rbp",  "rsp"  },
    { "rip", "eflags" }
};
#endif

StringHash halToIntel;
StringHash IntelToHal;

static QString names32[3][4] = {
    { "eax", "ebx", "ecx", "edx" },
    { "esi", "edi", "ebp", "esp" },
    { "eip", "eflags", "", "" }
};

static QString names32_2[5][2] = {
    { "eax", "edi" },
    { "ebx", "esi" },
    { "ecx", "ebp" },
    { "edx", "esp" },
    { "eip", "eflags" }
};

GenericRegisterWindow::GenericRegisterWindow(QWidget *parent)
    : QFrame(parent)
{
}
/*
 *  Constructor
 */
RegisterWindow::RegisterWindow(QWidget *parent)
    : GenericRegisterWindow(parent)
{
    buildTable();
}

void RegisterWindow::buildTable()
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
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(4);

    /*
     *  Create a table to display the registers
     */
    table = new EZTable(this);

    /*
     *  We need a layout for the table widget
     */
    QVBoxLayout *layout = new QVBoxLayout;

    /*
     *  Leave 10 pixels all around the table
     */
    layout->setContentsMargins(10, 10, 10, 10);

    /*
     *  There are 16 general purpose registers in 4 (or 8) rows.  Each register
     *  has a name and a value making 8 columns (or 4).
     *
     *  The fifth (or ninth) row is for rip and eflags
     */
    columns = ebe["register/columns"].toInt();

    if (wordSize == 64) {
        rows = columns == 4 ? 5 : 9;
    } else {
        rows = columns == 4 ? 3 : 5;
    }
    table->setRowCount(rows);
    table->setColumnCount(columns*2);

    /*
     *  Fill the table with register names and initial values ("0").
     *  Save the QTableWidgetItem pointers for the register values
     *  in registerMap.
     */
    if (wordSize == 64) {
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                table->setText(r, c * 2 + 1, "0");
                if ( columns == 4 ) {
                    table->setText(r, c * 2, " " + names[r][c]);
                    registerItems[names[r][c]] = r*10+c*2;
                    registerMap[names[r][c]] = table->cell(r,c*2+1);
                } else {
                    table->setText(r, c * 2, " " + names_2[r][c]);
                    registerItems[names_2[r][c]] = r*10+c*2;
                    registerMap[names_2[r][c]] = table->cell(r,c*2+1);
                }
            }
        }
    } else {
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                table->setText(r, c * 2 + 1, "0");
                if ( columns == 4 ) {
                    table->setText(r, c * 2, " " + names32[r][c]);
                    registerItems[names32[r][c]] = r*10+c*2;
                    registerMap[names32[r][c]] = table->cell(r,c*2+1);
                } else {
                    table->setText(r, c * 2, " " + names32_2[r][c]);
                    registerItems[names32_2[r][c]] = r*10+c*2;
                    registerMap[names32_2[r][c]] = table->cell(r,c*2+1);
                }
            }
        }
    }

    /*
     *  Set a tooltip to display when the cursor is over the table
     */
    table->setToolTip(tr("Right click on register names to change formats.\n"
        "Right click on a register's value to define a variable\n"
        "with the address contained in the register."));

    /*
     *  Add the table to the layout and set the layout for the frame.
     */
    scrollArea = new QScrollArea;
    scrollArea->setWidget(table);
    layout->addWidget(scrollArea);
    setLayout(layout);

    /*
     *  Initialize namesList with all register names
     */
    if (wordSize == 64) {
        namesList << "rax" << "rbx" << "rcx" << "rdx" << "rdi" << "rsi" << "rbp"
            << "rsp" << "r8" << "r9" << "r10" << "r11" << "r12" << "r13"
            << "r14" << "r15" << "rip" << "eflags";
    } else {
        namesList << "eax" << "ebx" << "ecx" << "edx" << "edi" << "esi" << "ebp"
            << "esp" << "eip" << "eflags";
    }

    /*
     *  For each register name, create a Register object and save the
     *  pointer in the regs map.
     */
    foreach ( QString name, namesList ) {
        regs[name] = new Register(name);
    }
}

void RegisterWindow::resetNames()
{
    bool columnChange;
    int new_columns;
    new_columns = ebe["register/columns"].toInt();
    columnChange = new_columns != columns;
    if ( columnChange ) {
        columns = new_columns;
        if (wordSize == 64) {
            rows = columns == 4 ? 5 : 9;
        } else {
            rows = columns == 4 ? 3 : 5;
        }
        table->setRowCount(rows);
        table->setColumnCount(columns*2);
    }

    if (wordSize == 64) {
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                if ( columnChange) table->setText(r, c * 2 + 1, "0");
                if ( columns == 4 ) {
                    table->setText(r,c*2," " + names[r][c]);
                    registerItems[names[r][c]] = r*10+c*2;
                    registerMap[names[r][c]] = table->cell(r,c*2+1);
                } else {
                    table->setText(r,c*2," " + names_2[r][c]);
                    registerItems[names_2[r][c]] = r*10+c*2;
                    registerMap[names_2[r][c]] = table->cell(r,c*2+1);
                }
            }
        }
    } else {
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                if ( columnChange) table->setText(r, c * 2 + 1, "0");
                if ( columns == 4 ) {
                    table->setText(r,c*2," " + names32[r][c]);
                    registerItems[names32[r][c]] = r*10+c*2;
                    registerMap[names32[r][c]] = table->cell(r,c*2+1);
                } else {
                    table->setText(r,c*2," " + names32_2[r][c]);
                    registerItems[names32_2[r][c]] = r*10+c*2;
                    registerMap[names32_2[r][c]] = table->cell(r,c*2+1);
                }
            }
        }
    }

    table->resizeToFitContents();
}

/*
 *  Crude size hint.  50 rows is not enough to display much, but it allows
 *  the user to shrink the register window.  300 columns is probably not
 *  enough.
 */
QSize GenericRegisterWindow::sizeHint() const
{
    return QSize(200, 10);
}

/*
 *  This function sets the row heights and column widths based on
 *  the contents of the table and the current font size.
 */
void RegisterWindow::setFontHeightAndWidth(int height, int width)
{
    fontHeight = height;
    fontWidth = width;
    table->setFontHeightAndWidth(height,width);
    table->resizeToFitContents();
}

void HalRegisterWindow::setFontHeightAndWidth(int height, int width)
{
    fontHeight = height;
    fontWidth = width;
    table->setFontHeightAndWidth(height,width);
    table->resizeToFitContents();
}

/*
 *  Set the content of a Register based on its name
 */
void GenericRegisterWindow::setRegister(QString name, QString val,
                                        EZ::Color highlight)
{
    if (registerMap.contains(name)) {
        registerMap[name]->setText(val,highlight);
    } else {
        qDebug() << "tried to set register " << name << endl;
    }
}

/*
 *  Slot triggered by the gdb class sending a map of register
 *  values.
 */
void GenericRegisterWindow::receiveRegs(StringHash map)
{
    foreach ( QString key, map.keys() ) {
        regs[key]->setValue(map[key]);
        setRegister(key,regs[key]->value(),EZ::Highlight);
    }
    table->resizeToFitContents();
}

/*
 *  Virtual function from the QFrame class over-ridden to provide
 *  popup menus for the register window.
 */
void GenericRegisterWindow::contextMenuEvent(QContextMenuEvent * /* event */)
{
    QMenu menu(tr("Register menu"));
    menu.addAction(tr("Decimal format"), this, SLOT(setDecimal()));
    menu.addAction(tr("Hexadecimal format"), this, SLOT(setHex()));
    menu.addSeparator();
    menu.addAction(tr("Decimal format - all"), this, SLOT(setDecimalAll()));
    menu.addAction(tr("Hexadecimal format - all"), this, SLOT(setHexAll()));
    menu.addAction(tr("Define a variable with this address"), this,
        SLOT(defineVariableByAddress()));
    menu.exec(QCursor::pos());
}

void GenericRegisterWindow::defineVariableByAddress()
{
    //item = 0; // table->currentItem();
    int row = 0; //item->row();
    int col = 0; //item->column();
    col = col | 1;                      // Make it odd
    QString address = table->text(row,col);
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
void GenericRegisterWindow::setDecimal()
{
    int row;
    int col;
    if ( latestCell ) {
        row = latestCell->row;
        col = latestCell->column;
        qDebug() << "setDecimal" << row << col;
    } else {
        qDebug() << "latestCell" << (long)latestCell;
    }
    col = col & ~1;                      // Make it even
    QString reg = table->text(row,col);
    reg = reg.trimmed();
    //qDebug() << "reg" << reg;
    //qDebug() << "regs" << regs;
    if (regs.contains(reg)) {
        regs[reg]->setFormat("decimal");
        setRegister(regs[reg]->name, regs[reg]->value());
    }
}

/*
 *  Change the current table item's format to hexadecimal.
 */
void GenericRegisterWindow::setHex()
{
    //qDebug() << table->currentItem();
    QString reg = table->text(0,0);
    //qDebug() << reg;
    reg = reg.trimmed();
    if (regs.contains(reg)) {
        regs[reg]->setFormat("hexadecimal");
        setRegister(regs[reg]->name, regs[reg]->value());
    }
}

/*
 *  Change all the register's formats to decimal.
 */
void GenericRegisterWindow::setDecimalAll()
{
    foreach ( QString reg, namesList ) {
        regs[reg]->setFormat("decimal");
        setRegister(reg,regs[reg]->value());
    }
}

/*
 *  Change all the register's formats to hexadecimal.
 */
void GenericRegisterWindow::setHexAll()
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

uLong Register::toULong()
{
    bool ok;
    return contents.toULongLong(&ok, 16);
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
    uLong dec;
    bool ok;
     //qDebug() << name << format << contents;
    if (name == "rip" || name == "eflags" || name == "eip") {
/*
 *      rip and eflags should be just like gdb prints them
 */
        return contents;
    } else if (format == "decimal") {
        dec = contents.toULongLong(&ok, 16);
        //qDebug() << "reg" << ok << contents << dec;
        return QString("%1").arg(dec);
    } else {
        return contents;
    }
}

HalRegisterWindow::HalRegisterWindow(QWidget *parent)
: GenericRegisterWindow(parent)
{
    buildTable();
}

void HalRegisterWindow::buildTable()
{
    halToIntel["rip"] = "rip";
    halToIntel["eflags"] = "eflags";
    halToIntel["acc"] = "rax";
    halToIntel["rbp"] = "rbp";
    halToIntel["rsp"] = "rsp";
    halToIntel["acc"] = "rax";
    halToIntel["sav1"] = "r12";
    halToIntel["sav2"] = "r13";
    halToIntel["sav3"] = "r14";
    halToIntel["sav4"] = "r15";
    halToIntel["sav5"] = "rbx";
    halToIntel["scr1"] = "r10";
    halToIntel["scr2"] = "r11";
#ifdef Q_WS_WIN
    halToIntel["par1"] = "rcx";
    halToIntel["par2"] = "rdx";
    halToIntel["par3"] = "r8";
    halToIntel["par4"] = "r9";
    halToIntel["sav6"] = "rdi";
    halToIntel["sav7"] = "rsi";
#else
    halToIntel["par1"] = "rdi";
    halToIntel["par2"] = "rsi";
    halToIntel["par3"] = "rdx";
    halToIntel["par4"] = "rcx";
    halToIntel["par5"] = "r8";
    halToIntel["par6"] = "r9";
#endif
    foreach ( QString key, halToIntel.keys() ) {
        IntelToHal[halToIntel[key]] = key;
    }
     //qDebug() << IntelToHal;
/*
 *  For the mainwindow's saveState/restoreState and
 *  saveGeometry/restoreGeometry to work properly each dock widget and toolbar
 *  needs a unique object name set.
 */
    setObjectName("HALRegister");

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
    QVBoxLayout *layout = new QVBoxLayout;

/*
 *  Leave 10 pixels all around the table
 */
    layout->setContentsMargins(10, 10, 10, 10);

/*
 *  There are 16 general purpose registers in 5 rows.  Each register
 *  has a name and a value making 8 columns.
 *
 *  The sixth row is for rip and eflags
 */

    columns = ebe["register/columns"].toInt();
    if ( columns == 4 ) {
        rows = 5;
    } else {
        rows = 9;
    }
    table->setRowCount(rows);
    table->setColumnCount(columns*2);

/*
 *  Fill the table with register names and initial values ("0").
 *  Save the QTableWidgetItem pointers for the register values
 *  in registerMap.
 */
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            table->setText(r, c * 2 + 1, "0");
            if ( columns == 4 ) {
                if (halNames[r][c] != "") {
                    table->setText(r, c * 2, " " + halNames[r][c]);
                    halItems[halNames[r][c]] = r*10+c*2;
                    registerMap[halToIntel[halNames[r][c]]] = 
                        table->cell(r,c);
                }
            } else {
                if (halNames_2[r][c] != "") {
                    table->setText(r, c * 2, " " + halNames_2[r][c]);
                    halItems[halNames_2[r][c]] = r*10+c*2;
                    registerMap[halToIntel[halNames_2[r][c]]] = 
                        table->cell(r,c);
                }
            }
        }
    }

    table->resizeToFitContents();
    table->show();

/*
 *  Set a tooltip to display when the cursor is over the table
 */
    table->setToolTip(tr("Right click on register names to change formats.\n"
                    "Right click on a register's value to define a variable\n"
                    "with the address contained in the register."));

/*
 *  Add the table to the layout and set the layout for the frame.
 */
    scrollArea = new QScrollArea;
    scrollArea->setWidget(table);
    layout->addWidget(scrollArea);

    setLayout(layout);

/*
 *  Initialize namesList with all register names
 */
    namesList << "rax" << "rbx" << "rcx" << "rdx" << "rdi" << "rsi" << "rbp"
              << "rsp" << "r8" << "r9" << "r10" << "r11" << "r12"
              << "r13" << "r14" << "r15" << "rip" << "eflags";

/*
 *  For each register name, create a Register object and save the
 *  pointer in the regs map.
 */
    foreach ( QString name, namesList ) {
        regs[name] = new Register(name);
        regs[IntelToHal[name]] = regs[name];
    }
}

void HalRegisterWindow::resetNames()
{
    columns = ebe["register/columns"].toInt();
    rows = columns == 4 ? 5 : 9;
    table->setRowCount(rows);
    table->setColumnCount(columns*2);
    if (wordSize == 64) {
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                if ( columns == 4 ) {
                    table->setText(r,c*2," " + halNames[r][c]);
                    if (halNames[r][c] != "") {
                        table->setText(r,c*2+1,"0");
                        halItems[halNames[r][c]] = r*10+c*2;
                        registerMap[halToIntel[halNames[r][c]]] = 
                            table->cell(r,c*2+1);
                    } else {
                        table->setText(r,c*2+1,"");
                    }
                } else {
                    table->setText(r,c*2," " + halNames_2[r][c]);
                    if (halNames_2[r][c] != "") {
                        table->setText(r,c*2+1,"0");
                        halItems[halNames_2[r][c]] = r*10+c*2;
                        registerMap[halToIntel[halNames_2[r][c]]] = 
                            table->cell(r,c*2+1);
                    } else {
                        table->setText(r,c*2+1,"");
                    }
                }
            }
        }
    }

    table->resizeToFitContents();
}

HalNamesWindow::HalNamesWindow(QWidget *parent)
    : QFrame(parent)
{
    table = new EZTable(this);
/*
 *  We need a layout for the table widget
 */
    QVBoxLayout *layout = new QVBoxLayout;

/*
 *  Leave 10 pixels all around the table
 */
    layout->setContentsMargins(10, 10, 10, 10);
    columns = ebe["register/columns"].toInt();
    if ( columns == 4 ) {
        rows = 4;
    } else {
        rows = 8;
    }
    table->setRowCount(rows);
    table->setColumnCount(columns);
    QString name;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            if ( columns == 4 ) {
                name = names[r][c];
                if ( name.length() == 2 ) name = " " + name;
                table->setText(r, c,
                     " " + name + "=" + IntelToHal[names[r][c]]);
            } else {
                name = names_2[r][c];
                if ( name.length() == 2 ) name = " " + name;
                table->setText(r, c,
                     " " + name + "=" + IntelToHal[names_2[r][c]]);
            }
        }
    }
    table->resizeToFitContents();
    table->show();

/*
 *  Add the table to the layout and set the layout for the frame.
 */
    scrollArea = new QScrollArea;
    scrollArea->setWidget(table);
    layout->addWidget(scrollArea);

    setLayout(layout);
}

void HalNamesWindow::setFontHeightAndWidth(int height, int width)
{
    fontHeight = height;
    fontWidth = width;
    table->setFontHeightAndWidth(height,width);
    table->resizeToFitContents();
}
