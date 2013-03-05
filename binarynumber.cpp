#include "binarynumber.h"
#include "settings.h"


BinaryNumber::BinaryNumber()
: QWidget()
{
    width = 16;
    charSize = 20;
    clear();
}

void BinaryNumber::clear()
{
    for ( int i = 0; i < 80; i++ ) chars[i] = ' ';
}

void BinaryNumber::setBits( int x, int w, int s )
{
    width = w;
    show = s >= 0 ? s : w;
    for ( int bit = 0; bit <= w; bit++ ) {
        if ( bit >= show ) chars[bit] = ' ';
        else chars[bit] = (x >> bit) & 1 ? '1' : '0';
    }
}

void BinaryNumber::setText(QString s, int w )
{
    int n = s.length();

    clear();
    for ( int i = 0; i < n; i++ ) {
        chars[n-i-1] = s[i].toAscii();
    }
}

QSize BinaryNumber::sizeHint() const
{
    return ( QSize(charSize*width,charSize) );
}

void BinaryNumber::paintEvent ( QPaintEvent *event )
{
    QString t;
    int x;
    int y;

    QPainter painter(this);
    //painter.setPen(pen);
    //painter.setBrush(brush);
    //painter.setRenderHint(QPainter::AntiAliasing,true);
    QFont font("courier");
    charSize = ebe["font_size"].toInt();
    font.setPixelSize(charSize+3);
    font.setBold(true);
    painter.setFont(font);

    setFixedSize(charSize*width, charSize+4 );
    y = 2+charSize;
    for ( int i = 0; i <= width; i++ ) {
        t = QChar(chars[i]);
        x = charSize/4 + charSize*(width-i-1);
        painter.drawText(x,y,t);
    }
}
