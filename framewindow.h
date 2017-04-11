#ifndef FRAMEWINDOW_H
#define FRAMEWINDOW_H

/*
 RegisterWindow class
 For display of general purpose registers, no public functions
 for set and get registers just yet
 */

#include "types.h"
#include "eztable.h"
#include <QFrame>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QStringList>
#include <QScrollArea>
#include <QSize>

class FrameData
{
public:
    FrameData ( int _currPars=0, int _locals=0, int _newPars=0 );
    int currPars;
    int locals;
    int newPars;
    StringHash *names;
    StringHash *aliasNames;
    StringHash *fpaliasNames;
    QString unalias;
    QString fpunalias;
};

/**
 *  \class FrameWindow
 *
 *  \brief Class to display the current stack frame
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
class FrameWindow: public QFrame
{
    Q_OBJECT

public:
    /**
     *  Constructor for the FrameWindow class
     *
     *  The constructor calls the QFrame constructor before its own
     *  initialization.  Then it sets the appearance of the frame.
     *
     *  The only widget displayed in the FrameWindow is a QTableWidget
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
    FrameWindow(QWidget *parent = 0);

    FrameData *limit;
    int rows;
    QVBoxLayout *layout;
    int returnRow;
    int local1Row;
    QScrollArea *scrollArea;

    void buildTable();
    void rebuildTable();

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
    void setFontHeightAndWidth(int height, int width);

    /**
     *  \fn setRegister
     *
     *  This function sets the values of a single named register.
     *
     *  \param name The name of the register to set
     *  \param value The new value for the register
     */
    void setValue(QString name, QString value);

    /**
     *  QTableWidget pointer to the table displayed in the RegisterWindow.
     */
    EZTable *table;

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

    void nextLine(QString file, int line);

signals:
    void requestStack(int rows);
public slots:

    void receiveStack(QStringList results);

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
    void receiveFrame(StringHash);

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
    void setFloat();
    void setDouble();
    void setFloatAll();
    void setDoubleAll();

};

#endif

