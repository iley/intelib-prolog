#include <stdio.h>
#include <prolog/prolog.hpp>
#include <prolog/utils.hpp>

int main() {
    using namespace PlgStdLib;

    PlgAtom man("man"), woman("woman"), human("human"), fry("fry"), leela("leela"), bender("bender"), print_humans("print_humans");
    PlgVariable X("X");

    PlgDatabase db;
    db.Add( human(X) <<= woman(X) | man(X) );
    db.Add( woman(leela) );
    db.Add( man(fry) );

    db.Add( print_humans <<= human(X) & write(X) & nl & fail );

    db.Once(print_humans);

    return 0;
}
