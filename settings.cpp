#include "settings.h"
#include <QDir>

Settings *ebeSettings;

Settings::Settings(QString firstname)
{
    QString name;
    name = firstname;
    QFile file(name);
    if ( !file.exists() ) {
        name = QDir::homePath() + "/" + name;
        file.setFileName ( name );
    }
    if ( file.exists() ) {
        settings = new QSettings(name,QSettings::IniFormat);
    } else {
        settings = new QSettings(firstname,QSettings::IniFormat);
    }
    setDefaults();
    ebeSettings = this;
}

QVariant Settings::value ( QString &key )
{
    if ( key.indexOf('/') == -1 ) {
        key = "ebe/" + key;
    }
    return settings->value(key);
}

void Settings::setValue ( QString &key, QVariant val )
{
    if ( key.indexOf('/') == -1 ) {
        key = "ebe/" + key;
    }
    settings->setValue(key,val);
}

void Settings::save()
{
    delete settings;
}

void Settings::setDefaults()
{
}

int ival ( QString &key )
{
    return ebeSettings->value(key).toInt();
}

bool bval ( QString &key )
{
    return ebeSettings->value(key).toBool();
}

double fval ( QString &key )
{
    return ebeSettings->value(key).toDouble();
}

QString sval ( QString &key )
{
    return ebeSettings->value(key).toString();
}

void set ( QString &key, QVariant val )
{
    ebeSettings->setValue ( key, val );
}
