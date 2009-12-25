#!/bin/sh
#
# Arguments: -h lfun_*.hpp
#
# Environment: CXX
# 

while true;
do
    if [ "$1" = -h ]; then
        shift
        HEADER=$1
        shift
    elif [ "$1" = -c ]; then
        shift
        CXXFILES=$1
        shift
    else
        break
    fi
done


echo "(ADD-LIB-HEADERS \"$HEADER\")"
$CXX -D INTELIB_LISP_TRANSLATOR_INFORMATION \
    -include ../ilisplib.hpp -E -P $CXXFILES
