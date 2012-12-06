#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QVariant>
#include <QString>

class Settings : public QObject
{
    Q_OBJECT

public:
    Settings();
    void clear();
    bool read();
    void write();

private slots:

private:
    void setDefaults();
    QSettings *settings;
    QString fileName;
};

#ifdef SETTINGS_CPP
QMap<QString,QVariant> ebe;
#else
extern QMap<QString,QVariant> ebe;
#endif

#endif
