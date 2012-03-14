num(X) --> [zero], { X is 0 }.
num(X) --> [succ], num(Y), { X is Y + 1 }.

check(List) :-
  write(List),
  phrase(num(X), List),
  !,
  write(' = '), write(X), nl
;
  write(' = NaN'), nl.

prolog :-
  member(List, [ [zero], [succ, zero], [pred, zero], [succ, succ, zero], [yoba] ]),
  check(List),
  fail
;
  true.
