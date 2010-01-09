//   InteLib                                    http://www.intelib.org
//   The file scheme/scheme.cpp
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
#include "schcont.hpp"
#include "scheme.hpp"


SchReference::SchReference(UserLispFunction fun)
    : SReference(new SExpressionUserCFunction(fun))
{} 

SchReference SchReference::Evaluate() const
{
    SchemeContinuation cont;
    return Evaluate(cont);
}

SchReference SchReference::Evaluate(SchemeContinuation &cont) const
{
    int mark = cont.GetMark();
    cont.PushTodo(SchemeContinuation::just_evaluate, *this);
    while(!cont.Ready(mark))
        cont.Step();
    return cont.Get();
}

SchReference SchReference::Apply(const SReference &args) const 
{
    SchemeContinuation cont;
    return Apply(args, cont);
}

SchReference SchReference::Apply(const SReference &args,
                                 SchemeContinuation &cont) const 
{
    int mark = cont.GetMark();
    cont.PushTodo(SchemeContinuation::evaluate_prepared,
                  SReference(*this,args));
    while(!cont.Ready(mark))
        cont.Step();
    return cont.Get();
}

bool SchReference::IsEql(const SReference& other) const
{
    // this is exactly the same code as for LReference
    // perhaps I should move it somewhere to sexpress/
    if(GetPtr()==other.GetPtr()) return true;
    if(!GetPtr() || !other.GetPtr()) return false;
    if(GetPtr()->TermType() != other.GetPtr()->TermType()) return false;
    return GetPtr()->SpecificEql(other.GetPtr()); 
}

bool SchReference::IsEqual(const SReference& other) const
{
    // this is exactly the same code as for LReference
    // perhaps I should move it somewhere to sexpress/
    if(IsEql(other)) return true;
    SExpressionCons *dp1 = DynamicCastGetPtr<SExpressionCons>();
    SExpressionCons *dp2 = dp1 ? 
        other.DynamicCastGetPtr<SExpressionCons>() : 0;
    if(dp1 && dp2) {
        return SchReference(dp1->Car()).IsEqual(dp2->Car()) &&
               SchReference(dp1->Cdr()).IsEqual(dp2->Cdr());
    } else {
        return false; 
    }
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
static SString SchReferenceTextRepresentationCallBack(const SReference& ex)
{
    return SchReference(ex).TextRepresentation();
}

SString SchReference::TextRepresentation() const
{
    // this code is almost the same as for LReference
    // sorry guys I know it's not wise to copy&paste, but I couldn't
    //   invent a better way to accomplish this
    if(!GetPtr()) return "#<UNBOUND>";
    if(GetPtr()->TermType()==SExpressionCons::TypeId) {
        SExpressionCons *dp = static_cast<SExpressionCons*>(GetPtr());
        if(dp->Car().GetPtr() == PTheSchemeSymbolQuote->GetPtr()) {
            // QUOTE form... check that there's one argument
            if(dp->Cdr().GetPtr()->TermType()==SExpressionCons::TypeId) {
                SExpressionCons *dp2 = 
                    static_cast<SExpressionCons*>(dp->Cdr().GetPtr());
                if(dp2->Cdr().IsEmptyList()) {
                    // YES!
                    return SString("\'") + 
                           SchReference(dp2->Car()).TextRepresentation(); 
                }
            }
        } 
        // seem not to be QUOTE form, but it may contain them...
        return SString("(") + 
               dp->CoreTextRepresentation(" ", " . ", 
                                  SchReferenceTextRepresentationCallBack) +
               SString(")");
    }
    return GetPtr()->TextRepresentation(); 
}
#endif



static SReference JustAnUnboundReferenceObject;

SReference *PTheSchemeSymbolQuote = &JustAnUnboundReferenceObject;
SReference *PTheSchemeBooleanTrue = &JustAnUnboundReferenceObject;
SReference *PTheSchemeBooleanFalse = &JustAnUnboundReferenceObject;

