#!/bin/sh
#
# @(#) $Revision: 1.1 $
export QTDIR=/usr/local/Qt5.11.1/5.11.1/gcc_64
export QMAKESPEC=linux-g++-64

TRAINDEVHOME=/usr/local/charts
export TRAINDEVHOME 
PATH=$TRAINDEVHOME/bin:$TRAINDEVHOME/lib:$QTDIR/bin:$QTDIR/lib:$PATH
export PATH
export TRAINBUILDTYPE=all
export TRAINBUILDBIT=64
LD_LIBRARY_PATH=$TRAINDEVHOME/lib:$LD_LIBRARY_PATH
export PATH LD_LIBRARY_PATH 

