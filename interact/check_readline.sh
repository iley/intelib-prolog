#!/bin/sh
#
#   InteLib                                    http://www.intelib.org
#   The file interact/check_readline.sh
# 
#   Copyright (c) Vasiliy Kulikov, 2010
# 
#   This is free software, licensed under GNU LGPL v.2
#   See the file COPYING for further details.
# 
#   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
#   Please see the file WARRANTY for the detailed explanation.
#



INPUT="#include <stdio.h>\n#include <readline/readline.h>"

echo $INPUT | $CXX -x c -c - -o /dev/null >/dev/null 2>&1 || {
    echo "NO GNU READLINE FOUND.";
    echo "INSTALL IT OR TRY";
    echo "             $MAKE [<your target>] USE_READLINE=no";
    exit 1
}
