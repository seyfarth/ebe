#define LIBRARYWINDOW_CPP
#include "librarywindow.h"
#include "sourceframe.h"
#include "settings.h"
#include <QApplication>
#include <QListWidget>
#include <QTextBrowser>
#include <QListWidgetItem>
#include <QClipboard>
#include <QVBoxLayout>
#include <QHBoxLayout>

extern SourceFrame *sourceFrame;

LibraryWindow::LibraryWindow(QWidget *parent)
    : QFrame(parent)
{
    setObjectName("Library");
    setWindowTitle(tr("Library"));
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(4);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(5);
    layout->setContentsMargins(10, 10, 10, 10);

    clipboard = QApplication::clipboard();
    list = new QListWidget(this);
    layout->addWidget(list);
    setLayout(layout);
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    show();
    text = NULL;
    connect ( list, SIGNAL(itemClicked(QListWidgetItem*)),
        this, SLOT(handleClick(QListWidgetItem*)) );
    cd(ebe["library/path"].toString());
}

QSize LibraryWindow::sizeHint() const
{
    return QSize(400, 350);
}

void LibraryWindow::cd(QString d)
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
    while (p != d && indent.length() < 20) {
        //qDebug() << "adding" << p;
        dirs << p;
        i = p.lastIndexOf("/");
        if (i == 1) {
            items << "top";
        } else {
            items << indent + p.mid(i + 1);
        }
        i = p.length() + 1;
        while (i < d.length() && d[i] != QChar('/'))
            i++;
        p = d.left(i);
        //qDebug() << "p is now" << p;
        indent += "  ";
    }
    dirs << p;
    i = p.lastIndexOf("/");
    if (i == 1) {
        items << "top";
    } else {
        items << indent + p.mid(i + 1);
    }
    indent += "  ";

    QFile f(p + "/index");
    if (f.open(QFile::ReadOnly)) {
        QString s;
        while ((s = f.readLine()) != "") {
            files << s.trimmed();
        }
        f.close();
    } else {
        files = dir.entryList();
    }
    foreach ( QString file, files )
    {
        dirs << d + "/" + file;
        items << indent + file.replace("_", " ").
        replace(".html","").replace(".htm","");
    }
    files = dirs;
    //qDebug() << files << items;
    list->addItems(items);
    libraryPath = d;

    QListWidgetItem *it;
    int n = files.count();
    for (i = 0; i < n; i++) {
        it = list->item(i);
        QFileInfo info(files[i]);
        if (info.isDir()) {
            //qDebug() << i << "dir" << files[i];
            it->setToolTip(tr("changes to a new directory"));
        } else if (info.isFile()) {
            if (files[i].indexOf(".htm") > 0) {
                it->setToolTip(tr("view a web page"));
            } else {
                it->setToolTip(tr("inserts code into the editor"));
            }
        }
    }
}

void LibraryWindow::handleClick(QListWidgetItem * /* it */)
{
    int fontSize = ebe["font_size"].toInt();
    //qDebug() << "row" << list->currentRow();
    QString file = files[list->currentRow()];
    //qDebug() << "file" << file;
    QFileInfo info(file);
    if (info.isDir()) {
        cd(file);
    } else if (info.isFile()) {
        QPoint p;
        if (text.isNull()) text = new QTextBrowser;
        p = pos();
        p.setX(p.x()+420);
        text->move(p);
        text->resize(900,800);
        text->setWindowTitle(file);
        text->setSource(QUrl("qrc" + file));
        text->setFontPointSize( (double)fontSize );
        text->show();
        //connect(text,SIGNAL(loadFinished(bool)),this,SLOT(copy()));
        QString s = text->toPlainText();
        s.chop(1);
        clipboard->setText(s);
    }
}

void LibraryWindow::copy()
{
    QString s = text->toPlainText();
    s.chop(1);
    clipboard->setText(s);
}
