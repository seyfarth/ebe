#define SETTINGS_CPP
#include "settings.h"
#include <QDir>
#include <QFile>
#include <QStringList>
#include <QFileDialog>
#include <QDebug>

Settings::Settings()
{
    setDefaults();
    fileName = ".ebe.ini";
}

bool Settings::read()
{
    if ( ! QFile::exists(fileName) ) return false;
    settings = new QSettings ( fileName, QSettings::IniFormat );
    QStringList keys = settings->allKeys();
    int n = keys.count();
    for ( int i = 0; i < n; i++ ) {
        ebe[keys[i]] = settings->value(keys[i]);
        //qDebug() << keys[i] << ebe[keys[i]].toString();
    }
    delete settings;
    return true;
}

void Settings::write()
{
    settings = new QSettings ( fileName, QSettings::IniFormat );
    QStringList keys = ebe.keys();
    int n = keys.count();
    for ( int i = 0; i < n; i++ ) {
        settings->setValue(keys[i],ebe[keys[i]]);
    }
    delete settings;
}

void Settings::setDefaults()
{
    ebe["quit_color"] = "#c00000";
    ebe["run_color"] = "#0000c0";
    ebe["next_color"] = "#0000c0";
    ebe["step_color"] = "#0000c0";
    ebe["continue_color"] = "#00a000";
    ebe["stop_color"] = "#c00000";
    ebe["bg_color"] = "#ffffd8";
    ebe["break_bg"] = "#ff0000";
    ebe["break_fg"] = "#00ffff";
    ebe["button_blue"] = "#000080";
    ebe["button_green"] = "#006000";
    ebe["button_red"] = "#800000";
    ebe["comment_fg"] = "#0000e0";
    ebe["find_bg"] = "#f0f0a0";
    ebe["find_fg"] = "#000080";
    ebe["fixed_font"] = "Courier";
    ebe["font_size"] = 12;
    ebe["id_fg"] = "#0000a0";
    ebe["illegal_bg"] = "#ff00ff";
    ebe["illegal_fg"] = "#00ff00";
    ebe["instruction_fg"] = "#007090";
    ebe["macro_fg"] = "#d00080";
    ebe["next_bg"] = "#b0ffff";
    ebe["next_fg"] = "#ff0000";
    ebe["numbers_bg"] = "#f8f8dc";
    ebe["numbers_fg"] = "#000078";
    ebe["numeric_fg"] = "#80o080";
    ebe["operator_fg"] = "#008860";
    ebe["preprocessor_fg"] = "#c00090";
    ebe["reg_fg"] = "#000080";
    ebe["reg_title_fg"] = "#800000";
    ebe["reserved_fg"] = "#b00000";
    ebe["space_fg"] = "#000000";
    ebe["string_fg"] = "#ffb000";
    ebe["text_bg"] = "#ffffe3";
    ebe["text_fg"] = "#000060";
    ebe["tooltip_bg"] = "#ffffd0";
    ebe["variable_font"] = "Arial";
    ebe["table_bg"] = "#f0ffff";
    ebe["list_bg"] = "#e0f0ff";
    ebe["tree_bg"] = "#e0fafa";

    ebe["edit/tab_width"] = 4;
    ebe["edit/auto_indent"] = true;

    ebe["os/linux"] = false;
    ebe["os/mac"] = true;
    ebe["os/os"] = "mac";
    ebe["os/windows"] = false;

    ebe["prettify"] = "astyle -A3 $source";
    ebe["build/asm"] = "yasm -f macho64 -l $base.lst $source";
    ebe["build/asmld"] = "ld -g -o $base";
    ebe["build/cc"] = "gcc -g -c -Wfatal-errors -Wall -O0 -o $base.o $source";
    ebe["build/ccld"] = "gcc -g -o $base";
    ebe["build/cpp"] = "g++ -g -c -Wfatal-errors -Wall -O0 -o $base.o $source";
    ebe["build/cppld"] = "g++ -g -o $base";
    ebe["build/fortran"] = "gfortran -g -c -Wfatal-errors -Wall -O0 -o $base.o $source";
    ebe["build/fortranld"] = "gfortran -g -o $base";

#ifdef Q_WS_WIN
    ebe["build/obj"] = "o";
#else
    ebe["build/obj"] = "o";
#endif

    ebe["buttons/visible"] = false;
    ebe["buttons/icons"] = true;
    ebe["buttons/icon_size"] = 32;

    ebe["toolbars/icon_size"] = 32;

    ebe["console/visible"] = false;

    ebe["data/visible"] = true;

    ebe["register/fg"] = "#c09000";
    ebe["register/visible"] = true;

    ebe["float/visible"] = false;

    ebe["project/visible"] = true;
    ebe["project/auto_open"] = true;

    ebe["terminal/visible"] = true;

    ebe["command/visible"] = true;

    ebe["backtrace/visible"] = true;

    ebe["tooltips/visible"] = true;

    ebe["complete/minimum"] = 4;

    ebe["xmm/reverse"] = false;
}
