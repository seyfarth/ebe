/*
    QTableWidget for the general purpose register window
    Displays general purpose registers, toggles view from
    integer to hexadecimal format and back, no 
*/

#include "registerwindow.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QSize>

/*
    Constructor sets rows and columns and initializes the cells
    with register names and initial default values of zero
*/
RegisterWindow::RegisterWindow(QWidget *parent) : QFrame(parent)
{
    setObjectName("Register");
    setFrameStyle ( QFrame::Panel | QFrame::Raised );
    setLineWidth(3);
    setMidLineWidth(1);

    QVBoxLayout *layout = new QVBoxLayout(this);
    table = new QTableWidget(this);
    //table->setMinimumSize(QSize(600,300));
    table->setMaximumSize(QSize(1000,500));

    // 9 rows, 4 columns
    // 18 registers, label and value for 2 registers per row
    table->setRowCount(9);
    table->setColumnCount(4);

    // Set the labels indicating the registers, set all to 0 first
    initRegisterLabelsAndValues();

    // Set width and possibly height
    // setColumnWidth(0, parent->width());
    table->verticalHeader()->hide();
    table->horizontalHeader()->hide();
    table->resizeRowsToContents();
    table->resizeColumnsToContents();
    table->setColumnWidth(0,60);
    table->setShowGrid(false);
    layout->addWidget(table);
    setLayout(layout);
}

/*
    Initialiser function used in constructor, sets the register
    names in the widget and default zero values, sets tool tips
    for register values and connects signals and slots for
    toggling the display of the register values and checking
    for accidental/invalid changes to cell values
*/
void RegisterWindow::initRegisterLabelsAndValues()
{
    // List of QString with registers and default values of 0
    QStringList registers;

    // List of QTableWidgetItem, with registers and init values of 0
    // First register then default value, for all registers
    QList<QTableWidgetItem *> registerList;

    // Register names and initial default values
    registers << "rax" << "0" << "rbx" << "0" << "rcx" << "0" << "rdx" << "0"
        << "rdi" << "0" << "rsi" << "0" << "rbp" << "0" << "rsp" << "0"
        << "r8" << "0" << "r9" << "0" << "r10" << "0" << "r11" << "0"
        << "r12" << "0" << "r13" << "0" << "r14" << "0" << "r15" << "0"
        << "rip" << "0" << "eflags" << "121342423422323424324";

    // Horizontal header text, we don't want any header text
    QStringList hHeaders;
    hHeaders << "    " << "                       " << "             " << "       " << "           ";

    // Vertical headers, again don't want anything
    QStringList vHeaders;
    vHeaders << "  " << "  " << "  " << "  " << "  " << "  " << "  " << "  " << "  ";

    table->setVerticalHeaderLabels(vHeaders);
    table->setHorizontalHeaderLabels(hHeaders);

    QStringList::const_iterator constIterator;

    // Iterate over register and value list and create a
    // QTableWidgetItem out of them, add tooltip to each register value
    // cell then add to list of QTableWidgetItems
    for (constIterator = registers.constBegin();
        constIterator != registers.constEnd(); ++constIterator)
    {
        // Create cell item
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        
        // Add register names and default zero values
        item->setText((*constIterator).toLocal8Bit().constData());
        
        // Any item zero is a register value cell, see initialization above
        if (item->text().compare("0") == 0)
            item->setToolTip(tr("Click to toggle between decimal and hexadecimal"));
        
        // Add cell items to list ready to go in the register table widget        
        registerList.append(item);
    }

    // Populate the QTableWidget which is our register window
    // Could have done it above but easier in terms of rows and columns
    
    int posCounter = 0;     // To index into registerList
    
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            // Extract the table widget from list
            QTableWidgetItem *item = (QTableWidgetItem *)registerList.at(posCounter);
            
            // Call superclass method setItem() to place the item at
            // row, col in the table widget. On each iteration we set
            // the item in all columns inside a row, that is, first
            // iteration row = 0, then iterate through columns 0 to 3,
            // then row 1, columns 0 to 3, and so on to row 8             
            table->setItem(row, col, item);
            
            posCounter++;
        }
    }

    // Construct list of registers for use with helper functions
    // regs is a QStringList member of RegisterWindow class
    regs << "rax" << "rbx" << "rcx" << "rdx" << "rdi" << "rsi"
        << "rbp" << "rsp" << "r8" << "r9" << "r10" << "r11" << "r12"
        << "r13" << "r14" << "r15" << "rip" << "eflags";


    // Might be good to handle signal itemClicked() as well?
    // Although just connecting itemPressed() or itemClicked() seems fine
    
    // Don't connect both without changing the code or negation will
    // happen as itemPressed() or itemClicked() fires, then the other...
    
    // itemClicked passes in QTableWidgetItem pointer
    // If connecting add the overloaded toggleNumericFormat() to
    // accept a pointer to QTableWidgetItem

    // connect(this, SIGNAL(itemClicked()),
        // this, SLOT(toggleNumericFormat()));

    // This behaves like cellClicked() signal
    connect(table, SIGNAL(cellPressed(int, int)),
        this, SLOT(toggleNumericFormat(int, int)));

    // If there's a double click, assume user wants to enter
    // a value in the cell unless it's a register name
    connect(table, SIGNAL(cellDoubleClicked(int, int)),
        this, SLOT(clearForDoubleClick(int, int)));

    // Prevent user changing cell value that's a register name or changing
    // a register value to something other than a valid numeric value
    connect(table, SIGNAL(cellChanged(int, int)),
        table, SLOT(cellChangeCheck(int, int)));
}

