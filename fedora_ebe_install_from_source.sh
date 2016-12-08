#!/bin/sh

echo "---------"
echo " "
echo Installing required tools and libraries
echo " "
echo "---------"

yum install make git gdb g++ astyle gcc-gfortran yasm qt5-qtbase qt5-qtwebkit

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
