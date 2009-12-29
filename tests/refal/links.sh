#!/bin/sh

FILES="start.ref lexical.ref errors.ref checklexical.ref syntax.ref brackets.ref types.ref library.ref checksyntax.ref directivessyntax.ref linker.ref morelinker.ref checklinker.ref generate.ref generatehxx.ref generatecxx.ref functionscode.ref version.ref verystart.ref removesc.ref"
i=1
for FILE in $FILES;
do
	[ -h test002_$i.ref ] || ln -s ../../irina/$FILE test002_$i.ref
	i=$(($i+1))
done
