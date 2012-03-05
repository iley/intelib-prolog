#include <stdio.h>
#include <prolog/prolog.hpp>
#include <prolog/utils.hpp>

int main() {
    using namespace PlgStdLib;
    PlgAtom next("next"), prev("prev"), zero("zero"), num("num");
    PlgVariable X("X");
    SListConstructor S;
    SReference &Nil = *PTheEmptyList;

    PlgDatabase db;

    (db.Query(trace))->Next();

    db.Add( num >>= (S|zero) );
    db.Add( num >>= (S|next) & num );

    PlgContinuation cont = db.Query(phrase(num, (S|next, next, zero)));
    if (cont->Next())
        printf("yes\n");
    else
        printf("no\n");

    cont = db.Query(phrase(num, (S|next, next, prev, zero)));
    if (cont->Next())
        printf("yes\n");
    else
        printf("no\n");

    //while (cont->Next()) {
        //printf("X = %s\n", Dump(cont->GetValue(X)));
    //}
    return 0;
}
