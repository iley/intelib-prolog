//   InteLib                                    http://www.intelib.org
//   The file tools/hshread.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2010
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include <string.h>
#include "../sexpress/shashtbl.hpp"
#include "sreader.hpp"


static SReference process_hashtable_seq(const SReference &ls, void*)
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
    reader.AddSequenceOpener("#s(", process_hashtable_seq, 0, ")");
    reader.AddSequenceOpener("#S(", process_hashtable_seq, 0, ")");
}
