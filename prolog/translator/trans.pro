#!/usr/bin/swipl -q -s

prolog :-
	current_prolog_flag(argv, Argv),
	actualArgs(Argv,Files),
	member(File,Files),
	translate(File).

actualArgs(['--'|Result],Result) :- !.
actualArgs([_|Args],Result) :- actualArgs(Args,Result).

translate(FileName) :-
	open(FileName, read, Stream),
	write_header,
	translate_terms(Stream),
	write_footer.

translate_terms(Stream) :-
	read(Stream, Term),
	Term \= end_of_file,
	write(Term), write('.'), nl,
	translate_terms(Stream).

translate_terms(Stream) :- close(Stream).

write_header :-
	write('/* Some C++ declarations here */'), nl.

write_footer :-
	write('/* Final part of C++ source */'), nl.
