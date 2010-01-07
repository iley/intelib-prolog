#!/bin/sh

INPUT="#include <stdio.h>\n#include <readline/readline.h>"

echo $INPUT | $CXX -x c -c - -o /dev/null >/dev/null 2>&1 || {
    echo "NO GNU READLINE FOUND.";
    echo "INSTALL IT OR TRY";
    echo "             $MAKE [<your target>] USE_READLINE=no";
    exit 1
}
