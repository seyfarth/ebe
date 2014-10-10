#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

/*
 RegisterWindow class
 For display of general purpose registers, no public functions
 for set and get registers just yet
 */

#include "types.h"
#include <QFrame>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStringList>
#include <QScrollArea>
#include <QSize>

/**
 *  \class Register
 *
 *  \brief Class to store and format the value for a general purpose register
 *
 *  The Register class stores the value of a general purpose register as a
 *  QString which is a hexadecimal representation starting with "0x".
 *
 *  In addition to storing the value of a register, a Register object stores
 *  the register's name.  This is used when formatting rip and eflags which
 *  should not be converted to decimal.
 *
 *  A Register has a format which is also a QString.  Presently the format
 *  should be either "decimal" or "hexadecimal".
 *
 *  It contains functions to set the value, set the format, and retrieve the
 *  value formatted for display.
 */
class Register
{
public:
    /**
     *  Constructor for the Register class
     *
     *  \param name The name of the register
     */
    Register(QString name);

    /**
     *  \fn value
     *
     *  Function to retrieve the value of the register formatted according to the
     *  format value in the Register object.
     *
     *  \return Formatted value as a QString
     */
    QString value();

    /**
     *  \fn setValue
     *
     *  Function to set the value of a Register.
     *
     *  \param value A QString containing the value of the register.
     */
    void setValue(QString value);

    /**
     *  \fn setFormat
     *
     *  Function to set the format for a Register.
     *
     *  \param format A QString holding the desired register format
     */
    void setFormat(QString format);

private:
    QString contents;   ///< The current value of the Register
    QString format;     ///< The format for displaying the Register
    QString name;       ///< The Register's name
};

/**
 *  \class RegisterWindow
 *
 *  \brief Class to display the general purpose registers
 *
 *  The RegisterWindow class is a derived class of QFrame which is a
 *  displayable widget of the Qt collection.  The register window in ebe
 *  displays the general purpose register in a QTableWidget and allows the
 *  user to select the format for either a single register or all the
 *  registers using a popup menu.
 *
 *  Internally registers are stored using a QHash which uses the register
 *  name as the key and a pointer to a Register object as the value for a key.
 *  To facilitate setting the proper item in the table, there is a second
 *  QHash which holds a pointer to a QTableWidgetItem.  This allows rapid
 *  access to the register's value and the place to store the value in
 *  the table.
 *
 *  The registers are received by the RegisterWindow class using the
 *  receiveRegs slot.  This function has a single parameter, a QHash
 *  containing register values indexed by their names.
 *
 *  If the user uses a right click in the register window the
 *  contextMenuEvent function is called.  This is a virtual function
 *  defined in the QFrame class.  This function builds a popup menu depending
 *  on whether the click was on a register name or its value.  If the
 *  click was on a register name, the user is presented with a popup
 *  menu allowing the user to set the format for that register or for all
 *  the registers.
 *
 *  The register window's setFontHeightAndWidth function is called from
 *  the main window whenever the user increases or decreases the font size.
 *  The font height and width values are retained for later internal calls
 *  the font adjustment function when register values change.
 */
class GenericRegisterWindow: public QFrame
{
    Q_OBJECT

public:
    /**
     *  Constructor for the RegisterWindow class
     *
     *  The constructor calls the QFrame constructor before its own
     *  initialization.  Then it sets the appearance of the frame.
     *
     *  The only widget displayed in the RegisterWindow is a QTableWidget
     *  which is created in the constructor and initialized with "0" values
     *  for all the registers.  At this time it sets the QTableWidgetItem
     *  pointers in the registerMap for convenient access when receiving
     *  registers from gdb.
     *
     *  The constructor initializes the QStringList namesList with the
     *  names of all the registers for later convenience.  Then it creates
     *  a Register object for each register and stores these pointers in
     *  the regs map.
     */
    GenericRegisterWindow(QWidget *parent = 0);

    virtual void buildTable() = 0;

