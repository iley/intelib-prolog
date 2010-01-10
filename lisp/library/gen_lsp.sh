#!/bin/sh
#
#   InteLib                                    http://www.intelib.org
#   The file lisp/library/gen_lsp.sh
# 
#   Copyright (c) Vasiliy Kulikov, 2010
# 
#   This is free software, licensed under GNU LGPL v.2
#   See the file COPYING for further details.
# 
#   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
#   Please see the file WARRANTY for the detailed explanation.
#


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
