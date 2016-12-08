#ifndef EZTREE_H
#define EZTREE_H

#include "eztable.h"

class EZTree;

class EZTreePlank: public EZPlank
{
    Q_OBJECT

public:
    EZTreePlank(QWidget *parent=0);
    int treeLevel;
    int plankNumber;
    int kidCount;
    EZTree *tree;
    EZTreePlank *parent;
    QVector<EZTreePlank *> kids;
    void collapse();
    EZTreePlank *child(int k);
    EZTreePlank *addPlank( QString name );
    EZ::State state;
};

//
//    The underlying table pretends that the first 10 columns are 2
//    columns.  Internally EZTree will translate all requests for
//    columns based on using some empty columns, a column for the
//    expand/collapse indicator and a span for the user's first column,
//    Each time a plank is expanded, the children will be indented 1
//    more level than the parent.  10 columns leaves 9 levels possible.
//    That should be more than enough.  For simplicity's sake the
//    maximum number of levels is a const int.  I may decide to reduce
//    it.  The level of an EZTreePlank will indicate how many columns
//    to leave empty (level-1). The span for the first user column (0
//    to the user) starts at the plank's level number and goes through
//    column 10.
//
class EZTree: public EZTable
{
    Q_OBJECT

public:
    EZTree(QWidget *parent = 0);
    QWidget *widget(int row, int col);

    EZTreePlank *addPlank( QString name );
    EZTreePlank *insertPlank( int index, QString name );
    EZTreePlank *insertPlankAfter( int p, QString name );
    void setText ( int r, int c, QString t, EZ::Color highlight=EZ::Default );
    void setText ( int p, int r, int c, QString t, EZ::Color highlight=EZ::Default );
    void setColumnCount(int cols);
    void setColumnWidth ( int col, int wid );
    void addWidget ( QWidget *w, int row, int col );
    EZCell *cell(int r, int c);
    void setSpan ( int r, int c, int numRows, int numCols );
    QString text(int r, int c);
};
#endif

