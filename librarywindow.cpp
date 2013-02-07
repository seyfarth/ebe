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
    QString indent;
    QString p = ":/library";
    QStringList items;
    int i;
    QStringList dirs;
    while ( p != d && indent.length() < 20 ) {
        qDebug() << "adding" << p;
        dirs << p;
        i = p.lastIndexOf("/");
        if ( i == 1 ) {
            items << "top";
        } else {
            items << indent + p.mid(i+1);
        }
        i = p.length()+1;
        while ( i < d.length() && d[i] != QChar('/') ) i++;
        p = d.left(i);
        qDebug() << "p is now" << p;
        indent += "  ";
    }

    files = dir.entryList();
    foreach ( QString file, files ) {
        dirs << d + "/" + file;
        items << indent + file.replace("_", " ");
    }
    files = dirs;
    qDebug() << files << items;
    list->addItems(items);
    libraryPath = d;
}

void LibraryWindow::handleClick(QListWidgetItem *it)
{
    qDebug() << "row" << list->currentRow();
    QString file = files[list->currentRow()];
    qDebug() << "file" << file;
    QFileInfo info(file);
    if ( info.isDir() ) {
        cd ( file );
    } else if ( info.isFile() ) {
        sourceFrame->insertFile(file);
    }
}
