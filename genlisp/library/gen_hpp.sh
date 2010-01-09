#!/bin/sh
#
#   InteLib                                    http://www.intelib.org
#   The file genlisp/library/gen_hpp.sh
# 
#   Copyright (c) Vasiliy Kulikov (<segooon_AT_gmail.com>), 2010
# 
#   This is free software, licensed under GNU LGPL v.2
#   See the file COPYING for further details.
# 
#   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
#   Please see the file WARRANTY for the detailed explanation.
#


# Run 'gcc -MM' with configurable object file name.
# It generate dependencies both for object and deps.mk
#
#
# Arguments: -h _hdr.inc
#            -s _SENTRY_NAME_
#
# Environment: CXX
#


while true;
do
    if [ "$1" = -c ]; then
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
echo "#if !defined($SENTRY)"
echo "#define $SENTRY"
echo '#include "genlisp/lispform.hpp"'
$CXX -D INTELIB_GENLISP_LIBRARY_HEADER_GENERATION -include ../genlslib.hpp -E -P $CXXFILES
echo '#endif'
