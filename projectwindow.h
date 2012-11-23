#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

#include <QPlainTextEdit>
#include <QString>

QT_BEGIN_NAMESPACE
class QKeyEvent;
QT_END_NAMESPACE

class ProjectWindow : public QPlainTextEdit
{
    Q_OBJECT

public:
    ProjectWindow(QWidget *parent=0);

private slots:

private:
    void keyPressEvent ( QKeyEvent *event );

};

#endif
