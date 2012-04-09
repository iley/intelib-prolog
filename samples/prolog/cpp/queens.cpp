#include <stdio.h>
#include <prolog/prolog.hpp>
#include <prolog/utils.hpp>

int main() {
    using namespace PlgStdLib;

    SListConstructor S;
    SReference &Nil = *PTheEmptyList;

    PlgAtom solution("solution"),
        noattack("noattack"),
        pos("pos"),
        pattern("pattern"),
        queens("queens");

    PlgVariable H("H"), T("T"),
        X("X"), Y("Y"),
        X1("X1"), Y1("Y1"),
        Y2("Y2"), Y3("Y3"), Y4("Y4"), Y5("Y5"), Y6("Y6"), Y7("Y7"), Y8("Y8");

    PlgAtom ne = int_not_equal;
    solution(Nil) <<= truth;
    solution(pos(X,Y) ^ T) <<= solution(T) & member(Y, (S|1,2,3,4,5,6,7,8)) & noattack(pos(X,Y), T);
    noattack(X, Nil) <<= truth;
    noattack(pos(X,Y), pos(X1,Y1) ^ T) <<=
        ne(Y, Y1) &
        ne(Y1 - Y, X1 - X) &
        ne(Y1 - Y, X - X1) &
        noattack(pos(X,Y), T);
    pattern((S|pos(1, Y1), pos(2, Y2), pos(3, Y3), pos(4, Y4), pos(5, Y5), pos(6, Y6), pos(7, Y7), pos(8, Y8))) <<= truth;
    queens(X) <<= pattern(X) & solution(X);

    PlgContinuation cont = queens(X).Query();
    while (cont->Next()) {
        printf("%s\n", Dump(cont->GetValue(X)));
    }
    return 0;
}
