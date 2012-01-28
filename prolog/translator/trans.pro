#!/usr/bin/swipl -q -s

prolog :-
    current_prolog_flag(argv, Argv),
    actual_args(Argv,Files),
    member(File,Files),
    translate(File).

actual_args(['--'|Result],Result) :- !.
actual_args([_|Args],Result) :- actual_args(Args,Result).

load(Stream) :-
    true.

write_ln(Term) :- write(Term), nl.
write_ln(Stream, Term) :- write(Stream, Term), nl(Stream).

translate(FileName) :-
    %trace,
    file_name_extension(Name, _, FileName),
    file_base_name(Name, ModuleName),

    (retract(module_name(_)); true), !,
    assert(module_name(ModuleName)),

    atom_concat(Name, '.hpp', HppFileName),
    atom_concat(Name, '.cpp', CppFileName),

    open(FileName, read, Input), !,
    (load(Input); true), !,
    close(Input),

    open(HppFileName, write, HppFile), !,
    write_ln(writing_stuff_to(HppFileName)),
    (write_hpp(HppFile); true), !,
    close(HppFile),

    open(CppFileName, write, CppFile),
    write_ln(writing_stuff_to(CppFileName)),
    (write_cpp(CppFile); true), !,
    close(CppFile).

write_hpp(Stream) :-
    write_ln(Stream, '//Header').

write_cpp(Stream) :-
    write_ln(Stream, '//Source').

