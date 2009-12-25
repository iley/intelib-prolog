#!/bin/sh
#
# Arguments: -h _hdr.inc
#            -s _SENTRY_NAME_
#
# Environment: CXX
#


while true;
do
    if [ "$1" = -h ]; then
        shift
        INC_FILE=$1
        shift
    elif [ "$1" = -c ]; then
        shift
        CXXFILES=$1
        shift
    elif [ "$1" = -s ]; then
        shift
        SENTRY=$1
        shift
    else
        break
    fi
done

echo '/* GENERATED FILE -- DO NOT EDIT */'
echo '#if !defined($SENTRY)'
echo '#define $SENTRY'
[ -f $INC_FILE ] && cat $INC_FILE
$CXX -D INTELIB_SCHEME_LIBRARY_HEADER_GENERATION -include ../schemlib.hpp -E -P $CXXFILES
echo '#endif'
