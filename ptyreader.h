#ifndef PTYREADER_H
#define PTYREADER_H

#include <QThread>
#include <QString>
#include <QKeyEvent>

class PtyReader : public QThread
{
    Q_OBJECT

public:
    PtyReader(int fd, QString name, QWidget *parent=0);
    int pty;
    QString ptyName;
    void run();

signals:
    void dataReady(char *,int);

};

#endif
