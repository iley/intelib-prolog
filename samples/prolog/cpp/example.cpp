#include <stdio.h>
#include <prolog/prolog.hpp>
#include <prolog/utils.hpp>

int main() {
    PlgAtom man("man"), woman("woman"), human("human"), son("son"), parent("parent");
    PlgAtom bob("bob"), mary("mary"), john("john"), jack("jack");
    PlgVariable X("X"), Y("Y");

    PlgDatabase db;
    db.Add( son(X, Y) <<= parent(Y, X) & man(X) );
    db.Add( woman(mary) );
    db.Add( man(bob) );
    db.Add( man(john) );
    db.Add( man(jack) );
    db.Add( parent(bob, mary) );
    db.Add( parent(bob, john) );
    db.Add( parent(bob, jack) );

    PlgContinuation cont = db.Query(son(X, bob));
    while (cont->Next()) {
        PlgReference result = cont->GetValue(X);
        printf("%s\n", result->TextRepresentation().c_str());
    }

    return 0;
}
