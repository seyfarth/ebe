#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

/*
    ProjectWindow class

    For display of project files initially
*/

#include <QListWidget>
#include <QStringList>
#include <QString>

class ProjectWindow : public QListWidget
{
    Q_OBJECT

public:
    ProjectWindow(QWidget *parent = 0);

private:

    // Initialize, called from constructor
    void initProjectWindow();

private slots:
    void newProject();
    void openProject();
    void closeProject();
    

private:
    QStringList fileNames;
    QString projectFileName;
    
};

#endif
