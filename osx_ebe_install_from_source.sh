#!/bin/sh


echo "---------"
echo " "
echo "Installing XCode command line tools"
echo " "
echo "---------"

xcode-select --install

echo "---------"
echo " "
echo Installing required tools and libraries
echo " "
echo "---------"

sudo port install git gdb g95 astyle yasm qt4-mac

echo "---------"
echo " "
echo "Signing the certificate for ggdb"
echo " "
echo "---------"

sudo codesign -s gdb-cert /opt/local/bin/ggdb

echo "---------"
echo " "
echo "Restarting taskgated"
echo " "
echo "---------"

sudo killall -9 taskgated

echo "---------"
echo " "
echo "Cloning ebe source code from sourceforge"
echo " "
echo "---------"

if [ -x ebe ]
then
    echo " "
    echo " "
    echo 'There appears to be an existing ebe directory (or file)'
    echo " "
    echo -n "Remove ebe and clone (Y/n)? "
    read ans
    if [ x$ans = 'xn' ]
    then
        exit 1
    fi
    rm -rf ebe
fi

git clone git://git.code.sf.net/p/qtebe/code ebe

echo "---------"
echo " "
echo "Compiling ebe.  This could take a few minutes"
echo " "
echo "---------"

cd ebe
./qrc
qmake
make -j 4 2>&1 > ebe.log

echo "---------"
echo " "
echo 'Copying ebe and its language files (*.qm) to /usr/bin'
echo " "
echo "---------"

sudo cp ebe *.qm /usr/bin
