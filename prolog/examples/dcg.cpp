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

    //db.Once(trace);

    db.Add( num >>= (S|zero) );
    db.Add( num >>= (S|next) & num );

    db.Once(listing);
    SReference tests = (S| (S|zero), (S|next,zero), (S|next,next,zero), (S|next,prev,zero) );

    for (SReference list = tests; !list.IsEmptyList(); list = list.Cdr()) {
        bool result = db.Once( num(list.Car(), Nil) );
        printf("%s -- %s\n", list.Car()->TextRepresentation().c_str(), (result ? "yes" : "no"));
    }
    return 0;
}
