#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QVariant>
#include <QString>

class Settings : QObject
{
    Q_OBJECT

public:
    Settings(QString file);
    QVariant value ( QString &key );
    void setValue ( QString &key, QVariant val );
    void save();

private slots:

private:
    void setDefaults();
    QSettings *settings;
};

long iVal ( QString &key );
bool bVal ( QString &key );
double fVal ( QString &key );
QString sVal ( QString &key );
void set ( QString &key, QVariant val );
#endif
