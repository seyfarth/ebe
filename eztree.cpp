#include "eztree.h"

static const int maxLevels = 9;

EZTreePlank::EZTreePlank(QWidget *parent)
    : EZPlank(parent)
{
    treeLevel = -1;
    parent = 0;
    kidCount = 0;
    state = EZ::Simple;
}

EZTreePlank *EZTreePlank::child( int k )
{
    return kids[k];
}

EZTreePlank *EZTreePlank::addPlank( QString name )
{
    EZTreePlank *p;
    kidCount++;
    p = tree->insertPlank(plankNumber+kidCount,name);
    p->treeLevel = treeLevel+1;
    p->tree = tree;
    kids.append(p);
    return p;
}

EZTree::EZTree(QWidget *parent)
    : EZTable(parent)
{
}

EZTreePlank *EZTree::addPlank( QString name )
{
    EZTreePlank *p;
    p = new EZTreePlank(this);
    table.append(p);
    p->show();
    p->plankNumber = planks;
    setText(planks,maxLevels,name);
    p->tree = this;
    planks++;
    return p;
}

EZTreePlank *EZTree::insertPlank( int index, QString name )
{
    EZTreePlank *p;
    p = new EZTreePlank(this);
    table.insert(index,p);
    p->show();
    p->plankNumber = planks;
    setText(planks,maxLevels,name);
    p->tree = this;
    planks++;
    return p;
}

EZCell * EZTree::cell ( int r, int c )
{
    return EZTable::cell(r,c+maxLevels);
}

QString EZTree::text ( int r, int c )
{
    return EZTable::text(r,c+maxLevels);
}

void EZTree::setText ( int r, int c, QString t, EZ::Color highlight )
{
    EZTable::setText ( r, c+maxLevels, t, highlight );
}

void EZTree::setText ( int p, int r, int c, QString t, EZ::Color highlight )
{
    EZTable::setText ( p, r, c+maxLevels, t, highlight );
}

QWidget * EZTree::widget ( int r, int c )
{
    return cell(r,c)->widget;
}

void EZTree::setColumnCount(int cols)
{
    EZTable::setColumnCount ( cols+maxLevels );
}

void EZTree::setColumnWidth ( int col, int wid )
{
    EZTable::setColumnWidth ( col+maxLevels, wid );
}

void EZTree::addWidget ( QWidget *w, int row, int col )
{
    EZTable::addWidget ( w, row, col+maxLevels );
}

void EZTree::setSpan ( int r, int c, int numRows, int numCols )
{
    EZTable::setSpan ( r, c+maxLevels, numRows, numCols );
}
