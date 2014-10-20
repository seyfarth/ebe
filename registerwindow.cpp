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

#ifdef Q_WS_WIN
static QString halNames[6][4] = {
    {   "acc",  "par1", "sav1",  "sav5"},
    {   "scr1", "par2", "sav2",  "sav6"},
    {   "scr2", "par3", "sav3",  "sav7"},
    {   "rbp", "par4",  "sav4",  ""},
    {   "rsp", "rip",   "eflags", ""}
};
#else
static QString halNames[6][4] = {
    { "acc",  "par1", "par4",   "sav1" },
    { "scr1", "par2", "par5",   "sav2" },
    { "scr2", "par3", "par6",   "sav3" },
    { "rbp",  "",     "",       "sav4" },
    { "rsp",  "rip",  "eflags", "sav5" }
};
#endif

StringHash halToIntel;
StringHash IntelToHal;

static QString names32[3][4] = {
    { "eax", "ebx", "ecx", "edx" },
    { "esi", "edi", "ebp", "esp" },
    { "eip", "eflags", "", "" }
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
    table = new QTableWidget(this);

    /*
     *  We need a layout for the table widget
     */
    QVBoxLayout *layout = new QVBoxLayout;

    /*
     *  Leave 10 pixels all around the table
     */
    layout->setContentsMargins(10, 10, 10, 10);

    /*
     *  There are 16 general purpose registers in 4 rows.  Each register
     *  has a name and a value making 8 columns.
     *
     *  The fifth row is for rip and eflags
     */

    if (wordSize == 64) {
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
    table->verticalHeader()->hide();
    table->horizontalHeader()->hide();
    if (wordSize == 64) {
        for (int r = 0; r < 5; r++) {
            for (int c = 0; c < 4; c++) {
                if (c > 0)
                    name = new QTableWidgetItem("  " + names[r][c] + " ");
                else
                    name = new QTableWidgetItem(names[r][c] + " ");
                name->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
                if (r == 4 && c > 1) {
                    val = new QTableWidgetItem("");
                } else {
                    val = new QTableWidgetItem("0");
                }
                registerMap[names[r][c]] = val;
                table->setItem(r, c * 2, name);
                table->setItem(r, c * 2 + 1, val);
            }
        }
    } else {
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 4; c++) {
                if (c > 0)
                    name = new QTableWidgetItem(
                        "  " + names32[r][c] + QString(" "));
                else
                    name = new QTableWidgetItem(names32[r][c] + QString(" "));
                name->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
                if (r == 2 && c > 1) {
                    val = new QTableWidgetItem("");
                } else {
                    val = new QTableWidgetItem("0");
                }
                registerMap[names32[r][c]] = val;
                table->setItem(r, c * 2, name);
                table->setItem(r, c * 2 + 1, val);
            }
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
    table->setToolTip(tr("Right click on register names to change formats.\n"
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

    connect(this, SIGNAL(sendVariableDefinition(QStringList)), dataWindow,
        SLOT(receiveVariableDefinition(QStringList)));
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
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}

void HalRegisterWindow::setFontHeightAndWidth(int height, int width)
{
    fontHeight = height;
    fontWidth = width;
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}

/*
 *  Set the content of a Register based on its name
 */
void GenericRegisterWindow::setRegister(QString name, QString val)
{
    if (registerMap.contains(name)) {
        registerMap[name]->setText(val);
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
        setRegister(key,regs[key]->value());
    }
    setFontHeightAndWidth(fontHeight, fontWidth);
}

/*
 *  Virtual function from the QFrame class over-ridden to provide
 *  popup menus for the register window.
 */
void GenericRegisterWindow::contextMenuEvent(QContextMenuEvent * /* event */)
{
    int column = table->currentColumn();
    QMenu menu(tr("Register menu"));
    if (column % 2 == 0) {
        /*
         *      If the column is an even number, this is a register name
         *      and the menu needs to allow changing formats.
         */
        menu.addAction(tr("Decimal format"), this, SLOT(setDecimal()));
        menu.addAction(tr("Hexadecimal format"), this, SLOT(setHex()));
        menu.addSeparator();
        menu.addAction(tr("Decimal format - all"), this, SLOT(setDecimalAll()));
        menu.addAction(tr("Hexadecimal format - all"), this, SLOT(setHexAll()));
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

void GenericRegisterWindow::defineVariableByAddress()
{
    QString address = table->currentItem()->text();
    DefineVariableDialog *dialog = new DefineVariableDialog;
    dialog->addressEdit->setText(address);
    if (dialog->exec()) {
        dialog->result.append("");
        dialog->result.append("");
        emit sendVariableDefinition(dialog->result);
    }
    delete dialog;
}

/*
 *  Change the current table item's format to decimal.
 */
void GenericRegisterWindow::setDecimal()
{
    QString reg = table->currentItem()->text();
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
    QString reg = table->currentItem()->text();
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
    table = new QTableWidget(this);
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

    table->setRowCount(5);
    table->setColumnCount(8);

/*
 *  Fill the table with register names and initial values ("0").
 *  Save the QTableWidgetItem pointers for the register values
 *  in registerMap.
 */
    QTableWidgetItem *name;
    QTableWidgetItem *val;
    table->verticalHeader()->hide();
    table->horizontalHeader()->hide();
    for (int r = 0; r < 5; r++) {
        for (int c = 0; c < 4; c++) {
            if (c > 0) {
                name = new QTableWidgetItem("  " + halNames[r][c] +
                           QString(" "));
            } else {
                name = new QTableWidgetItem(halNames[r][c] + QString(" "));
            }
            name->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            if (halNames[r][c] == "") {
                val = new QTableWidgetItem("");
            } else {
                val = new QTableWidgetItem("0");
            }
            if (halNames[r][c] != "") {
                registerMap[halToIntel[halNames[r][c]]] = val;
            }
            table->setItem(r, c * 2, name);
            table->setItem(r, c * 2 + 1, val);
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
    table->setToolTip(tr("Right click on register names to change formats.\n"
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

    connect(this, SIGNAL(sendVariableDefinition(QStringList)), dataWindow,
    SLOT(receiveVariableDefinition(QStringList)));
}

HalNamesWindow::HalNamesWindow(QWidget *parent)
    : QFrame(parent)
{
    QTableWidgetItem *name;
    table = new QTableWidget(this);
/*
 *  We need a layout for the table widget
 */
    QVBoxLayout *layout = new QVBoxLayout;

/*
 *  Leave 10 pixels all around the table
 */
    layout->setContentsMargins(10, 10, 10, 10);
    table->setRowCount(4);
    table->setColumnCount(4);
    table->verticalHeader()->hide();
    table->horizontalHeader()->hide();
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            name = new QTableWidgetItem(
                " " + names[r][c] + "=" + IntelToHal[names[r][c]] + " ");
            name->setTextAlignment(Qt::AlignHCenter);
            table->setItem(r, c, name);
        }
    }
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

/*
 *  Don't show a grid
 */
    table->setShowGrid(false);
/*
 *  Add the table to the layout and set the layout for the frame.
 */
    layout->addWidget(table);

    setLayout(layout);
}

void HalNamesWindow::setFontHeightAndWidth(int height, int width)
{
    fontHeight = height;
    fontWidth = width;
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}
