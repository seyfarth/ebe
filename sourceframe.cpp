#include "sourceframe.h"
#include "sourcewindow.h"
#include "commandline.h"
#include "stylesheet.h"
#include "settings.h"

SourceFrame::SourceFrame(QWidget *parent) : QFrame(parent)
{
    setFrameStyle ( QFrame::Panel | QFrame::Raised );
    setLineWidth(4);

    initStyleSheet("sourceframe","QPushButton { font-family: " +
                   ebe["variable_font"].toString() + "}" +
                   "QLabel { font-family:" +
                   ebe["variable_font"].toString() + "}" );

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(2);
    layout->setContentsMargins(10,10,10,10);
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(6);
    buttonLayout->setContentsMargins(2,2,2,2);

    quitButton     = new QPushButton ( "Quit" );
    quitButton->setStyleSheet ( "color: "+ebe["quit_color"].toString() );
    quitButton->setToolTip ( tr("Click this button to exit from ebe") );
    runButton      = new QPushButton ( "Run" );
    runButton->setStyleSheet ( "color: "+ebe["run_color"].toString() );
    nextButton     = new QPushButton ( "Next" );
    nextButton->setStyleSheet ( "color: "+ebe["next_color"].toString() );
    stepButton     = new QPushButton ( "Step" );
    stepButton->setStyleSheet ( "color: "+ebe["step_color"].toString() );
    continueButton = new QPushButton ( "Continue" );
    continueButton->setStyleSheet ( "color: "+ebe["continue_color"].toString() );
    stopButton     = new QPushButton ( "Stop" );
    stopButton->setStyleSheet ( "color: "+ebe["stop_color"].toString() );
    buttonLayout->addWidget ( quitButton );
    buttonLayout->addWidget ( runButton );
    buttonLayout->addWidget ( nextButton );
    buttonLayout->addWidget ( stepButton );
    buttonLayout->addWidget ( continueButton );
    buttonLayout->addWidget ( stopButton );
    buttonLayout->addStretch();


    connect ( quitButton, SIGNAL(clicked()), parent, SLOT(quit()) );

    commandLine = new CommandLine();

    tab = new QTabWidget;

    layout->addLayout(buttonLayout);
    layout->addWidget(commandLine);
    layout->addWidget(tab);


    setLayout(layout);

    source = new SourceWindow;
    int index = tab->addTab(source,"file 0");
    tab->setCurrentIndex(index);

}

void SourceFrame::setFontHeightAndWidth ( int height, int width )
{
    int current = tab->currentIndex();
    int count = tab->count();
    for ( int i = 0; i < count; i++ ) {
        source = (SourceWindow *)tab->widget(i);
        source->setFontHeightAndWidth(height,width);
    }
    tab->setCurrentIndex(current);
}

void SourceFrame::openInNewTab(QString name)
{
}

void SourceFrame::closeTabs()
{
}

void SourceFrame::setCommandLineVisible(bool visible)
{
    ebe["command/visible"] = visible;
    commandLine->setVisible(visible);
}

void SourceFrame::open()
{
    source = new SourceWindow;
    int count = tab->count();
    int index = tab->addTab(source,"file "+QString::number(count));
    tab->setCurrentIndex(index);
    source->open();
}

void SourceFrame::save()
{
    source->save();
}

void SourceFrame::saveAs()
{
    source->saveAs();
}
