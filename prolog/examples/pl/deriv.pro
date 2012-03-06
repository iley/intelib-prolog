d(X, X, 1) :- !.

d(A + B, X, D) :-
  d(A, X, DA),
  d(B, X, DB),
  add(DA, DB, D), !.

d(A - B, X, D) :-
  d(A, X, DA),
  d(B, X, DB),
  add(DA, -DB, D), !.

d(A * B, X, D) :-
  d(A, X, DA),
  d(B, X, DB),
  mul(A, DB, P1),
  mul(DA, B, P2),
  add(P1, P2, D), !.

d(A / B, X, D) :-
  d(A, X, DA),
  d(B, X, DB),
  mul(DA, B, P1),
  mul(A, DB, P2),
  sub(P1, P2, Dividend),
  pow(B, 2, Divisor),
  div(Dividend, Divisor, D), !.

d(-A, X, D) :-
  d(A, X, D1),
  sub(0, D1, D), !.

d(X^E, X, D) :-
  sub(E, 1, E1),
  pow(X, E1, P),
  mul(E, P, D), !.

d(sin(Y), X, D) :-
  d(Y, X, D1),
  mul(cos(Y), D1, D), !.

d(cos(Y), X, D) :-
  d(Y, X, D1),
  mul(-sin(Y), D1, D).

d(_, _, 0).

add(A, B, R) :- number(A), number(B), R is A + B, !.
add(A, A, R) :- mul(2, A, R), !.
add(A, 0, A) :- !.
add(0, B, B) :- !.
add(A, -B, S) :- sub(A, B, S), !.
add(A, B, A + B).

sub(A, B, R) :- number(A), number(B), R is A - B, !.
sub(A, 0, A) :- !.
sub(0, B, -B) :- !.
sub(A, A, 0) :- !.
sub(A, -B, R) :- add(A, B, R), !.
sub(A, B, A - B).

mul(A, B, P) :- number(A), number(B), P is A * B, !.
mul(_, 0, 0) :- !.
mul(0, _, 0) :- !.
mul(A, 1, A) :- !.
mul(1, B, B) :- !.
mul(A, A, M) :- pow(A, 2, M), !.
mul(X^N, X^M, M) :- sum(N, M, P), pow(X, P, M), !.
mul(-A, -B, M) :- mul(A, B, M), !.
mul(-A, B, M) :- mul(A, B, M1), sub(0, M1, M), !.
mul(A, -B, M) :- mul(A, B, M1), sub(0, M1, M), !.
mul(A, B, A * B).

div(X, 1, X) :- !.
div(A, B, A/B) :- !.

pow(_, 0, 1) :- !.
pow(X, 1, X) :- !.
pow(X, N, X^N).

prolog :-
  member(F, [x*x + sin(2*x), 5*x, sin(x)/cos(x)]),
  d(F, x, D),
  write(D), nl,
  fail
;
  true.
