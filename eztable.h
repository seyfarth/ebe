#ifndef EZTABLE_H
#define EZTABLE_H

#include "types.h"
#include "settings.h"
#include <QDebug>
#include <QVector>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QSize>

class EZ {
public:
    enum Color { Default, Bold, NoChange, Highlight };
    enum State { Simple, Expanded, Collapsed };
};

class EZPlank;
class EZTable;

class EZCell: QWidget
{
    Q_OBJECT

public:
    EZCell(int r, int c);
    EZCell();
    int row;
    int column;
    int pos_x;
    int pos_y;
    int cellWidth;
    int cellHeight;
    int spanWidth;
    int spanHeight;
    bool spanning;
    int spannedRows;
    int spannedColumns;
    bool spanned;
    EZPlank *parent;
    EZTable *table;
    QString text;
    EZ::Color color; 
    QLabel *label;
    QWidget *widget;
    bool visible;
    void hide();
    void show();
    void setText(QString t, EZ::Color highlight=EZ::Default);
};

class EZRow
{
public:
    EZRow();
    QVector<EZCell*> ezcells;
};

class EZPlank: public QWidget
{
    Q_OBJECT

public:
    EZPlank(QWidget *parent=0);
    QVector<EZRow*> ezrows;
    int rows;
    int columns;
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
};

class EZTable: public QWidget
{
    Q_OBJECT

public:
    EZTable(QWidget *parent = 0);
    QWidget *widget_;
    QWidget *widget(int row, int col);
    int fontHeight;
    int fontWidth;
    int ezrowHeight;
    EZPlank *currentPlank;
    int planks;
    int columns;
    QVBoxLayout *layout;

    QVector<EZPlank *> table;
    QVector<double> widthMultipliers;    // one per column
    QVector<int> columnWidths;
    int rowCount();
    void setCurrentPlank(EZPlank *plank);
    void setCurrentPlank(int i) {
        currentPlank = table[i];
    }
    QString getText ( int r, int c );
    void setText ( int r, int c, QString t, EZ::Color highlight=EZ::Default );
    void setText ( int p, int r, int c, QString t, EZ::Color highlight=EZ::Default );
    void setPlankCount(int planks);
    void setRowCount(int rows);
    void setColumnCount(int cols);
    void setColumnWidth ( int col, int wid );
    void addWidget ( QWidget *w, int row, int col );
    EZCell *cell(int r, int c);
    void setSpan ( int r, int c, int numRows, int numCols );
    void setFontHeightAndWidth(int h, int w);
    void resizeToFitContents(int fixedLevels=0);
    QSize sizeHint() const;
    void hideColumn(int c);
    QString text(int r, int c);
};
#endif

