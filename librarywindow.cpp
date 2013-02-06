#define LIBRARYWINDOW_CPP
#include "librarywindow.h"
#include "sourceframe.h"
#include "settings.h"
#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>

extern SourceFrame *sourceFrame;

LibraryWindow::LibraryWindow(QWidget *parent) : QFrame(parent)
{
    setObjectName("Library");
    setWindowTitle("Library");
    setFrameStyle ( QFrame::Panel | QFrame::Raised );
    setLineWidth(4);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(5);
    layout->setContentsMargins(10,10,10,10);

    list = new QListWidget(this);
    layout->addWidget(list);
    setLayout(layout);
    setWindowFlags(Qt::Window|Qt::WindowStaysOnTopHint);
    show();
    connect ( list, SIGNAL(itemClicked(QListWidgetItem*)),
              this, SLOT(handleClick(QListWidgetItem*)) );
    cd ( ebe["library/path"].toString() );
}

QSize LibraryWindow::sizeHint() const
{
    return QSize(300,300);
}

void LibraryWindow::cd ( QString d )
{
    ebe["library/path"] = d;
    list->clear();
    files.clear();
    QDir dir(d);
    files = dir.entryList();
    if ( d != ":/library") files.insert(0,"..");
    QStringList items;
    foreach ( QString file, files ) {
        items << file.replace("_", " ");
    }
    qDebug() << files << items;
    list->addItems(items);
    libraryPath = d;
}

void LibraryWindow::handleClick(QListWidgetItem *it)
{
    qDebug() << "row" << list->currentRow();
    QString file = libraryPath + "/" + files[list->currentRow()];
    qDebug() << "file" << file;
    QFileInfo fi(file);
    if ( fi.isDir() ) {
        cd ( file );
    } else if ( fi.isFile() ) {
        sourceFrame->insertFile(file);
    }
}
