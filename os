#!/bin/sh

name=$(uname -s)

if [ $name = Darwin ]
then
    os=osx
elif [ $name = SunOS ]
then
    os=solaris
elif [ $name = aix ]
then
    os=aix
elif echo $name | grep -qi cygwin 2> /dev/null
then
    os=cygwin
elif echo $name | grep -qi bsd 2> /dev/null
then
    os=bsd
elif [ $name = Linux ]
then
    if [ -f /etc/redhat-release ]
    then
        os=redhat
    elif [ -f /etc/debian-version ]
    then
        os=debian
    elif [ -f /etc/SuSE-release ]
    then
        os=suse
    elif [ -f /etc/mandrake-release ]
    then
        os=mandrake
    elif [ -f /etc/UnitedLinux-release ]
    then
        os=united
    fi
fi


echo $os
