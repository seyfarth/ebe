#include "eztable.h"

EZPlank *latestPlank;
EZPlank *thePlank;

EZCell::EZCell(int r, int c)
    : QWidget()
{
    row = r;
    column = c;
    cellWidth = 0;
    spanning = false;
    spanned = false;
    text = "";
    color = EZ::Default;
    visible = true;
    widget = 0;
    label = new QLabel();
    label->setText("");
    label->show();
}

EZCell::EZCell()
    : QWidget()
{
    row = -1;
    column = -1;
    cellWidth = 0;
    spanning = false;
    spanned = false;
    spannedRows = 1;
    spannedColumns = 1;
    text = "";
    color = EZ::Default;
    visible = true;
    widget = 0;
    label = new QLabel(thePlank);
    label->setText("");
    label->show();
}

void EZCell::setText ( QString t, EZ::Color highlight )
{
    QString old = text;
    text = t;

    switch ( highlight ) {
    case EZ::Default:
        break;
    case EZ::Highlight:
        if ( old != t ) {
            label->setStyleSheet("color: red;");
        } else {
            label->setStyleSheet("color: black;");
        }
        break;
    case EZ::Bold:
        //qDebug() << "setText" << r << c << old << t;
        label->setStyleSheet("font-family: arial; font-weight:800;");
        break;
    case EZ::NoChange:
        break;
    }
    label->setText(t);
}

EZRow::EZRow()
{
}

EZPlank::EZPlank(QWidget *parent)
    : QWidget(parent)
{
    rows = 0;
    columns = 0;
}

void EZPlank::enterEvent ( QEvent * /* e */)
{
    latestPlank = this;
}

void EZPlank::leaveEvent ( QEvent * /* e */)
{
}

EZTable::EZTable(QWidget *parent)
    : QWidget(parent)
{
    columns = 0;
    planks = 0;
    setPlankCount(1);
    thePlank = currentPlank = table[0];
    fontWidth = ebe["font_size"].toInt() * 2 / 3;
    fontHeight = 2*fontHeight;
    ezrowHeight = 1.1 * fontHeight;
    setStyleSheet( QString("font-weight:bold;"
                           "font-family: %1;"
                           " background: %2;")
                           .arg(ebe["fixed_font"].toString())
                           .arg(ebe["table_bg"].toString()));
}

int EZTable::rowCount()
{
    return currentPlank->rows;
}

void EZTable::setText ( int r, int c, QString t, EZ::Color highlight )
{
    currentPlank->ezrows[r]->ezcells[c]->setText(t, highlight);
}

QString EZTable::getText ( int r, int c )
{
    return currentPlank->ezrows[r]->ezcells[c]->text;
}

void EZTable::setText ( int p, int r, int c, QString t, EZ::Color highlight )
{
        table[p]->ezrows[r]->ezcells[c]->setText(t, highlight);
}

void EZTable::addWidget ( QWidget *w, int r, int c )
{
    EZCell *cell = currentPlank->ezrows[r]->ezcells[c];
    w->setParent(currentPlank);
    cell->widget = w;
    if ( w ) {
        if ( cell->spanning ) {
            w->resize ( cell->spanWidth, cell->spanHeight );
        } else {
            w->resize ( cell->cellWidth, cell->cellHeight );
        }
        w->show();
    }
}

EZCell * EZTable::cell ( int r, int c )
{
    return currentPlank->ezrows[r]->ezcells[c];
}

void EZTable::setSpan ( int r, int c, int numRows, int numCols ) 
{
    EZCell *cell = currentPlank->ezrows[r]->ezcells[c];
    if ( cell->spanned ) {
        qDebug() << "Cell at row" << r << "column" << c << "span problem";
        qDebug() << "It is part of an existing span";
    }

    if ( numRows < 1 ) numRows = 1;
    if ( numCols < 1 ) numCols = 1;
    EZCell *cell2;
    if ( cell->spanning ) {
        for ( int row=r; row < r+cell->spannedRows; row++ ) {
            for ( int col=c; col < c+cell->spannedColumns; col++ ) {
                if ( row != r || col != c ) {
                    cell2 = currentPlank->ezrows[row]->ezcells[col];
                    cell2->spanned = false;
                    cell2->visible = true;
                    if ( cell2->widget ) {
                        cell2->widget->show();
                    } else {
                        cell2->label->show();
                    }
                }
            }
        }
    }

    if ( numRows == 1 && numCols == 1 ) {
        cell->spanning = false;
        return;
    }
    cell->spanning = true;
    cell->spannedRows = numRows;
    cell->spannedColumns = numCols;
    cell->spanWidth = fontWidth;
    cell->spanHeight = ezrowHeight*cell->spannedRows;

    for ( int row=r; row < r+numRows; row++ ) {
        for ( int col=c; col < c+numCols; col++ ) {
            if ( row != r || col != c ) {
                cell2 = currentPlank->ezrows[row]->ezcells[col];
                cell2->spanned = true;
                cell2->visible = false;
                cell2->label->hide();
                if ( cell2->widget ) {
                    cell2->widget->hide();
                }
            }
        }
    }

    QWidget *w = cell->widget;
    QLabel *label = cell->label;
    if ( w ) {
        w->show();
        w->resize ( cell->spanWidth, cell->spanHeight );
    } else {
        label->show();
        label->resize ( cell->spanWidth, cell->spanHeight );
    }
}