    /**
     *  \fn setFontHeightAndWidth
     *
     *  This function sets the height of each row in the table to the
     *  height passed into the function plus 3.  Next it processes each
     *  column finding the maximum length string displayed in the column.
     *  Then it sets the width of the column based on the maximum width
     *  and the font width passed into the function.
     *
     *  \param height The height in pixels for the fixed with font
     *  \param width The width in pixels for the fixed width font
     */
    virtual void setFontHeightAndWidth(int height, int width) = 0;

    /**
     *  \fn setRegister
     *
     *  This function sets the values of a single named register.
     *
     *  \param name The name of the register to set
     *  \param value The new value for the register
     */
    void setRegister(QString name, QString value);

    /**
     *  QHash which provides a register pointer based on a register
     *  name as key.
     */
    QHash<QString, Register *> regs;

    /**
     *  QHash which provides a pointer to a QTableWidgetItem based
     *  on a register name as key.  The QTableWidgetItem is where
     *  the value for a particular register is stored in the table.
     */
    QHash<QString, QTableWidgetItem *> registerMap;

    /**
     *  QTableWidget pointer to the table displayed in the RegisterWindow.
     */
    QTableWidget *table;

    /**
     *  \fn sizeHint
     *
     *  This function is called by Qt when it needs to determine how much
     *  space a widget requires.  For the register window we return a
     *  size of 300 columns and 50 rows.
     *
     *  \return The suggested size for the RegisterWindow
     */
    QSize sizeHint() const;

    QStringList namesList;  ///< A list of all the register names
    int fontWidth;          ///< The width of a fixed width character
    int fontHeight;         ///< The height of a fixed width character

    /**
     *  \fn contextMenuEvent
     *
     *  This function is called when the user right clicks in the RegisterWindow.
     *  If the click happens on a name the user is presented with a popup menu
     *  allowing changing the format for that particular register or changing the
     *  format for all the registers.
     *
     *  If the click happens on a register value, the user will be presented
     *  with a dialog to define a user-defined variable with the address
     *  specified by the register's value.
     *
     *  \param event A pointer to a event object
     */
    void contextMenuEvent(QContextMenuEvent *event);

    signals:
    void sendVariableDefinition(QStringList);public slots:

    void defineVariableByAddress();

    /**
     *  \fn receiveRegs
     *
     *  This slot is called when the gdb object has a new set of register
     *  values to be displayed.  The namesList array is used to cycle
     *  through the names of the registers to set their values.  It calls
     *  setFontHeightAndWidth to resize the rows and columns of the table
     *  based on the latest register contents.
     */
    void receiveRegs(StringHash);

    /**
     *  \fn setDecimal
     *
     *  This function is called when the popup menu action is to set the
     *  format for a single register to "decimal".
     */
    void setDecimal();

    /**
     *  \fn setHex
     *
     *  This function is called when the popup menu action is to set the
     *  format for a single register to "hexadecimal".
     */
    void setHex();

    /**
     *  \fn setDecimalAll
     *
     *  This function is called when the popup menu action is to set the
     *  format for all registers to "decimal".
     */
    void setDecimalAll();

    /**
     *  \fn setHexAll
     *
     *  This function is called when the popup menu action is to set the
     *  format for all registers to "hexadecimal".
     */
    void setHexAll();

};

class RegisterWindow: public GenericRegisterWindow
{
public:
    RegisterWindow(QWidget *parent = 0);
    void buildTable();
    void setFontHeightAndWidth(int height, int width);
};

class HalRegisterWindow: public GenericRegisterWindow
{
public:
    HalRegisterWindow(QWidget *parent = 0);
    void buildTable();
    void setFontHeightAndWidth(int height, int width);
};

class HalNamesWindow: public QFrame
{
public:
    HalNamesWindow(QWidget *parent = 0);
    QTableWidget *table;
    void setFontHeightAndWidth(int height, int width);
    int fontHeight;
    int fontWidth;
};
#endif

