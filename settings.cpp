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
    read();
}

bool Settings::read()
{
    if ( ! QFile::exists(fileName) ) return false;
    settings = new QSettings ( fileName, QSettings::IniFormat );
    QStringList keys = settings->allKeys();
    int n = keys.count();
    for ( int i = 0; i < n; i++ ) {
        ebe[keys[i]] = settings->value(keys[i]);
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
    ebe["bg_color"] = "#f0f0d8";
    ebe["break_bg"] = "#ff0000";
    ebe["break_fg"] = "#00ffff";
    ebe["button_blue"] = "#000080";
    ebe["button_green"] = "#006000";
    ebe["button_red"] = "#800000";
    ebe["comment_fg"] = "#0000e0";
    ebe["find_bg"] = "#f0f0a0";
    ebe["find_fg"] = "#000080";
    ebe["fixed_font"] = "Courier";
    ebe["fixed_size"] = 10;
    ebe["id_fg"] = 0x0000a0;
    ebe["illegal_bg"] = 0xff00ff;
    ebe["illegal_fg"] = 0x00ff00;
    ebe["instruction_fg"] = 0x007090;
    ebe["macro_fg"] = 0xd00080;
    ebe["next_bg"] = 0xb0ffff;
    ebe["next_fg"] = 0xff0000;
    ebe["numbers_bg"] = 0xf8f8dc;
    ebe["numbers_fg"] = 0x000078;
    ebe["numeric_fg"] = 0x00c000;
    ebe["operator_fg"] = 0x009070;
    ebe["preprocessor_fg"] = 0xc00090;
    ebe["reg_fg"] = 0x000080;
    ebe["reg_title_fg"] = 0x800000;
    ebe["reserved_fg"] = 0xc00000;
    ebe["space_fg"] = 0x000000;
    ebe["string_fg"] = 0xffb000;
    ebe["text_bg"] = 0xffffe3;
    ebe["text_fg"] = 0x000060;
    ebe["tooltip_bg"] = 0xffffd0;
    ebe["variable_font"] = "Arial";
    ebe["variable_size"] = 10;

    ebe["os/linux"] = false;
    ebe["os/mac"] = true;
    ebe["os/os"] = "mac";
    ebe["os/windows"] = false;

    ebe["build/asm"] = "yasm -f macho64 -l $base.lst $source";
    ebe["build/asmld"] = "ld -g -o $base";
    ebe["build/cc"] = "gcc -g -c -Wfatal-errors -Wall -O0 -o $base.o $source";
    ebe["build/ccld"] = "gcc -g -o $base";
    ebe["build/cpp"] = "g++ -g -c -Wfatal-errors -Wall -O0 -o $base.o $source";
    ebe["build/cppld"] = "g++ -g -o $base";
    ebe["build/fortran"] = "gfortran -g -c -Wfatal-errors -Wall -O0 -o $base.o $source";
    ebe["build/fortranld"] = "gfortran -g -o $base";
    ebe["build/obj"] = "o";

    ebe["source/height"] = 800;
    ebe["source/left"] = 750;
    ebe["source/top"] = 0;
    ebe["source/width"] = 700;

    ebe["console/height"] = 800;
    ebe["console/left"] = 0;
    ebe["console/top"] = 0;
    ebe["console/visible"] = false;
    ebe["console/width"] = 700;

    ebe["data/height"] = 400;
    ebe["data/left"] = 0;
    ebe["data/top"] = 0;
    ebe["data/visible"] = true;
    ebe["data/width"] = 750;

    ebe["register/fg"] = 0xc09000;
    ebe["register/height"] = 400;
    ebe["register/left"] = 0;
    ebe["register/top"] = 400;
    ebe["register/visible"] = true;
    ebe["register/width"] = 750;

    ebe["terminal/columns"] = 80;
    ebe["terminal/font"] = "10x20";
    ebe["terminal/left"] = 1200;
    ebe["terminal/rows"] = 25;
    ebe["terminal/top"] = 500;

    ebe["project/height"] = 300;
    ebe["project/left"] = 1300;
    ebe["project/top"] = 100;
    ebe["project/visible"] = true;
    ebe["project/width"] = 500;

    ebe["command/visible"] = true;

    ebe["tooltips/visible"] = true;

    ebe["complete/minimum"] = 4;

    ebe["xmm/reverse"] = false;
}