void EZTable::setFontHeightAndWidth ( int ht, int wid )
{
    if ( ht == fontHeight && wid == fontWidth ) return;

    ezrowHeight = ht*1.1;
    fontHeight = ht;
    fontWidth = wid;
    //resizeToFitContents();
}

QString EZTable::text(int r, int c)
{
    return currentPlank->ezrows[r]->ezcells[c]->text;
}

void EZTable::resizeToFitContents(int f)
{
    EZCell *cell;
    QVector<int> maxWidth;
    int n;
    int x;
    int totalRows=0;

    //if (f) qDebug() << "resizeToFitContents" << ezrowHeight << planks << table.size() << f;

    maxWidth.resize(columns);

    for ( int c = 0; c < f; c++ ) {
        maxWidth[c] = fontWidth;
    }
    for ( int c= f; c < columns; c++ ) {
        maxWidth[c] = 0;
    }
    //if (f) qDebug() << "maxWidth b4" << maxWidth;

    foreach ( EZPlank *p, table ) {
        //if (f) qDebug() << p->rows;
        for ( int r=0; r < p->rows; r++ ) {
            for ( int c = 0; c < f; c++ ) {
                cell = p->ezrows[r]->ezcells[c];
                if ( cell->spanning ) {
                    n = (cell->text.size() - cell->spannedColumns)*fontWidth
                        + fontWidth;
                    if ( n > maxWidth[f] ) {
                        maxWidth[f] = n;
                    }
                    break;
                }
            }
            int first = 0;
            if ( f > 0 ) first = f+1;
            for ( int c = first; c < columns; c++ ) {
                cell = p->ezrows[r]->ezcells[c];
                n = cell->text.size()*fontWidth+fontWidth;
                if ( cell->spanning ) {
                    n = (n + cell->spannedColumns - 1) / cell->spannedColumns;
                    for ( int c2=c; c2 < c + cell->spannedColumns; c2++ ) {
                        if ( n > maxWidth[c2] ) {
                            maxWidth[c2] = n;
                        }
                    }
                } else if ( !cell->spanned && n > maxWidth[c] ) {
                    maxWidth[c] = n;
                }
            }
        }
        totalRows += p->rows;
    }
    //if (f) qDebug() << "maxWidth" << maxWidth;

    foreach ( EZPlank *p, table ) {
        //if (f) qDebug() << p->rows;
        for ( int r = 0; r < p->rows; r++ ) {
            x = 0;
            for ( int c = 0; c < f; c++ ) {
                cell = p->ezrows[r]->ezcells[c];
                cell->cellHeight = ezrowHeight;
                cell->spanHeight = cell->spannedRows*ezrowHeight;
                cell->pos_x = x;
                cell->pos_y = r * ezrowHeight;
                if ( cell->spanning ) {
                    cell->cellWidth = (maxWidth[f] +
                         (f-c)*fontWidth) + fontWidth;
                    x += cell->cellWidth;
                    break;
                } else {
                    cell->cellWidth = maxWidth[c];
                }
                x += cell->cellWidth;
            }
            int first = 0;
            if ( f > 0 ) first = f+1;
            for ( int c = first; c < columns; c++ ) {
                cell = p->ezrows[r]->ezcells[c];
                cell->cellWidth = maxWidth[c];
                cell->cellHeight = ezrowHeight;
                cell->spanHeight = cell->spannedRows*ezrowHeight;
                cell->pos_x = x;
                cell->pos_y = r * ezrowHeight;
                x += cell->cellWidth;
            }
        }
    }

    int y = 0;
    foreach ( EZPlank *p, table ) {
        p->setGeometry( QRect(0,y,x,p->rows*ezrowHeight) );
        y += p->rows * ezrowHeight;
        //if (f) qDebug() << p->rows << p->geometry();
        for ( int r = 0; r < p->rows; r++ ) {
            for ( int c = 0; c < f; c++ ) {
                cell = p->ezrows[r]->ezcells[c];
                if ( cell->spanning ) {
                    cell->spanWidth = (maxWidth[f] +
                            (f-c)*fontWidth) + fontWidth;
                    if ( cell->widget ) {
                        cell->widget->move(cell->pos_x,cell->pos_y);
                        cell->widget->resize ( cell->spanWidth, cell->spanHeight );
                    } else {
                        cell->label->move(cell->pos_x,cell->pos_y);
                        cell->label->resize ( cell->spanWidth, cell->spanHeight );
                        //if ( f ) qDebug() << r << c //<< cell->label->geometry();
                    }
                    break;
                } else {
                    if ( cell->widget ) {
                        cell->widget->move(cell->pos_x,cell->pos_y);
                        cell->widget->resize ( cell->cellWidth, cell->cellHeight );
                    } else {
                        cell->label->move(cell->pos_x,cell->pos_y);
                        cell->label->resize ( cell->cellWidth, cell->cellHeight );
                        //if ( f ) qDebug() << r << c << cell->label->geometry();
                    }
                }
            }
            int first = 0;
            if ( f > 0 ) first = f+1;
            for ( int c = first; c < columns; c++ ) {
                cell = p->ezrows[r]->ezcells[c];

                //if ( f ) qDebug() << r << c << cell->cellWidth << cell->text << "vis" << cell->visible;
                if ( cell->spanning ) {
                    cell->spanWidth = 0;
                    for ( int c2=c; c2 < c+cell->spannedColumns; c2++ ) {
                        cell->spanWidth += maxWidth[c2];
                    }
                    if ( cell->widget ) {
                        cell->widget->move(cell->pos_x,cell->pos_y);
                        cell->widget->resize ( cell->spanWidth, cell->spanHeight );
                    } else {
                        cell->label->move(cell->pos_x,cell->pos_y);
                        cell->label->resize ( cell->spanWidth, cell->spanHeight );
                    }
                    //if ( f ) qDebug() << r << c << "span" << cell->spanWidth << cell->text << cell->label->geometry();
                } else {
                    if ( cell->widget ) {
                        cell->widget->move(cell->pos_x,cell->pos_y);
                        cell->widget->resize ( cell->cellWidth, cell->cellHeight );
                    } else {
                        cell->label->move(cell->pos_x,cell->pos_y);
                        cell->label->resize ( cell->cellWidth, cell->cellHeight );
                    }
                }
            }
        }
    }

    //foreach ( EZPlank *p, table ) {
        //for ( int r = 0; r < p->rows; r++ ) {
            //for ( int c = 0; c < columns; c++ ) {
                //cell = &p->ezrows[r].ezcells[c];
                //if ( cell->spanning ) {
                    //if ( cell->widget ) {
                        //cell->widget->move(cell->pos_x,cell->pos_y);
                        //cell->widget->resize ( cell->spanWidth, cell->spanHeight );
                    //} else {
                        //cell->label->move(cell->pos_x,cell->pos_y);
                        //cell->label->resize ( cell->spanWidth, cell->spanHeight );
                    //}
                //} else {
                    //if ( cell->widget ) {
                        //cell->widget->move(cell->pos_x,cell->pos_y);
                        //cell->widget->resize ( cell->cellWidth, cell->cellHeight );
                    //} else {
                        //cell->label->move(cell->pos_x,cell->pos_y);
                        //cell->label->resize ( cell->cellWidth, cell->cellHeight );
                    //}
                //}
            //}
        //}
    //}
    //foreach ( EZPlank *p, table ) layout->removeWidget(p);
    //foreach ( EZPlank *p, table ) layout->addWidget(p);
    setGeometry ( QRect(0,0,x,totalRows*ezrowHeight) );
    //int y = 0;
    //foreach ( EZPlank *p, table ) {
        //p->setGeometry( QRect(0,y,x,p->rows*ezrowHeight) );
        //for ( int r = 0; r < p->rows; r++ ) {
            //for ( int c = 0; c < columns; c++ ) {
                //cell = &p->ezrows[r].ezcells[c];
                ////qDebug() << "cell" << r << c << cell->label->geometry();
                //if ( ! cell->spanned ) {
                    //if ( cell->widget ) {
                        //cell->widget->show();
                    //} else {
                        //cell->label->setText(cell->text);
                        //cell->label->show();
                    //}
                //}
            //}
        //}
        //y += p->height();
    //}
    //qDebug() << geometry();
}

