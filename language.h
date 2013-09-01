#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QApplication>
#include <QTranslator>
#include <QStringList>

#include "settings.h"
#include "types.h"



class Languages:public QObject
{
    Q_OBJECT

public:
    Languages();
    void initLanguages();
    void setLanguage();
    QStringList languageCodes;
    StringHash codeToName;
    StringHash nameToCode;
};

#endif
