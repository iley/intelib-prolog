#include <stdio.h>
#include <prolog/prolog.hpp>
#include <prolog/utils.hpp>

int main() {
    using namespace PlgStdLib;

    PlgAtom man("man"), woman("woman"), human("human"), son("son"), parent("parent");
    PlgAtom bob("bob"), mary("mary"), john("john");
    PlgVariable X("X"), Y("Y");

    PlgDatabase db;
    db.Add(
        son(X, Y) <<= parent(Y, X) & man(X),
        woman(mary),
        man(bob),
        man(john),
        parent(bob, mary),
        parent(bob, john)
    );

    PlgAtom print_sons("print_sons");
    db.Add( print_sons <<= son(X, bob) & write(X) & nl & fail );

    db.Once(print_sons);

    return 0;
}
