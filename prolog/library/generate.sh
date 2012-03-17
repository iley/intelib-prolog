#!/bin/sh

MODULES="grammar output"

for MODULE in $MODULES
do
	prolog -q -s pl/trans.pro -t "translate_stdlib('pl/$MODULE.pro')"
done
