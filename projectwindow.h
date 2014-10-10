#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

/*
 ProjectWindow class
 */

#include <QtGui>
#include <QListWidget>

class ProjectWindow: public QFrame
{
    Q_OBJECT

public:
    ProjectWindow(QWidget *parent = 0);
    void open(QString name);
    QStringList fileNames;
    QString projectFileName;
    void addFile(QString);
    void save();

private:

    // Initialize, called from constructor
    void initProjectWindow();
    void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void ignore();
    void newProject();
    void openProject();
    void closeProject();
    void dropFile();
    void editFile();
    void addFile();

private:
    QListWidget *list;
    QSize sizeHint() const;

};

#endif
