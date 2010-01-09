//   InteLib                                    http://www.intelib.org
//   The file lisp/lisp.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "../sexpress/sstring.hpp"
//#include "lsymbol.hpp"
#include "llambda.hpp"



LReference::LReference(UserLispFunction fun)
    : SReference(new SExpressionUserCFunction(fun))
{} 

LReference LReference::Evaluate() const 
{
    LispContinuation cont;
    return Evaluate(cont);
}

LReference LReference::Evaluate(LispContinuation &cont) const 
{
    int mark = cont.GetMark();
    cont.PushTodo(LispContinuation::just_evaluate, *this);
    while(!cont.Ready(mark))
        cont.Step();
    return cont.Get();
}

LReference LReference::Apply(const SReference &args) const 
{
    LispContinuation cont;
    return Apply(args, cont);
}

LReference LReference::Apply(const SReference &args,
                             LispContinuation &cont) const 
{
    int mark = cont.GetMark();
    cont.PushTodo(LispContinuation::evaluate_prepared,
                  SReference(*this,args));
    while(!cont.Ready(mark))
        cont.Step();
    return cont.Get();
}

bool LReference::IsEql(const SReference& other) const
{ 
    if(GetPtr()==other.GetPtr()) return true;
    if(!GetPtr() || !other.GetPtr()) return false;
    if(GetPtr()->TermType() != other.GetPtr()->TermType()) return false;
    return GetPtr()->SpecificEql(other.GetPtr()); 
}

bool LReference::IsEqual(const SReference& other) const
{
    if(IsEql(other)) return true;
    SExpressionCons *dp1 = DynamicCastGetPtr<SExpressionCons>();
    SExpressionCons *dp2 = dp1 ? 
        other.DynamicCastGetPtr<SExpressionCons>() : 0;
    if(dp1 && dp2) {
        return LReference(dp1->Car()).IsEqual(dp2->Car()) &&
               LReference(dp1->Cdr()).IsEqual(dp2->Cdr());
    } else {
        return false; 
    }
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
static SString LReferenceTextRepresentationCallBack(const SReference& ex)
{
    return LReference(ex).TextRepresentation();
}

SString LReference::TextRepresentation() const
{
    if(!GetPtr()) return "#<UNBOUND>";
    if(GetPtr()->TermType()==SExpressionCons::TypeId) {
        SExpressionCons *dp = static_cast<SExpressionCons*>(GetPtr());
        if(dp->Car().GetPtr() == PTheLispSymbolQuote->GetPtr()) {
            // QUOTE form... check that there's one argument
            if(dp->Cdr().GetPtr()->TermType()==SExpressionCons::TypeId) {
                SExpressionCons *dp2 = 
                    static_cast<SExpressionCons*>(dp->Cdr().GetPtr());
                if(dp2->Cdr().IsEmptyList()) {
                    // YES!
                    return SString("\'") + 
                           LReference(dp2->Car()).TextRepresentation(); 
                }
            }
        } else
        if(dp->Car().GetPtr() == PTheLispSymbolFunction->GetPtr()) {
            // FUNCTION form... check that there's one argument
            if(dp->Cdr().GetPtr()->TermType()==SExpressionCons::TypeId) {
                SExpressionCons *dp2 = 
                    static_cast<SExpressionCons*>(dp->Cdr().GetPtr());
                if(dp2->Cdr().IsEmptyList()) {
                    // YES!
                    return SString("#\'") + 
                           LReference(dp2->Car()).TextRepresentation(); 
                }
            }
        } 
        // seem not to be QUOTE nor FUNCTION form, but it may contain them...
        return SString("(") + 
               dp->CoreTextRepresentation(" ", " . ", 
                                  LReferenceTextRepresentationCallBack) +
               SString(")");
    }
    return GetPtr()->TextRepresentation(); 
}
#endif

static LReference JustAnUnboundLReferenceObject;

SReference *PTheLispSymbolLambda = &JustAnUnboundLReferenceObject;
SReference *PTheLispSymbolQuote = &JustAnUnboundLReferenceObject;
SReference *PTheLispSymbolFunction = &JustAnUnboundLReferenceObject;
SReference *PTheLispBooleanTrue = &JustAnUnboundLReferenceObject;
SReference *PTheLispBooleanFalse = &JustAnUnboundLReferenceObject;