void EZTable::setCurrentPlank ( EZPlank *p )
{
    currentPlank = p;
}

void EZTable::setColumnWidth ( int col, int wid )
{
    EZCell *cell;
    if ( columnWidths.size() < col+1 ) columnWidths.resize(col+1);
    columnWidths[col] = wid;
    foreach ( EZPlank *p, table ) {
        for ( int r = 0; r < p->rows; r++ ) {
            cell = p->ezrows[r]->ezcells[col];
            if ( cell->spanning ) {
                cell->spanWidth += wid - cell->spanWidth;
            }
            cell->cellWidth = wid;
        }
    }
}

QSize EZTable::sizeHint()
{
    return QSize(200,50);
}

void EZTable::hideColumn ( int /*c*/ )
{
}

void EZTable::setPlankCount ( int planks_ )
{
    table.resize(planks_);
    for ( int p=planks; p < planks_; p++ ) {
        table[p] = new EZPlank(this);
        table[p]->show();
        //layout->addWidget(table[p]);
    }
    planks = planks_;
}

void EZTable::setRowCount(int rows_)
{
    EZCell *cell;
    EZPlank *p = currentPlank;
    int x;

    thePlank = p;
    columnWidths.resize(columns);
    p->columns = columns;
    for ( int r=rows_; r < p->rows; r++ ) {
        for ( int c=0; c < p->columns; c++ ) {
            cell = p->ezrows[r]->ezcells[c];
            cell->label->hide();
            delete cell->label;
            if ( cell->widget ) {
                cell->widget->hide();
                delete cell->widget;
            }
            delete cell;
        }
        delete p->ezrows[r];
    }
    //qDebug() << "cleared old";
    p->ezrows.resize(rows_);
    
    for ( int r=p->rows; r < rows_; r++ ) {
        p->ezrows[r] = new EZRow;
        //qDebug() << "working on" << r;
        p->ezrows[r]->ezcells.resize(p->columns);
        x = 0;
        for ( int c = 0; c < columns; c++ ) {
            //qDebug() << "cell" << c;
            cell = new EZCell;
            p->ezrows[r]->ezcells[c] = cell;
            cell->table = this;
            cell->text = "";
            cell->label->setText("");
            cell->parent = p;
            cell->row = r;
            cell->column = c;
            cell->pos_x = x;
            cell->pos_y = r * ezrowHeight;
            cell->cellHeight = ezrowHeight;
            cell->cellWidth = columnWidths[c];
            x += cell->cellWidth;
        }
    }
    p->rows = rows_;
    //p->resize(x,p->rows*ezrowHeight);
    //resizeToFitContents();
}

