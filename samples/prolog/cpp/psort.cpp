#include <stdio.h>
#include <prolog/prolog.hpp>
#include <prolog/utils.hpp>

int main() {
    using namespace PlgStdLib;

    SListConstructor S;
    SReference &Nil = *PTheEmptyList;

    PlgAtom psort("psort"), sorted("sorted");
    PlgVariable H("H"), T("T"), R("R"), L("L"), X("X");

    psort(L,R) <<= permutation(L,R) & sorted(R);

    sorted(Nil) <<= truth;
    sorted((S|X)) <<= truth;
    sorted(X^(H^T)) <<= (X <= H) & sorted(H^T);

    SReference list = Nil;
    for (int i = 0; i < 9; ++i)
        list = SReference(i).MakeCons(list);
    printf("Sorting %s...\n", Dump(list));

    PlgContinuation cont = psort(list, X).Query();
    if (cont->Next()) {
        printf("Result: %s\n", Dump(cont->GetValue(X)));
    }

    return 0;
}
