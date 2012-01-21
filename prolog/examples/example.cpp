#include <stdio.h>
#include <prolog/prolog.hpp>

const char *str(const SReference &s) {
    static SString str;
    str = s->TextRepresentation();
    return str.c_str();
}

int main() {
    PlgAtom man("man"), woman("woman"), human("human"), fry("fry"), leela("leela"), bender("bender");
    PlgVariableName X("X");

    PlgDatabase db;
    db.Add( human(X) <<= woman(X) | man(X) );
    db.Add( woman(leela) );
    db.Add( man(fry) );

    PlgContinuation cont = db.Query(human(X));
    while (cont->Next()) {
        printf("X = %s\n", str(cont->GetValue(X)));
    }
    return 0;
}
