#!/usr/bin/swipl -q -s

prolog :-
    current_prolog_flag(argv, Argv),
    actual_args(Argv,Files),
    member(File,Files),
    translate(File), !.

prolog :-
	write_ln('translation failed').

actual_args(['--'|Result],Result) :- !.
actual_args([_|Args],Result) :- actual_args(Args,Result).

clean :-
    ignore(retract(module_name(_))),
    ignore(retract(src_atom(_))),
    ignore(retract(src_term(_))),
    ignore(retract(src_var(_))).

find_atoms(X) :-
    atom(X),
	(
		src_atom(X)
	;
		std_atom(X)
	;
		assert(src_atom(X))
	), !.

find_atoms(X) :-
    compound(X),
    functor(X, Functor, _),
    find_atoms(Functor),
    (
        arg(_, X, Arg), find_atoms(Arg), fail;
        true
    ), !.

find_atoms([]) :- !.

find_atoms([H|T]) :-
    find_atoms(H),
    find_atoms(T), !.

find_atoms(_).

load(Stream) :-
    read(Stream, Term),
    Term \= end_of_file, !,
    find_atoms(Term),
    numbervars(Term, 0, _),
    assertz(src_term(Term)),
    load(Stream).

load(_).

write_ln(Term) :- write(Term), nl.
write_ln(Stream, Term) :- write(Stream, Term), nl(Stream).

translate(FileName) :-
    %trace,
    file_name_extension(Name, _, FileName),
    file_base_name(Name, ModuleName),

    clean,
    assert(module_name(ModuleName)),

    atom_concat(Name, '.hpp', HppFileName),
    atom_concat(Name, '.cpp', CppFileName),

    open(FileName, read, Input),
    write_ln(loading),
    load(Input),
    write_ln(loaded),
    close(Input),

    open(HppFileName, write, HppFile),
    write_hpp(HppFile),
    close(HppFile),

    open(CppFileName, write, CppFile), !,
    write_cpp(CppFile),
    close(CppFile).

namespace_prefix('').

namespace(Namespace) :-
	module_name(Module),
	namespace_prefix(Prefix),
	atom_concat(Prefix, Module, Namespace).

guard(Guard) :-
	module_name(Module),
	upcase_atom(Module, MODULE),
	atom_concat(MODULE, '_HPP_SENTRY', Guard).

%TODO: format
write_hpp(Stream) :-
	guard(Guard),
	write(Stream, '#ifndef '), write(Stream, Guard), nl(Stream),
	write(Stream, '#define '), write(Stream, Guard), nl(Stream),
	write(Stream, '#include <prolog/prolog.hpp>'), nl(Stream),
	namespace(Namespace),
	write(Stream, 'namespace '), write(Stream, Namespace), write(Stream, ' {'), nl(Stream),
	write(Stream, 'PlgDatabase &Database();'), nl(Stream),
    (	
		src_atom(X),
		write(Stream, 'PlgAtom('),
		write(Stream, X),
		write(Stream, ');'),
		nl(Stream),
		fail
	;
		true
    ),
	write(Stream, '}'),
	nl(Stream),
	write(Stream, '#endif'),
	nl(Stream).

write_cpp(Stream) :-
    write_ln(Stream, '//Source'),
    (
        src_term(X),
		write(Stream, 'db.Add('),
		format_term(Stream, X),
		write(Stream, ');'),
		nl(Stream),
		fail
	;
		true
    ).

format_term(Stream, Head :- Body) :-
	format_term(Stream, Head),
	write(Stream, '<<='),
	format_term(Stream, Body),
	!.

format_term(Stream, (Head; Body)) :-
	format_term(Stream, Head),
	write(Stream, '|'),
	format_term(Stream, Body),
	!.

format_term(Stream, (Head, Body)) :-
	format_term(Stream, Head),
	write('Stream, &'),
	format_term(Stream, Body),
	!.

format_term(Stream, Term) :-
	Term = '$VAR'(_),
	write(Stream, Term),
	!.

format_term(Stream, [H|T]) :-
	write(Stream, '(S|'),
	format_list(Stream, [H|T]),
	write(Stream, ')'),
	!.

format_term(Stream, Term) :-
	compound(Term),
	Term =.. List,
	List = [Functor|Args],
	format_term(Stream, Functor),
	write(Stream, '('),
	format_list(Stream, Args),
	write(Stream, ')'),
	!.

format_term(Stream, Term) :-
	write(Stream, Term).

format_list(_, []).

format_list(Stream, [H|T]) :-
	format_term(Stream, H),
	format_list(Stream, T).

std_atom(':-').
std_atom(',').
std_atom(';').