void EZTable::setColumnCount(int cols)
{
    EZCell *cell;

    foreach ( EZPlank *p, table ) {
        thePlank = p;
        if ( p->rows > 0 ) {
            for ( int r=0; r < p->rows; r++ ) {
                //qDebug() << "setColumnCount" << r;
                for ( int c=cols; c < columns; c++ ) {
                    cell = p->ezrows[r]->ezcells[c];
                    cell->label->hide();
                    delete cell->label;
                    if ( cell->widget ) {
                        cell->widget->hide();
                        delete cell->widget;
                    }
                    delete cell;
                }
                p->ezrows[r]->ezcells.resize(cols);
                columnWidths.resize(cols);
                //qDebug() << "setColumnCount resized" << r;
                int x = 0;
                if ( columns > 0 ) x = p->ezrows[0]->ezcells[columns-1]->pos_x;
                for ( int c = p->columns; c < cols; c++ ) {
                    columnWidths[c] = 2.7*fontWidth;
                    cell = new EZCell;
                    //cell->setParent(p);
                    p->ezrows[r]->ezcells[c] = cell;
                    cell->table = this;
                    cell->parent = p;
                    cell->row = r;
                    cell->column = c;
                    cell->cellHeight = ezrowHeight;
                    cell->cellWidth = columnWidths[c];
                    cell->pos_y = ezrowHeight * r;
                    cell->pos_x = x;
                    x += cell->cellWidth;
                }
            }
        }
        p->columns = cols;
    }
    columns = cols;
}
