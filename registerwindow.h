#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

/*

*/

#include <QTableWidget>
#include <QStringList>
#include <QString>
#include <QTableWidgetItem>
#include <QList>
#include <QMessageBox>
#include <QRegExp>

class RegisterWindow : public QTableWidget
{
    Q_OBJECT

public:
    RegisterWindow(QWidget *parent = 0);

    // Getters and setters

    // Validate input and output as a valid decimal or hexadecimal
    // value with QRegExp first?

    // All require a QString giving the register, e.g. "rax"
    // qlonglong getRegisterValueLongLong(QString& reg);

    // Return string representation, set decimal to false for hexadecimal
    // QString getRegisterValueString(QString& reg, bool decimal = true);

    // Set a register value using a long long or number as a QString
    // void setRegisterValue(QString& reg, qlonglong value);
    // void setRegisterValue(QString& reg, QString value);

private:

    // Sets register name and value cells, called from constructor
    // Register value cells are set to integer format first, as 0
    void initRegisterLabelsAndValues();

    // Helper function
    // Determine whether string passed is in integer or hexadecimal format
    // Return true for hexadecimal, false for integer (positive or negative)
    bool isHexadecimal(QString& str);

    // Validates register name and returns index of that register value
    // in the cell view
    // int getRegisterPos(QString& reg);

    // Helper
    // void internalSetRegister(int regIndex, QString& value);

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

private:

    // Saves having to construct it every time we want it
    // Create in RegisterWindow constructor
    QStringList regs;

    // Table of QStrings to restore a register name changed in a cell
    // Also gives the display state of a register value in a cell, int or hex
    // All are intializated to int, i.e. 0 when constructor is called
    QString regPositions[9][4] = { {"rax", "int", "rbx", "int"},
        {"rcx", "int", "rdx", "int"}, {"rdi", "int", "rsi", "int"},
        {"rbp", "int", "rsp", "int"}, {"r8", "int", "r9", "int" },
        {"r10", "int", "r11", "int"}, {"r12", "int", "r13", "int"},
        {"r14", "int", "r15", "int"}, {"rip", "int", "eflags", "int"} };

    // Table of qlonglongs, the register values at any point in time
    // Note that register positions are not valid, the register values
    // correspond to positions of display state (see regPositions above)
    qlonglong regValues[9][4] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
};

#endif

