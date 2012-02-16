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
    ignore(retract(src_var(_))),
	ignore(retract(maxvar(_))).

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
    numbervars(Term, 0, End),
	update_maxvar(End),
    assertz(src_term(Term)),
    load(Stream).

load(_).

update_maxvar(New) :-
	(
		maxvar(Old)
	; 
		Old = 0
	), 
	!,
	(
		New > Old,
		ignore(retract(maxvar(Old))),
		assert(maxvar(New))
	;
		true
	),
	!.

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
    load(Input),
    close(Input),

    open(HppFileName, write, HppFile),
	with_output_to(HppFile, write_hpp),
    close(HppFile),

    open(CppFileName, write, CppFile), !,
	with_output_to(CppFile, write_cpp),
    close(CppFile),
	write(done), nl.

namespace_prefix('').

namespace(Namespace) :-
	module_name(Module),
	namespace_prefix(Prefix),
	atom_concat(Prefix, Module, Namespace).

guard(Guard) :-
	module_name(Module),
	upcase_atom(Module, MODULE),
	atom_concat(MODULE, '_HPP_SENTRY', Guard).

write_namespace_decl :-
	namespace(Namespace),
	write('namespace '), write(Namespace), write(' {'), nl.

write_hpp :-
	guard(Guard),
	write('#ifndef '), write(Guard), nl,
	write('#define '), write(Guard), nl,
	write('#include <prolog/prolog.hpp>'), nl,
	write_namespace_decl,
	write('  PlgDatabase &Database();'), nl,
    (	
		src_atom(X),
		write('  PlgAtom '),
		write(X),
		write('("'),
		write(X),
		write('");'),
		nl,
		fail
	;
		true
    ),
	write('}'),
	nl,
	write('#endif'),
	nl.

write_cpp :-
	module_name(Module),
    write('#include "'),
	write(Module),
	write_ln('.hpp"'),
	write_namespace_decl,
	write_ln('  PlgDatabase &Database() {'),
	write_ln('    using namespace PlgStdLib;'),
	write_ln('    static PlgDatabase db;'),
	write_ln('    static bool needsInit = true;'),
	write_vars,
	write_ln('    if (needsInit) {'),
	%trace,
    (
        src_term(X),
		write('      db.Add('),
		format_term(X),
		write(');'),
		nl,
		fail
	;
		true
    ),
	write_ln('      needsInit = false;'),
	write_ln('    }'),
	write_ln('    return db;'),
	write_ln('  }'),
	write_ln('}'),
	( src_atom(prolog), write_main ; true ).

write_main :-
	write_ln('int main() {'),
	namespace(Namespace),
	write('  using namespace '), write(Namespace), write_ln(';'),
	write_ln('  PlgDatabase &db = Database();'),
	write_ln('  PlgContinuation cont = db.Query(prolog);'),
	write_ln('  if (cont->Next())'),
	write_ln('    return 0;'),
	write_ln('  else'),
	write_ln('    return 1;'),
	write_ln('}').

write_vars :-
	write_vars(0).

write_vars(N) :-
	maxvar(Max),
	(
		N >= Max, !
	;
		Var = '$VAR'(N),
		write('    static PlgVariable '),
		write(Var),
		write('("'),
		write(Var),
		write('");'),
		nl,
		N1 is N + 1,
		write_vars(N1)
	).

format_term(Head :- Body) :-
	format_term(Head),
	write('<<='),
	format_term(Body),
	!.

format_term((Head; Body)) :-
	format_term(Head),
	write('|'),
	format_term(Body),
	!.

format_term((Head, Body)) :-
	format_term(Head),
	write('&'),
	format_term(Body),
	!.

format_term(Term) :-
	Term = '$VAR'(_),
	write(Term),
	!.

format_term([H|T]) :-
	write('(S|'),
	format_list([H|T]),
	write(')'),
	!.

format_term(Term) :-
	compound(Term),
	Term =.. [Functor|Args],
	format_term(Functor),
	write('('),
	format_list(Args),
	write(')'),
	!.

format_term(Atom) :-
	std_atom(Atom, Subst),
	write(Subst), !.

format_term(Atom) :-
	write(Atom).

format_list([]).

format_list([H|T]) :-
	format_term(H),
	format_list(T).

std_atom(':-').
std_atom(',').
std_atom(';').
std_atom(X) :- std_atom(X, _).
std_atom('!', cut).
std_atom(true, truth).
std_atom(not, nope).
