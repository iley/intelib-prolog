#include <stdio.h>
#include <prolog/prolog.hpp>
#include <prolog/plgutils.hpp>
#include <prolog/library/library.hpp>

int main() {
    PlgAtom man("man"), woman("woman"), human("human"), son("son"), parent("parent");
    PlgAtom bob("bob"), mary("mary"), john("john"), jack("jack");
    PlgVariable X("X"), Y("Y");

    using namespace PlgStdLib;

    son(X, Y) <<= parent(Y, X) & man(X);
    *woman(mary);
    *man(bob);
    *man(john);
    *man(jack);
    *parent(bob, mary);
    *parent(bob, john);
    *parent(bob, jack);

    PlgContinuation cont = son(X, bob).Query();
    while (cont->Next()) {
        PlgReference result = cont->GetValue(X);
        printf("%s\n", result->TextRepresentation().c_str());
    }

    return 0;
}
