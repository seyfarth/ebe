#include "bitbucket.h"
#include "stylesheet.h"
#include "settings.h"

#include <iostream>
using namespace std;

BitBucket::BitBucket(QWidget *parent)
    : QFrame(parent)
{
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(4);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(2);
    layout->setContentsMargins(10, 10, 10, 10);

    tab = new QTabWidget;

    unaryPanel = new UnaryBitPanel;
    binaryPanel = new BinaryBitPanel;
    intConvertPanel = new IntConvert;
    intMathPanel = new IntMath;
    floatConvertPanel = new FloatConvert;

    tab->addTab(unaryPanel, tr("Unary operators"));
    tab->addTab(binaryPanel, tr("Binary bit operators"));
    tab->addTab(intConvertPanel, tr("Integer conversions"));
    tab->addTab(intMathPanel, tr("Integer math"));
    tab->addTab(floatConvertPanel, tr("Float conversions"));

    layout->addWidget(tab);

    //connect ( tab, SIGNAL(currentChanged(int)), this, SLOT(changedTab(int)) );

    setLayout(layout);

}

void BitBucket::setFontHeightAndWidth(int height, int width)
{
    fontHeight = height;
    fontWidth = width;
    unaryPanel->setFontHeightAndWidth(height, width);
    binaryPanel->setFontHeightAndWidth(height, width);
    intConvertPanel->setFontHeightAndWidth(height, width);
    intMathPanel->setFontHeightAndWidth(height, width);
    floatConvertPanel->setFontHeightAndWidth(height, width);
}

