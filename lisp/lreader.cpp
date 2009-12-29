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

