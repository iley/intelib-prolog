// +-------------------------------------------------------------------------+
// |               I__n__t__e__L__i__b           0.6.21 development          |
// | Copyright (c) Andrey Vikt. Stolyarov <crocodil_AT_croco.net> 2000-2008. |
// |                                                                         |
// | This is free software. The library part is available under              |
// |                               GNU LESSER GENERAL PUBLIC LICENSE v.2.1.  |
// | GNU LGPL v2.1 is found in docs/gnu_gpl2.txt,  or at  http://www.gnu.org |
// |     Please see also docs/readme.txt and visit http://www.intelib.org    |
// |                                                                         |
// | !!! THERE IS NO WARRANTY OF ANY KIND, NEITHER EXPRESSED NOR IMPLIED !!! |
// +-------------------------------------------------------------------------+




#include <string.h>
#include "../sexpress/shashtbl.hpp"
#include "sreader.hpp"


static SReference process_hashtable_seq(const SReference &ls)
{
    SReference list(ls.Cdr()); // first atom is HASH-TABLE and it is ignored
    // now EQ, EQL or EQUAL follows
    // it is unlikely that Reader is used with text represetnations off    
    SString pred = list.Car()->TextRepresentation().c_str();
    SHashTableRef res;
    if(strcasecmp(pred.c_str(), "EQ") == 0)
        res = new SExpressionHashTable(SExprsAreEq);
    else
    if(strcasecmp(pred.c_str(), "EQL") == 0)
        res = new SExpressionHashTable(SExprsAreEql);
    else
    if(strcasecmp(pred.c_str(), "EQUAL") == 0)
        res = new SExpressionHashTable(SExprsAreEqual);
    else // by default, let's assume the default
        res = new SExpressionHashTable();
    list = list.Cdr();

    // now the sequence follows
    while(!list.IsEmptyList()) {
        res->AddItem(list.Car().Car(), list.Car().Cdr());
        list = list.Cdr();
    }

    return res;
}

void add_hashtables_to_reader(class IntelibReader &reader)
{
    reader.AddSequenceOpener("#s(", process_hashtable_seq, ")");
    reader.AddSequenceOpener("#S(", process_hashtable_seq, ")");
}
