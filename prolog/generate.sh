#!/bin/sh

for module in grammar
do
	prolog -q -s pl/trans.pro -t "translate_stdlib('pl/$module.pro')"
done
