#include <stdio.h>
#include <prolog/prolog.hpp>
#include <prolog/utils.hpp>

int main() {
    PlgAtom man("man"), woman("woman"), human("human"), fry("fry"), leela("leela"), bender("bender");
    PlgVariableName X("X");

    PlgDatabase db;
    db.Add( human(X) <<= woman(X) | man(X) );
    db.Add( woman(leela) );
    db.Add( man(fry) );

    PlgContinuation cont = db.Query(human(X));
    while (cont->Next()) {
        printf("X = %s\n", Dump(cont->GetValue(X)));
    }
    return 0;
}
