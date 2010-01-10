#!/bin/sh
#
#   InteLib                                    http://www.intelib.org
#   The file gen_deps_mk.sh
# 
#   Copyright (c) Vasiliy Kulikov, 2010
# 
#   This is free software, licensed under GNU LGPL v.2
#   See the file COPYING for further details.
# 
#   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
#   Please see the file WARRANTY for the detailed explanation.


# Runs 'gcc -MM' with configurable object file name.
# It generates dependencies both for object and deps.mk


SUFFIX=o

while true;
do
    if [ "$1" = '--cxxflags' ]; then
        shift
        CXXFLAGS=$1
        shift
    elif [ "$1" = '--deps-mk' ]; then
        shift
        DEPSMK=$1
        shift
    elif [ "$1" = '--files' ]; then
        shift
        CPPFILES=$1
        shift
    elif [ "$1" = '--prefix' ]; then
        shift
        PREFIX=$1
        shift
    elif [ "$1" = '--cxx' ]; then
        shift
        CXX=$1
        shift
    elif [ "$1" = '--suffix' ]; then
        shift
        SUFFIX=$1
        shift
    elif [ "$1" = '--output' ]; then
        shift
        OUTPUT=$1
        shift
    else
        break
    fi
done

for FILE in $CPPFILES
do
    if [ "$OUTPUT" = "" ]; then
        OBJECT=`echo $FILE | sed "s/\.[^.]*$/.$SUFFIX/"`
        $CXX $CXXFLAGS -MM -MT $PREFIX$OBJECT $FILE >> $DEPSMK
    else
        $CXX $CXXFLAGS -MM -MT $OUTPUT $FILE >> $DEPSMK
    fi
    $CXX $CXXFLAGS -MM -MT $DEPSMK $FILE >> $DEPSMK
done 
