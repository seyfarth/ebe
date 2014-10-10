#ifndef LIBRARYWINDOW_H
#define LIBRARYWINDOW_H

#include <QFrame>
#include <QPointer>
#include <QSize>
#include <QString>
#include <QStringList>

QT_BEGIN_NAMESPACE
class QListWidget;
class QListWidgetItem;
class QWebView;
class QClipboard;
QT_END_NAMESPACE

class LibraryWindow: public QFrame
{
    Q_OBJECT

public:
    LibraryWindow(QWidget *parent = 0);
    QSize sizeHint() const;
    QListWidget *list;
    QString libraryPath;
    QStringList components;
    QStringList files;
    QClipboard *clipboard;
    QPointer<QWebView> view;

private slots:
    void handleClick ( QListWidgetItem *it );
    void copy();

private:
    void cd(QString dir);
};

#ifdef LIBRARYWINDOW_CPP
QPointer<LibraryWindow> libraryWindow;
#else
extern QPointer<LibraryWindow> libraryWindow;
#endif

#endif
