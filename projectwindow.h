#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

/*
    ProjectWindow class

    For display of project files initially
*/

#include <QListWidget>
#include <QStringList>
#include <QString>
#include <QTableWidgetItem>
#include <QList>
#include <QMessageBox>
#include <QRegExp>

class ProjectWindow : public QListWidget
{
    Q_OBJECT

public:
    ProjectWindow(QWidget *parent = 0);

private:

    // Initialize, called from constructor
    void initProjectWindow();

private slots:

    

private:

    
};


#endif
