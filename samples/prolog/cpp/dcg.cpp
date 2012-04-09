#include <stdio.h>
#include <prolog/prolog.hpp>
#include <prolog/utils.hpp>

int main() {
    using namespace PlgStdLib;
    PlgAtom next("next"), prev("prev"), zero("zero"), num("num");
    PlgVariable X("X"), Y("Y");
    SListConstructor S;
    SReference &Nil = *PTheEmptyList;

    SReference sref_zero((int)0);

    Assert( num(X) >>= (S|zero) & action(X.is(sref_zero)) );
    Assert( num(X) >>= (S|next) & num(Y) & action(X.is(Y + SReference(1))) );

    PlgContinuation cont = phrase(num(X), (S|next, next, zero)).Query();

    while (cont->Next()) {
        printf("X = %s\n", cont->GetValue(X)->TextRepresentation().c_str());
    }

    return 0;
}
