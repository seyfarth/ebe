#include "projectwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QIODevice>
#include <QCursor>
#include <QAction>
#include <QMenu>

ProjectWindow::ProjectWindow(QWidget *parent) : QListWidget(parent)
{
    // Just basic initialization for now

    // Add functionality later for listing project files,
    // clicking to open, etc.

    initProjectWindow();
    setObjectName("Project");
    setToolTip(tr("Right click to see project options"));
}

void ProjectWindow::initProjectWindow()
{
}

void ProjectWindow::ignore()
{
}

void ProjectWindow::contextMenuEvent(QContextMenuEvent *event)
{
    if ( projectFileName == "" ) {
        QMenu menu("Project menu");
        menu.addAction(tr("New project"), this, SLOT(newProject()) );
        menu.addAction(tr("Open project"), this, SLOT(openProject()) );
        menu.addAction(tr("ignore"), this, SLOT(ignore()) );
        menu.exec(QCursor::pos());
    } else {
        QMenu menu("Project menu");
        menu.addAction(tr("Add file to project"), this, SLOT(addFile()) );
        menu.addAction(tr("Open file in editor"), this, SLOT(dropFile()) );
        menu.addAction(tr("Drop file from project"), this, SLOT(dropFile()) );
        menu.addAction(tr("Close project"), this, SLOT(closeProject()) );
        menu.addAction(tr("ignore"), this, SLOT(ignore()) );
        menu.exec(QCursor::pos());
    }
}

void ProjectWindow::newProject()
{
    if ( projectFileName != "" ) closeProject();
    QString filename = QFileDialog::getSaveFileName(this,
            tr("Select project name"), 0, tr("Projects (*.ebe)") );
    if ( filename != "" ) {
        QFile file(filename);
        if ( !file.open(QIODevice::WriteOnly|QIODevice::Text) ) return;
        file.resize(0);
        projectFileName = filename;
    }
}

void ProjectWindow::openProject()
{
    if ( projectFileName != "" ) closeProject();
    QString filename = QFileDialog::getOpenFileName(this,
            tr("Select project name"), 0, tr("Projects (*.ebe)") );
    if ( filename != "" ) {
        QFile file(filename);
        if ( !file.open(QIODevice::ReadOnly|QIODevice::Text) ) return;
        QTextStream in(&file);
        QString name;
        name = in.readLine();
        while ( !name.isNull() ) {
            fileNames.append(name);
            addItem(name);
            name = in.readLine();
        }
        projectFileName = filename;
    }

}

void ProjectWindow::closeProject()
{
    projectFileName = "";
    clear();
}
