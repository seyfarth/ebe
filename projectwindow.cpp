#include "projectwindow.h"
#include "sourceframe.h"
#include "settings.h"
#include <cstdio>
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>

extern SourceFrame *sourceFrame;

ProjectWindow::ProjectWindow(QWidget *parent)
    : QFrame(parent)
{
    setObjectName("Project");
    setToolTip(tr("Right click to see project options"));
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(4);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(10, 10, 10, 10);
    list = new QListWidget();
    layout->addWidget(list);
    setLayout(layout);
}

QSize ProjectWindow::sizeHint() const
{
    return QSize(200, 10);
}

void ProjectWindow::ignore()
{
}

void ProjectWindow::contextMenuEvent(QContextMenuEvent * /* event */)
{
    if (projectFileName == "") {
        QMenu menu(tr("Project menu"));
        menu.addAction(tr("New project"), this, SLOT(newProject()));
        menu.addAction(tr("Open project"), this, SLOT(openProject()));
        menu.addAction(tr("ignore"), this, SLOT(ignore()));
        menu.exec(QCursor::pos());
    } else {
        QMenu menu(tr("Project menu"));
        menu.addAction(tr("Add file to project"), this, SLOT(addFile()));
        menu.addAction(tr("Open file in editor"), this, SLOT(editFile()));
        menu.addAction(tr("Drop file from project"), this, SLOT(dropFile()));
        menu.addAction(tr("Close project"), this, SLOT(closeProject()));
        menu.addAction(tr("ignore"), this, SLOT(ignore()));
        menu.exec(QCursor::pos());
    }
}

void ProjectWindow::newProject()
{
    if (projectFileName != "") closeProject();
    QString filename = QFileDialog::getSaveFileName(this,
        tr("Select project name"), 0, tr("Projects (*.ebe)"));
    if (filename != "") {
        if (filename.right(4) != ".ebe") {
            //qDebug() << "append .ebe";
            filename += ".ebe";
        }
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
        file.resize(0);
        projectFileName = filename;
    }
}

void ProjectWindow::addFile(QString name)
{
    if (projectFileName == "") return;
    fileNames.append(QDir::current().absoluteFilePath(name));
    list->addItem(QDir::current().relativeFilePath(name));
    save();
}

void ProjectWindow::open(QString filename)
{
    bool autoOpen = ebe["project/auto_open"].toBool();
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream in(&file);
    QString name;
    name = in.readLine();
    int i = 0;
    while (!name.isNull()) {
        fileNames.append(QDir::current().absoluteFilePath(name));
        list->addItem(QDir::current().relativeFilePath(name));
        if (autoOpen) {
            //qDebug() << "opening " << name;
            sourceFrame->open(name, i);
            i++;
        }
        name = in.readLine();
    }
    projectFileName = filename;
}

void ProjectWindow::save()
{
    QFile file(projectFileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::critical(this, tr("Error"),
            tr("Failed to open ") + projectFileName + tr(" for writing"));
        return;
    }
    QTextStream out(&file);
    foreach ( QString name, fileNames ) {
        out << name << endl;
    }
    file.close();
}

void ProjectWindow::dropFile()
{
    QString file = QDir::current().absoluteFilePath(
        list->currentItem()->text());
    sourceFrame->close(file);
    list->takeItem(list->currentRow());
    fileNames.removeOne(file);
    save();
}

void ProjectWindow::editFile()
{
    QString file = QDir::current().absoluteFilePath(
        list->currentItem()->text());
    sourceFrame->open(file);
}
void ProjectWindow::addFile()
{
    sourceFrame->open(true);
}

void ProjectWindow::openProject()
{
    bool autoOpen = ebe["project/auto_open"].toBool();
    if (projectFileName != "") closeProject();
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Select project name"), 0, tr("Projects (*.ebe)"));
    if (filename != "") {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
        QTextStream in(&file);
        QString name;
        name = in.readLine();
        int i = 0;
        while (!name.isNull()) {
            fileNames.append(QDir::current().absoluteFilePath(name));
            list->addItem(QDir::current().relativeFilePath(name));
            if (autoOpen) {
                sourceFrame->open(name, i);
                i++;
            }
            name = in.readLine();
        }
        projectFileName = filename;
    }

}

void ProjectWindow::closeProject()
{
    projectFileName = "";
    list->clear();
    sourceFrame->closeTabs();
}
