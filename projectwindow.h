#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

/*
    ProjectWindow class

    For display of project files initially
*/

#include <QListWidget>
#include <QStringList>
#include <QString>
#include <QContextMenuEvent>

class ProjectWindow : public QListWidget
{
    Q_OBJECT

public:
    ProjectWindow(QWidget *parent = 0);

private:

    // Initialize, called from constructor
    void initProjectWindow();

private slots:
    void ignore();
    void newProject();
    void openProject();
    void closeProject();
    

private:
    QStringList fileNames;
    QString projectFileName;
    void contextMenuEvent ( QContextMenuEvent *event );
    
};

#endif