/*
    NOT USED
    May be used when all signals and slots are connected eventually
*/
bool RegisterWindow::isRegister(QString& item)
{
    // regs is a QStringList with each register as a QString
    // Get rid of whitespace and convert to lowercase before check
    if (regs.contains(item.simplified().toLower()))
        return true;

    // Whatever we got was not a register name
    return false;
}

/*
    Clear the cell, provided that it's not a register name
*/
void RegisterWindow::clearForDoubleClick(int row, int col)
{
    // This cell contains a register name, don't clear it
    if (col == 0 || col % 2 == 0)
        return;

    // Get the value in cell
    QTableWidgetItem *cellItem = table->item(row, col);

    // Clear string out ready to enter new value in cell
    QString str = cellItem->text();
    str.clear();

    // Put empty string back in cell
    cellItem->setText(str);
}

/*
    The slot for the cell changed signal. Check if a register name
    is being changed and restore it, check for invalid numeric
    format put in a cell and reset it to zero
*/
void RegisterWindow::cellChangeCheck(int row, int col)
{
    // Get the value in cell
    QTableWidgetItem *cellItem = table->item(row, col);

    // Remove all whitespace, convert it to lowercase
    QString str = cellItem->text().simplified().toLower();

    // Register name cell, restore register name
    if (col == 0 || col % 2 == 0)
    {
        str = regPositions[row][col];
        cellItem->setText(str);
        return;
    }

    // Empty? Fine, don't change it in case user has cleared cell
    // ready to enter new register value
    if (str.isEmpty())
        return;

    // If item in cell is valid numeric format, let it be
    // Note that isValidNumericFormat()
    if (isValidNumericFormat(str))
        return;

    // Invalid numeric format, set to zero
    str = "0";

    // Update internal register value
    regValues[row][col] = 0;

    // Put back reset value
    cellItem->setText(str);
}

/*
    This toggles the display of a register value from integer
    to hexadecimal and back again

    Along the way it checks to make sure the row and column
    correspond to a register value, performs some checks
    on the number format to see if it's valid first, then
    whether it's in integer or hexadecimal format, then
    does the conversion and sets the new display format
*/
void RegisterWindow::toggleNumericFormat(int row, int col)
{
    // 0 or even col is a register name, prevent "toggle"
    if (col == 0 || col % 2 == 0)
        return;

    // Get the cell that was clicked containing string
    QTableWidgetItem *cellItem = table->item(row, col);

    // Remove whitespace from string and convert to lowercase    
    QString str = cellItem->text().simplified().toLower();

    // Empty string?
    if (str.isEmpty())
        return;

    // Valid numeric format? If not, do nothing
    if (!isValidNumericFormat(str))
        return;

    // Holds the qlonglong representation of converted string
    qlonglong num;

    // For strtoull, holds first invalid character that could not be converted
    // Don't bother checking errno or return value of strtoull
    char *pInvalid;

    if (isHexadecimal(str))
        num = strtoull(str.toAscii(), &pInvalid, 16);
    else
        // This will convert base 10 or base 8 if leading zero
        num = strtoull(str.toAscii(), &pInvalid, 0);

    // Extra check to make sure conversion took place
    if (*pInvalid)
        // Failed for some reason
        return;

    // Do the toggle from hexadecimal to integer
    if (isHexadecimal(str))
        str = QString::number(num, 10);
    else
        // Convert to hex, letters in hex format in uppercase except x in 0x
        str = "0x" + QString::number(num, 16).toUpper();
    
    // Update internal register value
    regValues[row][col] = num;

    // Set the toggled number back in cell
    cellItem->setText(str);
}

/*
    Is the string passed in a hexadecimal formatted number?
    The function checks the obvious first, then uses a regular
    expression to allow optional leading digits, requires at
    least one hexadecimal character, optionally followed by at
    least one hexadecimal character or digit
*/
bool RegisterWindow::isHexadecimal(QString& str)
{
    // Definitely hex format string
    if (str.startsWith("0x"))
        return true;

    // Definitely signed int format
    if (str.startsWith("-") or str.startsWith("+"))
        return false;

    // If the regular expression matches, then matchPos is set to point to
    // the position of the matched string. -1 means no match
    int matchPos;
    
    QRegExp exprObj;

    // Optional leading digits, must have at least one hex char A-F, then
    // any number of hex chars or digits
    exprObj.setPattern("^\\d*[a-fA-F]+[a-fA-F0-9]*$");
    
    // This causes quantifiers to be non-greedy    
    exprObj.setMinimal(true);

    // Index of string that matched, -1 is no match
    matchPos = exprObj.indexIn(str);

    // Was there a matched position 0 or greater?
    bool matched = (matchPos >= 0) ? true : false;

    return matched;
}

/*
    Is the string a number, either signed or unsigned integer
    or hexadecimal? First check the obvious like a leading "0x"
    or sign "-" or "+", then check against regular expressions
*/
bool RegisterWindow::isValidNumericFormat(QString& number)
{
    // Match a valid number format, hex or int, leading zero
    // is fine, assumed to be octal in that case

    // Assumption is that string is trimmed and tested for null
    // or empty first, possibly made all lower case for hex strings

    QRegExp exprObj;

    // Hexadecimal format if leading 0x, otherwise hex, int base 10 or octal

    if (number.startsWith("0x"))
        exprObj.setPattern("^0x[0-9a-zA-Z]+$");
    else if (number.startsWith("-") or number.startsWith("+"))
        exprObj.setPattern("^(-|\\+)?[0-9]+$");
    else
        exprObj.setPattern("^[0-9a-zA-Z]+$");

    // Index of match, if there was one
    int matchPos = exprObj.indexIn(number);
    
    // If there was a match at position 0 or greater, valid is set to true
    bool valid = (matchPos >= 0) ? true : false;

    return valid;
}

