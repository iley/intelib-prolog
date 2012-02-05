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
    atom(X), !,
    assert(src_atom(X)).

find_atoms(X) :-
    compound(X), !,
    functor(X, Functor, _),
    find_atoms(Functor),
    (
        arg(_, X, Arg), find_atoms(Arg), fail;
        true
    ).

find_atoms([]) :- !.

find_atoms([H|T]) :-
    !,
    find_atoms(H),
    find_atoms(T).

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

write_hpp(Stream) :-
    write_ln(Stream, '//Header'),
    (
        src_atom(X), write_ln(Stream, X), fail;
        true
    ).

write_cpp(Stream) :-
    write_ln(Stream, '//Source'),
    (
        src_term(X), write_ln(Stream, X), fail;
        true
    ).
