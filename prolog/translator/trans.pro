prolog :-
    current_prolog_flag(argv, Argv),
    actual_args(Argv,Files),
    member(File,Files),
    translate(File), !.

prolog :-
	write_ln('translation failed'),
	fail.

actual_args(['--'|Result],Result) :- !.
actual_args([_|Args],Result) :- actual_args(Args,Result).

:- dynamic(src_atom/1).

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

:- dynamic(maxvar/1).
maxvar(0).

update_maxvar(New) :-
	maxvar(Old),
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
    file_name_extension(Name, _, FileName),
    file_base_name(Name, ModuleName),

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
		write('  PlgAtom '), write(X), write('("'), write(X), write_ln('");'),
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
	write_ln('    static SReference &Nil = *PTheEmptyList;'),
	write_vars,
	write_ln('    if (needsInit) {'),
    (
        src_term(X),
		write('      db.Add('), format_term(X), write_ln(');'),
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
		write('    static PlgVariable '), write(Var), write('("'), write(Var), write_ln('");'),
		N1 is N + 1,
		write_vars(N1)
	).

% no pragmas are supported for now
format_term(':-'(_)).

format_term(Term) :-
	Term = '$VAR'(_),
	write(Term),
	!.

format_term([H|T]) :-
	write('('), format_term(H), write(')'),
	write('^'),
	write('('), format_term(T), write(')'),
	!.

format_term([]) :-
	write('Nil'), !.

format_term(Term) :-
	compound(Term),
	Term =.. [Functor|Args],
	format_term(Functor, Args),
	!.

format_term(Atom) :-
	std_atom(Atom, Subst),
	write(Subst), !.

format_term(Number) :-
	number(Number),
	%write(Number), !.
	write('SReference('), write(Number), write(')'), !.

format_term(Atom) :-
	write(Atom).

% format_term/2 creates C++ representation of compound term
% infix operator with substitution
format_term(Functor, [X, Y]) :-
	std_infix(Functor, Op),
	write('('), format_term(X), format_term(Op), format_term(Y), write(')'),
	!.

% infix operator without substitution
format_term(Functor, [X, Y]) :-
	std_infix(Functor),
	write('('), format_term(X), format_term(Functor), format_term(Y), write(')'),
	!.

% predicate from std. library
format_term(Functor, Args) :-
	std_atom(Functor, Subst),
	format_term(Subst, Args),
	!.

% ordinary compound term
format_term(Functor, Args) :-
	format_term(Functor), write('('), format_list(Args), write(')'), !.

% format_list/1 creates C++ representation of comma-separated list
format_list([]) :- !.

format_list([X]) :-
	format_term(X), !.

format_list([H|T]) :-
	format_term(H),
	write(','),
	format_list(T).

std_atom(member).
std_atom(nl).
std_atom(write).
std_atom([]).
std_atom('.').
std_atom('+').
std_atom('-').
std_atom('*').
std_atom('/').

std_atom(X) :- std_atom(X, _).
std_atom(X) :- std_infix(X, _).

std_atom('!', cut).
std_atom(true, truth).
std_atom(not, nope).
std_atom(is, is).
std_atom('=\\=', int_not_equal).

std_infix(':-', '<<=').
std_infix(',', '&').
std_infix(';', '|').
std_infix('=', '^=').
std_infix('=<', '<=').

std_infix('+'). std_infix('-'). std_infix('*'). std_infix('/').
std_infix('<'). std_infix('>'). std_infix('>='). 
