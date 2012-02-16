human(X) :- man(X); woman(X).
man(fry).
woman(leela).

prolog :-
  human(X), write(X), nl, fail ; true.
