#define IN_LANGUAGE_CPP

#include "language.h"
#include "settings.h"
#include "types.h"


extern QApplication *app;
QTranslator translator;

Languages::Languages()
{
}

void Languages::initLanguages()
{
    languageCodes.clear();
    codeToName.clear();

    languageCodes << "ar";
    codeToName["ar"] = tr("Arabic");

    languageCodes << "zh";
    codeToName["zh"] = tr("Chinese");

    languageCodes << "da";
    codeToName["da"] = tr("Danish");

    languageCodes << "en";
    codeToName["en"] = tr("English");

    languageCodes << "fr";
    codeToName["fr"] = tr("French");

    languageCodes << "de";
    codeToName["de"] = tr("German");

    languageCodes << "in";
    codeToName["in"] = tr("Indonesian");

    languageCodes << "hi";
    codeToName["hi"] = tr("Hindi");

    languageCodes << "ja";
    codeToName["ja"] = tr("Japanese");

    languageCodes << "pt";
    codeToName["pt"] = tr("Portuguese");

    languageCodes << "ru";
    codeToName["ru"] = tr("Russian");

    languageCodes << "sp";
    codeToName["sp"] = tr("Spanish");

    languageCodes << "sv";
    codeToName["sv"] = tr("Swedish");

    foreach ( QString code, languageCodes ) {
        nameToCode[codeToName[code]] = code;
    }
}

void Languages::setLanguage()
{
    QString languageCode;
    languageCode = ebe["language_code"].toString();
    ebe["language_name"] = codeToName[languageCode];

    if ( languageCode != "en" && languageCodes.contains(languageCode) ) {
        translator.load("ebe_" + languageCode);
        app->installTranslator(&translator);
    } else {
        app->removeTranslator(&translator);
    }
}
