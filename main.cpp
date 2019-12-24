//
// Copyright (c) 2018 Ray Seyfarth
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <QApplication>
#include <QResource>
#include <QSet>
#include <stdio.h>
#include <string.h>

#include "mainwindow.h"
#include "settings.h"
#include "language.h"

bool userSetGeometry = false;
bool startedFromDesktop = false;
int userWidth;
int userHeight;

Languages languages;

MainWindow *mainWin;
QApplication *app;

Settings *settings;

int main(int argc, char *argv[])
{
#ifdef Q_OS_MAC
    setenv("QT_USE_NATIVE_WINDOWS","1",1);
#endif
    app = new QApplication(argc, argv);
    //QResource::registerResource(app.applicationDirPath()+"/ebe.rcc");

    if (argc >= 3) {
        if (strcmp(argv[1], "-g") == 0) {
            char *p = strchr(argv[2], 'x');
            if (p) {
                userSetGeometry = true;
                userWidth = atoi(argv[2]);
                userHeight = atoi(p + 1);
                argc -= 2;
                argv += 2;
            }
        }
    }

    if (argc >= 2) {
        if (strcmp(argv[1], "-d") == 0) {
            startedFromDesktop = true;
            argc--;
            argv++;
        }
    }
    MainWindow::setWordSize();
    settings = new Settings;
    settings->read();
    languages.initLanguages();
    languages.setLanguage();

    mainWin = new MainWindow;
    mainWin->show();
    if (argc > 1) mainWin->open(argv[1]);
    return app->exec();
}
