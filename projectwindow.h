#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

/*
    ProjectWindow class

    For display of project files initially
*/

#include <QtGui>

class ProjectWindow : public QFrame
{
    Q_OBJECT

public:
    ProjectWindow(QWidget *parent = 0);
    void open(QString name);
    QStringList fileNames;
    QString projectFileName;

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
    QListWidget *list;
    QSize sizeHint() const;
    
};

#endif
