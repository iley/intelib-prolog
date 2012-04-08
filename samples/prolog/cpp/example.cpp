#include <stdio.h>
#include <prolog/prolog.hpp>
#include <prolog/utils.hpp>
#include <prolog/library/library.hpp>

int main() {
    PlgAtom man("man"), woman("woman"), human("human"), son("son"), parent("parent");
    PlgAtom bob("bob"), mary("mary"), john("john"), jack("jack");
    PlgVariable X("X"), Y("Y");

    using namespace PlgStdLib;

    son(X, Y) <<= parent(Y, X) & man(X);
    woman(mary) <<= truth;
    man(bob) <<= truth;
    man(john) <<= truth;
    man(jack) <<= truth;
    parent(bob, mary) <<= truth;
    parent(bob, john) <<= truth;
    parent(bob, jack) <<= truth;

    PlgContinuation cont = son(X, bob).Query();
    while (cont->Next()) {
        PlgReference result = cont->GetValue(X);
        printf("%s\n", result->TextRepresentation().c_str());
    }

    return 0;
}
