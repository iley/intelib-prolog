solution([]).

 solution([X/Y|Others]) :-
   solution(Others),
   member(Y, [1,2,3,4,5,6,7,8]),
   noattack(X/Y, Others).

 noattack(_,[]).

  noattack(X/Y,[X1/Y1|Others]) :-
    Y =\= Y1,
    Y1 - Y =\= X1 - X,
    Y1 - Y =\= X - X1,
    noattack(X/Y,Others).

pattern([1/_,2/_,3/_,4/_,5/_,6/_,7/_,8/_]).

prolog :-
  pattern(X), solution(X), write(X), nl, fail.

prolog :-
  true.
