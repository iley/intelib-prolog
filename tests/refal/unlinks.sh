#!/bin/sh

for FILE in `ls test002_*.ref`;
do
	[ -h $FILE ] && rm $FILE
done
true
