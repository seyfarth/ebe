#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

/*
    ProjectWindow class

    For display of project files initially
*/

#include <QListWidget>
#include <QStringList>
#include <QFrame>
#include <QString>
#include <QContextMenuEvent>

class ProjectWindow : public QFrame
{
    Q_OBJECT

public:
    ProjectWindow(QWidget *parent = 0);

private:

    // Initialize, called from constructor
    void initProjectWindow();
    void contextMenuEvent ( QContextMenuEvent *event );

private slots:
    void ignore();
    void newProject();
    void openProject();
    void closeProject();

private:
    QStringList fileNames;
    QString projectFileName;
    QListWidget *list;
    
};

#endif
