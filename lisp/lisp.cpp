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




#include "../sexpress/sstring.hpp"
#include "lcont.hpp"
//#include "lsymbol.hpp"
#include "llambda.hpp"

#include "lisp.hpp"


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
