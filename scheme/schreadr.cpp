//   InteLib                                    http://www.intelib.org
//   The file scheme/schreadr.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "../sexpress/svector.hpp"
#include "../sexpress/shashtbl.hpp"
#include "scheme.hpp"
#include "schreadr.hpp"

//////////
// transformer

static SReference QuoteExpression(const SReference &ref)
{
    return ~(SchReference(ref));
}

static SReference InvalidLexem(const char *)
{
    static SLabel inv("#<UNREADABLE-TOKEN>");
    return inv;
}

/////////////////////////////
// SchemeReader

SchemeReader::SchemeReader()
{
    AddStringLiteral("#<", '>', InvalidLexem);
    AddQuoter("'", QuoteExpression);
    add_vectors_to_reader(*this);
    add_hashtables_to_reader(*this);
    AddDelimiter("#T", *PTheSchemeBooleanTrue);
    AddDelimiter("#t", *PTheSchemeBooleanTrue);
    AddDelimiter("#F", *PTheSchemeBooleanFalse);
    AddDelimiter("#f", *PTheSchemeBooleanFalse);
}

