#include <stdio.h>
#include <prolog/prolog.hpp>

int main() {
    using namespace PlgStdLib;

    SListConstructor S;
    SReference &nil = *PTheEmptyList;

    PlgAtom qsort("qsort"), split("split");
    PlgVariable H("H"), T("T"), R("R"), L("L"), X("X"), Res("Res"), LS("LS"), RS("RS");

    *qsort(nil, nil);
    qsort(H^T, Res) <<= split(H, T, L, R) & qsort(L, LS) & qsort(R, RS) & append(LS, H ^ RS, Res);

    *split(X, nil, nil, nil);
    split(X, H^T, H^LS, RS) <<= (H <= X) & split(X, T, LS, RS);
    split(X, H^T, LS, H^RS) <<= (H >  X) & split(X, T, LS, RS);

    SReference list = (S|3, 1, 2, 5, 4);

    PlgContinuation cont = qsort(list, X).Query();
    if (cont->Next()) {
        SReference sortedList = cont->GetValue(X);
        puts(sortedList->TextRepresentation().c_str());
    } else {
        puts("No solution found!");
    }

    return 0;
}
