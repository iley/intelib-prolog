:- use_module(library(ctypes)).
:- dynamic(src_atom/2).
:- dynamic(src_term/1).
:- dynamic(maxvar/1).
:- dynamic(translator_flag/1).

prolog :-
	current_prolog_flag(argv, Argv),
	actual_args(Argv,Files),
	member(File,Files),
	translate(File).

variable_prefix('_var_').

actual_args(['--'|Result],Result) :- !.
actual_args([_|Args],Result) :- actual_args(Args,Result).

rename_atom('', '') :- !.

rename_atom(Orig, Cpp) :-
	atom_chars(Orig, Chars),
	Chars = [First|Rest],
	rename_char(First, CppFirst),
	atom_chars(OrigRest, Rest),
	rename_atom(OrigRest, CppRest),
	atom_concat(CppFirst, CppRest, Cpp).

rename_char('_', '_') :- !.

rename_char(X, X) :- is_alnum(X), !.

rename_char(Orig, Cpp) :-
	char_code(Orig, Code),
	atom_concat('C', Code, Cpp).

find_atoms(X) :-
    atom(X),
	(
		src_atom(X, _)
	;
		std_atom(X)
	;
		rename_atom(X, CppName),
		assert(src_atom(X, CppName))
	),
	!.

find_atoms(X) :-
    compound(X),
    functor(X, Functor, _),
    find_atoms(Functor),
    (
        arg(_, X, Arg), find_atoms(Arg), fail
	;
        true
    ),
	!.

find_atoms([]) :- !.

find_atoms([H|T]) :-
    find_atoms(H),
    find_atoms(T),
	!.

find_atoms(_).

load(Stream) :-
    read(Stream, Term),
    Term \= end_of_file, !,
    find_atoms(Term),
    numbervars(Term, 0, End),
	update_maxvar(End),
	(
		Term \= ':-'(_), % skip pragmas for now
		assertz(src_term(Term))
	;
		true
	),
    load(Stream).

load(_).

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

translate_stdlib(FileName) :-
	assert(translator_flag(stdlib)),
	translate(FileName).

translate(FileName) :-
    file_name_extension(Name, _, FileName),
    file_base_name(Name, ModuleName),

    assert(module_name(ModuleName)),

    atom_concat(ModuleName, '.hpp', HppFileName),
    atom_concat(ModuleName, '.cpp', CppFileName),

    open(FileName, read, Input),
    load(Input),
    close(Input),

    open(HppFileName, write, HppFile),
	with_output_to(HppFile, write_hpp),
    close(HppFile),

    open(CppFileName, write, CppFile), !,
	with_output_to(CppFile, write_cpp),
    close(CppFile),
	!.

translate(_) :-
	write_ln('translation failed').

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
	(
		translator_flag(stdlib), ! % no includes in this case
	;
		write('#include <prolog/prolog.hpp>'), nl
	),
	write_namespace_decl,
	write('  PlgDatabase &Database();'), nl,
	write('  void InitDatabase(PlgDatabase &db);'), nl,
    (	
		src_atom(_, Cpp),
		write('  extern PlgAtom '), write(Cpp), write_ln(';'),
		fail
	;
		true
    ),
	write('}'),
	nl,
	write('#endif'),
	nl.

write_add(Term) :-
	write('    '),
	(
		translator_flag(stdlib), !, write('db.AddWithoutExpansion(')
	;
		write('db.Add(')
	),
	format_term(Term), write_ln(');').

write_atom_decl(Orig, Cpp) :-
  write('  PlgAtom '), write(Cpp), write('("'), write(Orig),
  (
    translator_flag(stdlib), !,  write_ln('", PlgLibraryPredicate, false);')
  ;
    write_ln('");')
  ).


write_cpp :-
	module_name(Module),
	(
		translator_flag(stdlib), !, write('#include "prolog.hpp"'), nl
	;
		true
	),
	write('#include "'),
	write(Module),
	write_ln('.hpp"'),
	write_namespace_decl,

	(	
		src_atom(Orig, Cpp),
    write_atom_decl(Orig, Cpp),
		fail
	;
		true
	),
	nl,

	write_ln('  void InitDatabase(PlgDatabase &db) {'),
	write_ln('    using namespace PlgStdLib;'),
	write_ln('    static SReference &Nil = *PTheEmptyList;'),
	write_vars,
	(
		src_term(Term),
		write_add(Term),
		fail
	;
		true
	),
	write_ln('  }'),

	write_ln('  PlgDatabase &Database() {'),
	write_ln('    static PlgDatabase db;'),
	write_ln('    static bool needsInit = true;'),
	write_ln('    if (needsInit) {'),
	write_ln('      InitDatabase(db);'),
	write_ln('      needsInit = false;'),
	write_ln('    }'),
	write_ln('    return db;'),
	write_ln('  }'),
	write_ln('}'),
	( src_atom(prolog, _), write_main ; true ).

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
		variable_prefix(Var_prefix),
		write('    static PlgVariable '), write(Var_prefix), write(Var), write('("'), write(Var), write_ln('");'),
		N1 is N + 1,
		write_vars(N1)
	).

format_term(Term) :-
	Term = '$VAR'(_),
	!,
	variable_prefix(Var_prefix),
	write(Var_prefix),
	write(Term).

format_term(Term) :-
	src_atom(Term, CppTerm),
	write(CppTerm),
	!.

format_term([H|T]) :-
	write('('), format_term(H), write(')'),
	write('^'),
	write('('), format_term(T), write(')'),
	!.

format_term([]) :-
	write('Nil'),
	!.

format_term(Term) :-
	compound(Term),
	Term =.. [Functor|Args],
	format_term(Functor, Args),
	!.

format_term(Atom) :-
	std_atom(Atom, Subst), !,
	write(Subst).

format_term(Number) :-
	number(Number),
	write('SReference('), write(Number), write(')'),
	!.

format_term(Atom) :-
	write(Atom).

% format_term/2 creates C++ representation of compound term
% infix operator with substitution
format_term(Functor, [X, Y]) :-
	infix(Functor), !,
	write('('), format_term(X), format_term(Functor), format_term(Y), write(')').

% compound term
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

std_atom(append).
std_atom(atom).
std_atom(atomic).
std_atom(fail).
std_atom(member).
std_atom(nl).
std_atom(notrace).
std_atom(trace).
std_atom(var).
std_atom(write).
std_atom([]).
std_atom('.').
std_atom('dynamic').

std_atom(X) :- std_atom(X, _).
std_atom(X,Y) :- std_infix(X, Y).

std_atom('!', cut).
std_atom(true, truth).
std_atom(not, nope).
std_atom(is, is).
std_atom('=\\=', int_not_equal).
std_atom('^', power).
std_atom(\+, nope).
std_atom(+, plus).
std_atom(-, minus).
std_atom(*, multiply).
std_atom(/, divide).
std_atom('<', int_less).
std_atom(=.., univ).

std_infix('>', int_greater).
std_infix('>=', int_greater_or_equal). 
std_infix('=<', int_less_or_equal). 

std_infix(':-', '<<=').
std_infix('-->', '>>=').
std_infix(',', '&').
std_infix(';', '|').
std_infix('=', '^=').
std_infix('=<', '<=').
std_infix('|', '^').
%std_infix(X, X) :- std_infix(X).

infix(Atom) :- std_infix(Atom,_).
