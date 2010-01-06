#!/bin/sh -v
#
# Run 'gcc -MM' with configurable object file name.
# It generate dependencies both for object and deps.mk
#

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
    else
        break
    fi
done

for FILE in $CPPFILES
do
    OBJECT=`echo $FILE | sed 's/\.[^.]*$/.o/'`
    $CXX $CXXFLAGS -MM -MT $PREFIX$OBJECT $FILE >> $DEPSMK
    $CXX $CXXFLAGS -MM -MT $DEPSMK $FILE >> $DEPSMK
done 
