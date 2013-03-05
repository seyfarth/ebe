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
    void setBits( int x, int w, int show=-1 );
    void setText( QString s, int w );
    QSize sizeHint() const;
    int width;
    int show;
    int charSize;

protected:
    void paintEvent ( QPaintEvent *event );
};

#endif
