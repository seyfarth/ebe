#!/bin/bash

function error()
{
    echo
    echo Error on line $1
    set -- $BASH_COMMAND
    if ! which $1 2>&1 > /dev/null 
    then
        echo Could not find command $1
    fi
    echo "Command \"$BASH_COMMAND\" failed"
    exit 1
}

trap 'error $LINENO $0' ERR

./qrc ana dsf
qmake
make clean
lrelease ebe.pro
make
sudo cp ebe *.qm /usr/bin
echo " "
echo ebe is now ready.  Type in \"ebe\" to run the program.
