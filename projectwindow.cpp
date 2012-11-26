#include "projectwindow.h"

ProjectWindow::ProjectWindow(QWidget *parent) : QListWidget(parent)
{
    // Just basic initialization for now

    // Add functionality later for listing project files,
    // clicking to open, etc.

    initProjectWindow();
}

void ProjectWindow::initProjectWindow()
{
    QStringList fileList;

    fileList << "Makefile" << "project.ebe" << "game.asm"
        << "io.asm" << "files.asm" << "gamefuncs.asm";

    insertItems(0, fileList);
}
