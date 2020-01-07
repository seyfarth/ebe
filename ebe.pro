######################################################################
# Automatically generated by qmake (2.01a) Sun Nov 18 14:35:57 2012
######################################################################

TEMPLATE = app
#TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
#INCLUDEPATH += /usr/local/include
#LIBS += -L/usr/local/lib/qt4
#LIBS += -lsupc++

macx {
    #QMAKE_CC = gcc-8
    #QMAKE_CXX = g++-8
    QMAKE_CXXFLAGS += -std=c++11
    exists(/usr/local/bin/brew) {
        DESTDIR = /usr/local/bin
    } else {
        DESTDIR = /opt/local/bin
    }
    ICON = ebe.icns
}
equals(QT_MAJOR_VERSION,5) {
    QT += widgets
}

QT += svg

#CONFIG -= app_bundle
CONFIG += console embed_manifest_exe
#CONFIG += console debug embed_manifest_exe
#CONFIG += console debug static

TRANSLATIONS = ebe_fr.ts ebe_sp.ts ebe_sv.ts ebe_de.ts ebe_pt.ts \
               ebe_hi.ts ebe_zh.ts ebe_ru.ts ebe_ar.ts ebe_bn.ts \
               ebe_in.ts ebe_ja.ts ebe_da.ts

CODECFORTR   = ISO-9=8859-5

# Input
HEADERS += mainwindow.h sourcewindow.h commandline.h sourceframe.h \
           terminalwindow.h registerwindow.h consolewindow.h \
           projectwindow.h datawindow.h floatwindow.h \
           settings.h listwidget.h stylesheet.h ptyreader.h \
           errorwindow.h gdb.h variable.h backtracewindow.h \
           highlighter.h toybox.h librarywindow.h instructions.h \
           types.h bitbucket.h unarybitpanel.h validators.h \
           binarynumber.h binarybitpanel.h integeredit.h \
           intconvert.h intmath.h floatedit.h floatconvert.h file.h \
           language.h framewindow.h asmdatawindow.h eztable.h \
           debugger.h lldb.h dockwidget.h

SOURCES += main.cpp sourcewindow.cpp mainwindow.cpp commandline.cpp \
           sourceframe.cpp terminalwindow.cpp registerwindow.cpp \
           consolewindow.cpp projectwindow.cpp datawindow.cpp floatwindow.cpp \
           settings.cpp listwidget.cpp stylesheet.cpp ptyreader.cpp \
           errorwindow.cpp gdb.cpp variable.cpp backtracewindow.cpp \
           highlighter.cpp toybox.cpp librarywindow.cpp instructions.cpp \
           types.cpp bitbucket.cpp unarybitpanel.cpp validators.cpp \
           binarynumber.cpp binarybitpanel.cpp integeredit.cpp \
           intconvert.cpp intmath.cpp floatedit.cpp floatconvert.cpp file.cpp \
           language.cpp framewindow.cpp asmdatawindow.cpp eztable.cpp \
           debugger.cpp lldb.cpp dockwidget.cpp

RESOURCES += ebe.qrc
