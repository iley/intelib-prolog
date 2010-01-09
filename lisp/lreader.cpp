//   InteLib                                    http://www.intelib.org
//   The file lisp/lreader.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "lisp.hpp"
#include "lreader.hpp"

SString SExpressionExtvarname::TextRepresentation() const 
{
    return SString("#&")+this->GetValue();
}

IntelibTypeId SExpressionExtvarname::TypeId(&SExpressionString::TypeId);

//////////
// transformers

static SReference QuoteExpression(const SReference &ref)
{
    return ~(LReference(ref));
}

static SReference FunquoteExpression(const SReference &ref)
{
    LFunctionConstructor F;
    return F^(LReference(ref));
}

static SReference ExternVarProcess(const char *str)
{
    return SReference(new SExpressionExtvarname(str));
}

static SReference InvalidLexem(const char *)
{
    static SLabel inv("#<UNREADABLE-TOKEN>");
    return inv;
}

/////////////////////////////
// LispReader

LispReader::LispReader()
{
    AddStringLiteral("#<", '>', InvalidLexem);
    AddTokenType("#&", ExternVarProcess);
    AddQuoter("'", QuoteExpression);
    AddQuoter("#'", FunquoteExpression);
    add_vectors_to_reader(*this);
    add_hashtables_to_reader(*this);
}

