#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include "types.h"
#include <QtGui>
#include "variable.h"
#include <QTreeWidget>

class DataGroup;
class DataItem;

/**
 *  \typedef DataMap
 *
 *  A \c DataMap is a \c QHash mapping the name of a \c DataItem
 *  to the pointer to the \c DataItem in the tree widget displayed in
 *  the data window.  Maps of this type allow updating a \c DataItem
 *  rapidly starting with the name of the \c DataItem.
 */
typedef QHash<QString,DataItem*> DataMap;

/**
 *  \class DataItem
 *
 *  \brief The \c DataItem class define one item in the \c QTreeWidget
 *         corresponding to one variable, array element or class member.
 *
 *  \c DataItem is derived from the \c QTreeWidgetItem class which is
 *  the type for the individual items in a \c QTreeWidget.  In a tree
 *  widget some items can be expanded into multiple sub-items and others
 *  cannot be expanded.  In the data tree the simple data items like
 *  char, short, double, etc can't be expanded while complex data items
 *  like arrays, classes and structs can be expanded into sub-items.
 *
 *  Internally a \c DataItem with simple data holds it in binary form
 *  in an anonymous union of size 8 bytes.  The data received from
 *  \c gdb is in hexadecimal format and is stored in an unsigned
 *  portion of the union of the proper *  size with all the remaining
 *  bits set to 0.  The union has all the simple types in it and, 
 *  depending on the format, the proper member of the union is used
 *  to retrieve the formatted value of the \c DataItem.
 *
 *  In addition to the anonymous union a \c DataItem holds the name
 *  of the data item.  For an array with name "stuff", when expanded
 *  to show 3 array elements with indexes 0, 1 and 2, the child nodes
 *  would have names "stuff[0]", "stuff[1]" and "stuff[2]".  Likewise
 *  if the array is an array of class items, the child nodes of "stuff[0]"
 *  might be named "stuff[0].name" and "stuff[0].address".  This
 *  strategy simplifies the expansion of nodes to arbitrary depth.
 *
 *  There are 4 categories of data in the data window: globals, locals,
 *  parameters and user-defined.  Each of these categories has an
 *  associated DataMap and item \c DataItem contains a pointer to its
 *  \c DataMap.
 */
class DataItem : public QTreeWidgetItem
{
public:
    DataItem();
    QString value();
    QString name;       ///< Full name of the \c DataItem
    QString type;       ///< Type as reported by \c gdb
    QString address;    ///< For user-defined variables
    QString format;     ///< How the \c DataItem should look
    QVector<Limits> dimensions;  ///< Array limits
    bool isSimple;      ///< Is the data simple, like char or int
    bool isFortran;     ///< Was this a fortran variable
    int size;           ///< Number of bytes of data
    int first;          ///< First index to expand for arrays/pointers
    int last;           ///< Last index to expand for arrays/pointers
    bool userDefined;   ///< Is this defined by the user during debugging
    void setName(QString n);
    void setValue(QString v);
    void setType(QString t);
    void setRange(int f, int l);
    void removeSubTree();
    QString valueFromGdb();
    DataMap *map;       ///< Map containing this \c DataItem
    QString stringValue;///< Value printed by \c gdb

    AllTypes a;         ///< Union of all basic types
};

/**
 *  \class DataTree
 *
 *  \c DataTree is derived from the Qt \c QTreeWidget class which
 *  provides a tree view for displaying data.
 *
 *  There is one \c DataTree in the data window and it contains
 *  4 top-level items: \c globals, \c locals, \c parameters and
 *  \c userDefined.  Each of these \c DataItems serves as the root
 *  of their associated data items.
 *
 *  Global data is any variable defined outside a function (or class) or defined
 *  as \c static inside a function (or class).  These items are discovered
 *  by inspecting the object files for a program before linking to avoid
 *  cluttering the screen with a large number of global data associated with
 *  library functions.
 *
 *  Local data means variable defined without the keyword \c static inside
 *  a function.  These are provided by \c gdb at each point that the debugger
 *  stops and reports the state of the debugged program.
 *
 *  Parameters are the data items enclosed in parentheses 
 */
class DataTree : public QTreeWidget
{
    Q_OBJECT

public:
    DataTree(QWidget *parent = 0);
    ~DataTree();
    void contextMenuEvent(QContextMenuEvent *event);
    DataItem *globals;
    DataItem *locals;
    DataItem *parameters;
    DataItem *userDefined;
    DataMap *globalMap;
    DataMap *localMap;
    DataMap *parameterMap;
    DataMap *userDefinedMap;
    DataItem *addDataItem(DataMap *map, QString n,
                          QString t, QString v);
    QList<Limits> dimensions;


public slots:
    void expandDataItem(QTreeWidgetItem*);
    void collapseDataItem(QTreeWidgetItem*);
    void editUserVariable();
    void deleteUserVariable();
    void setDecimal();
    void setFloatingPoint();
    void setBool();
    void setBinary();
    void setBinaryFP();
    void setFields();
    void setCharacter();
    void setUnsignedDecimal();
    void setHexadecimal();

private:

signals:
};

class DataWindow : public QFrame
{
    Q_OBJECT

public:
    DataWindow(QWidget *parent=0);
    void setFontHeightAndWidth(int height, int width);
    void request(DataItem *item);
    int level;
    QVBoxLayout *layout;

private slots:
    void receiveVariableDefinition(QStringList);
    void resetData();
    void receiveClasses(QHash<QString,ClassDefinition> c);
    void receiveVar(DataMap *group, QString name, QString value);
    void receiveGlobals(QList<VariableDefinition>);
    void receiveLocals(QList<VariableDefinition>);
    void receiveParameters(QList<VariableDefinition>);

private:
    QSize sizeHint() const;
    int fontHeight;
    int fontWidth;

signals:
    void requestVar(DataMap*,QString,QString,QString,QString,int,int,int);
};

#endif
