#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

/*
    RegisterWindow class
    For display of general purpose registers, no public functions
    for set and get registers just yet
*/

#include <QTableWidget>
#include <QFrame>
#include <QStringList>
#include <QString>
#include <QTableWidgetItem>
#include <QList>
#include <QMessageBox>
#include <QRegExp>

class RegisterWindow : public QFrame
{
    Q_OBJECT

public:
    RegisterWindow(QWidget *parent = 0);

private:

    // Sets register name and value cells, called from constructor
    // Register value cells are set to integer format first, as 0
    void initRegisterLabelsAndValues();

    // Helper function
    // Determine whether string passed is in integer or hexadecimal format
    // Return true for hexadecimal, false for integer (positive or negative)
    bool isHexadecimal(QString& str);

    // Helper
    // Is it a valid numeric format? Either in hexadecimal, integer
    // positive or negative, or octal if it begins with 0
    bool isValidNumericFormat(QString& number);

    // Helper
    // Is item string a register name?
    bool isRegister(QString& item);

private slots:

    // Toggle between integer (positive or negative) and hexadecimal
    // representation
    void toggleNumericFormat(int row, int col);

    // Stop a cell from changing if it's a register name or something
    // other than a valid int or hex number for a register value cell
    void cellChangeCheck(int row, int col);

    // Stop change of register name on double click, allow
    // change of register values. First just clear the cell.
    void clearForDoubleClick(int row, int col);

private:

    QTableWidget *table;

    // Saves having to construct it every time we want it
    // Create in RegisterWindow constructor
    QStringList regs;

    // NOT USED
    // Register names are used, but internal display states not used yet,
    // maybe they won't be

    // Table of QStrings to restore a register name changed in a cell
    // Also gives the display state of a register value in a cell, int or hex
    // All are intializated to int, i.e. 0 when constructor is called
    QString regPositions[9][4] = { {"rax", "int", "rbx", "int"},
        {"rcx", "int", "rdx", "int"}, {"rdi", "int", "rsi", "int"},
        {"rbp", "int", "rsp", "int"}, {"r8", "int", "r9", "int" },
        {"r10", "int", "r11", "int"}, {"r12", "int", "r13", "int"},
        {"r14", "int", "r15", "int"}, {"rip", "int", "eflags", "int"} };

    // NOT USED with any functions yet, but set in the code ready

    // Table of qlonglongs, the register values at any point in time
    // Note that register positions are not valid, the register values
    // correspond to positions of display state (see regPositions above)
    qlonglong regValues[9][4] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
};

#endif

