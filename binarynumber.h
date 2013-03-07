#ifndef BINARYNUMBER_H
#define BINARYNUMBER_H

#include <QtGui>


class BinaryNumber : public QWidget
{
    Q_OBJECT

public:
    BinaryNumber();
    char chars[80];

    void clear();
    void setNibbles( int x, int w, int show=-1 );
    void setBits( int x, int w, int show=-1 );
    void setBit(int x, int b);
    void setText( QString s, int w );
    void setHighlight(int l, int h=-1);
    QSize sizeHint() const;
    int width;
    int show;
    int highlightLow, highlightHigh;
    int charSize;

protected:
    void paintEvent ( QPaintEvent *event );
};

#endif
